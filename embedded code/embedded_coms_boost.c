/* embedded_coms_boost.c
 *
 *  Author: Steve Gunn & Klaus-Peter Zauner 
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *          View this license at http://creativecommons.org/about/licenses/
 *   Notes: 
 *          - Use with host_coms_boost.c
 *  
 *          - F_CPU must be defined to match the clock frequency
 *
 *          - Compile with the options to enable floating point
 *            numbers in printf(): 
 *               -Wl,-u,vfprintf -lprintf_flt -lm
 *
 *          - Pin assignment: 
 *            | Port | Pin | Use                         |
 *            |------+-----+-----------------------------|
 *            | A    | PA0 | Voltage at load             |
 *            | D    | PD0 | Host connection TX (orange) |
 *            | D    | PD1 | Host connection RX (yellow) |
 *            | D    | PD7 | PWM out to drive MOSFET     |
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define DELAY_MS      50
#define BDRATE_BAUD  9600
#define BUFFSIZE 200
#define VOLTDIV 5.68 //(R1+R2)/R2
#define ADCREF_V     3.3
#define ADCMAXREAD   1023 
   /* 10 bit ADC */

/* Find out what value gives the maximum
   output voltage for your circuit:
*/
#define PWM_DUTY_MAX 240    /* 94% duty cycle */
		
void init_stdio2uart0(void);
int uputchar0(char c, FILE *stream);
int ugetchar0(FILE *stream);
		
void init_adc(void);
double v_load(void);

void init_pwm(void);
void pwm_duty(uint8_t x);



int main(void)
{
	uint16_t cnt =0;

	char cmd[BUFFSIZE];	
	int prm;
	int res;
	int set_point;
	DDRB = 0xFF;
	int x = 0;
        	
	init_stdio2uart0();
	init_pwm(); 
	init_adc();
	

	printf("\r\nIlMatto Coms Boost READY!\r\n");
	 res = scanf("%s %u", cmd, &prm);	 
	for(;;) {
	     
		set_point = prm;
		cmd[0] = '\0';
		while(1){
		for(;;) {	
	/* 
	 if ((AB == 0x0 && lastAB == 0x2) || (AB == 0x3 && lastAB == 0x1)){

		MY_SETPOINT_V++;
		
	}
	if ((AB == 0x3 && lastAB == 0x2) || (AB == 0x0 && lastAB == 0x1)){
		
		MY_SETPOINT_V--;
		
}
*/
		printf( "SETPOINT: %d    ", set_point);
	    printf( "%04d:  ", cnt );
		 if ((v_load() * VOLTDIV > (set_point - (set_point * 0.05)))&& (v_load() * VOLTDIV < (set_point + (set_point*0.05)))){ //Taken from Ben Smith (my lab partner)
				pwm_duty(x);
			printf("\n VOLTAGE = %f \n", v_load()*VOLTDIV);
			if (v_load() * VOLTDIV > (1 - (1 * 0.05))&& (v_load() * VOLTDIV < (1 + (1*0.05)))){
				PORTB |= _BV(0);
				PORTB &= ~_BV(3);
				
			}
			if (v_load() * VOLTDIV > (2 - (2 * 0.05))&& (v_load() * VOLTDIV < (2 + (2*0.05)))){
				PORTB |= _BV(2);
				PORTB &= ~_BV(3);
				PORTB &= ~_BV(0);
			}
			if (v_load() * VOLTDIV > (3 - (3 * 0.05))&& (v_load() * VOLTDIV < (3 + (3*0.05)))){
				PORTB |= _BV(0);
				PORTB |= _BV(2);
				PORTB &= ~_BV(3);
			}
			if (v_load() * VOLTDIV > (4 - (4 * 0.05))&& (v_load() * VOLTDIV < (4 + (4*0.05)))){
				PORTB |= _BV(3);
				PORTB &= ~_BV(0);
			}
			if (v_load() * VOLTDIV > (5 - (5 * 0.05))&& (v_load() * VOLTDIV < (5 + (7*0.05)))){
				PORTB |= _BV(3);
				PORTB |= _BV(0);
			}
			if (v_load() * VOLTDIV > (6 - (6 * 0.05))&& (v_load() * VOLTDIV < (6 + (7*0.05)))){
				PORTB |= _BV(3);
				PORTB |= _BV(2);
				PORTB &= ~_BV(0);
			}
			if (v_load() * VOLTDIV > (7 - (7 * 0.05))&& (v_load() * VOLTDIV < (7 + (7*0.05)))){
				PORTB |= _BV(3);
				PORTB |= _BV(0);
				PORTB |= _BV(2);
			}
			cmd[0] = '\0';
			res = scanf("%s %u", cmd, &prm);	   
			set_point = prm;
			
			
		 }
		 else if ((v_load() * VOLTDIV) > set_point){
			PORTB |= _BV(0);
			PORTB &= ~_BV(3);
			PORTB &= ~_BV(2);
			pwm_duty(x--);
			pwm_duty(x--);
			pwm_duty(x--);
		}
		else if ((v_load() * VOLTDIV) < set_point){ 
	       PORTB |= _BV(3);
		   PORTB &= ~_BV(0);
		   PORTB &= ~_BV(2);
		   pwm_duty(x++);
			pwm_duty(x++);
			pwm_duty(x++);		   /* Limited by PWM_DUTY_MAX */
	 }
            
		
			

             //_delay_ms(DELAY_MS);
	    printf( "  -->  %5.3f V\r\n", (v_load()*VOLTDIV) );
	    cnt++;
		 
}
		}
}

	    //if ( res !=2 ) {   /* Error! */
		//scanf("%*s");  /* Clear input buffer */
	    //} else {
		//pwm_duty((uint8_t)prm);   /* Limited by PWM_DUTY_MAX */		
	    //}
	   
	    
	    printf( "%04d:  ", cnt );

	    	
	    printf( "  -->  %5.3f V\r\n", v_load() );

	    cnt++;
	}


