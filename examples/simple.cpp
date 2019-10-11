#include <Arduino.h>
#include <Homie.h>
#include <LoggerNode.h>
#include <HomieLedRingTh.h>

#define FW_NAME "homie-ledring"
#define FW_VERSION "0.0.1"
LoggerNode LN;
HomieLedRingTh ledringth;

void setup() {
  Serial.begin(74880);
  Serial.println("Start");
  Serial.flush();

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.disableLedFeedback();
  Homie.disableResetTrigger();
  Homie.setLoggingPrinter(&Serial);
  Homie.setup();
}

void loop() {
	Homie.loop();
}
