#include "AnimRC.h"

//parentSprite as nullptr is safe as long as you pass the correct parent Sprite* in the push() function
AnimRC::AnimRC(File file) : gif(nullptr, file){

}

void AnimRC::setAnim(File file){
	gif = GIFAnimatedSprite(nullptr, file);

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

void AnimRC::push(Sprite* parent, PixelDim pos, float rot) const{
	if(rot == 0){
		gif.push(parent, pos.x, pos.y);
	}else{
		gif.pushRotate(parent, pos.x + gif.getWidth() / 2, pos.y + gif.getHeight() / 2, rot);
	}
}

void AnimRC::setLoopDoneCallback(std::function<void(uint32_t)> cb){
	gif.setLoopDoneCallback(cb);
}
