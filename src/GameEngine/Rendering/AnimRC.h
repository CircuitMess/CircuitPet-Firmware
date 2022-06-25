#ifndef CIRCUITPET_FIRMWARE_ANIMRC_H
#define CIRCUITPET_FIRMWARE_ANIMRC_H

#include "RenderComponent.h"
#include <Display/GIFAnimatedSprite.h>

class AnimRC : public RenderComponent{
public:
	/**
	 * @param file GIF file.
	 */
	AnimRC(File file);
	/**
	 * Replaces the current animation with the one provided.
	 * Will auto-play the anim if the last one was running when the new one was set.
	 * @param file GIF file.
	 */
	void setAnim(File file);

	void start();
	void stop();

protected:
	void push(Sprite* parent, PixelDim pos) const override;

private:
	GIFAnimatedSprite gif;
	bool playing = false;
};


#endif //CIRCUITPET_FIRMWARE_ANIMRC_H
