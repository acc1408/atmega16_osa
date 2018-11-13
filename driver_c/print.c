#ifndef PRINT_C
#define PRINT_C
#include <atmega16_32.h>

volatile static VoidFuncChar_t rputchar=0;
//***********************************************************************
//		Фунции для вывода строк из ОЗУ и ПЗУ							*
//----------------------------------------------------------------------*
// Настройка функции для приема данных									*
void printInit(void (*putchar_func)(char c))						//	*
{
rputchar = putchar_func;
}
//----------------------------------------------------------------------*
// Вывод строки из ОЗУ													*
void printStr(char str[])											//	*
{
if (!str) return;
if (!rputchar) return;
while (*str)
rputchar(*str++);
}
//----------------------------------------------------------------------*
// Вывод строки из ПЗУ через указатель									*
void printProgStr(const prog_char str[])							//	*
{
// print a string stored in program memory
register char c;
 
// check to make sure we have a good pointer
if (!str) return;
if (!rputchar) return;
// print the string until the null-terminator
while((c = pgm_read_byte(str++)))
rputchar(c);
}
//***********************************************************************
//----------------------------------------------------------------------*
// Поиск длины строки в памяти ОЗУ										*
uint8_t str_len(char* str)											//	*
{																	//	*
    uint8_t l=0;													//	*
    while (str[l]!=0)												//	*
        l++;														//	*
    return l;														//	*
}																	//	*
//----------------------------------------------------------------------*
// Поиск длины строки в памяти ПЗУ
uint8_t str_len_pgm(char* pgm_str)									//	*
{																	//	*
    uint8_t l=0;													//	*
    while (pgm_read_byte(&(pgm_str[l]))!=0)							//	*
        l++;														//	*
    return l;														//	*
}																	//	*
//----------------------------------------------------------------------*
// Сравнение равенства строк ОЗУ и ПЗУ
uint8_t str_equal_pgm(char* str,char* pgm_str)						//	*
{																	//	*
    uint8_t i,l;													//	*
    l=str_len(str);													//	*
    if (l!=str_len_pgm(pgm_str))// сравниваем длину строк				*
        return 0;				// если длина разная строки разные		*
    for (i=0; i<l; i++)	// если длина одинаковая сравниваем посимвольно	*
    {																//	*
    if (str[i]!=pgm_read_byte(&(pgm_str[i])))// если символы отличаются,*
    	return 0;							// то значит строки разные	*
    }																//	*
    return 1;														//	*
}
//-----------------------------------------------------------------------

uint8_t is_regular_char(char x)
{
    if (((x>='A') && (x<='Z')) || ((x>='a') && (x<='z')) || ((x>='0') && (x<='9')))
        return 1;
    else
        return 0;
}
//-----------------------------------------------------------------------
uint8_t is_digit(char x)
{
    if ((x>='0') && (x<='9'))
        return 1;
    else
        return 0;
}
//------------------------------------
void flash_to_ram(char *pgm, char *str)
{
uint8_t i=0;
char temp;
while( (temp=pgm_read_byte(&(pgm[i])))!=0 )
{
str[i++]=temp;
}
str[i]='\0';
}
//--------------

uint16_t str_to_uint16(char *s_num)
{
    uint8_t i=0,dgt_len=0;
    uint16_t r_val=0,mult=1;

    while (is_digit(s_num[dgt_len]))
        dgt_len++;

    for (i=dgt_len; i>0; i--)
    {
        r_val+=(s_num[i-1]-'0')*mult;
        mult*=10;
    }

    return r_val;
}

																	//	*
#endif
