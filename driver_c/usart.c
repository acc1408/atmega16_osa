#ifndef USART_C
#define USART_C
#include <atmega16_32.h>

//--------------------------------------
OST_SMSG  rx[NUM_RX];
OST_SMSG  tx[NUM_TX];
OST_SQUEUE rx_q;   // ������� �������� ���������
OST_SQUEUE tx_q;   // ������� �������� ���������
//--------------------------------------------------------------------------------
void UsartInit(void)
{
OS_Squeue_Create(rx_q,rx,NUM_RX);
OS_Squeue_Create(tx_q,tx,NUM_TX);
usart_t uartsetdef=
		{
		.rx=UART_RX,
		.tx=UART_TX,
		.bit=UART_BIT,
		.par=UART_PAR,
		.bits=UART_STOP
		};
uartSetBaudRate(UART_SPEED);
usart_set(uartsetdef);
}
//--------------------------------------------------------------------------------
void usart_set(usart_t set)
{
ucsrc_t temp; 
temp.ursel=1;      //���������� �� ������ //
temp.ucsz=set.bit; // ���-�� ��� ��� ��������
temp.usbs=set.bits; // ���-�� ����-�����
temp.upm=set.par; // �������� �� ��������
#ifndef only_asynch_uart
temp.umsel=set.syn; // ����� �������������
temp.ucpol=set.edge; // ����� ������������
#else
temp.umsel=0;
#endif
UCSRC=temp.all;
//--------------------
ucsrb->ucsz2=0; 	// ����. 8 ���
ucsrb->txen=set.tx; // ���/���� �����������
ucsrb->rxen=set.rx; // ���/���� ���������
ucsrb->rxcie=set.rx;// ��������� ���������� ��� ������
}
//----------------------------------------------------------------------

void uartSetBaudRate(uint32_t baudrate)
{
	// calculate division factor for requested baud rate, and set it
	uint16hl_t bauddiv;
	bauddiv.HL = ((F_CPU+(baudrate*8L))/(baudrate*16L)-1);
	UBRRL=bauddiv.L;
	UBRRH=bauddiv.H;
}
//------------------------------

void UartSendChar(char ch)
{
while(OS_Squeue_IsFull (tx_q)); // ������� ��������� ��������� ������
OS_Squeue_Send_Now(tx_q, ch);	// ��������� ������ � �����
ucsrb->udrie=1;					// �������� �������� ������ ����� ����������
}
//---------------------

volatile static VoidFuncChar_t UartRxFunc=0;
//---------------------------------
// ��������� ������� ��� ������ ������
void UartSetRxHandler(void (*rx_func)(char c))
{
// set the receive interrupt to run the supplied user function
UartRxFunc = rx_func;
}
//--------------------------------------------------------
ISR(USART_RXC_vect)
{
uint8_t temp=UDR;
if(UartRxFunc)
    {
    // call it and pass the received data
    UartRxFunc((char)temp);
    }
else
	{
	if (!OS_Squeue_IsFull_I(rx_q)) OS_Squeue_Send_I(rx_q, temp);
	}
}
//--------------
//static char temp_uart;
ISR(USART_UDRE_vect)
{

OS_Squeue_Accept_I(tx_q,UDR);

if (!OS_Squeue_Check_I(tx_q)) ucsrb->udrie=0;
}
//------------------------



#endif
