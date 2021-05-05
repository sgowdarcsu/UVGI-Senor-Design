#include "Adafruit_Keypad.h"
#include <Wire.h>
#include <DS1307.h>


#define updata_flag 0         //1: updates time; 0: show time
#define TIME_YR    21
#define TIME_MTH   4
#define TIME_DATE  15
#define TIME_DOW   4
#define TIME_HR    15
#define TIME_MIN   52
#define TIME_SEC   30

#define KEYPAD_PID1332
#define R1    2
#define C1    8
#define C2    9
#define C3    10
#define C4    11


const int switch1 = 30;
const int switch2 = 31;
int relay1 = 5;
int relay2 = 6;
int state1;
int state2;
char* str[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"}; //week
int rtc[7];

#include "keypad_config.h"
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  customKeypad.begin();
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  if (updata_flag)
  { Serial.println("OldTime:  ");
    showtime();
    RTC.stop();

    RTC.set(DS1307_SEC, TIME_SEC); //---Update time----
    RTC.set(DS1307_MIN, TIME_MIN);
    RTC.set(DS1307_HR, TIME_HR);
    RTC.set(DS1307_DOW, TIME_DOW);
    RTC.set(DS1307_DATE, TIME_DATE);
    RTC.set(DS1307_MTH, TIME_MTH);
    RTC.set(DS1307_YR, TIME_YR);
    RTC.start();
    Serial.println("SetTime:  ");
    showtime();
  }
}

void loop()
{
  state1 = digitalRead(switch1);
  state2 = digitalRead(switch2);

  if (state1 == LOW && state2 == LOW) {
    Serial.println("Latched");
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
  }
  else {
    Serial.println("Unlatched");
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  }

  customKeypad.tick();

  while (customKeypad.available()) {
    keypadEvent e = customKeypad.read();
    Serial.print((char)e.bit.KEY);
    if (String((char)e.bit.KEY) == "4"){
      if (e.bit.EVENT == KEY_JUST_PRESSED) Serial.println("yes----------------------------");
    }
   // if (e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
   // else if (e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }

  if (updata_flag == 0) {
    showtime();
  }

  delay(1000);
}

void showtime()     //
{
  RTC.get(rtc, true);
  {
    /*YEAR MONTH DATE*/
    Serial.print( rtc[5]);
    Serial.print("/");
    Serial.print( rtc[4]);
    Serial.print("/");
    Serial.println( rtc[6]);

    Serial.print( rtc[2]); /*HOUR  MIN SEC */
    Serial.print(":");
    Serial.print( rtc[1]);
    Serial.print(":");
    Serial.println( rtc[0]);

    Serial.println( str[rtc[3] - 1]); /********WEEK*****/
    Serial.println("**********");
  }
}
