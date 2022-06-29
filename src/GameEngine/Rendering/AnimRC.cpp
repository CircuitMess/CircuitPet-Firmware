#include "AnimRC.h"

//parentSprite as nullptr is safe as long as you pass the correct parent Sprite* in the push() function
AnimRC::AnimRC(File file) : gif(nullptr, file){

}

void AnimRC::setAnim(File file){
	gif = GIFAnimatedSprite(nullptr, file);
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

void AnimRC::push(Sprite* parent, PixelDim pos) const{
	gif.push(parent, pos.x, pos.y);
}

void AnimRC::setLoopDoneCallback(std::function<void(uint32_t)> cb){
	gif.setLoopDoneCallback(cb);
}
