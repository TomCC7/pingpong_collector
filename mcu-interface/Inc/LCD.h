
/** Put this in the src folder **/
#ifndef LCD_HEADER
#define LCD_HEADER

#include "stm32f4xx_hal.h"

class LCD
{
protected:
	I2C_HandleTypeDef &i2c;
	uint8_t address;

public:
	static constexpr uint8_t defaultSlaveAddress = 0x4E;
	static constexpr uint8_t enableHigh = 0x0C;
	static constexpr uint8_t enableLow = 0x08;
	static constexpr bool DATA = true;
	static constexpr bool INSTRUCTION = false;
	static constexpr uint8_t commandDefault = 0x80;

	LCD(I2C_HandleTypeDef &_i2c, uint8_t _address = defaultSlaveAddress);

	void start(void);
	void send(uint8_t byte, bool type = true);
	void send(const uint8_t *seq, size_t length, bool type = true);
	void setCursor(uint8_t row, uint8_t column);
	void clear(void);
};
#endif
