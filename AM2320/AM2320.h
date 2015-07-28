#ifndef AM2320_H
#define AM2320_H

#include <Arduino.h>

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

class AM2320
{
	public:
		AM2320();
		float getTemperature(void);
		float getHumidity(void);
		bool CRCCheck(void);
	protected:
		uint8_t data_buffer[8];
		void read_sensor_readings(void); 
	private:
		int temperature;
		unsigned int humidity;
		unsigned long CRC16(uint8_t*, uint8_t);

};

#endif
