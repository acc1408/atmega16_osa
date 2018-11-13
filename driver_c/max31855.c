#ifndef MAX31855_C
#define MAX31855_C
#include "avr_cmsis.h"
#include <driver_c\spi.c>
//----------------------------------------------

void init_max31855(void)
{
SpiSet_t spi;
spi.div=div_8;
spi.dord=MSB;
spi.cpol=idle_low;
spi.cpha=read_first;
//------------------
init_spi(spi);
MAX31855[0]=12;
MAX31855[1]=100;
MAX31855[2]=200;
MAX31855[0]=125;
}
//==================================================
void format_max31855(max31855_t *rez, uint8_t *a)
{
uint16hl_t temp;
temp.H=*a;
temp.L=*(a+1);
rez->fault=temp.L&0b1;
rez->coup_dec=((temp.L>>2)&0b11)*25;
temp.HL>>=4;
if(temp.H&0x08) temp.H|=0xF0;
rez->coup_int=temp.HL;
//------------------------------
temp.H=*(a+2);
temp.L=*(a+3);
rez->no_connect=temp.L&0b1;
rez->short_GND=(temp.L>>1)&0b1;
rez->short_Vcc=(temp.L>>2)&0b1;
rez->temp_int=temp.H;
temp.HL>>=4;
temp.L&=0x0F;
uint8_t temp2;
temp2=temp.L;
temp.HL=temp2*625;
rez->temp_dec=temp.H;
}
//---------------------------------------------


//---------------------------------------------
#endif
