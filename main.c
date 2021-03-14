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
const unsigned char MAX_NOTE = 96;
const unsigned char MIDI_CHANNEL = 0;
const unsigned char NOTE_ON = 0x90 + MIDI_CHANNEL;
const unsigned char NOTE_OFF = 0x80 + MIDI_CHANNEL;
char shortDelay = 0;
int loopLength(void);

unsigned char convert(char input){
    ADCON0bits.CHS = input;
    for(shortDelay=0; shortDelay<10;shortDelay++){
        
    }
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
    ANSELbits.ANS1 = 1;     // ANS1 = analog pin
    ADCON0bits.CHS = 0;     // A/D Channel   
    ADCON0bits.ADON = 1;    // Activate A/Domvandlaren
    TRISC = 0x00;
    ADFM = 0;
    int y = 0;
    char out = 0;
    unsigned char notes[11] = {24,28,31,34,36,48,47,46,43,31,36};
    unsigned char melodyLength = sizeof(notes)/sizeof(notes[0]);
    unsigned char velo[4] = {100,60,80,70};
    while(1)
    {
        unsigned char note;
        for(y=0; y < melodyLength; y++)
        {
            int loop = 0;
            note = notes[y] + (convert(1)>>3);  // you can change note with 31 semitones
            if(note> MAX_NOTE){
                note = MAX_NOTE;
            }
            UART_Write(NOTE_ON);
            UART_Write(note);
            UART_Write(velo[(y % 4)]);
            for(loop = 0; loop < loopLength(); loop++ )
            { }
            
            PORTC = out;
            out++;
            UART_Write(NOTE_OFF);
            UART_Write(note);
            UART_Write(0);
            for(loop = 0; loop < loopLength(); loop++ )
            { }
        }
    }
    return;
}

int loopLength(void){
    unsigned char speed = 1;
    speed = convert(0);
    if(speed<2)
    {
        speed = 2;
    }
    return speed << 6;
}