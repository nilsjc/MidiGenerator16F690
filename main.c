/*
 * File:   main.c
 * Author: NilsEd
 *
 * Created on den 13 mars 2021, 16:19
 */

// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

#include <xc.h>
unsigned char convert(void){
    ADCON0bits.GO_DONE = 1;
    while(ADCON0bits.GO_DONE){
        
    }
    return ADRESH;
}
void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Init_TR(void)
{
    TRISB5 = 1;  // As stated in the datasheet
    TRISB7 = 1;  // As stated in the datasheet
    BRGH = 0; // Set For High-Speed Baud Rate
    SPBRG = 3; // Set The Baud Rate To Be 31250 bps
    //--[ Enable The Ascynchronous Serial Port ]--
    SYNC = 0;
    SPEN = 1;
    //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
    TXEN = 1; // Enable UART Transmission
}
void main(void) {
    OSCCON = 0b01110000;
    UART_Init_TR();
    
    TRISA = 0b00000001;
    ANSELbits.ANS0 = 1;     // ANS0 = analog pin
    ADCON0bits.CHS = 0;     // A/D Channel   
    ADCON0bits.ADON = 1;    // Activate A/Domvandlaren
    TRISC = 0x00;
    ADFM = 0;
    int x = 0;
    int y = 0;
    int i = 0;
    char out = 1;
    unsigned char speed = 1;
    unsigned int speedLoop = 1;
    unsigned char notes[11] = {40,42,44,45,47,49,30,34,32,37,35};
    unsigned char velo[7] = {120,100,60,100,80,120,90};
    while(1)
    {
        speed = convert();
        if(speed<2){
            speed = 2;
        }
        speedLoop = speed << 8;
        for(y = 0; y < 11; y++){
            
            for(x = 0; x < speedLoop; x++){
            
            }
            UART_Write(0x90);
            UART_Write(notes[y]);
            UART_Write(velo[i]);
            out++;
            i++;
            if(i>6){
                i=0;
            }
            PORTC = out;
            speed = convert();
            if(speed<2)
            {
                speed = 2;
            }
            speedLoop = speed << 8;
            for(x = 0; x < speedLoop; x++){
            
            }
            UART_Write(0x80);
            UART_Write(notes[y]);
            UART_Write(0);
        }
        
    }
    return;
}
