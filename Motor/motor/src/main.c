/* Project: MEAM 510 Lab3.2
 * Author: Xinyue Wei
 * Description: waldo
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h" 

void setup_ADC(unsigned int ADCchannel);
void select_ADC(unsigned int ADCchannel);
void set_DIDR(unsigned int ADCchannel);
int filter(int newValue, int oldValue, int weight);

int main(void)
{
    teensy_clockdivide(1); // 8MHz

    // setup usb connection
    m_usb_init();
    while (!m_usb_isconnected()); // wait for usb connection to be established

    set(TCCR3B, CS32);  //timer3 prescalar /256
    //set(TCCR1B, CS12);  //timer1 prescalar /256

    set(DDRC, 6);	// timer3
    //set(DDRB, 5);	//timer1
    set(TCCR3A, COM3A1);//timer 3 clear at OCR3A,set at rollover
   // set(TCCR1A, COM1A1);//timer 1 clear at OCR1A,set at rollover
    set(TCCR3B, WGM32); set(TCCR3A, WGM30);//timer3 mode5: up to 0x00FF
    //set(TCCR1B, WGM12); set(TCCR1A, WGM10);//timer1 mode5: up to 0x00FF
  
    float dutycycle = 0; 
    int i = 0;
    int dir = 1;
    while(1){
        // timer3 PWM
        // dutycycle = filter(ADC, channel_data1, weight) / 1023.0 * (0.315-0.055)+ 0.055;
        // if((i <= 100) && (i >= 0)){
        //         dutycycle = 0.055 + (0.315-0.055)*i/100.0;
        //         if(dir == 1) i++;
        //         else i--;
        // }
        // if(i == 100) dir = 0;
        // else if(i == 0) dir = 1;
        // //else dir = 1;
        // OCR3A = dutycycle * 255;	//mode 5: clear at OCR3A,set at rollover
        // teensy_wait(10);

        dutycycle = 0.055 + (0.315-0.055)*i/100.0;
        if(dir == 1) i++;
        else i--;
        if(i == 100) dir = 0;
        else if(i == 0) dir = 1;
        OCR3A = dutycycle * 255;	//mode 5: clear at OCR3A,set at rollover
        teensy_wait(10);

        // //input 2 & servo 2
        // setup_ADC(ADCchannel2);  //set ADC channel               
        // set(ADCSRA , ADSC); //start conversion
        // while(!bit_is_set(ADCSRA, ADIF));
        // while(TCNT1);
        // //timer1 PWM
        // dutycycle = filter(ADC, channel_data2, weight) / 1023.0 * (0.315-0.055)+ 0.055;
        // channel_data2 = ADC;
        // OCR1A = dutycycle * 255;	//mode 5: clear at OCR1A,set at rollover
        // clear(ADCSRA, ADIF);  //clear flag
    }
    return 0;
}

// subroutine: set up ADC 
void setup_ADC(unsigned int ADCchannel){
    set(ADMUX, REFS0); //set Vref = Vcc
    set(ADCSRA, ADPS2); set(ADCSRA, ADPS1); set(ADCSRA, ADPS0); //ADC prescalar /128
    set_DIDR(ADCchannel);    //set DIDR
    set(ADCSRA, ADATE);      //triggering
    select_ADC(ADCchannel);  //select ADC channel
    set(ADCSRA, ADEN);// enable ADC subsystem
}

void set_DIDR(unsigned int ADCchannel){
    unsigned int tmp;
    tmp = 1 << ADCchannel;
    DIDR0 |= tmp & 0x00FF;
    DIDR2 |= tmp >> 8;
}

void select_ADC(unsigned int ADCchannel){
     switch(ADCchannel){
         case 0: clear(ADCSRB,MUX5); clear(ADMUX,MUX2); 
                 clear(ADMUX,MUX1); clear(ADMUX,MUX0); break;
         case 1: clear(ADCSRB,MUX5); clear(ADMUX,MUX2); 
                 clear(ADMUX,MUX1); set(ADMUX,MUX0); break;
         case 4: clear(ADCSRB,MUX5); set(ADMUX,MUX2); 
                 clear(ADMUX,MUX1); clear(ADMUX,MUX0); break;
         case 5: clear(ADCSRB,MUX5); set(ADMUX,MUX2); 
                 clear(ADMUX,MUX1); set(ADMUX,MUX0); break;
         case 6: clear(ADCSRB,MUX5); set(ADMUX,MUX2); 
                 set(ADMUX,MUX1); clear(ADMUX,MUX0); break;
         case 7: clear(ADCSRB,MUX5); set(ADMUX,MUX2); 
                 set(ADMUX,MUX1); clear(ADMUX,MUX0); break;
         case 8: set(ADCSRB,MUX5); clear(ADMUX,MUX2); 
                 clear(ADMUX,MUX1); clear(ADMUX,MUX0); break;
         case 9: set(ADCSRB,MUX5); clear(ADMUX,MUX2); 
                 clear(ADMUX,MUX1); set(ADMUX,MUX0); break;
         case 10: set(ADCSRB,MUX5); clear(ADMUX,MUX2); 
                  set(ADMUX,MUX1); clear(ADMUX,MUX0); break;
         case 11: set(ADCSRB,MUX5); clear(ADMUX,MUX2); 
                  set(ADMUX,MUX1); set(ADMUX,MUX0); break;
         case 12: set(ADCSRB,MUX5); set(ADMUX,MUX2); 
                  clear(ADMUX,MUX1); clear(ADMUX,MUX0); break;
         case 13: set(ADCSRB,MUX5); set(ADMUX,MUX2); 
                  clear(ADMUX,MUX1); set(ADMUX,MUX0); break;
     }
}

//subroutine: software filter
int filter(int newValue, int oldValue, int weight){
    return (weight * oldValue + newValue) / (weight + 1); 
}