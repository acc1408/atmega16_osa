#ifndef DRIVER_H
#define DRIVER_H
#include <atmega16_32.h>	// ����������� ���������

//----------------------------------------------------------
// ������� ��� ���
#ifdef ADC_Enable
#include <driver_h\adc.h>	
#endif
//----------------------------------------------------------
// ������� ��� ����������
#ifdef COMP_Enable
#include <driver_h\comparator.h>
#endif
//----------------------------------------------------------
// �������� ������� ����������
#ifdef EXT_INTER_Enable
#include <driver_h\exter_int.h> 
#endif
//----------------------------------------------------------
// �������� SPI
#ifdef SPI_Enable
#include <driver_h\spi.h>
#endif
//----------------------------------------------------------
// �������� TIMER8_0
#ifdef TIMER8_0_Enable
#include <driver_h\timer8_0.h>
#endif
//----------------------------------------------------------
// �������� TIMER16_1
#ifdef TIMER16_1_Enable
#include <driver_h\timer16_1.h>
#endif
//----------------------------------------------------------
// �������� TIMER8_2
#ifdef TIMER8_2_Enable
#include <driver_h\timer8_2.h>
#endif
//----------------------------------------------------------
// �������� TWI
#ifdef TWI_Enable
#include <driver_h\i2c.h>
#endif
//----------------------------------------------------------
// �������� USART
#ifdef USART_Enable
#include <driver_h\usart.h>
#endif
//----------------------------------------------------------
// �������� PRINT
#ifdef PRINT_Enable
#include <driver_h\print.h>
#endif
//----------------------------------------------------------
// �������� CMDline
#ifdef CMDLINE_Enable
#include <driver_h\cmdline.h>
#endif

//==========================================================
//----------------------------------------------------------
// ����� � ����������� �����
#ifdef Sdvig_Enable
#include <driver_h/sdvig.h>
#endif
//----------------------------------------------------------
// CRC8_Flash
#ifdef Sdvig_Enable
#include <driver_h/crc8.h>
#endif
//----------------------------------------------------------
// ��� ����������
#ifdef PID_controller
#include <driver_h/pid.h>
#endif
//==============================================================
// ������� ���������
//--------------------------------------------------------------
// �������� LCD_I2C
#ifdef LCD_I2C_Enable
#include <driver_h\lcd2004_i2c.h>
#endif
//--------------------------------------------------------------
// �������� DS18B20
#ifdef DS18B20_Enable
#include <driver_h/ds18b20_t.h>
#endif
//--------------------------------------------------------------
//--------------------------------------------------------------
//#include <driver_h/sdvig.h>  // ����� � ����������� �����
//#include <driver_h/ds18b20_t.h>  // ����� � ����������� �����
//#include <driver_h/crc8.h>  // ����� � ����������� �����

//#include <math.h> // ���������� ����������� libm.a 
//#include <util/delay.h> // �������� �����������
//#include <driver_h\timer8.h>  // ������ 8_0(2)
//-----------------------------------------------------
//  // ������ 8_0
//#include <driver_h\timer16_1.h> // ������ 16_1
//#include <driver_h\timer8_2.h>	// ������ 8_2
	
//#include <driver_h\exter_int.h> // ������� ����������
//// ����������
//#include <driver_h\spi.h>		// SPI �������
//#include <driver_h\i2c.h>		// TWI �������
//#include <driver_h\lcd2004_i2c.h>		// TWI �������
//#include <driver_h\max31855.h>		// TWI �������
//#include <driver_h\max6675.h>		// TWI �������
#endif
