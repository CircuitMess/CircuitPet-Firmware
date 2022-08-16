#include "Intro.h"
#include <SPIFFS.h>
#include "HatchingState.h"
#include "Home/DuckScreen.h"
#include "Stats/StatsManager.h"
#include <Loop/LoopManager.h>
#include "DeathState.h"
#include <FS/CompressedFile.h>
#include <CircuitPet.h>

typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;
static rgb hsv2rgb(hsv in){
	double hh, p, q, t, ff;
	long i;
	rgb out;

	if(in.s <= 0.0){       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long) hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i){
		case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;

		case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
		case 5:
		default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
	}
	return out;
}

Intro* Intro::instance = nullptr;
Intro::Intro(Sprite* base) : gif(base, CompressedFile::open(SPIFFS.open("/intro.hs"), 10, 5)), base(base){
	instance = this;
}

void Intro::onStart(){
	gif.start();
	gif.setLoop(false);
	gif.setLoopDoneCallback([](){
		instance->exit = true;
	});

	LoopManager::addListener(this);
}



void Intro::onStop(){
	gif.stop();
	LoopManager::removeListener(this);
}

void Intro::loop(uint micros){
	if(exit){
		RGB.setColor(Pixel::Black);
		delay(500);
		volatile auto temp = instance->base;
		instance->stop();
		delete instance;


		if(StatMan.isHatched()){
			if(StatMan.hasDied()){
				auto duck = new DeathState(temp);
				duck->start();
			}else{
				auto duck = new DuckScreen(temp);
				duck->start();
			}
		}else{
			auto hatch = new HatchingState(temp);
			hatch->start();
		}

		return;
	}

	double hue = 360.0f * ((double)((millis() % 2000) / 2000.0));

	hsv h = {hue, 1, 1};
	rgb c = hsv2rgb(h);

	RGB.setColor({(uint8_t)(c.r*255), (uint8_t)(c.g*255), (uint8_t)(c.b*255)});


	if(gif.checkFrame()){
		gif.nextFrame();
	}
	gif.push();
}
