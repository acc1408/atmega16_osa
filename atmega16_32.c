#ifndef DRIVER_C
#define DRIVER_C
#include <atmega16_32_conf.h>	// подключение библиотек
//----------------------------------------------------------
// драйвер для АЦП
#ifdef ADC_Enable
#include <driver_c\adc.c>	
#endif
//----------------------------------------------------------
// драйвер для Компаратор
#ifdef COMP_Enable
#include <driver_c\comparator.c>
#endif
//----------------------------------------------------------
// Включить ВНЕШНИЕ ПРЕРЫВАНИЯ
#ifdef EXT_INTER_Enable
#include <driver_c\exter_int.c> 
#endif
//----------------------------------------------------------
// Включить SPI
#ifdef SPI_Enable
#include <driver_c\spi.c>
#endif
//----------------------------------------------------------
// Включить TIMER8_0
#ifdef TIMER8_0_Enable
#include <driver_c\timer8_0.c>
#endif
//----------------------------------------------------------
// Включить TIMER16_1
#ifdef TIMER16_1_Enable
#include <driver_c\timer16_1.c>
#endif
//----------------------------------------------------------
// Включить TIMER8_2
#ifdef TIMER8_2_Enable
#include <driver_c\timer8_2.c>
#endif
//----------------------------------------------------------
// Включить TWI
#ifdef TWI_Enable
#include <driver_c\i2c.c>
#endif
//----------------------------------------------------------
// Включить USART
#ifdef USART_Enable
#include <driver_c\usart.c>
#endif
//----------------------------------------------------------
// Включить PRINT
#ifdef PRINT_Enable
#include <driver_c\print.c>
#endif
//----------------------------------------------------------
// Включить CMDline
#ifdef CMDLINE_Enable
#include <driver_c\cmdline.c>
#endif
//==========================================================
//----------------------------------------------------------
// Сдвиг с сохранением знака
#ifdef Sdvig_Enable
#include <driver_c/sdvig.c>
#endif
//----------------------------------------------------------
// CRC8_Flash
#ifdef Sdvig_Enable
#include <driver_c/crc8.c>
#endif
//----------------------------------------------------------
// ПИД контроллер
#ifdef PID_controller
#include <driver_c/pid.c>
#endif
//----------------------------------------------------------

//==============================================================
// Внешняя периферия
//--------------------------------------------------------------
// Включить LCD_I2C
#ifdef LCD_I2C_Enable
#include <driver_c\lcd2004_i2c.c>
#endif
//--------------------------------------------------------------
// Включить DS18B20
#ifdef DS18B20_Enable
#include <driver_c/ds18b20_t.c>
#endif
//--------------------------------------------------------------

// Драйвер для настройки 8 битного таймера
//#include <driver_c/sdvig.c>
//#include <driver_c/ds18b20_t.c>
//#include <driver_c/crc8.c>
//#include <driver_c\timer8.c>
//#include <driver_c\timer8_0.c>
//#include <driver_c\timer16_1.c>
//#include <driver_c\timer8_2.c>
//#include <driver_c\adc.c>
//#include <driver_c\exter_int.c>
//#include <driver_c\comparator.c>
//#include <driver_c\spi.c>
//#include <driver_c\i2c.c>
//#include <driver_c\lcd2004_i2c.c>
//#include <driver_c\max31855.c>
//#include <driver_c\max6675.c>
#endif
