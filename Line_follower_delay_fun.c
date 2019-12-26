#include"autobot.h"
#include<PIC18F4550.h>
#define NULL        (char)0
#define null        (char)0

typedef void (*l_fun_ptr)();

/* Polling Timer with Function Input */

/*Timer Function*/

/* Timer 0 works on 1 mili sec Ticks*/
void timer_poll(unsigned int time_ms ,l_fun_ptr fun_ptr ){
   unsigned int timer_overFlow_count = 0;
   
    while( (time_ms -1 ) >=  timer_overFlow_count)
    {
        while(RESET == INTCONbits.TMR0IF )      //Polling UptoTimer Expire
        {
            if(NULL != fun_ptr)
            {
                   /* Function Can be performed for particular time No delay In function other wise 
                    * Code will Lose Original Delay
                    * Only Add Few Line SO for few Instruction few Micro Seconds will be taken*/
                fun_ptr();
            }
        }
        INTCONbits.TMR0IF=RESET;                // Clearing Flag
        TMR0 = 65504;                           // Value Refill for 1ms (Timer is still ON)
        timer_overFlow_count++;                 // No. of Ms Count
    }

}

void timer0_init()
{
    T0CONbits.T08BIT = RESET;      // 16 Bit Mode
    T0CONbits.T0CS   = RESET;        // Internal Instruction Clock
    T0CONbits.T0SE   = RESET;      //  Increment on Low to High Transition
    T0CONbits.PSA    = RESET;      //  Prescaler Configured
    T0CONbits.T0PS  = 0b101;       //  Prescaler 1/(64*4)  // for Inter Clock it is FOSC/4
    INTCONbits.TMR0IF=RESET;        // Over Flow Flag Bit 
    TMR0=RESET;                    // Timer clear
    return ;
}


void timer0_clear()      //   make macro
{
     TMR0=RESET;                    //Timer Clear
    return ;
}

void timer0_on()         //   make macro
{
    T0CONbits.TMR0ON = SET;         //Timer0 ON
     return ;
}

void timer0_off()        //   make macro
{
       T0CONbits.TMR0ON = RESET;    //Timer0 OFF
        return ;
}

/* Timer 0 */

void timer_delay(int time_ms, l_fun_ptr fun_ptr)         
{
    TMR0 = 65504;       //Value for 1 ms
    timer0_on();
    timer_poll(time_ms,fun_ptr);
    timer0_off();
}



/* Software Timer Section*/