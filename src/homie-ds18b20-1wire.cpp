#include <Homie.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>

#define FW_NAME "homie-ds18b20-wire"
#define FW_VERSION "2.1.3"

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
// Instead, we just iterate by index, using .getTempCByIndex

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

                if (ROM_Temp0.setProperty("degrees").send(String(temp0))) {
                            last_temp_sent = millis();
                }
                ROM_Temp1.setProperty("degrees").send(String(temp1));
                ROM_Temp2.setProperty("degrees").send(String(temp2));

                ROM_Temp0.setProperty("freeheap").send(String(ESP.getFreeHeap(),DEC));
        }

}

void setupHandler() {
        ROM_Temp0.setProperty("unit").send("c");
        ROM_Temp1.setProperty("unit").send("c");
        ROM_Temp2.setProperty("unit").send("c");
}

void setup() {
        Serial.begin(115200);

        DS18B20.begin();

        Homie_setFirmware(FW_NAME, FW_VERSION);
        Homie.setLedPin(2, HIGH); // Status LED

        Homie.setSetupFunction(setupHandler);
        Homie.setLoopFunction(loopHandler);

        Homie.setup();
}

void loop() {
        Homie.loop();
}
