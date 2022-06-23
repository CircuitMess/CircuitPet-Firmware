
#include "StatSprite.h"
#include <SPIFFS.h>

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

static hsv rgb2hsv(rgb in);
static rgb hsv2rgb(hsv in);

rgb hsv2rgb(hsv in){
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

hsv rgb2hsv(rgb in){
	hsv out;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if(delta < 0.00001){
		out.s = 0;
		out.h = 0; // undefined, maybe nan?
		return out;
	}
	if(max > 0.0){ // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}else{
		// if max is 0, then r = g = b = 0
		// s = 0, iconHeight is undefined
		out.s = 0.0;
		out.h = NAN;                            // its now undefined
		return out;
	}
	if(in.r >= max)                           // > is bogus, just keeps compilor happy
		out.h = (in.g - in.b) / delta;        // between yellow & magenta
	else if(in.g >= max)
		out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
	else
		out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if(out.h < 0.0)
		out.h += 360.0;

	return out;
}


const char* StatSprite::paths[] = {
		"/Stats/Happiness.raw",
		"/Stats/OilLevel.raw",
		"/Stats/Battery.raw"
};

const char* StatSprite::barPath = "/Stats/Bar.raw";

StatSprite::StatSprite(Sprite* parent, StatSprite::Type type, uint8_t level) : sprite(parent, iconWidth + barWidth, iconHeight), type(type), level(level){
	draw();
}

void StatSprite::setLevel(uint8_t level){
	this->level = level;
}

void StatSprite::push(){
	sprite.push();
}

void StatSprite::setPos(uint8_t x, uint8_t y){
	sprite.setPos(x, y);
}

void StatSprite::draw(){
	//draw icon
	sprite.clear(TFT_TRANSPARENT);

	fs::File barFile = SPIFFS.open(barPath);
	sprite.drawIcon(barFile, iconWidth, 2, barWidth, barHeight);

	fs::File statTypeFile = SPIFFS.open(paths[type]);
	if(type != Type::Battery){
		sprite.drawIcon(statTypeFile, 3, 0, iconWidth, iconHeight);
	}else{
		sprite.drawIcon(statTypeFile, 0, 0, iconWidth, iconHeight);
	};
	int width = map(level, 0, 100, 0, barWidth - 4); //-4 pixels from the edge bar

	double hue = (float) level / 100.0 * 60.0 / 255.0 * 360;
	rgb rgbColor = hsv2rgb({ hue, 1.0, 1.0 });
	uint16_t c = lgfx::color565(rgbColor.r * 255.0, rgbColor.g * 255.0, rgbColor.b * 255.0);

	sprite.fillRect(iconWidth + 2, 4, width, fillHeight, c); //+2 pixels from the edge bar
}
