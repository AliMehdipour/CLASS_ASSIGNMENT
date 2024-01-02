/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : MICRO1
Version : 1
Date    : 2/1/2022
Author  : ALIMEHDIPOUR
Company : 
Comments: 


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <stdio.h>
#include <stdlib.h>
#include <delay.h>

// Declare your global variables here
    //Declare Functions:
    void Move_Forward(int speed);
    void Move_Right(void);
    void Stop(void);
    void Move_Down(void);
    void Move_Left(void);
    void Send_Temp(int);
    void Receive_Temp(void);
    //END
    //Declare Variables  
     
    int Dir=0; 
    int Dir2=0;
    int h1;
    int h2;
    int status=2; //0:CLOCKWISE - 1:ANTICLOCKWISE -2:OFF
    //END
//END

#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)

// USART Receiver buffer
#define RX_BUFFER_SIZE 16
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index=0,rx_rd_index=0;
#else
unsigned int rx_wr_index=0,rx_rd_index=0;
#endif

#if RX_BUFFER_SIZE < 256
unsigned char rx_counter=0;
#else
unsigned int rx_counter=0;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer[rx_wr_index++]=data;
#if RX_BUFFER_SIZE == 256
   // special case for receiver buffer size=256
   if (++rx_counter == 0) rx_buffer_overflow=1;
#else
   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
      rx_buffer_overflow=1;
      }
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index++];
#if RX_BUFFER_SIZE != 256
if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#endif
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif

// USART Transmitter buffer
#define TX_BUFFER_SIZE 16
char tx_buffer[TX_BUFFER_SIZE];

#if TX_BUFFER_SIZE <= 256
unsigned char tx_wr_index=0,tx_rd_index=0;
#else
unsigned int tx_wr_index=0,tx_rd_index=0;
#endif

#if TX_BUFFER_SIZE < 256
unsigned char tx_counter=0;
#else
unsigned int tx_counter=0;
#endif

