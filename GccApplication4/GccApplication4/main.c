io/*
 * GccApplication4.c
 *
 * Created: 24/02/2023 9:24:27 PM
 * Author : Adham Abu Elkheir
 */

#define  F_CPU 16000000UL

#include <util/delay.h>

#include <avr/io.h>

#include "Bit_Math.h"

#include <avr/interrupt.h>

void on_mode(void);

void off_mode(void);

void ADC_Init(void);

void Cool_Mode(void);

void Heat_Mode(void);

void up_mode(void);

int ADC_Read(char);

float Temp_value(int);

void interrupt_Init(void);

void down_mode(void);

float Temp;

int Temp_of_set;

int overflow;


volatile char k;
volatile char n;

ISR(INT0_vect)
{
SET_BIT(PORTD,6);
k=1;
CLR_BIT(PORTD,1);
CLR_BIT(PORTC,0);
CLR_BIT(PORTC,1);
CLR_BIT(PORTC,2);
CLR_BIT(GIFR,6);
}

ISR(INT1_vect)
{
SET_BIT(PORTD,7);
n=1;
CLR_BIT(PORTD,1);
CLR_BIT(PORTC,0);
CLR_BIT(PORTC,1);
CLR_BIT(PORTC,2);
CLR_BIT(GIFR,6);
}



int main(void)
{
    /* Replace with your application code */
  DDRA=0x00;
  DDRB=0x00;
  DDRD=0xF3;
  DDRC=0xFF;
  PORTD=0x00;
  k=0;
  n=0;
  interrupt_Init();
  sei();

	while (1)
    {
	if((GET_BIT(PINB,0)==1))
	{
		on_mode();
	}
	if((GET_BIT(PINB,3)==1))
	{
		off_mode();
	}

	if((GET_BIT(PORTD,0)==1))
	{
		ADC_Init();
         short int Tempnum[10];
		 char i;
		 /*calculating average temperature of 10 sequential readings from LM35 */

		 for (i=0;i<10;i++)
		 {
		 Tempnum[i]=Temp_value(ADC_Read(1));
		 _delay_ms(5);
		 }

		Temp=(Tempnum[0]+Tempnum[1]+Tempnum[2]+Tempnum[3]+Tempnum[4]+Tempnum[5]+Tempnum[6]+Tempnum[7]+Tempnum[8]+Tempnum[9])/10;


		/*temperature comparison with setting temperature*/
		if(Temp<Temp_of_set)
		{
			SET_BIT(PORTD,1); /* temperature indication bit(temperature is higher than set temperature)*/
		    Heat_Mode();     /* heating mode */
		}
		else if (Temp>Temp_of_set)
		{
			CLR_BIT(PORTD,1);/* temperature indication bit(temperature is lower than set temperature)*/
			Cool_Mode();      /*cooling mode*/

		}
	 else if (Temp==Temp_of_set)
	 {
      CLR_BIT (PORTC,1);

	  CLR_BIT (PORTC,2);

	  CLR_BIT (PORTD,1);

	  CLR_BIT (PORTC,0);
	 }

	if((k==1)||(n==1)) /*entering setting mode */
	{
		 _delay_ms(5);
		  overflow=0;
		 CLR_BIT(PORTD,1);
		 CLR_BIT(PORTC,0);
		 CLR_BIT(PORTC,1);
		 CLR_BIT(PORTC,2);
		 TCNT0=0x00;
		 TCCR0=0x05;
		 while(overflow<100)
		 {
			Seven_Segments(Temp_of_set);

            if((GET_BIT(PINB,3)==1))
            {
	           off_mode();
			   overflow=100;
			   break;
            }
			 if(TCNT0==255)
			 {
				 overflow++;
                TCNT0=0;
			 }
			  DDRA=0x00;

			  DDRA=0xFD;

			 if((overflow<100)&&(GET_BIT(PIND,2)==1))
			 {
				 overflow=0;
				 up_mode();

			 }


		  else if((overflow<100)&&(GET_BIT(PIND,3)==1))
		 {
			 overflow=0;
			 down_mode();

		 }

		 }
	}

	if(overflow==100)
	{
	TCCR0=0x00;
	CLR_BIT(PORTD,6);
	n=0;
	k=0;
	overflow=0;
	CLR_BIT(PORTD,7);
	}
	}

	}
}
 void System_Init()
{
 DDRA=0x00;
 DDRB=0x00;
 DDRD=0xF3;
 DDRC=0xFF;
 PORTD=0x00;
 k=0;
 n=0;
}




