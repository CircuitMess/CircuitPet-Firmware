#ifndef CIRCUITPET_FIRMWARE_STATS_HPP
#define CIRCUITPET_FIRMWARE_STATS_HPP

#include <Arduino.h>

struct Stats {
	uint8_t happiness; //0-100
	uint8_t oilLevel; //0-100
	uint16_t experience; //0-UINT16_MAX

	Stats operator+(const Stats& right){
		return { (uint8_t)(min(max(0, happiness + right.happiness), 100)),
				 (uint8_t)(min(max(0, oilLevel + right.oilLevel), 100)),
				 (uint16_t)(min(max(0, experience + right.experience), UINT16_MAX)) };
	}

	Stats operator-(const Stats& right){
		return { (uint8_t)(min(max(0, happiness - right.happiness), 100)),
				 (uint8_t)(min(max(0, oilLevel - right.oilLevel), 100)),
				 (uint16_t)(min(max(0, experience - right.experience), UINT16_MAX)) };
	}

	Stats& operator+=(const Stats& right){
		*this = *this + right;
		return *this;
	}

	Stats& operator-=(const Stats& right){
		*this = *this - right;
		return *this;
	}

	bool operator==(const Stats& right){
		return oilLevel == right.oilLevel &&
			   happiness == right.happiness &&
			   experience == right.experience;
	}

	bool operator!=(const Stats& right){
		return !(*this == right);
	}
};

#endif //CIRCUITPET_FIRMWARE_STATS_HPP
