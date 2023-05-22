// plant_watering_Lab2
// For ESE515 2022


#include "config.h"

#include "DHT.h"
#define pumpPin D2    //D2 in Node MCU to drive watering pump
#define DHT_SENSOR_TYPE DHT11 //notice our sensor number here
#define DHT_SENSOR_PIN D1 // assign name of variable and pin number
DHT dht( DHT_SENSOR_PIN, DHT_SENSOR_TYPE ); //command specific to library

// set up the 'soil_moisture', 'temperatureDHT' and 'humidityDHT' feeds
AdafruitIO_Feed *GasSensor  = io.feed("MQ");
AdafruitIO_Feed *temperatureDHT = io.feed("temperatureDHT");
AdafruitIO_Feed *humidityDHT = io.feed("humidityDHT");

// these correspond to the pins on your NodeMCU

int MQ = 0;
float temperature = 0; //float variables allow for decimals
float humidity = 0;

void send_MQ_Sensor()
{
  MQ = analogRead(A0); //input from soil moisture sensor

  GasSensor->save(MQ);
  delay(2000);
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
      delay(1000);
      }
    else {
      temperatureDHT->save(temperature);
      delay(2000);
      humidityDHT->save(humidity);
      delay(2000);
    }   
 
}

void setup() {
  // declare pins as inputs and outputs
  pinMode (pumpPin, OUTPUT );
   
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
  send_MQ_Sensor();
  delay(2000);
  send_dht_Sensor();
  delay(2000);

  //water_plant(); 
}
