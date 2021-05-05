#include <Wire.h>
#include <DS1307.h>

int rtc[7];
char* str[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};


void setup()
{ Serial.begin(9600);
  Wire.begin();

}

void loop()
{
  showtime();
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
  delay(1000);
}


void showtime()     //
{
  RTC.get(rtc, true);
}
