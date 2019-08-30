#include "xc_confi_settings.h"
#include"plib/delays.h"
#include"plib/timers.h"

void config(void);
volatile int distance;

main()
{
    config();
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_8);
    TRISAbits.TRISA4=0;
    LATAbits.LATA4=1;
    TRISF=0;
    TRISJbits.TRISJ0=0;
    TRISBbits.TRISB0=1;
    TRISDbits.TRISD0=0;
   
    while(1)
    {
        LATJbits.LATJ0=1;
        Delay1TCYx(25);
        LATJbits.LATJ0=0;
        Delay1KTCYx(25);
        LATF=distance; 
    }
}

void interrupt ISR(void)
{
        INTCONbits.INT0IF=0;
        if(INTCON2bits.INTEDG0==1)
        {
            INTCON2bits.INTEDG0=0;
            WriteTimer0(0);
        }
        else
        {
            INTCON2bits.INTEDG0=1;
            distance=ReadTimer0()*8*0.4/58;
        }
        if(distance>20)
            LATDbits.LATD0=0;
        else      
            LATDbits.LATD0=1;
    }

void config(void)
{
    INTCONbits.GIE=1;
    INTCONbits.INT0IE=1;
    INTCON2bits.INTEDG0=1;
    INTCONbits.PEIE=1;
}