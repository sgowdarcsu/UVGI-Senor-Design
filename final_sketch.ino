#include <Wire.h>
#include "Adafruit_Keypad.h"
#include <DS1307.h>
#include <Nextion.h>
#include <Adafruit_I2CDevice.h>
#include "Filter.h"

#define I2C_ADDRESS 0x4D
#define KEYPAD_PID1332
#define R1    2
#define C1    8
#define C2    9
#define C3    10
#define C4    11
Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(I2C_ADDRESS);
ExponentialFilter<long> LS1Filter(7, 1);

const int switch1 = 40;
const int switch2 = 41;
int relay1 = 50;
int relay2 = 51;
int state1;
int state2;

int CurrentPage = 0;
int Progress = 0;
int Timecount = 0;
int rtc[7];
char* str[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
uint16_t raw_data;
float voltage;
float power;
char demo_text[ 20 ];

//NexProgressBar j0 = NexProgressBar(5, 1, "j0");
//NexText t3 = NexText(5, 8, "t3");
NexButton b0 = NexButton(6, 2, "b0");
NexButton b8 = NexButton(5, 3, "b8");
NexButton b1 = NexButton(6, 3, "b1");
NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexPage page3 = NexPage(3, 0, "page3");
NexPage page4 = NexPage(4, 0, "page4");
NexPage page5 = NexPage(5, 0, "page5");
NexPage page6 = NexPage(6, 0, "page6");

NexTouch *nex_listen_list[] =
{
  // &j0,
  //  &t3,

  &b0,
  &b8,
  &b1,
  &page0,  // Page added as a touch event
  &page1,
  &page2,
  &page3,
  &page4,
  &page5,
  &page6,
  NULL  // String terminated
};


void page0PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 0;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}

void page1PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 1;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}

void page2PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 2;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}

void page3PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 3;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}

void page4PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 4;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}

void page5PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 5;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}

void page6PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 6;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}

void b0PopCallback(void *ptr)
{
  state1 = digitalRead(switch1);
  state2 = digitalRead(switch2);

  if (state1 == LOW && state2 == LOW) {
    //Serial.println("Latched");
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
  }
  else {
    //Serial.println("Unlatched");
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  }

}

void b8PopCallback(void *ptr)
{
  //Serial.print("yello");
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}

void b1PopCallback(void *ptr)
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}


#include "keypad_config.h"
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {

  Serial2.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  customKeypad.begin();
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  page0.attachPush(page0PushCallback);
  page1.attachPush(page1PushCallback);
  page2.attachPush(page2PushCallback);
  page3.attachPush(page3PushCallback);
  page4.attachPush(page4PushCallback);
  page5.attachPush(page5PushCallback);
  page6.attachPush(page6PushCallback);
  b0.attachPop(b0PopCallback);
  b8.attachPop(b8PopCallback);
  b1.attachPop(b1PopCallback);

  if (!i2c_dev.begin()) {
    while (1);
  }
}

