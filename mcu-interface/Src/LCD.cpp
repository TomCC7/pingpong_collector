#include "LCD.h"

LCD::LCD(I2C_HandleTypeDef &_i2c, uint8_t _address) : i2c(_i2c), address(_address)
{
}

void LCD::send(uint8_t byte, bool type)
{
	uint8_t encoded[4] = {0};
	uint8_t byteHigh = byte & 0xF0;
	uint8_t byteLow = (byte & 0x0F) << 4;
	encoded[0] = byteHigh | enableHigh | (type & 0b1);
	encoded[1] = byteHigh | enableLow | (type & 0b1);
	encoded[2] = byteLow | enableHigh | (type & 0b1);
	encoded[3] = byteLow | enableLow | (type & 0b1);
	HAL_I2C_Master_Transmit(&i2c, address, encoded, 4, HAL_MAX_DELAY);
}

void LCD::send(const uint8_t *seq, size_t length, bool type)
{
	for (size_t i = 0; i < length; i++)
	{
		send(seq[i], type);
	}
}

void LCD::clear(void)
{
	send(commandDefault, INSTRUCTION);
	for (uint8_t i = 0; i < 70; i++)
	{
		send(' ');
	}
}

void LCD::setCursor(uint8_t row, uint8_t column)
{
	uint8_t cursor = commandDefault;
	cursor |= (row << 6) | column;
	send(cursor, INSTRUCTION);
}

void LCD::start(void)
{
	// 4 bit initialization
	HAL_Delay(50); // wait for >40ms
	send(0x30, INSTRUCTION);
	HAL_Delay(5); // wait for >4.1ms
	send(0x30, INSTRUCTION);
	HAL_Delay(1); // wait for >100us
	send(0x30, INSTRUCTION);
	HAL_Delay(10);
	send(0x20, INSTRUCTION); // 4bit mode
	HAL_Delay(10);

	// dislay initialisation
	send(0x28, INSTRUCTION); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	send(0x08, INSTRUCTION); // Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	send(0x01, INSTRUCTION); // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	send(0x06, INSTRUCTION); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	send(0x0C, INSTRUCTION); // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
							 // send(0x08);
}
