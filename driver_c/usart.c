#ifndef USART_C
#define USART_C
#include <atmega16_32.h>

//--------------------------------------
OST_SMSG  rx[NUM_RX];
OST_SMSG  tx[NUM_TX];
OST_SQUEUE rx_q;   // Очередь коротких сообщений
OST_SQUEUE tx_q;   // Очередь коротких сообщений
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
temp.ursel=1;      //разрешение на запись //
temp.ucsz=set.bit; // кол-во бит для передачи
temp.usbs=set.bits; // кол-во стоп-битов
temp.upm=set.par; // проверка на четность
#ifndef only_asynch_uart
temp.umsel=set.syn; // режим синхронизации
temp.ucpol=set.edge; // режим тактирования
#else
temp.umsel=0;
#endif
UCSRC=temp.all;
//--------------------
ucsrb->ucsz2=0; 	// макс. 8 бит
ucsrb->txen=set.tx; // вкл/откл передатчика
ucsrb->rxen=set.rx; // вкл/откл приемника
ucsrb->rxcie=set.rx;// включение прерывания при приеме
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
while(OS_Squeue_IsFull (tx_q)); // Ожидаем появление свободной ячейки
OS_Squeue_Send_Now(tx_q, ch);	// Добавляем символ в буфер
ucsrb->udrie=1;					// включаем передачу данных через прерывание
}
//---------------------

volatile static VoidFuncChar_t UartRxFunc=0;
//---------------------------------
// настройка функции для приема данных
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