// USART Transmitter interrupt service routine
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter)
   {
   --tx_counter;
   UDR=tx_buffer[tx_rd_index++];
#if TX_BUFFER_SIZE != 256
   if (tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c)
{
while (tx_counter == TX_BUFFER_SIZE);
#asm("cli")
if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer[tx_wr_index++]=c;
#if TX_BUFFER_SIZE != 256
   if (tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
#endif
   ++tx_counter;
   }
else
   UDR=c;
#asm("sei")
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>

#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 7
unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
// Voltage Reference: Int., cap. on AREF
#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// ADC interrupt service routine
// with auto input scanning
interrupt [ADC_INT] void adc_isr(void)
{
static unsigned char input_index=0;
// Read the AD conversion result
adc_data[input_index]=ADCW;
// Select next ADC input
if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
   input_index=0;
ADMUX=(FIRST_ADC_INPUT | ADC_VREF_TYPE)+input_index;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
}

void main(void)
{
// Declare your local variables here
 DDRB.2=0;
// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(1<<RXCIE) | (1<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 125.000 kHz
// ADC Voltage Reference: Int., cap. on AREF
// ADC Auto Trigger Source: ADC Stopped
ADMUX=FIRST_ADC_INPUT | ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (1<<ADSC) | (0<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Global enable interrupts
#asm("sei")
ADCSRA |=(1<<ADSC);

 DDRD&=0xFC;
 PORTD|=0xFC;
 GICR=(1<<INT2);
while (1)
      {
//      int j=0;
//      //JUST FOR TEMPRATOR FINE WORKING 
//      DDRB |=0xFF;
//      for(j;j<=1;j++){
//      PORTB=adc_data[j]/4.00;
//      }
      //Buttons Check : 

    if ((PIND & 0x04)==0){
       //call move_forward 
       Move_Forward(1);
       }
    if ((PIND & 0x08)==0){
       //call move_right
       Move_Right();
       }
   if ((PIND & 0x10)==0){
       //call stop  
      Stop();
      }
    if ((PIND & 0x20)==0){
       //call move_down
       Move_Down();
       }
    if ((PIND & 0x40)==0){
      //call move_left
      Move_Left();
       }        
      }
}
void Move_Forward(int speed){
    //1 m/s =1RPS ==> 60 X RPS=RPM  
    
    int rpm1=60*speed;
    int rpm2=60*speed;
    status=0;
    OCR0=100;
    OCR2=100;
    DDRC=0x0F;
    PORTC=0x0A;
    DDRB.3=1; //make OC0 output 
    DDRD.7=1; //make OC2 output
    TCCR0=0x6D;
    TCCR2=0x6F;
while (1){
     //*********** Speed Measurement DC1 *************
      while (PINC.7==1);  
      while (PINC.7==0);  
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024  
      Dir=PINC.6;
     while (PINC.7==1);  
      while (PINC.7==0);  
      TCCR1B=0;    
      h1=46875/TCNT1; 
     //*********** Speed Measurement DC2 *************
      while (PINC.5==1);  
      while (PINC.5==0);  
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024 
      Dir2=PINC.4;
      while (PINC.5==1);  
      while (PINC.5==0); 
      TCCR1B=0;
      h2=46875/TCNT1; 
      //************ Speed Control DC1 & DC2 *****************      
      if (rpm1>h1 && OCR0<255) {OCR0++;}
      else if (rpm1<h1 && OCR0>0){OCR0--;}    
      if (rpm2>h2 && OCR2<255) {OCR2++;}
      else if (rpm2<h2 && OCR2>0){OCR2--;}   
      delay_ms(200);
      if((PIND & 0x04)!=0){
      
        break;
}}}
void Move_Right(void){
     //1 m/s =1RPS ==> 60 X RPS=RPM
    int rpm1=30;
    int rpm2=30;
    OCR0=65;
    OCR2=65;
    DDRC=0x0F;
    PORTC=0x09;
    DDRB.3=1; //make OC0 output 
    DDRD.7=1; //make OC2 output
    TCCR0=0x6D;
    TCCR2=0x6F;
while (1){
    
     //*********** Speed Measurement DC1 *************
     while (PINC.7==1);  
      while (PINC.7==0);
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024  
     Dir=PINC.6;
     while (PINC.7==1);  
      while (PINC.7==0);
      TCCR1B=0;    
      h1=46875/TCNT1; 
     //*********** Speed Measurement DC2 *************
     while (PINC.5==1);  
      while (PINC.5==0);
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024 
      Dir2=PINC.6;
      while (PINC.5==1);  
      while (PINC.5==0);
      TCCR1B=0;
      h2=46875/TCNT1;  
      //************ Speed Control DC1 & DC2 *****************      
      if (rpm1>h1 && OCR0<255) {OCR0++;}
      else if (rpm1<h1 && OCR0>0){OCR0--;}    
      if (rpm2>h2 && OCR2<255) {OCR2++;}
      else if (rpm2<h2 && OCR2>0){OCR2--;}  
      delay_ms(200);
      delay_ms(1500);          //FOR DELAY INTO 30 RPM
      PORTC=0x00;
      break;
}}
void Stop(void){
    int rpm1=0.1;
    int rpm2=0.1;
switch (status){
case (0):
            //Move_Down();  
    OCR0=30;
    OCR2=30;
    DDRC=0x0F;
    PORTC=0x05;
    DDRB.3=1; //make OC0 output 
    DDRD.7=1; //make OC2 output
    TCCR0=0x6D;
    TCCR2=0x6F;
while (1){

     //*********** Speed Measurement DC1 *************
      while (PINC.7==1);  
      while (PINC.7==0);  
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024  
      Dir=PINC.6;
     while (PINC.7==1);  
      while (PINC.7==0);  
      TCCR1B=0;    
      h1=46875/TCNT1; 
     //*********** Speed Measurement DC2 *************
      while (PINC.5==1);  
      while (PINC.5==0);  
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024 
      Dir2=PINC.4;
      while (PINC.5==1);  
      while (PINC.5==0); 
      TCCR1B=0;
      h2=46875/TCNT1; 
      //************ Speed Control DC1 & DC2 *****************      
      if (rpm1>h1 && OCR0<255) {OCR0++;}
      else if (rpm1<h1 && OCR0>0){OCR0--;}    
      if (rpm2>h2 && OCR2<255) {OCR2++;}
      else if (rpm2<h2 && OCR2>0){OCR2--;}   
       
        status=2;
        PORTC=0x00;
        break;
       
      }
break;
case (1):
            //Move_Forward(1);  
    OCR0=15;
    OCR2=15;
    DDRC=0x0F;
    PORTC=0x0A;
    DDRB.3=1; //make OC0 output 
    DDRD.7=1; //make OC2 output
    TCCR0=0x6D;
    TCCR2=0x6F;
while (1){
     //*********** Speed Measurement DC1 *************
      while (PINC.7==1);  
      while (PINC.7==0);  
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024  
      Dir=PINC.6;
     while (PINC.7==1);  
      while (PINC.7==0);  
      TCCR1B=0;    
      h1=46875/TCNT1; 
     //*********** Speed Measurement DC2 *************
      while (PINC.5==1);  
      while (PINC.5==0);  
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024 
      Dir2=PINC.4;
      while (PINC.5==1);  
      while (PINC.5==0); 
      TCCR1B=0;
      h2=46875/TCNT1; 
      //************ Speed Control DC1 & DC2 *****************      
      if (rpm1>h1 && OCR0<255) {OCR0++;}
      else if (rpm1<h1 && OCR0>0){OCR0--;}    
      if (rpm2>h2 && OCR2<255) {OCR2++;}
      else if (rpm2<h2 && OCR2>0){OCR2--;}   
        status=2;
        PORTC=0x00;
        break;
        }
break;
default : 
break;
}}
void Move_Down(void){
    //1 m/s =1RPS ==> 60 X RPS=RPM
    int rpm1=30;
    int rpm2=30;
    status=1;
    OCR0=65;
    OCR2=65;
    DDRC=0x0F;
    PORTC=0x05;
    DDRB.3=1; //make OC0 output 
    DDRD.7=1; //make OC2 output
    TCCR0=0x6D;
    TCCR2=0x6F;
while (1){
    
   
     //*********** Speed Measurement DC1 *************
    while (PINC.7==1);  
      while (PINC.7==0); 
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024  
     Dir=PINC.6;
    while (PINC.7==1);  
      while (PINC.7==0); 
      TCCR1B=0;    
      h1=46875/TCNT1; 
     //*********** Speed Measurement DC2 *************
      while (PINC.5==1);  
      while (PINC.5==0); 
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024
      Dir2=PINC.4;
      while (PINC.5==1);  
      while (PINC.5==0); 
      TCCR1B=0;
      h2=46875/TCNT1;  
      //************ Speed Control DC1 & DC2 *****************      
      if (rpm1>h1 && OCR0<255) {OCR0++;}
      else if (rpm1<h1 && OCR0>0){OCR0--;}    
      if (rpm2>h2 && OCR2<255) {OCR2++;}
      else if (rpm2<h2 && OCR2>0){OCR2--;}   
      delay_ms(200);


if ((PIND & 0x20)!=0)
    
        //PORTC=0x00;
        break;
} }
void Move_Left(void){
     //1 m/s =1RPS ==> 60 X RPS=RPM
    int rpm1=30;
    int rpm2=30;
    OCR0=65;
    OCR2=65;
    DDRC=0x0F;
    PORTC=0x06;
    DDRB.3=1; //make OC0 output 
    DDRD.7=1; //make OC2 output
    TCCR0=0x6D;
    TCCR2=0x6F;
while (1){
     //*********** Speed Measurement DC1 *************
     while (PINC.7==1);  
      while (PINC.7==0);
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024  
     // Dir=PINC.6;
     while (PINC.7==1);  
      while (PINC.7==0);
      TCCR1B=0;    
      h1=46875/TCNT1; 
     //*********** Speed Measurement DC2 *************
      while (PINC.5==1);  
      while (PINC.5==0);
      TCNT1=0;
      TCCR1A=0;  
      TCCR1B=5;  //normal prescaler 1024 
      while (PINC.5==1);  
      while (PINC.5==0);
      TCCR1B=0;
      h2=46875/TCNT1;  
      //************ Speed Control DC1 & DC2 *****************      
      if (rpm1>h1 && OCR0<255) {OCR0++;}
      else if (rpm1<h1 && OCR0>0){OCR0--;}    
      if (rpm2>h2 && OCR2<255) {OCR2++;}
      else if (rpm2<h2 && OCR2>0){OCR2--;}   
      delay_ms(1500);          //FOR DELAY INTO 30 RPM
      PORTC=0x00;
      break;
}   
}
void Receive_Temp(void){
char ch;
ch=getchar();
if(ch=='I')
Send_Temp(adc_data[0]);
else if (ch=='O')
Send_Temp(adc_data[1]);
}
void Send_Temp(int data){
char transmit_data = data + '0';
putchar(transmit_data);
}
//EXTERNAL INTERUPT Temperture FOR MICRO2 
      interrupt [EXT_INT2] void ext_int2_isr(void){
      Receive_Temp();
      }
