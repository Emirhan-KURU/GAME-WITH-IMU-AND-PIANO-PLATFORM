#include <16F877A.h>
#device ADC=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv= pin_C7, parity = N, stop = 1)

#define La PIN_B0
#define Si PIN_B1
#define D0 PIN_B2
#define Re PIN_B3
#define Mi PIN_B4
#define Fa PIN_B5
#define Sol PIN_B6
#define La2 PIN_B7

#define Hoparlor PIN_E0

#byte T1CON = 0x10
#byte TMR1L = 0x0E
#byte TMR1H = 0x0F

#byte INTCON = 0x0B
#bit GIE = INTCON.7
#bit PEIE = INTCON.6

#byte PIE1   = 0x8C
#byte PIR1   = 0x0C  
#bit  TMR1E  = PIE1.0 
#bit  TMR1IF = PIR1.0
#bit  RCIF   = PIR1.5
#bit  TXIF   = PIR1.4


//Imu registers
#use i2c(Master,Fast,sda=PIN_C4,scl=PIN_C3)
#define W_DATA         0xD0 
#define R_DATA         0xD1
#define PWR_MGMT_1     0x6B
#define PWR_MGMT_2     0x6C
#define CONFIG         0x1A 
#define ACCEL_CONFIG   0x1C
#define ACCEL_XOUT_H   0x3B
#define ACCEL_XOUT_L   0x3C



void MPU6050_write(int add, int data)
{
   i2c_start();
   i2c_write(W_DATA);
   i2c_write(add);
   i2c_write(data);
   i2c_stop();
}
      
int16 MPU6050_read(int add)
{
   int retval;
   i2c_start();
   i2c_write(W_DATA);
   i2c_write(add);
   i2c_start();
   i2c_write(R_DATA);
   retval = i2c_read(0);
   i2c_stop();
   return retval;
}

void MPU6050_init()
{
   MPU6050_write(PWR_MGMT_1, 0x80);
   delay_ms(100);
   MPU6050_write(PWR_MGMT_2 , 0x00);
   delay_ms(100);
   MPU6050_write(CONFIG     , 0x01);
   delay_ms(10);
   MPU6050_write(ACCEL_CONFIG, 0x98);
}

float MPU6050_get_Ax()
{
   signed int8 A_data_x[2];
   signed int16 accel_value_x;
   A_data_x[0] = MPU6050_read(ACCEL_XOUT_H);
   A_data_x[1] = MPU6050_read(ACCEL_XOUT_L);
   accel_value_x = make16(A_data_x[0], A_data_x[1]);
   float acx = (float)accel_value_x/(float)16384;
   return acx;
}

int Ax,gelen =0;

#priority rda,timer1

#int_timer1
void kesme_timer1()
{
   set_timer1(63535);
   TMR1IF = 0;
   GIE = 1;   
}

#int_rda
void serial_kesme()
{
   gelen = getc();   
   RCIF = 0;
   GIE = 1;
}

void main()
{
   MPU6050_init();
   delay_us(20);
   
   T1CON = T1CON || 0x07;
   set_timer1(63535);
   
   INTCON = INTCON || 0xC0;
   PIE1    = PIE1  || 0x21; 

   while(TRUE)
   {   
   sleep();
   Ax = MPU6050_get_Ax();
   putc(Ax);
    
     while(gelen)
     {
     if(input(La)==0)
      {         
         output_high(Hoparlor);
         delay_us(1140); //1136
         output_low(Hoparlor); 
         delay_us(1140);
      }
      if(input(Si) == 0)
      {
      
         output_high(Hoparlor);
         delay_us(1020); //1012
         output_low(Hoparlor); 
         delay_us(1020);
      }
      if(input(D0) == 0)
      {
         output_high(Hoparlor);
         delay_us(960);//956
         output_low(Hoparlor); 
         delay_us(960);
      }
      if(input(Re)== 0)
      {
         output_high(Hoparlor);
         delay_us(860);//852
         output_low(Hoparlor); 
         delay_us(860);
      }
      if(input(Mi)== 0)
      {
         output_high(Hoparlor);
         delay_us(760);//768
         output_low(Hoparlor); 
         delay_us(760);
      }
      if(input(Fa)==0)
      {
         output_high(Hoparlor);
         delay_us(720);//716
         output_low(Hoparlor); 
         delay_us(720);
      }
      if(input(Sol)== 0)
      {
         output_high(Hoparlor);
         delay_us(640);
         output_low(Hoparlor); 
         delay_us(640);
      }
      if(input(La2)==0)
      {
         output_high(Hoparlor);
         delay_us(560);//568
         output_low(Hoparlor); 
         delay_us(560);
      }
      output_low(Hoparlor);

   }   
   }  
}

