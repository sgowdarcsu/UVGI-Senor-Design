#include <Adafruit_I2CDevice.h>

#define I2C_ADDRESS 0x4D
Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(I2C_ADDRESS);

#include <Wire.h>



void setup() {
    while (!Serial) {
      delay(10);
    }
    Serial.begin(115200);  // start serial for output
  
    if (!i2c_dev.begin()) {
      Serial.print("Did not find device at 0x");
      Serial.println(i2c_dev.address(), HEX);
      while (1);
    }
    Serial.print("Device found on address 0x");
    Serial.println(i2c_dev.address(), HEX);
  
 // Wire.begin();
 // Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t raw_data;
  float voltage;
  float power;
  char demo_text[ 20 ];

  raw_data = uvc_read_raw_data();
  //IntToStr( raw_data, demo_text );
  //mikrobus_logWrite( "Raw data: ", _LOG_TEXT );
  //mikrobus_logWrite( demo_text, _LOG_LINE );
  Serial.print("Raw data: ");
  Serial.println(raw_data);

  voltage = uvc_get_voltage();
  //FloatToStr( voltage, demo_text );
  //mikrobus_logWrite( "Voltage: ", _LOG_TEXT );
  //mikrobus_logWrite( demo_text, _LOG_TEXT );
  //mikrobus_logWrite( " mV", _LOG_LINE );
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" mV");

  power = uvc_calculate_power( voltage );
  //  FloatToStr( power, demo_text );
  //  mikrobus_logWrite( "Power: ", _LOG_TEXT );
  //  mikrobus_logWrite( demo_text, _LOG_TEXT );
  //  mikrobus_logWrite( " mW/cm2", _LOG_LINE );
  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" mW/cm2");

  Serial.println("-----------------------------------");
  delay( 1500 );


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
