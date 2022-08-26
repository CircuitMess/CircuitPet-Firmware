#ifndef CIRCUITPET_FIRMWARE_AUTOSHUTDOWN_H
#define CIRCUITPET_FIRMWARE_AUTOSHUTDOWN_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <Input/InputListener.h>

class AutoShutdown : private LoopListener, private InputListener {
public:
	void begin();

private:
	void loop(uint micros) override;
	void buttonPressed(uint i) override;

	uint32_t activity = 0;

	static const uint8_t minutes[];

};


#endif //CIRCUITPET_FIRMWARE_AUTOSHUTDOWN_H
