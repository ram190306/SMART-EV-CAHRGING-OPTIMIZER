#include <Arduino.h>
#include <WiFi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include "Network.h"
#include "Telemetery.h"
#include "model.h"
#include "edge_ai.h"
#include "optimization.h"
#include "rpc.h"
#include"attributes.h"

void setup()
{
    // initialize serial communication at 115200 baud rate
    Serial.begin(115200);
    dht.begin();  //initialize DHT sensor
    // cpnfig esp32 with real time
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    //configure pin modes for buttons, relay and LEDs
    pinMode(BTN_PLUGIN, INPUT_PULLUP);
    pinMode(BTN_PLUGOUT, INPUT_PULLUP);

    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    // connect board to WiFi
    connectWiFi();

    // config MQTT server
    mqtt.setServer(MQTT_SERVER, MQTT_PORT);
    // set callback function upon receiving data from the cloud
    mqtt.setCallback(mqttCallback);
    mqtt.setBufferSize(512); // increase buffer size to handle larger payloads
    //connect mqtt to the cloud
    connectMQTT();
}

unsigned long now;
unsigned long last_print = 0;

void loop()
{
    mqtt.loop(); // process incoming MQTT messages
    // keep the MQTT connection alive and reconnect if it drops
    if (!mqtt.connected())
    {
        connectMQTT();
    }
    mqtt.loop();

    // push data every 5 seconds
    now = millis();
    if ((now - last_print) > 5000)
    {
        last_print = now;
        //read data from sensors   // voltage, current, power, temperature , bayStatus
        Sample_sensor();
        // run AI get prediction
        runEdgeAIInference();
        // decide the load based on the predictions
        if (manualOverrideActive == 0)
         {
            runOptimization();
        }


        //publish the data
        publishTelemetry();
        

    }
    plug_status();

    updateLeds();

}
