#include "Driver_USART.h"               // ::CMSIS Driver:USART

char data[100];

extern ARM_DRIVER_USART Driver_USART7;
void main(void)
{
	
Driver_USART7.Initialize(NULL);
	
	Driver_USART7.PowerControl(ARM_POWER_FULL);
	Driver_USART7.Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 9600);
	Driver_USART7.Control (ARM_USART_CONTROL_TX, 1);
	Driver_USART7.Control (ARM_USART_CONTROL_RX, 1);
	
	Driver_USART7.Send("AT+BAUD8", 8);
	
	Driver_USART7.Receive(data,8);
	while(Driver_USART7.GetRxCount()!=8);
	
	while(1);
	
}