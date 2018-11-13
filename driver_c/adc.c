#ifndef ADC_C
#define ADC_C
#include <atmega16_32.h>
#include <util/atomic.h>
#include <avr/sleep.h>
//***********************************************************************
static volatile uint8_t adc_conv_end=1; // флаг завершения АЦП
//uint16hl_t conv_isr;
#ifdef ADC_REDUCE_FLASH
void adc_convert(void); //
#endif 

//***********************************************************************
// Инициализация АЦП
void adc_init(rate_convert_t rate, // предделитель для АЦП
			  ref_volt_t volt)	   // выбор опорного напряжения
{
admux->adlar=1; 	// Выравнивание влево
admux->refs=volt;	// Настройка опорного напряжения
adcsra->adps=rate; 	// Настройка предделителя АЦП 
adcsra->adie=1;		// Включаем прерывание
}

//***********************************************************************
// Дополнительные источники запуска АЦП
void ADC_SOURCE_START(FuncState_t state,
						adc_start_t start)
{
adcsra->adate=state; // настраиваем источники запуска
sfior->adts=start;	//включаем дополнительные настройки
}
//--------------------------------------------
// Дифференциальные выходы
int8_t  ADC_DIFF_INPUT_BIT7(adc_diff_t adc)
{
admux->mux=adc;
#ifndef ADC_REDUCE_FLASH
adc_conv_end=0;
sfior->acme=0;
adcsra->adsc=1;
set_sleep_mode(SLEEP_MODE_ADC); // Настройка спящего режима
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // Неатомарный режим
{
sleep_enable(); // разрешение включения спящего режима
while(adc_conv_end==0) // ожидание преобразования
{
sleep_cpu();	// перевод в режим ожидания
}
sleep_disable(); //запрет спящего режима
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
set_sleep_mode(SLEEP_MODE_ADC); // Настройка спящего режима
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // Неатомарный режим
{
sleep_enable(); // разрешение включения спящего режима
while(adc_conv_end==0) // ожидание преобразования
{
sleep_cpu();	// перевод в режим ожидания
}
sleep_disable(); //запрет спящего режима
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
// Несимметричный выход
uint8_t  ADC_SINGLE_BIT8(adc_single_t adc)
{
admux->mux=adc; // Выбор канала для АЦП
#ifndef ADC_REDUCE_FLASH
adc_conv_end=0;
sfior->acme=0; // отключаем компаратор от каналов АЦП
adcsra->adsc=1;
set_sleep_mode(SLEEP_MODE_ADC); // Настройка спящего режима
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // Неатомарный режим
{
sleep_enable(); // разрешение включения спящего режима
while(adc_conv_end==0) // ожидание преобразования
{
sleep_cpu();	// перевод в режим ожидания
}
sleep_disable(); //запрет спящего режима
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
set_sleep_mode(SLEEP_MODE_ADC); // Настройка спящего режима
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // Неатомарный режим
{
sleep_enable(); // разрешение включения спящего режима
while(adc_conv_end==0) // ожидание преобразования
{
sleep_cpu();	// перевод в режим ожидания
}
sleep_disable(); //запрет спящего режима
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
set_sleep_mode(SLEEP_MODE_ADC); // Настройка спящего режима
NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) // Неатомарный режим
{
sleep_enable(); // разрешение включения спящего режима
while(adc_conv_end==0) // ожидание преобразования
{
sleep_cpu();	// перевод в режим ожидания
}
sleep_disable(); //запрет спящего режима
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
