#ifndef LCD2004_I2C_C
#define LCD2004_I2C_C
//----------------------------------------------------------
#include <avr_cmsis.h>
#include <driver_c\i2c.c>
//-------------------------------------------------------------
//Номера выводов к которым подключены управляющие выводы ЖКД 
#define RS 	0	// Команды= 0, Данные =1
#define RW 	1	// не используем
#define EN 	2	// стромб, Падающий стромб
#define led 3	// включение подсветки
#define Maska	  0xF0
//------------------------------------------------------------
// Комманды 
#define bit_8		0x30
#define bit_4_fisrt	0x20
#define cmd			0
#define	info		1
//------------------------------------------------------------
// Переменные 
static volatile uint8_t backlight=0x08; // подсветка включена по умолчанию
static volatile SetLCD_t set_common ; // настройки по умолчанию
//------------------------------------------------------------
//*********************************************************************************
//
//	
//
//---------------------------------------------------------------------
//	Передача байта в 4-битном формате
// Успех-1/Провал-0
uint8_t func_4bit(uint8_t add_lcd, uint8_t com, uint8_t data)
{
uint8_t i2c_lcd, kol; 
for (uint8_t i=0;i<2;i++)
{
i2c_lcd=backlight; // загружаем подсветку
i2c_lcd|=(data&Maska)|(1<<EN)|(0<<RW)|(com<<RS); // загружаем первую половину байта
i2cMasterUploadBuf(i2c_lcd); // загружаем в буфер
i2c_lcd&=~(1<<EN); // стромб
i2cMasterUploadBuf(i2c_lcd); // загружаем в буфер
data=data<<4; // смещаем вторую часть 
}
kol=i2cMasterSendBuf(add_lcd);
_delay_us(40);
return kol>>2;
}
//--------------------------------------------------------------
void func_8bit(uint8_t add_lcd, uint8_t data)
{
uint8_t i2c_lcd; 
i2c_lcd=backlight; // загружаем подсветку
i2c_lcd|=(0<<RS)|(0<<RW)|(1<<EN)|(data&Maska); // загружаем первую половину байта
i2cMasterUploadBuf(i2c_lcd); // загружаем в буфер
i2c_lcd&=~(1<<EN); // стромб
i2cMasterUploadBuf(i2c_lcd); // загружаем в буфер
i2cMasterSendBuf(add_lcd);
_delay_us(40);
}
//------------------------------------------------------------
void setting_lcd(uint8_t add_lcd, SetLCD_t SetLCD)
{
func_4bit( add_lcd, cmd, (1<<5)|(SetLCD.set>>3));
func_4bit( add_lcd, cmd, (1<<2)|(SetLCD.set&0x03));
func_4bit( add_lcd, cmd, (1<<3)|((SetLCD.set>>2)&0x07));
}
//------------------------------------------------------------
// Отправить строку из RAM
void LCD_SendString(uint8_t addres_lcd, char *str)
{
  uint8_t data; // rez-успешность отправки данных  
  while (*str)
  {
    data = *str++;
    func_4bit(addres_lcd,info, data);    
  }
 
}
//------------------------------------------------------------
// Отправить строку из ROM
void LCD_SendStringFlash(uint8_t addres_lcd, prog_char *str)
{
  uint8_t data = pgm_read_byte(str);
  while (data)
  {
    func_4bit(addres_lcd,info, data); 
  	data = pgm_read_byte(++str);
	//str++;
  }
  
  
}
//------------------------------------------------------------
void CursorGoTo(uint8_t add_lcd, uint8_t x, uint8_t y)
{
uint8_t data=(1<<7);
switch(x%4)
{
case 0: data|=y%20;
		break;
case 1: data|=y%20+0x40;
		break;
case 2: data|=y%20+0x14;
		break;
case 3: data|=y%20+0x54;
		break;
}
func_4bit(add_lcd,cmd, data);
}
//------------------------------------------------------------
// Дисплей включить/выключить
void Lcd_Display(void)
{
set_common.display=~set_common.display;
func_4bit(Addres_lcd, cmd, (1<<3)|((set_common.set>>2)&0x07));
}
//------------------------------------------------------------
// Курсор подчеркнутый
void Cursor_underline(void)
{
set_common.cursor=~set_common.cursor;
func_4bit(Addres_lcd, cmd, (1<<3)|((set_common.set>>2)&0x07));
}
//------------------------------------------------------------
// Курсор Мигающий
void Cursor_blink(void)
{
set_common.blink=~set_common.blink;
func_4bit(Addres_lcd, cmd, (1<<3)|((set_common.set>>2)&0x07));
}
//------------------------------------------------------------
// Полная очистка экрана
void LCD_clear(uint8_t add_lcd)
{
func_4bit(add_lcd, cmd, 1);
delay_lcd_clear();
}

//------------------------------------------------------------
// Подстветка включить/выключить
void BackLight(void)
{
backlight=(~backlight)&0x08;
}

//------------------------------------------------------------
SetLCD_t Init_LCD(uint8_t add_lcd)
{
i2cInit();	// Инициализация шины
SetLCD_t setlcd;
setlcd.set=(0<<7)|(Lines<<6)|(Font<<5)|(Display<<4)|(Cursor<<3)|(Blink<<2)|(Shift_Curr<<1)|(Shift_Disp);
set_common.set=setlcd.set;
//------------------------------------------
// Инициалиазция шины 
// ожидаем загрузки экрана
_delay_ms(20);
func_8bit( add_lcd, bit_8);
func_8bit( add_lcd, bit_4_fisrt);
setting_lcd(add_lcd, setlcd);
return setlcd;
}
//------------------------------------------------------------------
#endif
