#include "AnimRC.h"

#include <utility>

//parentSprite as nullptr is safe as long as you pass the correct parent Sprite* in the push() function
AnimRC::AnimRC(File file) : gif(nullptr, file){
	gif.setLoopMode(loopMode);
}

void AnimRC::setAnim(File file){
	gif = GIFAnimatedSprite(nullptr, file);
	gif.setLoopMode(loopMode);

	if(playing){
		gif.start();
	}
}

void AnimRC::start(){
	if(playing) return;

	gif.start();
	playing = true;
}

void AnimRC::stop(){
	if(!playing) return;

	gif.stop();
	playing = false;
}

void AnimRC::reset(){
	gif.reset();
}

void AnimRC::push(Sprite* parent, PixelDim pos) const{
	gif.push(parent, pos.x, pos.y);
}

void AnimRC::setLoopDoneCallback(std::function<void(uint32_t)> cb){
	gif.setLoopDoneCallback(std::move(cb));
}

void AnimRC::setLoopMode(GIF::LoopMode loopMode){
	this->loopMode = loopMode;
	gif.setLoopMode(loopMode);
}
