/***************************************************************************
 * For ESE515 Spring 2022
 ***************************************************************************/
 
/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.

/************************ Program Starts Here *******************************/

// PIR Sensor_Lab4

#include "config.h"

#include "DHT.h"

#define DHT_SENSOR_TYPE DHT11 //notice our sensor number here
#define DHT_SENSOR_PIN D1 // assign name of variable and pin number
DHT dht( DHT_SENSOR_PIN, DHT_SENSOR_TYPE ); //command specific to library

#define SM_SENSOR_PIN D2 // soil moisture sensor pin signal
#define Light_SENSOR_PIN D3 // photoresistor sensor pin signal
#define ANALOG_PIN A0 // analog pin signal

// set up the 'light', 'soil_moisture', 'temperatureDHT' and 'humidityDHT' feeds
AdafruitIO_Feed *light  = io.feed("light");
AdafruitIO_Feed *soil_moisture  = io.feed("soil_moisture");
AdafruitIO_Feed *temperatureDHT = io.feed("temperatureDHT");
AdafruitIO_Feed *humidityDHT = io.feed("humidityDHT");

float sm = 0;
float temperature = 0; //float variables allow for decimals
float humidity = 0;
float light_value = 0;

void send_dht_Sensor()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      delay(1000);
      }
    else {
      temperatureDHT->save(temperature);
      delay(2000);
      humidityDHT->save(humidity);
      delay(2000);
    }   
 
}

void send_Analog_Sensor()
{

  digitalWrite(SM_SENSOR_PIN, HIGH);  //activate soil moisture sensor
  delay(1000);
  sm = analogRead(A0);
  soil_moisture->save(sm);
  digitalWrite(SM_SENSOR_PIN, LOW);  //deactivate soil moisture sensor
  delay(1000);
  
  digitalWrite(Light_SENSOR_PIN, HIGH);  //activate photoresistor
  delay(1000);
  light_value = analogRead(A0);
  light->save(light_value);
  digitalWrite(Light_SENSOR_PIN, LOW);  //deactivate photoresistor 
  delay(1000);
 
}
 

void setup() {
  // declare pins as inputs and outputs
  pinMode(SM_SENSOR_PIN, OUTPUT);
  pinMode(Light_SENSOR_PIN, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
   
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // send soil moisture data
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. 
  send_Analog_Sensor();
  delay(2000);
  send_dht_Sensor();
  delay(2000);

}
