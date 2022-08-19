#include "RGBController.h"
#include <Devices/Matrix/Matrix.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include <FS/RamFile.h>

RGBController RGBTrack;
RGBController RGBSlot;

RGBController::RGBController(){

}

void RGBController::begin(){
	LoopManager::addListener(this);
}

void RGBController::setColor(Pixel color){
	slotColors = color;

	if(slotStates == Solid){
		RGB.setColor(color);
	}
}

void RGBController::clear(){
	slotStates = Solid;
	blinkStates = false;
	blinkColors = Pixel::Black;
	blinkStartTimes = 0;

	slotColors = Pixel::Black;
	RGB.setColor(Pixel::Black);
}

void RGBController::setSolid(Pixel color){

	slotStates = Solid;

	setColor(color);
}

void RGBController::blink(Pixel color){
	slotStates = Once;
	blinkColors = color;
	blinkStartTimes = millis();
	blinkStates = true;

	RGB.setColor(color);
}

void RGBController::blinkTwice(Pixel color){
	blink(color);
	slotStates = Twice;
}

void RGBController::blinkContinuous(Pixel color){
	blink(color);
	slotStates = Continuous;
}

void RGBController::loop(uint micros){
	if(slotStates == Solid) return;
	if(millis() - blinkStartTimes < blinkDuration) return;

	if(slotStates == Continuous){
		if(millis() - blinkStartTimes < blinkContinuousDuration) return;

		blinkStates = !blinkStates;
		blinkStartTimes = millis();
		RGB.setColor(blinkStates ? blinkColors : Pixel::Black);
	}else if(slotStates == Twice && blinkStates){
		blinkStates = false;
		blinkStartTimes = millis();
		RGB.setColor(Pixel::Black);
	}else if(slotStates == Twice && !blinkStates){
		blinkStates = true;
		blinkStartTimes = millis();
		slotStates = Once;
		RGB.setColor(blinkColors);
	}else if(slotStates == Once){
		blinkStates = false;
		blinkStartTimes = 0;
		blinkColors = Pixel::Black;
		slotStates = Solid;
		RGB.setColor(slotColors);
	}
}
