// photoresistor_Lab3
// For ESE515 2022


#include "config.h"

// set up the photoresistor output feeds
AdafruitIO_Feed *light  = io.feed("light");

// these correspond to the pins on your NodeMCU
#define LEDPin D3    //D3 in Node MCU to control LED

int light_value = 0;

void send_light_Sensor()
{
  light_value = analogRead(A0); //input from photoresistor
  light->save(light_value);
  delay(2000);
}

void setup() {
  // declare pins as inputs and outputs
  pinMode (LEDPin, OUTPUT );
      
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

  // send sensor data
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. 
  //send_sm_Sensor();  //uncomment if you want to use soil moisture sensor from previous HW code
  //send_dht_Sensor();  //uncomment if you want to use DHT sensor from previous HW code
  //water_plant(); //uncomment if you want to use watering pump from previous HW code
  
  send_light_Sensor();
  
  delay(2000);
  
}
