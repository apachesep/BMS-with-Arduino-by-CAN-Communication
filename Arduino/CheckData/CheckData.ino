#include <Arduino.h>
#include <avr/wdt.h>

char data1, data2;   // for incoming serial data


void setup() {
  wdt_disable();
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial1.begin(9600); 
  delay(100);
  TimerSetup();
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  if (Serial1.available()) {
    // read the incoming byte:
    data2 = Serial1.read(); // Read by this function is too slow that we cant use in our system

    // say what you got:
    Serial.print(data2);
  }
  
}

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  TCNT1 = 0;
  //ProcessData();
  //DeleteVariables();
  ReadBasicInformation();
}


void ReadBasicInformation (void){
  Serial1.write(0xDD);
  Serial1.write(0xA5);
  Serial1.write(0x03);
  Serial1.write(0x00);
  Serial1.write(0xFF);
  Serial1.write(0xFD);
  Serial1.write(0x77);
}

void ReadCellsInformation (void){
  Serial1.write(0xDD);
  Serial1.write(0xA5);
  Serial1.write(0x04);
  Serial1.write(0x00);
  Serial1.write(0xFF);
  Serial1.write(0xFC);
  Serial1.write(0x77);
}

void ReadHardwareInformation (void){
  Serial1.write(0xDD);
  Serial1.write(0xA5);
  Serial1.write(0x05);
  Serial1.write(0x00);
  Serial1.write(0xFF);
  Serial1.write(0xFB);
  Serial1.write(0x77);
}

void TimerSetup (void){
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 0;            // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}



