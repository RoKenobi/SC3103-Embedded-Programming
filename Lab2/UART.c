#include "UART.h"

void uart3_init(u32 bound)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //Enable the gpio clock
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //Enable the Usart clock
GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART2);
GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART2);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
GPIO_Init(GPIOC, &GPIO_InitStructure);
//UsartNVIC configuration
NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//Preempt priority
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
//Preempt priority
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//Enable the IRQ channel
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//Initialize the VIC register with the specified parameters
NVIC_Init(&NVIC_InitStructure);
//USART Initialization Settings
USART_InitStructure.USART_BaudRate = bound; //Baud rate
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //The word length is 8 bit data
USART_InitStructure.USART_StopBits = USART_StopBits_1; //A stop bit
USART_InitStructure.USART_Parity = USART_Parity_No; //No parity bits
//No hardware data flow control
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//Sending and receiving mode
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_Init(USART3, &USART_InitStructure); //Initialize serial port 3
USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //Open the serial port to accept interrupts
USART_Cmd(USART3, ENABLE); //Enable serial port 3
}

void usart3_send(u8 data)
{
	USART3->DR = (uint16_t) data;
	while ((USART3->SR & USART_FLAG_TC)== RESET);
}

void uart3_send_name()
{
	int i = 0;
	unsigned char name[]="Rohan\n\r";
	
	while(name[i] != '\0'){
		USART3->DR = name[i]; //send the current character
		while ((USART3->SR & 0x40)==0); // wait for transmission
     i++;		 
		
	}
}



int USART3_IRQHandler()
{
  int newPeriod;
  u8 Usart_Receive;
  uint8_t msg[20]="Last Char Rcd:"; 
  uint8_t Usart_Receive_msg[2]; //include 1 space for null terminator 
  OLED_ShowString(0,10,msg);
  OLED_Refresh_Gram();
if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
{
  Usart_Receive = USART_ReceiveData(USART3);
 // USART_Receive is the received data
  Usart_Receive_msg[0] = (char)Usart_Receive;
  Usart_Receive_msg[1] = '\0';
  //OLED_SHOW => FIRST IS LEFT TO RIGHT, SECOND IS VERTICAL
  
  OLED_ShowString(110,10,Usart_Receive_msg);
  OLED_Refresh_Gram();
  //in sec
  if (Usart_Receive_msg[0] >= '1' && Usart_Receive_msg[0] <= '9') {
    newPeriod = Usart_Receive_msg[0]-'0';
  xTimerChangePeriod(xAutoTimer, pdMS_TO_TICKS(newPeriod*1000), 100);
  xTimerChangePeriod(uartTimer, pdMS_TO_TICKS(newPeriod*1000), 100);  
    
}
  }
 return 0;
}


void uart_task_receive(void *pvParameters){
	USART3_IRQHandler();
}
