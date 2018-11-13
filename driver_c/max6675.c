#ifndef MAX6675_C
#define MAX6675_C
#include <avr_cmsis.h>
#include <driver_c\spi.c>
#include <driver_h\max6675.h>
//*******************************************************************
//		Настройка модуля SPI										*
//------------------------------------------------------------------*
void init_max6675(void)
{
SpiSet_t spi;
spi.dord=MSB;
spi.cpol=idle_low;
spi.cpha=read_second;
spi.div=div_8;
init_spi(spi);
}
//------------------------------------------------------------------*
uint8_t read_max6675(uint16_t *temper)
{

uint8_t a[2];
SPI_Array(a, a, 2);
uint16hl_t temp;
temp.H=a[0];
temp.L=a[1];
temp.HL>>=3;
*temper=temp.HL;
//
a[1]>>=2;
a[1]=~a[1];
return ((a[1])&0x1);
}
//------------------------------------------------------------------*
uint16_t temp6675_i(uint16_t temper)
{
return  temper>>1;
}
//------------------------------------------------------------------*
double temp6675_f(uint16_t temper)
{
return  temper*0.25;
}
//----------------------
//*******************************************************************
#endif
