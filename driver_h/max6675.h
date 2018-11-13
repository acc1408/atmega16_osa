#ifndef MAX6675_H
#define MAX6675_H
#include <avr_cmsis.h>
#include <driver_h\spi.h>		// SPI �������
// ��� ������ ������ ���������� ����������� ���������� libm.a
//*******************************************************************
//		��������� ������ MAX6675 ����� ���������� SPI				*
//------------------------------------------------------------------*
// ������������� SPI ��� MAX6675									*
void init_max6675(void);										//	*
//------------------------------------------------------------------*
// ������ ������ �� �����, 											*
// ���� ��������� ����������, �� ��������� 1, ����� 0			//	*
uint8_t read_max6675(uint16_t *temper);							//	*
//------------------------------------------------------------------*
// ������� �������� � ����� �������									*
inline uint16_t temp6675_i(uint16_t temper);					//	*
//------------------------------------------------------------------*
// ������� �������� � ������� �������								*
inline double temp6675_f(uint16_t temper);						//	*
//*******************************************************************
#endif