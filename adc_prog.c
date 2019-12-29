/*
 * File:   adc_prog.c
 * Author: Vinit
 *
 * Created on 28 December, 2019, 10:20 PM
 */


#include  <xc.h>
#include  <PIC18F4550.h>
#include  <stdio.h>
#include  <stdlib.h>
#define _XTAL_FREQ 8000000

typedef unsigned int  u_int;
typedef unsigned short int  us_int;
typedef unsigned char byte_int;

/* UART Code for Debugging Purpose */

void uart_init(unsigned int baudRate);
void uart_send_char(char ch);
void uart_send_str(char *send_str);

/* ADC Function*/
void adc_init(byte_int adc_channel_active);
u_int adc_read(us_int adc_channel);

/* Generic Function*/
void delay(unsigned int delay);
void read_print_no_adc_val_channels(us_int no_channels);

void *memset(void * p1, int c, register size_t n);

void main(void) {
    
    u_int adc_value   = 0;
    char adc_val_str[50];
    
        OSCCON=0X73;                                            //8Mhz crystal (Internal Crystal)
        
        uart_init(9600);
        adc_init(1);                                            //Only One Channel Is Active (AN0)
        
        uart_send_str("ADC Prog. Start... \r\n");
                        
        while(1) 
        {
       
         adc_value =  adc_read(0);                              // Read First Channel
         sprintf(adc_val_str, "ADC_value=%d\r\n", adc_value);
         uart_send_str(adc_val_str);
         
       //  read_print_no_adc_val_channels(2);                   //Pass The Arguments How Many Channels To Read
       
         delay(400);                                            //After Every 400ms Conversion
        }

    
    return;
}


/*Uart Code For Debugging*/

void uart_init(unsigned int baudRate)
{
    /* UART TRANSMISSION REGISTER BITS */
    BAUDCON = 0X00;
    
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
          
    SPBRG = ((unsigned long)_XTAL_FREQ/((unsigned long)baudRate*64))-1;      //BAUD rate Set
            
    TXSTAbits.CSRC = 1;                                                      // Asynchronous Mode
    TXSTAbits.TX9  = 0;                                                      // 8 Bit transmition Enabled
    TXSTAbits.SYNC = 0;                                                      // Asynch Mode
    TXSTAbits.BRGH = 0;                                                      // low speed Baud rate
    TXSTAbits.TXEN = 1;                                                      //Transmit Enabled

    /* UART RECIEVER REGISTER BITS */
    
    RCSTAbits.SPEN = 1;       //Rx & Tx Pins Configured
    RCSTAbits.RX9  = 0;     // 8 Bit Reception
    RCSTAbits.CREN = 1;       //continues recieve mode
   
    return ;
}

void uart_send_char(char ch)
{
   
    TXREG = ch;
    while(0 == PIR1bits.TXIF);  // Wait upto transmission Completed
}

void uart_send_str(char *send_str)
{
    while(*send_str)            //On Null it will terminate
    {
         uart_send_char( *send_str);
         send_str++;
    }
   
}

/* ADC Module  Functions */

void adc_init(byte_int adc_channel_active)        //Arg: How Many ADC channel to Activate
{
   /* ADC Running On 250 Khz */
    ADCON0bits.ADON   = 1;                                      //ADC conversion module is enabled
    ADCON1bits.VCFG0  = 0;                                      // VSS as Ref
    ADCON1bits.VCFG01 = 0;                                      // VDD as Ref
    ADCON1bits.PCFG = (0b00001111)&( ~(adc_channel_active));    //ADC_Channel_Activate
    
    ADCON2bits.ADFM = 0;                                        // Left Justified
    ADCON2bits.ACQT = 0b101;                                    //12 Tad ADC acquisition Time
    ADCON2bits.ADCS = 0b010;                                    // Fosc/32 
    ADCON0bits.GO=0;                                            // ADC IDLE state
    ADRES = 0;                                                  // Clear Adc Value Register
}

u_int adc_read(us_int adc_channel)
{
    
    u_int adc_value;
    
    ADCON0bits.CHS = adc_channel;   //adc Channel
    ADCON0bits.GO = 1;          //ADC Start Of Conversion
    while(1 == ADCON0bits.DONE);  //Conversion is in Progress
    adc_value = (ADRESH<<2)|(ADRESL>>6);
   
     return adc_value;
}


/* Generic Function */
void delay(unsigned int delay){
    for(int i=0;i<delay;i++)
    {
    __delay_ms(1);
    }
}

 void read_print_no_adc_val_channels(us_int no_channels)
 {
     u_int  adc_val = 0;
     char all_adc_val_str[50];
     
     for(int i=0; i<no_channels; i++)
     {
         memset(all_adc_val_str,0,sizeof(all_adc_val_str));
         adc_val = adc_read(i);
         sprintf(all_adc_val_str,"ADC_CHN = %d ->ADC_VAL = %d\r\n",i,adc_val);
         uart_send_str(all_adc_val_str);
       
     }
 
 
 
 }