#ifndef SPI_C
#define SPI_C
#include "avr_cmsis.h"
//***********************************************************************
//-------------------------------------------------------------
void init_spi(SpiSet_t set)
{
DDR_SPI |= (1<<MOSI)|(1<<SCK)|(1<<SS);
DDR_SPI &=~(1<<MISO);
PORT_SPI |=(1<<MOSI)|(1<<SCK)|(1<<SS)|(1<<MISO);
spcr->dord=set.dord;
spcr->cpol=set.cpol;
spcr->cpha=set.cpha;
spcr->spr= set.div;
spsr->spi2x=(set.div>>2);
spcr->mstr=1;
spcr->spe=1;
}
//------------------------------------------------------------
uint8_t SPI_Byte(uint8_t data)
{
   uint8_t report;
   #ifdef def_SS
   PORT_SPI &= ~(1<<SS);
   #endif
   SPDR = data;
   while(spsr->spif==0);
   report = SPDR;
   #ifdef def_SS
   PORT_SPI |= (1<<SS); 
   #endif
   return report; 
}
//------------------------------------------------------------
void SPI_Array(uint8_t *input, uint8_t *output, uint8_t num)
{
   #ifdef def_SS
   PORT_SPI &= ~(1<<SS);
   #endif 
   while(num--){
      SPDR = *input;
      while(spsr->spif==0);
	  *output=SPDR;
	  input++;
	  output++;
   }
   #ifdef def_SS
   PORT_SPI |= (1<<SS);
   #endif 
}
//------------------------------------------------------------
#endif
