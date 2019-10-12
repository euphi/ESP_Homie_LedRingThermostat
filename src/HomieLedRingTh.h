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
	int16_t getSetTemp() const;
	void setSetTemp(int16_t setTemp);
	void setActTemp(int16_t actTemp);

protected:
	virtual void onReadyToOperate() override;
	virtual bool handleInput(const HomieRange &range, const String &property, const String &value) override;
	virtual void loop() override;
	virtual void setup() override;

private:
	static const uint8_t DataPIN = 13;
	static const uint8_t TotalLED = 24;
	CRGB ledring[TotalLED];

	enum ELED { LED_CONNECTED = 11 , LED_MODE = 13, LED_TSTART = -9, LED_TEND = 9 };

	int16_t setTemp;		// scaling 1 bit = 0,1°C
	int16_t actTemp;		// scaling 1 bit = 0,1°C
	static const int16_t MinTemp = 180;		// scaling 1 bit = 0,1°C
	static const int16_t MaxTemp = 250;		// scaling 1 bit = 0,1°C

	enum EThermMode { UNKNOWN = 0, MANUAL_OFF, MANUAL_ON, AUTO_OFF, AUTO_ON };
	static const CRGB ModeColor[AUTO_ON + 1];
	EThermMode mode;


	static const int8_t LedCount = LED_TEND - LED_TSTART + 1;
	static const int16_t DegPerLED = (10*(MaxTemp - MinTemp)) / LedCount; // scaling 1 bit = 0,1°C

	void updateLEDRing();

};

#endif /* SRC_HOMIELEDRINGTH_H_ */
