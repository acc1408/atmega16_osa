#ifndef CMDLINE_C
#define CMDLINE_C
#include <atmega16_32.h>
//--------------------------------------------
static uint8_t argc=0; //кол-во аргументов
char* argv[SHELL_ARGS_MAX]; // массив для хранения указателей для аргументов функций
//uint8_t Func_Num; // кол-во функций для поиска в базе данных
//------------------------------------------
OST_FLAG      cmd_flag;
#define CMDstart 1<<0 // флаг запуска обработки команды
#define overflow 1<<1 // флаг переполнения буфера
//-------------------------
char  cmd_rx[Num_RX_cmd]; // кол-во символов 
uint8_t cmd_rx_curr=0;	// индекс следующего символа в массиве

//***********************************************************************
//		Функции для вывода строк из ОЗУ и ПЗУ							*
//----------------------------------------------------------------------*
// Инициализация функции командной строки								*
// Параметр функции кол-во функций для поиска							*
void cmdlineInit(void)												//	*
{																	//	*
OS_Flag_Create (cmd_flag); // Создаем флаг								*
}																	//	*
//----------------------------------------------------------------------*
// Поиск и выполнение функций 											* 
uint8_t cmd_exec(char *cmdline)										//	*
{																	//	*
uint8_t i=0, func_num;
func_num=pgm_read_byte(&CmdFunctionNumber);							//	*
while (!(str_equal_pgm(cmdline,(char *)pgm_read_word(&(sys_func_names[i])))))
    {																//	*
        i++;														//	*
        if (i>=func_num)											//	*
        {															//	*
            return 0;		// ни одная из функций не найдена			*
        }															//	*
    }																//	*
((void (*)(uint8_t,char**))pgm_read_word(&(sys_func[i])))(argc,argv);
return 1;															//	*
}																	//	*
//----------------------------------------------------------------------*
// Ввод данных в буфер массива											*
void cmdline_input(char c)
{
// Если флаг старта обработки данных и флаг переполнения буфера сброшен
if (OS_Flag_Check_AllOff_I(cmd_flag,CMDstart||overflow)) // 
	{													//   
	
	switch(c)
		{
		case '\r':	OS_Flag_Set_I(cmd_flag,CMDstart);
					cmd_rx[cmd_rx_curr]='\0';
					if (!*argv[argc-1]) // если значение указателя имеет символ '\0'
						argc=argc-1;// то это не аргумент функции
					return;					 
					break;
		case ' ':	if (cmd_rx_curr==0) // если первый символ провел,
						return; // то пропустить
					if (cmd_rx[cmd_rx_curr-1]=='\0') // если 2 пробела,
						return; // то пропустить	
					cmd_rx[cmd_rx_curr++]='\0';
					if (argc<SHELL_ARGS_MAX) 
						argv[argc++]=&(cmd_rx[cmd_rx_curr]);
		case '\n':	
					break;
		default:	cmd_rx[cmd_rx_curr++]=c;	
					break;
		}
	// если остался 1 символ,	
	if (cmd_rx_curr==(Num_RX_cmd-1)) // проверяем место в буфере
		{
		// то ставим флаг переполнения буфера
		OS_Flag_Set_I(cmd_flag,overflow); 
		return;
		}
	}
// если сивмол ввода даннных и флаг переполнения установлен, то
if ((c=='\r')&&(OS_Flag_Check_On_I(cmd_flag,overflow))) 
		{	// высталяем флаг обработки данных
			OS_Flag_Set_I(cmd_flag,CMDstart);// высталяем флаг обработки данных 
			cmd_rx[cmd_rx_curr]='\0';// ставим знак конца строки
			if (!*argv[argc-1]) argc=argc-1;
		}
}

//-----------------------------------------------------------------

void cmdline_loop(void)
{
printfProgStrM("CMD>>");
while(1)
{
OS_Flag_Wait_On(cmd_flag,CMDstart);
if (!(cmd_exec(cmd_rx))) 	// поиск описанных функций
	{  //если имя функции не найдено вывести ее имя
	  	printfProgStrM("Function not found\r\n");
	}	
cmd_rx_curr=0;
argc=0;
OS_Flag_Clear(cmd_flag,CMDstart||overflow);
printfProgStrM("CMD>>");
}
}
//***********************************************************************

#endif
