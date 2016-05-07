//////////////////////////////////////////////////////////////////////////////////////////////////
// \brief  AM2320 temperature sensor and DS3231 RTC demo
// \author Pavel Perina (based on work of other people)

#include <Wire.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
// AM2320 temperature sensor
// based on https://github.com/thakshak/AM2320/commit/e6fb268f1d8580835209285bc15ec56fd60a907a

#define AM2320_address (0xB8 >> 1)
#define I2C_write_cmd 0x00
#define I2C_read_cmd 0x01

#define AM2320_read_sensor_data 0x03
#define AM2320_write_multiple_registers 0x10

#define AM2320_RH_hb 0x00
#define AM2320_RH_lb 0x01
#define AM2320_T_hb 0x02
#define AM2320_T_lb 0x03
#define AM2320_read_count 0x04

/// \brief   Read temperature and humidity from ANSONG AM2320 sensor
/// \warning No CRC / errors checks
void readAM2320(float *temp, float *hum)
{
  uint8_t data_buffer[8] = {};
  int len = 2+ AM2320_read_count +2;// COMMAND + DATA + REGCOUNT + CRCLSB + CRCMSB
  Wire.beginTransmission(AM2320_address);
  //delayMicroseconds(2000);  //>1.5ms
  Wire.endTransmission();
  delayMicroseconds(2000);    //>1.5ms

  // Read Command
  Wire.beginTransmission(AM2320_address);
  Wire.write(AM2320_read_sensor_data);
  Wire.write(AM2320_RH_hb);
  Wire.write(AM2320_read_count);
  Wire.endTransmission();

  // Waiting
  delayMicroseconds(1600);    //>1.5ms

  // Read
  Wire.requestFrom(AM2320_address, len);
  for (int i = 0; i < len; i++) {
    data_buffer[i] = Wire.read();
    delayMicroseconds(1600);  //>1.5ms
  }
  int          _tmp = (data_buffer[4] << 8) + data_buffer[5];
  unsigned int _hum = (data_buffer[2] << 8) + data_buffer[3];
  *temp = _tmp/10.0;
  *hum  = _hum/10.0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// RTC CODE

#define DS3231_I2C_ADDRESS 0x68

/// \brief Get datetime of DS3231 RTC, assumes UTC (same date format as GPX file)
char * readDS3231DateTimeStr()
{
  //                        0123456789012345678
  static char datetime[] = "20YY-MM-DDThh:mm:ssZ\0";
  byte bcd;
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);                              // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);    // request seven bytes of data from DS3231 starting from register 00h
  // second, minute, hour
  bcd = Wire.read() & 0x7f;
  datetime[17] = '0' + (bcd>>4);
  datetime[18] = '0' + (bcd&0x0f);
  bcd = Wire.read();
  datetime[14] = '0' + (bcd>>4);
  datetime[15] = '0' + (bcd&0x0f);
  bcd = Wire.read() & 0x3f;
  datetime[11] = '0' + (bcd>>4);
  datetime[12] = '0' + (bcd&0x0f);
  // day of week (ignored), day, month, year
  bcd = Wire.read();
  bcd = Wire.read();
  datetime[ 8] = '0' + (bcd>>4);
  datetime[ 9] = '0' + (bcd&0x0f);
  bcd = Wire.read();
  datetime[ 5] = '0' + (bcd>>4);
  datetime[ 6] = '0' + (bcd&0x0f);
  bcd = Wire.read();
  datetime[ 2] = '0' + (bcd>>4);
  datetime[ 3] = '0' + (bcd&0x0f);
  return datetime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// PROGRAM

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  Wire.begin();
}


void loop()
{
  float am2320_temp, am2320_hum;
  readAM2320(&am2320_temp, &am2320_hum);
  char *datetime = readDS3231DateTimeStr();
  Serial.print("\"");
  Serial.print(datetime);
  Serial.print("\",");
  Serial.print(am2320_temp);
  Serial.print(",");
  Serial.println(am2320_hum);
  delay(2000);
}

