/*
 * HomieLedRingTh.cpp
 *
 *  Created on: 09.10.2019
 *      Author: ian
 */

#include <HomieLedRingTh.h>
#include <Homie.hpp>
#include <LoggerNode.h>


//Why????? (--> https://stackoverflow.com/questions/32433554/c11-uniform-initialization-field-initializer-is-not-constant)
const CRGB HomieLedRingTh::ModeColor[AUTO_ON+1] {CRGB::Beige, CRGB::Blue, CRGB::Yellow, CRGB::Green, CRGB::Red };

HomieLedRingTh::HomieLedRingTh():
	HomieNode("lthermo", "Thermostat", "settemp"),
	setTemp(210),
	actTemp(200),
	mode(UNKNOWN)
{
	advertise("settemp").setDatatype("float").setName("Soll-Temperatur").setUnit("°C").settable();
	advertise("acttemp").setDatatype("float").setName("IST-Temperatur").setUnit("°C").settable();
	advertise("brightness").setDatatype("int").setName("Helligkeit des LED-Ring").setUnit("percent").settable();

}

void HomieLedRingTh::onReadyToOperate() {
	updateLEDRing();
	FastLED.show();
}

void HomieLedRingTh::loop() {
}

void HomieLedRingTh::setup() {
	FastLED.addLeds<NEOPIXEL, DataPIN>(ledring, TotalLED);
	FastLED.setBrightness(50);
	FastLED.setCorrection(TypicalPixelString);
	updateLEDRing();
}

bool HomieLedRingTh::handleInput(const HomieRange &range, const String &property, const String &value) {
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Received input for property %s, value %s", property.c_str(), value.c_str());
	if (property.equals("settemp")) {
		float v  = value.toFloat();
		if (v >= (MinTemp/10) && v <= (MaxTemp/10)) {
			setSetTemp(roundf(v*10.0));
		} else {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received value %.2f out of range", v);
		}
		return true;
	} else if (property.equals("acttemp")) {
		float v = value.toFloat();
		if (v > (MinTemp/10) && v < (MaxTemp/10)) {
			setActTemp(roundf(v * 10.0));
		} else {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received value %.2f out of range", v);
		}
		return true;
	} else if (property.equals("brightness")) {
		uint_fast8_t v = value.toInt();
		if (v>100) return false;
		FastLED.setBrightness(v);
		setProperty("brightness").send(String(v));
		updateLEDRing();
		return true;
	}

	return false;
}

int16_t HomieLedRingTh::getSetTemp() const {
	return setTemp;
}

void HomieLedRingTh::setSetTemp(int16_t _setTemp) {
	setTemp = _setTemp;
	setProperty("settemp").setRetained(true).send(String(static_cast<float>(setTemp)/10.0, 1));
	updateLEDRing();
}

void HomieLedRingTh::setActTemp(int16_t actTemp) {
	this->actTemp = actTemp;
	setProperty("acttemp").setRetained(true).send(String(static_cast<float>(actTemp)/10.0, 1));
	updateLEDRing();
}

void HomieLedRingTh::updateLEDRing() {
	ledring[LED_CONNECTED] =  Homie.isConnected() ? CRGB::Green : CRGB::Red;
	ledring[LED_MODE] = ModeColor[mode];

	int_fast8_t highlight = 10*(setTemp-MinTemp) / DegPerLED;
	if (highlight >= LedCount) highlight = LedCount-1;
	Serial.printf("Highlight-LED: %d\n", highlight);
	for (int_fast8_t i = 0; i<LedCount; i++) {
		uint_fast8_t hue = HUE_BLUE - ((HUE_BLUE - HUE_RED) / (LedCount-1))*i;
		uint_fast8_t sat = 140;
		uint_fast8_t bright = 70;
		// Pixels for actual te1mp
		if ((MinTemp + (DegPerLED * i)/10) <= actTemp) {
			sat = (i == highlight) ? 160:255;
			bright +=100;
		} else if (i == highlight) {  // Highlight above ActTemp
			sat -=20;
		}
		if (i == highlight) bright = 255;

		Serial.printf("i: %02d T-Start: %d --> hsv: (%03d, %03d, %03d)\n", i, MinTemp + (DegPerLED * i)/10, hue, sat, bright);
		CHSV hC(hue, sat, bright);
		int_fast8_t led_no = LED_TSTART + i;
		if (led_no < 0) led_no+=TotalLED;
		ledring[led_no] = hC;
	}
	FastLED.show();
}
