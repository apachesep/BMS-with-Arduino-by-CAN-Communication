#include <Arduino.h>
#include <avr/wdt.h>

char data1, data2;   // for incoming serial data
char state = 0;


void setup() {
  wdt_disable();
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial1.begin(9600); 
  delay(100);
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();

  // send data only when you receive data:
  if (Serial.available()) {
    // read the incoming byte:
    data1 = Serial.read(); // Read by this function is too slow that we cant use in our system

    // say what you got:
    Serial1.print(data1);
  }

  if (Serial1.available()) {
    // read the incoming byte:
    data2 = Serial1.read(); // Read by this function is too slow that we cant use in our system

    // say what you got:
    Serial.print(data2);
  }
}



