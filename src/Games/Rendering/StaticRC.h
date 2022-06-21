#ifndef CIRCUITPET_FIRMWARE_STATICRC_H
#define CIRCUITPET_FIRMWARE_STATICRC_H

#include "RenderComponent.h"

class StaticRC : public RenderComponent{
public:
	/**
	 * @param file Raw RGB565 image file.
	 * @param dim Width, height.
	 */
	StaticRC(File file, const PixelDim& dim);

protected:
	void push(Sprite* parent, PixelDim pos) const override;

private:
	File file;
	PixelDim dim;
};


#endif //CIRCUITPET_FIRMWARE_STATICRC_H
