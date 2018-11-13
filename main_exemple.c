#include <osa.h>
#include <atmega16_32.h>
#include <avr/boot.h>
// 1. Описываем исполняемые функции 
void pid(uint8_t argc,char* argv[])
{
PORTA=0xFF;
}

void adc(uint8_t argc,char* argv[])
{
char str[30];
int16_t adc;
adc=ADC_DIFF_INPUT_BIT9(ADC1_ADC0_10x);
//adc=ADC_SINGLE_BIT10(ADC1);
sprintf(str,"adc=%d\r\n",adc);
printStr(str);
}

void shim(uint8_t argc,char* argv[])
{

if (argc==1) 
	{
	//str_to_uint16(argv[0]);
	TIMER16_1_PWM_OCR1A(str_to_uint16(argv[0]), // значение регистра СРАВНЕНИЯ	*
	OFF, // настройка ПРЕРЫВАНИЯ			*
	OC0_pwm_clear); // настройка режима ВЫВОДОВ ОС1A	*
	}
}
//--------------------------------------------------------
// 2. Заполняем количество функций для поиска
prog_uint8_t CmdFunctionNumber=3;
//----------------------------------------------------------
// 3. Записываем указатели на функции в ПЗУ
void (*sys_func[])(uint8_t argc,char* argv[]) PROGMEM = 
{
    pid,
	adc,
	shim
};
//----------------------------------------------------------------
// 4. Описываем эквивалентные названия фукнций для командной строки
char funcname1[] PROGMEM = {"pid"};
char funcname2[] PROGMEM = {"adc"};
char funcname3[] PROGMEM = {"shim"};
//--------------------------------------------------------------
// 5. Записываем названия функций в соответствии с указтелем на функцию 
char *sys_func_names[] PROGMEM = 
{
    funcname1,
    funcname2,
	funcname3
};
//---------------------------------------------------------------




ISR(TIMER0_COMP_vect)
{
OS_Timer();
}
void init_perif(void)
{
TIMER8_0_CTC_OCR0(124, // значение регистра перезагрузки	*
		 ON); // состояние прерывний			*
TIMER8_0_RUN(TIMER_CLK_DIV64);
//----------------------
UsartInit();
cmdlineInit(); // инициализация командной строки
UartSetRxHandler(cmdline_input); // добавлем фукнцию для приема символов
printInit(UartSendChar);						//	*
//------------------------
ADC_SWITCH(ON);
adc_init(adc_div128, // предделитель для АЦП
			  source_AVcc);	   // выбор опорного напряжения
//-----------------------
TIMER16_1_FAST_PWM_ICR1(0xFFFF, // значение рег-ра перез-ки	*
		 OFF); // состояние прерывний			*
TIMER16_1_RUN(TIMER_CLK_DIV1);

}

int16_t setPoint_my=400;
pidData_t*  pid_my;
uint16_t tnt=0;
void pid_loop(void)
{
pid_Init(217, 25376, 16, pid_my);
int16_t adc;
while(1)
{
OS_Delay(1000);
adc=ADC_DIFF_INPUT_BIT9(ADC1_ADC0_10x);
tnt=tnt+pid_Controller(setPoint_my, adc, pid_my);
TIMER16_1_PWM_OCR1A(tnt, // значение регистра СРАВНЕНИЯ	*
	OFF, // настройка ПРЕРЫВАНИЯ			*
	OC0_pwm_clear); // настройка режима ВЫВОДОВ ОС1A	*
}

}

int main(void)
{
OS_Init();
init_perif();
OS_EI ();

OS_Task_Create (1, cmdline_loop); // помещаем фукнцию в перечень задач
OS_Task_Create (1, pid_loop); // помещаем фукнцию в перечень задач
OS_Run();

}
