#ifndef EXTER_INT_C
#define EXTER_INT_C
#include "avr_cmsis.h"
//***********************************************************************
// Настройка события прерывания
void INT0_SETTING(int_set0_t set)
{
mcucr->isc0=set;
}

void INT1_SETTING(int_set0_t set)
{
mcucr->isc1=set;
}

void INT2_SETTING(int_set2_t set)
{
mcucsr->isc2=set;
}
//************************************************************************
#endif
