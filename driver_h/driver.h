#ifndef DRIVER_H
#define DRIVER_H
#include <atmega16_32.h>	// подключение библиотек

//----------------------------------------------------------
// драйвер для АЦП
#ifdef ADC_Enable
#include <driver_h\adc.h>	
#endif
//----------------------------------------------------------
// драйвер для Компаратор
#ifdef COMP_Enable
#include <driver_h\comparator.h>
#endif
//----------------------------------------------------------
// Включить ВНЕШНИЕ ПРЕРЫВАНИЯ
#ifdef EXT_INTER_Enable
#include <driver_h\exter_int.h> 
#endif
//----------------------------------------------------------
// Включить SPI
#ifdef SPI_Enable
#include <driver_h\spi.h>
#endif
//----------------------------------------------------------
// Включить TIMER8_0
#ifdef TIMER8_0_Enable
#include <driver_h\timer8_0.h>
#endif
//----------------------------------------------------------
// Включить TIMER16_1
#ifdef TIMER16_1_Enable
#include <driver_h\timer16_1.h>
#endif
//----------------------------------------------------------
// Включить TIMER8_2
#ifdef TIMER8_2_Enable
#include <driver_h\timer8_2.h>
#endif
//----------------------------------------------------------
// Включить TWI
#ifdef TWI_Enable
#include <driver_h\i2c.h>
#endif
//----------------------------------------------------------
// Включить USART
#ifdef USART_Enable
#include <driver_h\usart.h>
#endif
//----------------------------------------------------------
// Включить PRINT
#ifdef PRINT_Enable
#include <driver_h\print.h>
#endif
//----------------------------------------------------------
// Включить CMDline
#ifdef CMDLINE_Enable
#include <driver_h\cmdline.h>
#endif

//==========================================================
//----------------------------------------------------------
// Сдвиг с сохранением знака
#ifdef Sdvig_Enable
#include <driver_h/sdvig.h>
#endif
//----------------------------------------------------------
// CRC8_Flash
#ifdef Sdvig_Enable
#include <driver_h/crc8.h>
#endif
//----------------------------------------------------------
// ПИД контроллер
#ifdef PID_controller
#include <driver_h/pid.h>
#endif
//==============================================================
// Внешняя периферия
//--------------------------------------------------------------
// Включить LCD_I2C
#ifdef LCD_I2C_Enable
#include <driver_h\lcd2004_i2c.h>
#endif
//--------------------------------------------------------------
// Включить DS18B20
#ifdef DS18B20_Enable
#include <driver_h/ds18b20_t.h>
#endif
//--------------------------------------------------------------
//--------------------------------------------------------------
//#include <driver_h/sdvig.h>  // сдвиг с сохранением знака
//#include <driver_h/ds18b20_t.h>  // сдвиг с сохранением знака
//#include <driver_h/crc8.h>  // сдвиг с сохранением знака

//#include <math.h> // необходимо подключение libm.a 
//#include <util/delay.h> // задержки программные
//#include <driver_h\timer8.h>  // таймер 8_0(2)
//-----------------------------------------------------
//  // Таймер 8_0
//#include <driver_h\timer16_1.h> // Таймер 16_1
//#include <driver_h\timer8_2.h>	// Таймер 8_2
	
//#include <driver_h\exter_int.h> // Внешние прерывания
//// Компаратор
//#include <driver_h\spi.h>		// SPI драйвер
//#include <driver_h\i2c.h>		// TWI драйвер
//#include <driver_h\lcd2004_i2c.h>		// TWI драйвер
//#include <driver_h\max31855.h>		// TWI драйвер
//#include <driver_h\max6675.h>		// TWI драйвер
#endif
