

#include <xc.h>
#define _XTAL_FREQ 8000000
#include<PIC18F4550.h>
#define TMR16_OVERFLOW_VAL 65535
#define PWM_PERIOD 500

short int speed = 0;
static unsigned int tmr_pwm_flag = 0;
 unsigned int pwm_on_time  = 0;
 unsigned int pwm_off_time = 0;

 unsigned int dum1=0;
 unsigned int dum2=0;
 




void timer1_pwm_channel_1(unsigned int tm_pwm)
{
    if(0 == tm_pwm)
    {
        LATCbits.LATC1 = 0;
          T1CONbits.TMR1ON = 0;       // Timer 1 OFF
    }
    else if(100 == tm_pwm)
    {
         LATCbits.LATC1 = 1;
         T1CONbits.TMR1ON = 0;       // Timer 1 OFF
    }
    else
    {
    T1CONbits.RD16 = 1;          // Timer 1 16 bit mode  1: 16Bit Mode 0: 8Bit Mode
    T1CONbits.T1CKPS = 0b11;     // 1/8 Prescalar 
    T1CONbits.T1OSCEN = 0   ;    // Timer 1 Oscillator off
    T1CONbits.TMR1CS = 0;        //Internal clock fosc/4
                                 //total prescalar is 32
    //TMR1L=5;
   // TMR1 = 65285;               // PWM Period
    
    TRISCbits.TRISC1 = 0;       // C1 As a output
    LATCbits.LATC1   = 0;       //Initial Val
   
    pwm_on_time  = (tm_pwm*500)/100;
    
    pwm_off_time = 500 - pwm_on_time;
   
    TMR1 = 65535 -  pwm_off_time ;
                                            // pwm_on_time = 100;
                                            // pwm_off_time = 400;
                    
    PIE1bits.TMR1IE = 1;        // Timer 1 Interrupt Enable
    PIR1bits.TMR1IF = 0;        // Timer 1 Flag Clear
    T1CONbits.TMR1ON = 1;       // Timer 1 ON
       
    
    }
    //In interrupt subroutine PWM pin will be toggledSS
    // C1 is togggled in interrupt
}



void interrupt ISR(void)
{
   
   
    if(1 ==  PIR1bits.TMR1IF)
    {
         T1CONbits.TMR1ON = 0;       // Timer 1 OFF
     
         if(0 == tmr_pwm_flag)
        {
            
            LATCbits.LATC1 = 1 ;           //Hardware PWM toggle
           dum1  = ((unsigned short int)TMR16_OVERFLOW_VAL - (unsigned short int) 300); //pwm_on_time) ;                   //ON Time 
           TMR1 =  dum1;
          
        }
        else if(1 == tmr_pwm_flag)
        {
            LATCbits.LATC1 = 0 ;           //Hardware PWM toggle
           dum2 = ((unsigned short int)TMR16_OVERFLOW_VAL -(unsigned short int)200);//pwm_off_time) ;                    //OFF Time
           TMR1 =   dum2;
          
        }
          tmr_pwm_flag = !tmr_pwm_flag;                    // For 500 HZ PWM
          
       PIR1bits.TMR1IF = 0;                       //Clear Flag
        T1CONbits.TMR1ON = 1;       // Timer 1 ON
    }
}


void delay(int ms){

    for(int x=0;x<ms;x++)
    {
    __delay_ms(1);
    }

}

void main(void) {
    
    OSCCON=0X73;          //8Mhz crystal (Internal Crystal)
      TRISCbits.TRISC1 = 0;   //    EN1 (PWM Channel)
      TRISCbits.TRISC2 = 0;   //    EN2 (PWM Channel)
      
      
    RCONbits.IPEN   =    0;       //Unmasked Interrupt Enabled
    INTCONbits.GIE  =    1;        //Global Interrupt Enable
    INTCONbits.PEIE =    1;       //Peripheral Interrupt Enable
    
  
    timer1_pwm_channel_1(50);
     
      while(1)
      {
         
      }
      
    return;
}
