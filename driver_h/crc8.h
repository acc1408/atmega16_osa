#ifndef CRC8_H
#define CRC8_H
#include <avr_cmsis.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
//----------------------------
// ���������� ���� ��� �������
// �������� �������, ����� �������
uint8_t crc8_dallas(uint8_t *mas, uint8_t  len);
#endif
