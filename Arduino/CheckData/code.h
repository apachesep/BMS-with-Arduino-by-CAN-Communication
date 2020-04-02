#include <Arduino.h>
#include <avr/wdt.h>
#include <mcp_can.h>
#include <SPI.h>

#define IDLESTATE 0
#define CHARGING 1
#define DISCHARGING 2
#define WORKING 3
#define CAN_ADDRESS 0x01

// the cs pin of the version v1.4 is default to D10
const int SPI_CS_PIN = 53;
MCP_CAN CAN(SPI_CS_PIN);

char data1, data2;   // for incoming serial data
char state = 0;
int n = 100, i = 0, j = 0;
unsigned int value[40];
unsigned char data[80];

unsigned int  totalVoltage;
unsigned int  totalCurrent;
unsigned int  remainingCapacity;
unsigned int  temperature1;
unsigned int  temperature2;
unsigned int  temperature3;
unsigned int  temperature4;
unsigned int  capacityPercentage;
unsigned int  MOSstatus;


void setup() {
  wdt_disable();
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial1.begin(9600); 
  CANsetup();
  delay(100);
  TimerSetup();
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();

  if(Serial1.available()){
    data[i] = Serial1.read();
    if ( i%2 == 1) {
      value[j] = data[i-1];
      value[j] = (value[j]<<8) | data[i];
      j++;
    }
    i++;
    if (i == 26) i++;
  }

  
}

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  TCNT1 = 0;
  ProcessData();
  DeleteVariables();
  ReadBasicInformation();
  state = 1;
}

void ProcessData(void){
  for (int k = 0; k < 30; k++) {
     totalVoltage = readIntToDec(value[2]);
     totalCurrent = readIntToDec(value[3]);
     remainingCapacity = readIntToDec(value[4])*10;
     temperature1 = readIntToDec(value[14])-2731;
     temperature2 = readIntToDec(value[15])-2731;
     temperature3 = readIntToDec(value[16])-2731;
     temperature4 = readIntToDec(value[17])-2731;
  } 
  capacityPercentage = readByteToDec(data[23]);
  MOSstatus = readState(readByteToDec(data[24]));
  
  if ( state != 0){
    Serial.print(String(totalVoltage));Serial.print("\t");
    Serial.print(String(totalCurrent));Serial.print("\t");
    Serial.print(String(remainingCapacity));Serial.print("\t");
    Serial.print(String(capacityPercentage));Serial.print("\t");
    Serial.print(String(temperature1));Serial.print("\t");
    Serial.print(String(temperature2));Serial.print("\t");
    Serial.print(String(temperature3));Serial.print("\t");
    Serial.print(String(temperature4));Serial.print("\t");
    Serial.print(String(MOSstatus));Serial.print("\n");

    unsigned char buff1[8],buff2[8],buff3[8],buff4[8],buff5[8],buff6[8],buff7[8],buff8[8],buff9[8];

    String StotalVoltage = "V" + String(totalVoltage) + "E";
    String StotalCurrent = "I" + String(totalCurrent) + "E";
    String SremainingCapacity = "C" + String(remainingCapacity) + "E";
    String ScapacityPercentage = "P" + String(capacityPercentage) + "E";
    String Stemperature1 = "T" + String(temperature1) + "E";
    String Stemperature2 = "U" + String(temperature2) + "E";
    String Stemperature3 = "W" + String(temperature3) + "E";
    String Stemperature4 = "X" + String(temperature4) + "E";
    String SMOSstatus = "S" + String(MOSstatus) + "E";
    
    StotalVoltage.toCharArray(buff1,8);
    StotalCurrent.toCharArray(buff2,8);
    SremainingCapacity.toCharArray(buff3,8);
    ScapacityPercentage.toCharArray(buff4,8);
    Stemperature1.toCharArray(buff5,8);
    Stemperature2.toCharArray(buff6,8);
    Stemperature3.toCharArray(buff7,8);
    Stemperature4.toCharArray(buff8,8);
    SMOSstatus.toCharArray(buff9,8);

    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff1);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff2);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff3);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff4);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff5);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff6);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff7);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff8);
    CAN.sendMsgBuf(CAN_ADDRESS, 0, 8, buff9);   
  }
}

void CANsetup (void){
  START_INIT:
  
  if(CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
  {
      Serial.println("CAN BUS Shield init ok!");
  }
  else
  {
      Serial.println("CAN BUS Shield init fail");
      Serial.println("Init CAN BUS Shield again");
      delay(100);
      goto START_INIT;
  }
}

unsigned int readState( char data ){
  if (data == 0x01){
    return CHARGING;
  }
  if (data == 0x02){
    return DISCHARGING;
  }
  if (data == 0x03){
    return WORKING;
  }
  return IDLESTATE;
}

int readByteToDec ( char hexVari){
  int hexDec = (hexVari&0x80) + (hexVari&0x40) + (hexVari&0x20) + (hexVari&0x10) + (hexVari&0x08) + (hexVari&0x04) + (hexVari&0x02) + (hexVari&0x01);
  return hexDec;
}

unsigned int readIntToDec ( int hexVari){
  unsigned int hexDec = (hexVari&0x8000) + (hexVari&0x4000) + (hexVari&0x2000) + (hexVari&0x1000) + (hexVari&0x0800) + (hexVari&0x0400) + (hexVari&0x0200) + (hexVari&0x0100) + (hexVari&0x80) + (hexVari&0x40) + (hexVari&0x20) + (hexVari&0x10) + (hexVari&0x08) + (hexVari&0x04) + (hexVari&0x02) + (hexVari&0x01);
  return hexDec;
}


void DeleteVariables(void){
  i = 0;
  j = 0;
  for (int k = 0; k < 80; k++) {
     data[k] = '\0';
  }
  for (int k = 0; k < 40; k++) {
     value[k] = '\0';
  }
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


