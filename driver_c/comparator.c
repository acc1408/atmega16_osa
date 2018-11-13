#ifndef COMPARATOR_C
#define COMPARATOR_C
#include "avr_cmsis.h"
//***********************************************************************
void AnComp_SETTING_AINO(ain0_t pin)
{
acsr->acbg=pin;
}
//***********************************************************************
void AnComp_AIN1_to_ADC(FuncState_t state,// ��������� ������ ������
						adc_single_t pin) // ����� ������
{
adcsra->aden=0;
sfior->acme=state;
sfior->adts=pin;
}
//***********************************************************************
// ��������� ������� ����������
void AnComp_Setting_ISR(an_comp_isr isr)
{
acsr->acis=isr;
}
//***********************************************************************
#endif
