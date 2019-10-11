/*
 * HomieLedRingTh.h
 *
 *  Created on: 09.10.2019
 *      Author: ian
 */


#ifndef SRC_HOMIELEDRINGTH_H_
#define SRC_HOMIELEDRINGTH_H_

#include <HomieNode.hpp>
#include <FastLED.h>

class HomieLedRingTh: public HomieNode {
public:
	HomieLedRingTh();

protected:
	virtual void onReadyToOperate() override;
	virtual bool handleInput(const HomieRange &range, const String &property, const String &value) override;
	virtual void loop() override;
	virtual void setup() override;

private:
	static const uint8_t DataPIN = 13;
	static const uint8_t TotalLED = 24;
	CRGB ledring[TotalLED];

	enum ELED { LED_CONNECTED = 11 , LED_MODE = 13, LED_TSTART = -8, LED_TEND = 8 };

	uint16_t setTemp;		// scaling 1 bit = 0,1°C
	static const uint16_t MinTemp = 180;		// scaling 1 bit = 0,1°C
	static const uint16_t MaxTemp = 250;		// scaling 1 bit = 0,1°C

	enum EThermMode { UNKNOWN = 0, MANUAL_OFF, MANUAL_ON, AUTO_OFF, AUTO_ON };
	static const CRGB ModeColor[AUTO_ON+1];
	EThermMode mode;


	static const uint8_t LedCount = LED_TEND-LED_TSTART + 1;
	static const uint16_t DegPerLED = (MaxTemp-MinTemp) / LedCount;

	void updateLEDRing();

};

#endif /* SRC_HOMIELEDRINGTH_H_ */
