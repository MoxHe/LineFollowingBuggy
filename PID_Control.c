/*
 pseudo code
................................
read digital values from sensors//interrupt
    determine position using position = value1*1000 +value2*2000 + value3*3000+value4*4000+value5*5000+value6*6000/value1+value2+value3+value4+value5+value6 
    proportional = position-3500
    integral+=proportional
    derivative = proportional - last_proportional 
---------------------------------------------
*/

#include "xc_config_settings.h"
#include "p18f8722.h"
#include "plib/delays.h"
#include "plib/timers.h"
//#include "stdio.h"


void configure(void);
void calculateSpeed(void);
void determinePowerDiff (void);

void stop(void);
void direction ( float speedA,  float speedB, char mode);
//void configure(void);
void manual(void);
volatile unsigned char value1,value2,value3,value4,value5,value6,last_proportional,sensor_value=0;
volatile unsigned char x=0;
 float motorLeft=0,motorRight=0,powerDifference=0;
float integral=0,derivative=0,proportional=0;
long int position=0;
unsigned char flag=0;
//----------

volatile unsigned char distance_flag = 0;
void config(void);
volatile int distance;
volatile int TIMER1_COUNTER = 0;
//-----------------


int main (void) { 
    configure();
    TRISD=0X00;
    TRISB=0XFF;
     OpenTimer0( TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_8);
     OpenTimer3( TIMER_INT_ON &
       T3_8BIT_RW &
       T3_SOURCE_INT &
       T3_PS_1_1 &
        
        T3_SYNC_EXT_OFF );
     WriteTimer3(253 );
     
    while(1) { 
        
        //sensor_value=(PORTB>>1) & 0b00111111;
        //LATD=sensor_value;
        
             
    
    /*while ((!PORTBbits.RB0) && (!PORTBbits.RB1) && (!PORTBbits.RB2) && (!PORTBbits.RB3) && (!PORTBbits.RB4) && (!PORTBbits.RB5)) {
       
        if (powerDifference>0) {
         direction(0,0,'r'); // 200,500 prevs    //0,200 prev //turn right
        }
        
        
        else {
            direction(0,0,'l'); //500,200 prevs // 200,0 prev// turn left
        } }*/
    
        // flag = 0;
        while ((!PORTBbits.RB1) && (!PORTBbits.RB2) && (!PORTBbits.RB3) && (!PORTBbits.RB4) && (!PORTBbits.RB5) && (!PORTBbits.RB6)) {
            if (position >-5 && position<5) {
                stop();
                flag=0;
            }
            
        }
        if (distance_flag) {
            while (!PORTBbits.RB1 && !flag) {
                direction(512,512,'r');
                
            }
            flag=1;
            
        }      
        
       
             switch (sensor_value) { 
                 case 0b00000001: position =5;break;
                 case 0b00000011: position = 4; break;
                 case 0b00000010: position = 3; break;
                 case 0b00000110: position = 2; break;
                 case 0b00000100: position = 1; break;
                 case 0b00001100: position=0;break;
                 case 0b00001000: position=-1;break;
                 case 0b00011000:position=-2;break;
                 case 0b00010000:position=-3;break;
                 case 0b00110000: position=-4;break;
                 case 0b00100000: position=-5;break;
             }


    proportional = position;
    integral+=proportional;
    derivative = proportional - last_proportional;
    last_proportional = proportional;
        determinePowerDiff();
        calculateSpeed(); 
      
}
return 0;}


void configure(void) { 
    TRISB=0xFF; //TRISB is input for sensor readings
    INTCONbits.GIE=1;
    INTCONbits.INT0IE=1; // pin<0>
    INTCON3bits.INT1IE=1; //pin<1>
    INTCON3bits.INT2IE=1; // pin<2>
    INTCON3bits.INT3IE=1; // pin<3>
    INTCONbits.RBIE=1; // RB<4:7>*/
     TRISE=0X00;
    TRISF=0X00;
    TRISA=0X00;
    TRISC=0X00;
    TRISG=0X00;
    TRISB=0XFF;
    LATFbits.LF0=1; //ENABLE
     LATFbits.LF1=0; //UNIPOLAR MOTOR A
    LATFbits.LF7=0; //UNIPOLAR MOTOR B
    OpenPWM5(249);
    OpenPWM4(249);
    TRISJbits.TRISJ0=0; // TRIGGER
     INTCONbits.INT0IE=1;
    INTCON2bits.INTEDG0=1;
    INTCONbits.PEIE=1;
    //PIE1bits.TMR1IE = 1;
    //PIE3bits.TMR4IE = 1;
    PIE2bits.TMR3IE = 1;
}

