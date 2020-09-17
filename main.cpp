#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define MCP4725 0x61
unsigned int val;
byte buffer[3];

LiquidCrystal_I2C lcd0(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

int voltPin = PB1; 
int currPin = PA6; 

const int currsensPin = PA3;
const int voltsensPin = PA2;
void setup()
{
  pinMode(currsensPin , INPUT_ANALOG);
  pinMode(voltsensPin , INPUT_ANALOG);  
  pinMode(voltPin, INPUT_ANALOG);
  pinMode(currPin, INPUT_ANALOG);
  Wire.begin();
  lcd0.begin(20, 4);
  lcd0.backlight();

}

int val0 = 1024;
int val1 = 2048;

float set_voltage = 00.000;
float set_curr = 00.000;
float RawValue = 0;
float real_current  = 0;
float real_curren_offset = 0;
float real_output = 0;
float real_output2 = 00.000;
float real_current_ma = 0000;
volatile int dac_value;

void loop()
{
  analogReadResolution(12);
   int svolt = analogRead(voltPin); 
  int scurr = analogRead(currPin);  
  set_voltage = (3.3/4096)*svolt;
  set_curr =  (3.3/4096)*scurr;
  RawValue = analogRead(currsensPin);
  real_current = RawValue * (3.3 / 4096);
  real_current = real_current - real_curren_offset;
  real_output= analogRead(voltsensPin);
  real_output2 = (3.3/4096)*real_output; 
  
  real_current_ma = real_current; //*1000;   
  dac_value = dac_value
          + (real_current <= set_curr) * (set_voltage > real_output2)
          - (real_current < set_curr) * (set_voltage < real_output2) 
          - (real_current > set_curr);

  buffer[0] = 0b01000000;
  buffer[1] = dac_value >> 4; 
  buffer[2] = dac_value << 4;
  Wire.beginTransmission(MCP4725);
  Wire.write(buffer[0]); 
  Wire.write(buffer[1]); 
  Wire.write(buffer[2]); 
  Wire.endTransmission();
  lcd0.setCursor(0,0);
  lcd0.print("Set:");
  lcd0.setCursor(5,0); 
int set_whole_volts = (int)set_voltage;
int set_millivolts = (int)((set_voltage - set_whole_volts) * 1000);

if (set_whole_volts < 10) {
  lcd0.print("0");
}
  lcd0.print(set_whole_volts);
  lcd0.print(".");
if (set_millivolts < 10) {
  lcd0.print("00");
}
else if (set_millivolts < 100) {
  lcd0.print("0");
}
  lcd0.print(set_millivolts);
  
  lcd0.print("V");

  lcd0.setCursor(13,0);
int set_whole_amps = (int)set_curr;
int set_milliamps = (int)((set_curr - set_whole_amps) * 1000);

if (set_whole_amps < 10) {
  lcd0.print("0");
}
  lcd0.print(set_whole_amps);
  lcd0.print(".");
if (set_milliamps < 10) {
  lcd0.print("00");
}
else if (set_milliamps < 100) {
  lcd0.print("0");
}
  lcd0.print(set_milliamps);
  lcd0.print("A");

  lcd0.setCursor(0,1);
  lcd0.print("Out:");
  lcd0.setCursor(5,1);
int out_whole_volts = (int)real_output2;
int out_millivolts = (int)((real_output2 - out_whole_volts) * 1000);

if (out_whole_volts < 10) {
  lcd0.print("0");
}
  lcd0.print(out_whole_volts);
  lcd0.print(".");
if (out_millivolts < 10) {
  lcd0.print("00");
}
else if (out_millivolts < 100) {
  lcd0.print("0");
}
  lcd0.print(out_millivolts);
  lcd0.print("V");

  lcd0.setCursor(13,1);
int out_whole_amps = (int)real_current;
int out_milliamps = (int)((real_current - out_whole_amps) * 1000);

if (out_whole_amps < 10) {
  lcd0.print("0");
}
  lcd0.print(out_whole_amps);
  lcd0.print(".");
if (out_milliamps < 10) {
  lcd0.print("00");
}
else if (out_milliamps < 100) {
  lcd0.print("0");
}
  lcd0.print(out_milliamps);
  lcd0.print("A");
  lcd0.setCursor(0,2);
  lcd0.print("dac 1: ");
  lcd0.print(svolt);
  lcd0.print(svolt);
  lcd0.setCursor(0,3);
  lcd0.print(" Mode: ");
}
