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
	// TODO - transparent pixels should be TFT_TRANSPARENT
	gif.push(parent, pos.x, pos.y, TFT_BLACK);
}