void determinePowerDiff (void) { 
    float Ki,Kd,Kp;
    Ki=0;
    Kd=0.1037; //old value =0.1 - 0.11//0.09 //0.101
    Kp=40.7; //old value=40//40.5
    powerDifference = integral*Ki + derivative*Kd + proportional*Kp;
    
    
}
void calculateSpeed(void) { 
    

    motorRight =  powerDifference;
    motorLeft= - powerDifference;
        
    motorRight = (motorRight*1.5)+705; //prevs 3.5 //inrease this guy
    motorLeft = (motorLeft*1.5)+665; // prev 3.5 //decrease this guy



       /* if (motorRight<0) { 
            motorRight=0;}
        if (motorLeft>700) {
            motorRight=700;
        }
        if (motorLeft<0) {
            motorLeft=0;
        }
        if (motorLeft>700) {
                motorLeft=700;
        }
        */

        direction(motorLeft,motorRight,'f');
}

void stop(void) {
     //LATFbits.LF0=0; //ENABLE
    SetDCPWM5(1023);
    SetDCPWM4(1023);
  
}

/*void configure(void) {
   // INTCONbits.GIE=1;
   // INTCONbits.INT0IE=1;
    //INTCON2bits.INTEDG0 =1;
    TRISE=0X00;
    TRISF=0X00;
    TRISA=0X00;
    TRISC=0X00;
    TRISG=0X00;
    TRISB=0XFF;
    LATFbits.LF0=1; //ENABLE
     LATFbits.LF1=0; //UNIPOLAR MOTOR A
    LATFbits.LF7=0; //UNIPOLAR MOTOR B
    OpenPWM5(249);
    OpenPWM4(249);
   // SetDCPWM1(1023);
    //SetDCPWM2(1023);
}*/


void direction (float speedA, float speedB, char mode) {
    if (mode=='l') { //left
        LATAbits.LA1=0; //DIRECTION MOTOR A
        LATCbits.LC3=1; // DIRECTION MOTOR B
    }
    else if (mode == 'r') { //right
        LATAbits.LA1=1; //DIRECTION MOTOR A
        LATCbits.LC3=0; // DIRECTION MOTOR B

    }
    else if (mode =='f') {  // forward
         LATAbits.LA1=1; //DIRECTION MOTOR A 
        LATCbits.LC3=1; // DIRECTION MOTOR B
    }
    else if (mode =='b') {
        LATAbits.LA1=0; //DIRECTION MOTOR A
         LATCbits.LC3=0; // DIRECTION MOTOR B
    }
    SetDCPWM4(speedA); // left motor
    SetDCPWM5(speedB); //right motor
}


void interrupt isr(void) { 
    
    INTCON3bits.INT1IF=0; // pin<0>
    INTCON3bits.INT2IF=0; //pin<1>
    INTCON3bits.INT3IF=0; // pin<2>
    
    INTCONbits.RBIF=0; // RB<4:7>
    
    /*value6 = PORTBbits.RB0;
         value5 = PORTBbits.RB1;
          value4 = PORTBbits.RB2; 
           value3 = PORTBbits.RB3;
            value2 = PORTBbits.RB4;
             value1 = PORTBbits.RB5;*/
    
    sensor_value=(PORTB>>1) & 0b00111111;
    
    LATD=sensor_value;
    
    
    
    
    //------
    
         if(INTCONbits.INT0IF ){
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
            if((distance<20)&&(distance>15))
               distance_flag=1;
            else      
                distance_flag=0;

        }
       if (PIR2bits.TMR3IF ){         //PIR1bits.TMR1IF      //PIR2bits.TMR3IF
            PIR2bits.TMR3IF = 0;             //PIR2bits.TMR3IF
            WriteTimer3(253);
            LATJbits.LATJ0=0;
            TIMER1_COUNTER++;
            
            if (TIMER1_COUNTER==1){
              // LATF = 0xFF;
               TIMER1_COUNTER = 0 ;
               LATJbits.LATJ0=1;    
            }   
    
       }}

