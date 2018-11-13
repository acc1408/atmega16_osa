#ifndef CMDLINE_C
#define CMDLINE_C
#include <atmega16_32.h>
//--------------------------------------------
static uint8_t argc=0; //���-�� ����������
char* argv[SHELL_ARGS_MAX]; // ������ ��� �������� ���������� ��� ���������� �������
//uint8_t Func_Num; // ���-�� ������� ��� ������ � ���� ������
//------------------------------------------
OST_FLAG      cmd_flag;
#define CMDstart 1<<0 // ���� ������� ��������� �������
#define overflow 1<<1 // ���� ������������ ������
//-------------------------
char  cmd_rx[Num_RX_cmd]; // ���-�� �������� 
uint8_t cmd_rx_curr=0;	// ������ ���������� ������� � �������

//***********************************************************************
//		������� ��� ������ ����� �� ��� � ���							*
//----------------------------------------------------------------------*
// ������������� ������� ��������� ������								*
// �������� ������� ���-�� ������� ��� ������							*
void cmdlineInit(void)												//	*
{																	//	*
OS_Flag_Create (cmd_flag); // ������� ����								*
}																	//	*
//----------------------------------------------------------------------*
// ����� � ���������� ������� 											* 
uint8_t cmd_exec(char *cmdline)										//	*
{																	//	*
uint8_t i=0, func_num;
func_num=pgm_read_byte(&CmdFunctionNumber);							//	*
while (!(str_equal_pgm(cmdline,(char *)pgm_read_word(&(sys_func_names[i])))))
    {																//	*
        i++;														//	*
        if (i>=func_num)											//	*
        {															//	*
            return 0;		// �� ����� �� ������� �� �������			*
        }															//	*
    }																//	*
((void (*)(uint8_t,char**))pgm_read_word(&(sys_func[i])))(argc,argv);
return 1;															//	*
}																	//	*
//----------------------------------------------------------------------*
// ���� ������ � ����� �������											*
void cmdline_input(char c)
{
// ���� ���� ������ ��������� ������ � ���� ������������ ������ �������
if (OS_Flag_Check_AllOff_I(cmd_flag,CMDstart||overflow)) // 
	{													//   
	
	switch(c)
		{
		case '\r':	OS_Flag_Set_I(cmd_flag,CMDstart);
					cmd_rx[cmd_rx_curr]='\0';
					if (!*argv[argc-1]) // ���� �������� ��������� ����� ������ '\0'
						argc=argc-1;// �� ��� �� �������� �������
					return;					 
					break;
		case ' ':	if (cmd_rx_curr==0) // ���� ������ ������ ������,
						return; // �� ����������
					if (cmd_rx[cmd_rx_curr-1]=='\0') // ���� 2 �������,
						return; // �� ����������	
					cmd_rx[cmd_rx_curr++]='\0';
					if (argc<SHELL_ARGS_MAX) 
						argv[argc++]=&(cmd_rx[cmd_rx_curr]);
		case '\n':	
					break;
		default:	cmd_rx[cmd_rx_curr++]=c;	
					break;
		}
	// ���� ������� 1 ������,	
	if (cmd_rx_curr==(Num_RX_cmd-1)) // ��������� ����� � ������
		{
		// �� ������ ���� ������������ ������
		OS_Flag_Set_I(cmd_flag,overflow); 
		return;
		}
	}
// ���� ������ ����� ������� � ���� ������������ ����������, ��
if ((c=='\r')&&(OS_Flag_Check_On_I(cmd_flag,overflow))) 
		{	// ��������� ���� ��������� ������
			OS_Flag_Set_I(cmd_flag,CMDstart);// ��������� ���� ��������� ������ 
			cmd_rx[cmd_rx_curr]='\0';// ������ ���� ����� ������
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
if (!(cmd_exec(cmd_rx))) 	// ����� ��������� �������
	{  //���� ��� ������� �� ������� ������� �� ���
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