void Seven_Segments(unsigned char t)
{
	switch(t)
	{
		case(35):
		PORTA=0x35;
		break;

		case(40):
		PORTA=0x40;
		break;

		case(45):
		PORTA=0x45;
		break;

		case(50):
		PORTA=0x50;
		break;

		case(55):
		PORTA=0x55;
		break;

		case(60):
		PORTA=0x60;
		break;

		case(65):
		PORTA=0x65;
		break;

		case(70):
		PORTA=0x70;
		break;

		case(75):
		PORTA=0x75;
		break;
	}
	}
void on_mode()
{
	char x=GET_BIT(PORTD,0);/* cheaking off mode */
	if(x==0)
	SET_BIT(PORTD,0);
	Seven_Segments(60);
     Temp_of_set=60;
}
	void off_mode()
	{
		char y=GET_BIT(PORTD,0); /* cheaking on mode */
		if(y==1)
		CLR_BIT(PORTD,0);
		DDRA=0X00;
	   CLR_BIT(PORTD,1);
	   CLR_BIT(PORTC,0);
	   CLR_BIT(PORTC,1);
	   CLR_BIT(PORTC,2);
	}
	void ADC_Init(){
		DDRA = 0xFD;	        /* Make ADC port as output except channel 1*/
		ADCSRA = 0x87;          /* Enable ADC, with freq/128  */
		ADMUX = 0x40;           /* Vref: Avcc, ADC channel: 1 */
	}

	int ADC_Read(char channel)
	{
		ADMUX = 0x40 | (channel & 0x07);   /* set input channel to read */
		ADCSRA |= (1<<ADSC);               /* Start ADC conversion */
		while (!(ADCSRA & (1<<ADIF)));     /* Wait until end of conversion by polling ADC interrupt flag */
		ADCSRA |= (1<<ADIF);               /* Clear interrupt flag */
		_delay_ms(1);                      /* Wait a little bit */
		return ADCW;                       /* Return ADC word */
	}
	float Temp_value(int ADC_VAR)
	{
	     float celsius;
		 celsius = (ADC_Read(1)*4.88);
		 celsius = (celsius/10.00);
		 return celsius;
	}
	void Cool_Mode()
	{
		SET_BIT(PORTC,1);       /*cooling element is off*/
		SET_BIT(PORTC,0);
	}
	void Heat_Mode()
	{
		SET_BIT(PORTC,2);      /*heating element is on*/
	if(GET_BIT(PORTC,2)==1)
		{
			SET_BIT(PORTC,0);

			_delay_ms(50);

			CLR_BIT(PORTC,0);
		}
	}
	void interrupt_Init()
	{
		MCUCR=0X0F;    /* determining the sense control of interrupt 0 & 1 as rising edge detection */
		GICR=0XFF;     /* enable for external interrupt 0 & 1 */
	}
	void up_mode() /*entered during setting mode */
	{

		if((Temp_of_set>=35)&&(Temp_of_set<75))
		Temp_of_set=Temp_of_set+5;
		Seven_Segments(Temp_of_set);
		_delay_ms(50);
	}
	void down_mode() /*entered during setting mode */
	{
		if((Temp_of_set>35)&&(Temp_of_set<=75))
		Temp_of_set=Temp_of_set-5;
		Seven_Segments(Temp_of_set);
		_delay_ms(50);
	}
