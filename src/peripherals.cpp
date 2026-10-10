#include <Arduino.h>

#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"

unsigned long sessionStartms = 0;

DHT   dht(DHT_PIN , DHT_TYPE);

float mapFloat(long x, long inMin, long inMax, float outMin, float outMax) {
return (x - inMin) * (outMax - outMin) / (float)(inMax - inMin) + outMin;
}

void Sample_sensor(void)
{
    int raw_current = analogRead(CURRENT_PIN); // 0 to 4095    // 0 to 32A
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095    // 0 to 250v
    // map voltage 0 to 250v
    voltage = mapFloat(raw_voltage, 0, 4095, 0, 250); // convert to voltage

    if (bayStatus == "CHARGING")
     {
    // map current 0 to 32A
    current = mapFloat(raw_current, 0, 4095, 0, 32); // convert to current
     }
     else
     {
    // no session in progress: nothing is flowing
    current = 0;
     }

     //  read current and 5 values array 

     // calculate power
     power = voltage * current;

    //to read temperature (no argument = Celsius; passing a pin number here
    //used to be misread as the library's "return Fahrenheit" flag)
   float t = dht.readTemperature();
  if (!isnan(t)) temperature = t;


    }



float recentAvgCurrent(void) 
{
  float sum =0;
   // read recent 5 current values
   for (int i = 0; i < 5; i++)
   {
       sum = sum + current ;

   }
   return sum / 5;
}



bool plugin_flag_once = 1;
bool plugiout_flag_once = 1;

void plug_status(void)
{
   bool pluginReading = digitalRead(BTN_PLUGIN );

   // detect the sw is pressed
   if (pluginReading == LOW && plugin_flag_once)
  {
    sessionStartMs = millis();  // record the start time of the session
   // plug in switch is pressed
    plugin_flag_once = 0;
   // change bay_status FREE to charging
     if (bayStatus == "FREE")
     {
         bayStatus = "CHARGING";
         Serial.println("Bay1 plugedin  detected,bay is charging");
         // turn on the relay
         digitalWrite(RELAY_PIN, HIGH);
     }

   //update leds
  }
  if (pluginReading == HIGH )
  {
    plugin_flag_once = 1;

  }
   // plug out switch is pressed

   bool plugoutReading = digitalRead(BTN_PLUGOUT );


   // detect the sw is pressed
   if (plugoutReading == LOW && plugiout_flag_once)
  {

   // plug out switch is pressed
    plugiout_flag_once = 0;
   // change bay_status  charging TO free
     if (bayStatus == "CHARGING")
     {
         bayStatus = "FREE";
         Serial.println("Bay1 plugout detected,bay is free");
         // turn off the relay
         digitalWrite(RELAY_PIN, LOW);
     }

   //update leds
  }
  if (plugoutReading == HIGH )
  {
    plugiout_flag_once = 1;

  }
   // change bay_status  charging to FREE
   //update leds




}

/*void update_led_status(void)
{
 // if the bay status is free turn on the green led

 if (bayStatus == "FREE")
 {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
 }
    else
     {
      digitalWrite(LED_GREEN, LOW);
         digitalWrite(LED_YELLOW, HIGH);
      }


 // if the bay status is charging turn on the yellowled

}
*/