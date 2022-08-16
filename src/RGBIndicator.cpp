#include "RGBIndicator.h"
#include "Stats/StatsManager.h"
#include <Loop/LoopManager.h>
#include <CircuitPet.h>

RGBIndicator OilRGBIndicator;

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


void RGBIndicator::start(){
	StatMan.addListener(this);
	LoopManager::addListener(this);

	currentOil = prevOil = targetOil = StatMan.get().oilLevel;
}

void RGBIndicator::stop(){
	StatMan.removeListener(this);
	LoopManager::removeListener(this);

	RGB.setColor(Pixel::Black);
}

void RGBIndicator::loop(uint micros){
	if(currentOil != targetOil){

		easeTimer += micros / 1000000.0;
		float x = easeTimer / easeTime;

		if(x >= 1.f){
			currentOil = targetOil;
		}else{

			float ease = 1.0f - cos((x * PI) / 2);

			currentOil = prevOil + ((float)(targetOil - prevOil)) * ease;
		}

	}

	if(currentOil < 15){
		if(millis() % 1000 < 200){
			RGB.setColor(Pixel::Red);
		}else{
			RGB.setColor(Pixel::Black);
		}
	}else{
		double hue = (float)(currentOil - 15) / 85.0 * 60.0 / 255.0 * 360;
		rgb r = hsv2rgb({hue, 1, 1});
		Pixel p = {(uint8_t)(r.r*255.0), (uint8_t)(r.g*255.0), (uint8_t)(r.b*255.0)};
		RGB.setColor(p);
	}
}

void RGBIndicator::statsChanged(const Stats& stats, bool leveledUp){
	targetOil = stats.oilLevel;
	prevOil = currentOil;

	easeTimer = 0;
}
