 /***************************************************
 * Real Time Clock Module
 * ****************************************************
 * How to apply the DS1307 clock module.
 * @author Dongzi
 * @version  V1.0
 * @date  2017-08-21
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.

 * All above must be included in any redistribution
 * ****************************************************/
 #include <Wire.h>
 #include <DS1307.h>
 #define updata_flag 0 //1 updata time,0 only show time
 //Modify the following data
 #define TIME_YR    21
 #define TIME_MTH   4
 #define TIME_DATE  15
 #define TIME_DOW   4
 #define TIME_HR    15
 #define TIME_MIN   52
 #define TIME_SEC   30
 char* str[]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};//week
 int rtc[7];
 void showtime()     //
   {
    RTC.get(rtc,true);
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

    Serial.println( str[rtc[3]-1]); /********WEEK*****/
    Serial.println("**********");
    }
   }
 void setup()
 {  Serial.begin(9600);
   Wire.begin();

   if(updata_flag)
  { Serial.println("OldTime:  ");
   showtime();
    RTC.stop();

    RTC.set(DS1307_SEC,TIME_SEC); //---Update time----
    RTC.set(DS1307_MIN,TIME_MIN);
    RTC.set(DS1307_HR,TIME_HR);
    RTC.set(DS1307_DOW,TIME_DOW);
    RTC.set(DS1307_DATE,TIME_DATE);
    RTC.set(DS1307_MTH,TIME_MTH);
    RTC.set(DS1307_YR,TIME_YR);
    RTC.start();
    Serial.println("SetTime:  ");
    showtime();
   }
 }
 void loop()
 { if(updata_flag==0)
     showtime();    //Output data from serial port
    delay(1000);
 }
