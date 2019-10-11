/*
 * HomieLedRingTh.cpp
 *
 *  Created on: 09.10.2019
 *      Author: ian
 */

#include <HomieLedRingTh.h>
#include <Homie.hpp>


//Why????? (--> https://stackoverflow.com/questions/32433554/c11-uniform-initialization-field-initializer-is-not-constant)
const CRGB HomieLedRingTh::ModeColor[AUTO_ON+1] {CRGB::Beige, CRGB::Blue, CRGB::Yellow, CRGB::Green, CRGB::Red };

HomieLedRingTh::HomieLedRingTh():
	HomieNode("lthermo", "Thermostat", "settemp"),
	setTemp(210),
	mode(UNKNOWN)
{
	advertise("settemp").setDatatype("float").setName("Soll-Temperatur").setUnit("°C").settable();
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
	updateLEDRing();
}

bool HomieLedRingTh::handleInput(const HomieRange &range, const String &property, const String &value) {
	if (property.equals("settemp")) {
		float v  = value.toFloat();
		if (v > 15.0 && v < 30.0) {
			setTemp = roundf(v*10.0);
		}
		setProperty("settemp").setRetained(true).send(String(static_cast<float>(setTemp)/10.0, 1));
		return true;
	} else if (property.equals("brightness")) {
		uint_fast8_t v = value.toInt();
		if (v>100) return false;
		FastLED.setBrightness(v);
		setProperty("brightness").send(String(v));
	}

	return false;
}

void HomieLedRingTh::updateLEDRing() {
	ledring[LED_CONNECTED] =  Homie.isConnected() ? CRGB::Green : CRGB::Red;
	ledring[LED_MODE] = ModeColor[mode];
	for (uint_fast8_t i = 0; i<LedCount; i++) {
		uint_fast8_t hue = HUE_BLUE - ((HUE_BLUE - HUE_RED) / (LedCount-1))*i;
		CHSV hC(hue, 255, 255);
		int_fast8_t led_no = LED_TSTART + i;
		if (led_no < 0) led_no+=TotalLED;
		ledring[led_no] = hC;
	}

	FastLED.show();
}
