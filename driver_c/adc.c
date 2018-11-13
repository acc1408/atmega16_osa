#ifndef ADC_C
#define ADC_C
#include <atmega16_32.h>
#include <util/atomic.h>
#include <avr/sleep.h>
//***********************************************************************
static volatile uint8_t adc_conv_end=1; // ���� ���������� ���
//uint16hl_t conv_isr;
#ifdef ADC_REDUCE_FLASH
void adc_convert(void); //
#endif 

//***********************************************************************
// ������������� ���
void adc_init(rate_convert_t rate, // ������������ ��� ���
			  ref_volt_t volt)	   // ����� �������� ����������
{
admux->adlar=1; 	// ������������ �����
admux->refs=volt;	// ��������� �������� ����������
adcsra->adps=rate; 	// ��������� ������������ ��� 
adcsra->adie=1;		// �������� ����������
}

//***********************************************************************
// �������������� ��������� ������� ���
void ADC_SOURCE_START(FuncState_t state,
						adc_start_t start)
{
adcsra->adate=state; // ����������� ��������� �������
sfior->adts=start;	//�������� �������������� ���������
}
//--------------------------------------------
// ���������������� ������
int8_t  ADC_DIFF_INPUT_BIT7(adc_diff_t adc)
{
admux->mux=adc;
#ifndef ADC_REDUCE_FLASH
adc_conv_end=0;
sfior->acme=0;
adcsra->adsc=1;
set_sleep_mode(SLEEP_MODE_ADC); // ��������� ������� ������
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // ����������� �����
{
sleep_enable(); // ���������� ��������� ������� ������
while(adc_conv_end==0) // �������� ��������������
{
sleep_cpu();	// ������� � ����� ��������
}
sleep_disable(); //������ ������� ������
}
#else
adc_convert();
#endif

return ADCH;
}
//--------------------------------------------
int16_t ADC_DIFF_INPUT_BIT9(adc_diff_t adc)
{
admux->mux=adc;

#ifndef ADC_REDUCE_FLASH
adc_conv_end=0;
sfior->acme=0;
adcsra->adsc=1;
set_sleep_mode(SLEEP_MODE_ADC); // ��������� ������� ������
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // ����������� �����
{
sleep_enable(); // ���������� ��������� ������� ������
while(adc_conv_end==0) // �������� ��������������
{
sleep_cpu();	// ������� � ����� ��������
}
sleep_disable(); //������ ������� ������
}
#else
adc_convert();
#endif

uint16hl_t conv;
conv.L=ADCL;
conv.H=ADCH;
conv.HL=(conv.HL>>6);
/*
if ((conv_isr.H&0b10)==0b10) conv_isr.H|=0b11111100;
return conv_isr.HL;
*/
if ((conv.H&0b10)==0b10) conv.H|=0b11111100;
return conv.HL;

}
//--------------------------------------------
// �������������� �����
uint8_t  ADC_SINGLE_BIT8(adc_single_t adc)
{
admux->mux=adc; // ����� ������ ��� ���
#ifndef ADC_REDUCE_FLASH
adc_conv_end=0;
sfior->acme=0; // ��������� ���������� �� ������� ���
adcsra->adsc=1;
set_sleep_mode(SLEEP_MODE_ADC); // ��������� ������� ������
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // ����������� �����
{
sleep_enable(); // ���������� ��������� ������� ������
while(adc_conv_end==0) // �������� ��������������
{
sleep_cpu();	// ������� � ����� ��������
}
sleep_disable(); //������ ������� ������
}
#else
adc_convert();
#endif

return ADCH;
//return conv_isr.H;
}
//--------------------------------------------
uint16_t ADC_SINGLE_BIT10(adc_single_t adc)
{
admux->mux=adc;
#ifndef ADC_REDUCE_FLASH
adc_conv_end=0;
sfior->acme=0;
adcsra->adsc=1;
set_sleep_mode(SLEEP_MODE_ADC); // ��������� ������� ������
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // ����������� �����
{
sleep_enable(); // ���������� ��������� ������� ������
while(adc_conv_end==0) // �������� ��������������
{
sleep_cpu();	// ������� � ����� ��������
}
sleep_disable(); //������ ������� ������
}
#else
adc_convert();
#endif
uint16hl_t conv;
conv.L=ADCL;
conv.H=ADCH;
conv.HL=(conv.HL>>6);
return conv.HL;
//return (conv_isr.HL>>6);
}
//------------------------------------------------------
#ifdef ADC_REDUCE_FLASH
void adc_convert(void)
{
adc_conv_end=0;
sfior->acme=0;
adcsra->adsc=1;
set_sleep_mode(SLEEP_MODE_ADC); // ��������� ������� ������
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // ����������� �����
{
sleep_enable(); // ���������� ��������� ������� ������
while(adc_conv_end==0) // �������� ��������������
{
sleep_cpu();	// ������� � ����� ��������
}
sleep_disable(); //������ ������� ������
}
}
#endif
//------------------------------------------------------
ISR(ADC_vect)
{
/*
conv_isr.L=ADCL;
conv_isr.H=ADCH;
*/
adc_conv_end=1;
}
//---------------------------------------------
#endif