void loop() {

  state1 = digitalRead(switch1);
  state2 = digitalRead(switch2);

  if (state1 != LOW && state2 != LOW) {
    //Serial.println("Latched");
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  }

  delay(100);

  Serial2.print("np.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial2.print(CurrentPage);  // This is the value you want to send to that object and atribute mentioned before.
  Serial2.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial2.write(0xff);
  Serial2.write(0xff);

  if (CurrentPage == 0) {
    showtime();

    String command0 = "mon.txt=\"" + String(rtc[5]) + " /" + "\"";
    Serial2.print(command0);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    String command1 = "day.txt=\"" + String(rtc[4]) + " /" + "\"";
    Serial2.print(command1);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    String command2 = "year.txt=\"" + String(rtc[6]) + "\"";
    Serial2.print(command2);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    String command3 = "hour.txt=\"" + String(rtc[2]) + "\"";
    Serial2.print(command3);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    String command4 = "week.txt=\"" + String(str[rtc[3] - 1]) + "\"";
    Serial2.print(command4);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    String command5 = "minute.txt=\"" + String(rtc[1]) + "\"";
    Serial2.print(command5);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    String command6 = "sec.txt=\"" + String(rtc[0]) + "\"";
    Serial2.print(command6);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);

    customKeypad.tick();
    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      Serial.print((char)e.bit.KEY);
      if (String((char)e.bit.KEY) == "1") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page1.show();
      }
      if (String((char)e.bit.KEY) == "2") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page2.show();
      }
      if (String((char)e.bit.KEY) == "3") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page3.show();
      }
      if (String((char)e.bit.KEY) == "4") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page4.show();
      }
    }

  }

  if (CurrentPage == 1) {

    customKeypad.tick();
    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      Serial.print((char)e.bit.KEY);
      if (String((char)e.bit.KEY) == "1" ) {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page6.show();
      }
      if (String((char)e.bit.KEY) == "2") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page6.show();
      }
      if (String((char)e.bit.KEY) == "3") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page6.show();
      }
      if (String((char)e.bit.KEY) == "4" ) {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page0.show();
      }

    }

  }

  if (CurrentPage == 2) {
    customKeypad.tick();
    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      Serial.print((char)e.bit.KEY);
      if (String((char)e.bit.KEY) == "1" ) {
        if (e.bit.EVENT == KEY_JUST_PRESSED) ;
      }
      if (String((char)e.bit.KEY) == "2") {
        if (e.bit.EVENT == KEY_JUST_PRESSED);
      }
      if (String((char)e.bit.KEY) == "3") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) {
          page5.show();
          state1 = digitalRead(switch1);
          state2 = digitalRead(switch2);

          if (state1 == LOW && state2 == LOW) {
            //Serial.println("Latched");
            digitalWrite(relay1, HIGH);
            digitalWrite(relay2, HIGH);
          }
          else {
            //Serial.println("Unlatched");
            digitalWrite(relay1, LOW);
            digitalWrite(relay2, LOW);
          }
        }
      }
      if (String((char)e.bit.KEY) == "4") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page0.show();
      }
    }

  }

  if (CurrentPage == 3) {
    customKeypad.tick();
    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      Serial.print((char)e.bit.KEY);
      if (String((char)e.bit.KEY) == "3") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page0.show();
      }
    }

  }

  if (CurrentPage == 4) {
    customKeypad.tick();
    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      Serial.print((char)e.bit.KEY);
      if (String((char)e.bit.KEY) == "1") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) page0.show();
      }
    }

  }

  if (CurrentPage == 5) {

    //    Timecount++;
    //    //delay(10);
    //    if (Timecount < 101) {
    //      Progress = map(Timecount, 0, 100, 0, 100);
    //      j0.setValue(Progress);
    //      Serial2.print("t3.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    //      Serial2.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
    //      Serial2.print(Progress);  // This is the text you want to send to that object and atribute mentioned before.
    //      Serial2.print(" s");
    //      Serial2.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
    //      Serial2.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    //      Serial2.write(0xff);
    //      Serial2.write(0xff);
    //    }
    //    if (Timecount == 101) {
    //      Serial2.print("t3.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    //      Serial2.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
    //      Serial2.print("Done!!!");
    //      Serial2.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
    //      Serial2.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    //      Serial2.write(0xff);
    //      Serial2.write(0xff);
    //    }
    //    if (Timecount > 100) {
    //
    //    }

    customKeypad.tick();
    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      Serial.print((char)e.bit.KEY);
      if (String((char)e.bit.KEY) == "1") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) {
          page0.show();
          digitalWrite(relay1, LOW);
          digitalWrite(relay2, LOW);
        }
      }
    }

    raw_data = uvc_read_raw_data();
    voltage = uvc_get_voltage();
    power = uvc_calculate_power( voltage );
    LS1Filter.Filter(power);

    //String command7 = "irr.txt=\"" + String(LS1Filter.Current()) + " [mW/cm2]" + "\"";
    String command7 = "irr.txt=\"" + String(power) + " [mW/cm2]" + "\"";
    //Serial2.println(command7);
    Serial2.print(command7);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);

  }

  if (CurrentPage == 6) {
    customKeypad.tick();
    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      Serial.print((char)e.bit.KEY);
      if (String((char)e.bit.KEY) == "1") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) {
          page5.show();

          state1 = digitalRead(switch1);
          state2 = digitalRead(switch2);

          if (state1 == LOW && state2 == LOW) {
            //Serial.println("Latched");
            digitalWrite(relay1, HIGH);
            digitalWrite(relay2, HIGH);
          }
          else {
            //Serial.println("Unlatched");
            digitalWrite(relay1, LOW);
            digitalWrite(relay2, LOW);
          }
        }
      }
      if (String((char)e.bit.KEY) == "2") {
        if (e.bit.EVENT == KEY_JUST_PRESSED) {
          page0.show();
          digitalWrite(relay1, LOW);
          digitalWrite(relay2, LOW);
        }
      }

    }

  }

  nexLoop(nex_listen_list);

}

void showtime()     //
{
  RTC.get(rtc, true);
}

uint16_t uvc_read_raw_data( )
{
  uint8_t raw_buf[ 2 ];
  uint16_t raw_data;

  //hal_i2c_start( );
  //hal_i2c_read( dev_slave_addr, raw_buf, 2, END_MODE_STOP );
  i2c_dev.read(raw_buf, 2);

  raw_data = raw_buf[ 0 ];
  raw_data <<= 8;
  raw_data |= raw_buf[ 1 ];

  return ( raw_data & 0x0FFF );
}

float uvc_get_voltage( void )
{
  uint16_t raw_data;
  float voltage;

  raw_data =  uvc_read_raw_data( );
  voltage = ( raw_data * 3300 ) / 4096.0;

  return voltage;
}

float uvc_calculate_power( float voltage )
{
  if ( voltage > 0 )
  {
    return voltage / 355.0;
  }
  return 0;
}
