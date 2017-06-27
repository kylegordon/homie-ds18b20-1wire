#include <Homie.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define FW_NAME "homie-ds18b20-wire-kitchen"
#define FW_VERSION "0.1.2"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */


const int TEMP_INTERVAL = 10;                   // seconds
unsigned long last_temp_sent = 0;

#define ONE_WIRE_BUS 2 // Bus hanging off of pin 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

// This isn't right, as ideally it would search the bus and find all valid ROM_IDs
// ROM ID to device translation is done upstream, in node-red

// "28b8c81d300e5": Rack
// "28ac871d300e4": Garage


HomieNode ROM_Temp0("Temp0", "temperature");
HomieNode ROM_Temp1("Temp1", "temperature");
HomieNode ROM_Temp2("Temp2", "temperature");



void loopHandler() {

        if (millis() - last_temp_sent >= TEMP_INTERVAL * 1000UL
            || last_temp_sent == 0)
        {

                float temp0 = 22.12;
                float temp1 = 33.33;
                float temp2 = 44.44;

                DS18B20.requestTemperatures();
                temp0 = DS18B20.getTempCByIndex(0);
                temp1 = DS18B20.getTempCByIndex(1);
                temp2 = DS18B20.getTempCByIndex(2);

                Serial.print("Temperature: ");
                Serial.print(temp0);
                Serial.println(" °C");

                Serial.print("Temperature: ");
                Serial.print(temp1);
                Serial.println(" °C");

                Serial.print("Temperature: ");
                Serial.print(temp2);
                Serial.println(" °C");


                if (Homie.setNodeProperty(ROM_Temp0, "degrees", String(temp0), true)) {
                            last_temp_sent = millis();
                }

                Homie.setNodeProperty(ROM_Temp1, "degrees", String(temp1), true);
                Homie.setNodeProperty(ROM_Temp2, "degrees", String(temp2), true);

                // Homie.setNodeProperty(ROM_Temp0, "freeheap", String(ESP.getFreeHeap(),DEC), false);
        }

}

void setupHandler() {
        Homie.setNodeProperty(ROM_Temp0, "unit", "c", true);
        Homie.setNodeProperty(ROM_Temp1, "unit", "c", true);
        Homie.setNodeProperty(ROM_Temp2, "unit", "c", true);
}

void setup() {

        DS18B20.begin();

        Homie.setFirmware(FW_NAME, FW_VERSION);
        Homie.setLedPin(2, HIGH); // Status LED

        Homie.registerNode(ROM_Temp0);
        Homie.registerNode(ROM_Temp1);
        Homie.registerNode(ROM_Temp2);

        Homie.setSetupFunction(setupHandler);
        Homie.setLoopFunction(loopHandler);

        Homie.setup();
}

void loop() {
        Homie.loop();
}
