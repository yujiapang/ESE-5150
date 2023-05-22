// plant_watering_Lab2
// For ESE515 2022


#include "config.h"

#include "DHT.h"

#define DHT_SENSOR_TYPE DHT11 //notice our sensor number here
#define DHT_SENSOR_PIN D1 // assign name of variable and pin number
DHT dht( DHT_SENSOR_PIN, DHT_SENSOR_TYPE ); //command specific to library

// set up the 'soil_moisture', 'temperatureDHT' and 'humidityDHT' feeds
AdafruitIO_Feed *soil_moisture  = io.feed("soil_moisture");
AdafruitIO_Feed *temperatureDHT = io.feed("temperatureDHT");
AdafruitIO_Feed *humidityDHT = io.feed("humidityDHT");
AdafruitIO_Feed *thermistor = io.feed("thermistor");
AdafruitIO_Feed *IsPlantWatered = io.feed("IsPlantWatered");
// these correspond to the pins on your NodeMCU
#define pumpPin D2    //D2 in Node MCU to drive watering pump
#define threshold 700 //after tinkering with your moisture sensor, feel free to change 'threshold' to a desired value

int sm = 0, water=0 ;
float temperature = 0; //float variables allow for decimals
float humidity = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

/*void send_thermistor_sensor() {

  Vo = analogRead(A0);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  thermistor->save(Tc); 
  Serial.print("Temperature: "); 
  Serial.print(Tc);
  Serial.println(" C"); 
  delay(500);
}*/

void send_sm_Sensor()
{
  sm = analogRead(A0); //input from soil moisture sensor

  soil_moisture->save(sm);
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
      delay(5000);
      }
    else {
      temperatureDHT->save(temperature);
      delay(5000);
      humidityDHT->save(humidity);
      delay(5000);
    }   
 
}

void water_plant ()
{
  if(sm < threshold) //this means soil is wet and doesn't need to be watered
  {
    digitalWrite (pumpPin , LOW);
    water=0;
    delay(3000);
  }
  else
  {
    digitalWrite (pumpPin , HIGH);
    water=100;
    delay(3000);
    digitalWrite (pumpPin, LOW); //prevent pump from staying on
  }
  IsPlantWatered->save(water);
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
  send_sm_Sensor();
  delay(2000);
  send_dht_Sensor();
  delay(2000);

  water_plant(); 
}
