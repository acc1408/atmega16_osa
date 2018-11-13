#ifndef LCD2004_I2C_C
#define LCD2004_I2C_C
//----------------------------------------------------------
#include <avr_cmsis.h>
#include <driver_c\i2c.c>
//-------------------------------------------------------------
//������ ������� � ������� ���������� ����������� ������ ��� 
#define RS 	0	// �������= 0, ������ =1
#define RW 	1	// �� ����������
#define EN 	2	// ������, �������� ������
#define led 3	// ��������� ���������
#define Maska	  0xF0
//------------------------------------------------------------
// �������� 
#define bit_8		0x30
#define bit_4_fisrt	0x20
#define cmd			0
#define	info		1
//------------------------------------------------------------
// ���������� 
static volatile uint8_t backlight=0x08; // ��������� �������� �� ���������
static volatile SetLCD_t set_common ; // ��������� �� ���������
//------------------------------------------------------------
//*********************************************************************************
//
//	
//
//---------------------------------------------------------------------
//	�������� ����� � 4-������ �������
// �����-1/������-0
uint8_t func_4bit(uint8_t add_lcd, uint8_t com, uint8_t data)
{
uint8_t i2c_lcd, kol; 
for (uint8_t i=0;i<2;i++)
{
i2c_lcd=backlight; // ��������� ���������
i2c_lcd|=(data&Maska)|(1<<EN)|(0<<RW)|(com<<RS); // ��������� ������ �������� �����
i2cMasterUploadBuf(i2c_lcd); // ��������� � �����
i2c_lcd&=~(1<<EN); // ������
i2cMasterUploadBuf(i2c_lcd); // ��������� � �����
data=data<<4; // ������� ������ ����� 
}
kol=i2cMasterSendBuf(add_lcd);
_delay_us(40);
return kol>>2;
}
//--------------------------------------------------------------
void func_8bit(uint8_t add_lcd, uint8_t data)
{
uint8_t i2c_lcd; 
i2c_lcd=backlight; // ��������� ���������
i2c_lcd|=(0<<RS)|(0<<RW)|(1<<EN)|(data&Maska); // ��������� ������ �������� �����
i2cMasterUploadBuf(i2c_lcd); // ��������� � �����
i2c_lcd&=~(1<<EN); // ������
i2cMasterUploadBuf(i2c_lcd); // ��������� � �����
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
// ��������� ������ �� RAM
void LCD_SendString(uint8_t addres_lcd, char *str)
{
  uint8_t data; // rez-���������� �������� ������  
  while (*str)
  {
    data = *str++;
    func_4bit(addres_lcd,info, data);    
  }
 
}
//------------------------------------------------------------
// ��������� ������ �� ROM
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
// ������� ��������/���������
void Lcd_Display(void)
{
set_common.display=~set_common.display;
func_4bit(Addres_lcd, cmd, (1<<3)|((set_common.set>>2)&0x07));
}
//------------------------------------------------------------
// ������ ������������
void Cursor_underline(void)
{
set_common.cursor=~set_common.cursor;
func_4bit(Addres_lcd, cmd, (1<<3)|((set_common.set>>2)&0x07));
}
//------------------------------------------------------------
// ������ ��������
void Cursor_blink(void)
{
set_common.blink=~set_common.blink;
func_4bit(Addres_lcd, cmd, (1<<3)|((set_common.set>>2)&0x07));
}
//------------------------------------------------------------
// ������ ������� ������
void LCD_clear(uint8_t add_lcd)
{
func_4bit(add_lcd, cmd, 1);
delay_lcd_clear();
}

//------------------------------------------------------------
// ���������� ��������/���������
void BackLight(void)
{
backlight=(~backlight)&0x08;
}

//------------------------------------------------------------
SetLCD_t Init_LCD(uint8_t add_lcd)
{
i2cInit();	// ������������� ����
SetLCD_t setlcd;
setlcd.set=(0<<7)|(Lines<<6)|(Font<<5)|(Display<<4)|(Cursor<<3)|(Blink<<2)|(Shift_Curr<<1)|(Shift_Disp);
set_common.set=setlcd.set;
//------------------------------------------
// ������������� ���� 
// ������� �������� ������
_delay_ms(20);
func_8bit( add_lcd, bit_8);
func_8bit( add_lcd, bit_4_fisrt);
setting_lcd(add_lcd, setlcd);
return setlcd;
}
//------------------------------------------------------------------
#endif
