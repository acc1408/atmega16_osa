#ifndef DS18B20_C
#define DS18B20_C
#include <avr_cmsis.h>
#include <driver_c/crc8.c>
#include <driver_c/sdvig.c>
//#include <util/crc16.h>
//-------------
//-----------------------------------------------
uint8_t dallasReset(bus_1wire_t bus)
{
	/*static*/ volatile uint8_t *port_io, *ddr_io,*pin_io;
	/*static*/ volatile uint8_t ret_reset, p_io;
	ddr_io =bus.ddr;
	port_io=ddr_io+1;
	pin_io =ddr_io-1;
	p_io=bus.pin;
	
	*ddr_io|=(1<<p_io);
	*port_io&=~(1<<p_io);
	_delay_us(480); 		  // ������� 480 ���
	*ddr_io&=~(1<<p_io);
	_delay_us(70);			  // ������� 70 ���
	ret_reset= ((*pin_io)&(1<<p_io));
	_delay_us(410);
	if (ret_reset) return DALLAS_NO_PRESENCE;
			else   return DALLAS_PRESENCE;
}
//-------------
void dallasWriteByte_my(bus_1wire_t bus,uint8_t byte)
{
	/*static*/ volatile uint8_t *port_io, *ddr_io,*pin_io;
	/*static*/ volatile uint8_t  p_io, bit;
	ddr_io =bus.ddr;
	port_io=ddr_io+1;
	pin_io =ddr_io-1;
	p_io=bus.pin;
	
	uint8_t i;
	*ddr_io|=(1<<p_io); // ���� �������� �������
	for(i=0;i<8;i++)
	{
	bit=(byte>>i)&0b01;
	*port_io&=~(1<<p_io); // ������� ����� � �����
	if (bit)			  // �������� ��������� ���
	{
		_delay_us(6);
	}
	else
	{
		_delay_us(60);
	}
	*port_io|=(1<<p_io); // ������� � �������
	if (bit)
	{
		_delay_us(64);
	}
	else
	{
		_delay_us(10);
	}	
		// allow a us delay between each write
	_delay_us(1);
	} // for end

}
//-------------
uint8_t dallasReadByte_my(bus_1wire_t bus)
{
	/*static*/ volatile uint8_t *port_io, *ddr_io,*pin_io;
	/*static*/ volatile uint8_t  p_io, bit;
	ddr_io =bus.ddr;
	port_io=ddr_io+1;
	pin_io =ddr_io-1;
	p_io=bus.pin;
	uint8_t i,byte=0;
	for(i=0;i<8;i++)
	{
	*ddr_io|=(1<<p_io);
	*port_io&=~(1<<p_io); // ������� ����� � �����
	_delay_us(6);
	*ddr_io&=~(1<<p_io);
	_delay_us(9);
	bit=(*pin_io>>p_io)&0b01;
	_delay_us(55);
	byte|=(bit<<i);
	_delay_us(1);
	}
return byte;
}


//--------------------------
// ������ ��������������� ��� ������ �������� 
uint8_t ds18b20_Start_ROM_less(bus_1wire_t bus)
{
if (dallasReset(bus)==DALLAS_PRESENCE) 	 // ��������� ������������� �
{										 // � ��������� ��������� �������������
dallasWriteByte_my(bus,DALLAS_SKIP_ROM); // ���������� ROM-���
dallasWriteByte_my(bus,0x44); // ������ ��������������
return DALLAS_PRESENCE;
}
else  
return DALLAS_NO_PRESENCE;
}
//*************************************************************************************
// �������� ��������� ���������������
uint8_t ds18b20_Result_ROM_less(bus_1wire_t bus, uint16_t *temp)
{
 uint8_t  repeat=5; // ���-�� ������� ������ ������ �������
 
 union
 {
 uint16_t HL;		// ��������� ��������������
 uint8_t t[9];		// ���������� ��������� �������
 }tmp;
 do{
 if (dallasReset(bus)!=DALLAS_PRESENCE) 		// ������������� � ��������
 					return DALLAS_NO_PRESENCE;	// ���� ��� �������, �� �������					  			
 dallasWriteByte_my(bus,DALLAS_SKIP_ROM); 		  	// ���������� ROM-���
 dallasWriteByte_my(bus,DS18B20_READ_SCRATCHPAD); 	// ������ ������ �������
 for(uint8_t i=0;i<9;i++)							// ��������� ������ ���������
 	tmp.t[i]=dallasReadByte_my(bus);
 if(crc8_dallas(tmp.t, 8)==tmp.t[8]) 	{			// ��������� ����������� �����
 								//tmp.t[0]=a[0];		// 
 								//tmp.t[1]=a[1];
 								*temp=tmp.HL;
								return DALLAS_NO_ERROR;
								}
 repeat--;   // ��������� ���������� ������� ������� ������ �������
 }
 while(repeat);
 return DALLAS_CRC_ERROR; // 5 ��� �� �������� ��������� ���������� ���

}
//**********************************************************************************
uint8_t ds18b20_setting_ROM_less(bus_1wire_t bus, // ��������� ����
								 resolution	resol,// ��������� ����������
								 int8_t	min,  // ����������� ����������� � ��������
								 int8_t	max  // ������������ ����������� � ��������
								)
{
uint16_t t;
union{
	 int8_t t1;
	uint8_t t2;
	}temp;
uint8_t count=5, // ���-�� ������� ������ ���������
		ret;	 // ��������� ���������
//---------------------
	do {
	dallasReset(bus);
	dallasWriteByte_my(bus,DALLAS_SKIP_ROM);
	dallasWriteByte_my(bus,DS18B20_WRITE_SCRATCHPAD);
	temp.t1=min;
	dallasWriteByte_my(bus,temp.t2);
	temp.t1=max;
	dallasWriteByte_my(bus,temp.t2);
	dallasWriteByte_my(bus,resol);
	count--;
	ret=ds18b20_Result_ROM_less(bus, &t);
	//t++;
	}while(ret&count);
return ret;	
}
//***********************************************************************************
void delay_convert_temp(resolution resol)
{
uint8_t i;
for(i=0;i<(1<<(resol>>5));i++)
{
	delay_ms94;
}
}
//--------------------------------------------------
//-------------------------------------------------------
// ������� �������������� � ����� ����������
int8_t conver_int(uint16_t temp)
{
uint16hl_t t;
t.HL=sdvig16(temp, 4);
return t.L; 
}

//-------------------------------------------------------------------
uint8_t  dallasReadROM(bus_1wire_t bus,  // ��������� ����
					dallas_rom_id_t* rom)// ��������� �� ����� ROM ������
{
if (dallasReset(bus)==DALLAS_PRESENCE)
   {
	dallasWriteByte_my(bus,DALLAS_READ_ROM);
	for(uint8_t i=0;i<8;i++)
	{
	rom->byte[i]=dallasReadByte_my(bus);
	}
  	return DALLAS_NO_ERROR;
   }	
return DALLAS_NO_PRESENCE;
}
#endif