int uputchar0(char c, FILE *stream)
{
	if (c == '\n') uputchar0('\r', stream);
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = c;
	return c;
}

int ugetchar0(FILE *stream)
{
	while(!(UCSR0A & _BV(RXC0)));
	return UDR0;
}

void init_stdio2uart0(void)
{
	/* Configure UART0 baud rate, one start bit, 8-bit, no parity and one stop bit */
	UBRR0H = (F_CPU/(BDRATE_BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/(BDRATE_BAUD*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

	/* Setup new streams for input and output */
	static FILE uout = FDEV_SETUP_STREAM(uputchar0, NULL, _FDEV_SETUP_WRITE);
	static FILE uin = FDEV_SETUP_STREAM(NULL, ugetchar0, _FDEV_SETUP_READ);

	/* Redirect all standard streams to UART0 */
	stdout = &uout;
	stderr = &uout;
	stdin = &uin;
}


void init_adc (void)
{
    /* REFSx = 0 : Select AREF as reference
     * ADLAR = 0 : Right shift result
     *  MUXx = 0 : Default to channel 0
     */
    ADMUX = 0x00;
    /*  ADEN = 1 : Enable the ADC
     * ADPS2 = 1 : Configure ADC prescaler
     * ADPS1 = 1 : F_ADC = F_CPU / 64
     * ADPS0 = 0 :       = 187.5 kHz
     */
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
}


double v_load(void)
{
     uint16_t adcread;
         
     /* Start single conversion */
     ADCSRA |= _BV ( ADSC );
     /* Wait for conversion to complete */
     while ( ADCSRA & _BV ( ADSC ) );
     adcread = ADC;
    
     printf("ADC=%4d", adcread);  
 
     return (double) (adcread * ADCREF_V/ADCMAXREAD);
}



void init_pwm(void)
{
    /* TIMER 2 */
    DDRD |= _BV(PD6); /* PWM out */
    DDRD |= _BV(PD7); /* inv. PWM out */
    

    TCCR2A = _BV(WGM20) | /* fast PWM/MAX */
	     _BV(WGM21) | /* fast PWM/MAX */
	     _BV(COM2A1); /* A output */
    TCCR2B = _BV(CS20);   /* no prescaling */   
}


/* Adjust PWM duty cycle
   Keep in mind this is not monotonic
   a 100% duty cycle has no switching
   and consequently will not boost.  
*/
void pwm_duty(uint8_t x) 
{
    x = x > PWM_DUTY_MAX ? PWM_DUTY_MAX : x;
    
    printf("PWM=%3u  ==>  ", x);  

    OCR2A = x;
}

