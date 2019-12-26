/*
 * File:   pwm_pic.c
 * Author: HP
 *
 * Created on 12 December, 2019, 10:04 PM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include<PIC18F4550.h>
#define TMR16_OVERFLOW_VAL 65535
#define PWM_PERIOD 500


 
void pwm_channel_1(unsigned int value)
{
    value=(value*255)/100;          //PWM range 0 - 100
    CCP2=0X00;
    PR2=255;                        // PR2= Fosc/ ( (4 * Fpwm * PreScalar) ) - 1
                                    // Current Fpwm Freq = 488 Hz
    
    CCPR2L = value;                 // Duty Cycle value;
    
    
    CCP2CONbits.DC2B=0b00;          //For 10 bit PWM 2 LSD bits
    CCP2CONbits.CCP2M = 0b1100;     // PWM Mode
    
   TRISCbits.TRISC1 = 0;   //    EN1 (PWM Channel)
   
    TMR2=0X00;                       //TIMER CLEAR
    T2CONbits.T2CKPS =0b11;          //config 1/16 (Prescale)
    T2CONbits.TMR2ON=1;              // Timer2 ON
    PIR1bits.TMR2IF=0;               //FLAG CLEAR
  
    /* C1 is toggled */
 
}
void pwm_channel_2(unsigned int value)
{
    value=(value*255)/100;          //PWM range 0 - 100
    CCP1=0X00;
    PR2=255;                        // PR2= Fosc/ ( (4 * Fpwm * PreScalar) ) - 1
                                    // Current Fpwm Freq = 488 Hz
    
    CCPR1L = value;                 // Duty Cycle value;
    
    
    CCP1CONbits.DC1B=0b00;          //For 10 bit PWM 2 LSD bits
    CCP1CONbits.CCP1M = 0b1100;     // PWM Mode
  
  TRISCbits.TRISC2 = 0;   //    EN2 (PWM Channel)
  
    TMR2=0X00;                       //TIMER CLEAR
    T2CONbits.T2CKPS =0b11;          //config 1/16 (Prescale)
    T2CONbits.TMR2ON=1;              // Timer2 ON
    PIR1bits.TMR2IF=0;               //FLAG CLEAR
  
    /* C2 is toggled */
 
}


void delay(int ms){

    for(int x=0;x<ms;x++)
    {
    __delay_ms(1);
    }

}

void main(void) {
    
      OSCCON=0X73;            //8Mhz crystal (Internal Crystal)
   pwm_channel_1(30);
   pwm_channel_2(40);
      while(1)
      {
        
      }
      
    return;
}
