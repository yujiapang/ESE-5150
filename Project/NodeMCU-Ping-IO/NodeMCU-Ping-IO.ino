//#include "config.h"
#include "DHT.h"
#define DHT_SENSOR_TYPE DHT11
#define DHT_SENSOR_PIN D1
#include "CayenneMQTTESP8266.h"
#include "ESP8266WiFi.h"

int GasPin = D2;
int redLED = D4;
int SENSOR_OUTPUT_PIN = D5;
int greenLED = D7;     
int MQ = 0;
float temperature = 0; //float variables allow for decimals
float humidity = 0;  
char ssid[] = "18140100010"; // YOUR SSID
char password[]="201108Pyj"; // YOUR PASSWORD
char username[] = "ca533530-dda0-11ed-8485-5b7d3ef089d0"; // 
char mqtt_password[] = "9111b50e24943d3e174261f70ebfa4f6d399df79"; 
char client_id[] = "e0fbaf10-dda0-11ed-8485-5b7d3ef089d0"; 
int PIR=0;

DHT dht( DHT_SENSOR_PIN, DHT_SENSOR_TYPE ); //command specific to library
// set up the 'GasSensor', 'temperatureDHT' and 'humidityDHT' feeds
// AdafruitIO_Feed *GasSensor  = io.feed("MQ");
// AdafruitIO_Feed *temperatureDHT = io.feed("temperatureDHT");
// AdafruitIO_Feed *humidityDHT = io.feed("humidityDHT");
// AdafruitIO_Feed *PIRsensor = io.feed("PIR");
// For Adafruit


void send_MQ_Sensor()
{
  MQ = analogRead(A0); //input from soil moisture sensor
//  GasSensor->save(MQ);
// For Adafruit
  Cayenne.virtualWrite(0,MQ);
  if (MQ>400) {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  Serial.println("gas detected!"); 
  digitalWrite(GasPin,HIGH);
  delay(20000);
  }
  else {
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED,LOW);
  Serial.println("gas not detected!"); 
  digitalWrite(GasPin,HIGH);
  delay(20);
  }
}

void send_dht_Sensor()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temperature = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      delay(2000);
      }
    else {
      Cayenne.virtualWrite(1,temperature);
      //temperatureDHT->save(temperature);
      // For Adafruit
      delay(2000);
      //humidityDHT->save(humidity);
      // For Adafruit
      Cayenne.virtualWrite(2,humidity);
      delay(2000);
    }   
 
}

void send_PIR() {
  long PIR = digitalRead(SENSOR_OUTPUT_PIN);
  Cayenne.virtualWrite(3,PIR);
  //PIRsensor->save(PIR);
  // For Adafruit
  if (PIR== HIGH) {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  Serial.println("Motion detected!"); 
  delay(20000);
  }
  else {
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED,LOW);
  Serial.println("Motion absent!");
  //delay(20);
  }
}


void setup() {
  // start the serial connection
  Serial.begin(115200);
  
  // // wait for serial monitor to open
  // while(! Serial);

  // Serial.print("Connecting to Adafruit IO");

  // // connect to io.adafruit.com
  // io.connect();

  // // wait for a connection
  // while(io.status() < AIO_CONNECTED) {
  //   Serial.print(".");
  //   delay(500);
  // }
  // // we are connected
  //  Serial.println(io.statusText());
  // For Adafruit
  Serial.println();
  pinMode(SENSOR_OUTPUT_PIN, INPUT);  
  pinMode(greenLED, OUTPUT);     
  pinMode(redLED, OUTPUT); 
  Cayenne.begin(username,mqtt_password,client_id,ssid,password);
}


void loop() {
//  io.run();
// For Adafruit
  Cayenne.loop();
  send_MQ_Sensor();
  send_dht_Sensor();
  send_PIR();
}