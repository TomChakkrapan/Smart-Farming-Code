/* IOT 1
#define BLYNK_TEMPLATE_ID "TMPL6jz3CLaSt"
#define BLYNK_TEMPLATE_NAME "SMARTFARM IOT"
#define BLYNK_AUTH_TOKEN "fIoBU4iJjRNWwWuZEvLVWYOSwiGjp790"
*/
/* IOT 2
#define BLYNK_TEMPLATE_ID "TMPL6l6kBQ3y-"
#define BLYNK_TEMPLATE_NAME "SMARTFARM IOT2"
#define BLYNK_AUTH_TOKEN "d0E-Txi7i9QGVyGk6ryUZ-Ha2Uf_MPwA"
*/
/* IOT 3
#define BLYNK_TEMPLATE_ID "TMPL63h1otZaf"
#define BLYNK_TEMPLATE_NAME "SMARTFARM IOT3"
#define BLYNK_AUTH_TOKEN "D9Sek_8Z_DxGzYDx6gmYu6lBmY2Bh_Yp"
*/
#define BLYNK_TEMPLATE_ID "TMPL63h1otZaf"
#define BLYNK_TEMPLATE_NAME "SMARTFARM IOT3"
#define BLYNK_AUTH_TOKEN "D9Sek_8Z_DxGzYDx6gmYu6lBmY2Bh_Yp"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4     
#define DHTTYPE    DHT11   

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

char ssid[] = "7SE"; //name WIFI
char pass[] = "45454545"; // Password WIFI

int sensorPin = A0; // select the input pin for the Soil moisture sensor
int sensorValue = 0; // variable to store the value coming from the sensor

int Value_Pump = 750;
int Value_Fan = 70;

int ON_Pump = 0 ;
int ON_Fan = 0 ;
int OFF_Pump = 0 ;
int OFF_Fan = 0 ;


void setup()
{
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D5, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);

  ON_Pump = 0 ;
  ON_Fan = 0 ;
  OFF_Pump = 0 ;
  OFF_Fan = 0 ;
}

void loop()
{
  Blynk.run();

  WidgetLED led(V6);
  WidgetLED led2(V7);

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Blynk.virtualWrite(V3,event.temperature);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Blynk.virtualWrite(V4,event.relative_humidity);
  }
  sensorValue = analogRead(sensorPin);
  Blynk.virtualWrite(V5,sensorValue); 
  //////////////////////////////////////////////////
  /////////////////////////////////////////////////
  if (sensorValue > Value_Pump) 
  {
    led.on();
    digitalWrite(D5, LOW);
    OFF_Pump = 1 ;

  }  
  else
  {
    if(ON_Pump == 0)
    {
      led.off();
      digitalWrite(D5, HIGH);
      OFF_Pump = 0 ;
    }
  }
  //////////////////////////////////////////////
  //////////////////////////////////////////
  if (event.relative_humidity > Value_Fan) 
  {
    led2.on();
    digitalWrite(D6, LOW);
    OFF_Fan = 1 ;

  }  
  else
  {
    if(ON_Fan == 0)
    {
      led2.off();
      digitalWrite(D6, HIGH);
      OFF_Fan = 0 ;
    }
  }
}


BLYNK_WRITE(V0)
{
  int button_pump = param.asInt();
  WidgetLED led(V6);
  if (button_pump == HIGH ) 
  {
    led.on();
    digitalWrite(D5, LOW);
    ON_Pump = 1 ;
  }  
  else
  {
    if(OFF_Pump == 0 && button_pump != HIGH)
    {
      led.off();
      digitalWrite(D5, HIGH);
      ON_Pump = 0 ;
    }
  }


}
BLYNK_WRITE(V1)
{
  int button_Fan = param.asInt();
  WidgetLED led2(V7);
  if (button_Fan == HIGH ) 
  {
    led2.on();
    digitalWrite(D6, LOW);
    ON_Fan = 1 ;
  }  
  else
  {
    if(OFF_Fan == 0 && button_Fan != HIGH)
    {
      led2.off();
      digitalWrite(D6, HIGH);
      ON_Fan = 0 ;
    }
  }

}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  analogWrite(D7, pinValue);
}





