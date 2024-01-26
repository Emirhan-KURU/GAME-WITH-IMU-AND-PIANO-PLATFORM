#include <16F877A.h>
#device ADC=8

#FUSES WDT                    //Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv= pin_C7, parity = N, stop = 1)

//LCD ayarlarý
#define use_portb_lcd TRUE
#include "LCD420.c"

#use fast_io(D)
//Register 
#byte OPTION_REG = 0x81
#byte TIMER0 = 0x01
#byte T1CON = 0x10
#byte INTCON = 0x0B
#bit GIE = INTCON.7
#bit PEIE = INTCON.6
#bit TOIE = INTCON.5
#bit TOIF = INTCON.2
#byte ADCON0 = 0x1F
#byte ADCON1 = 0x9F
#bit SET_ADC = ADCON0.3

#byte PIE1 = 0x8C
#byte PIR1 = 0x0C  
#bit  RCIF = PIR1.5
#bit  TXIF = PIR1.4


#define zarbir PIN_C0
#define zariki PIN_C1


int zar_tablo[7] = {0x00,0x08,0x22,0x1C,0x55,0x5D,0x77};
int pul_sayisi = 5;
int pul_sayisi_1,pul_sayisi_2;
int count_timer0 =0;
char imu =0;
unsigned int adc_value_x = 128;
unsigned int adc_value_y = 128;
int zar1 = 0,zar2 =0;
int devamet =0,loop;
int Cik = 0,zar_timer=1;
char lcdState = 1;

void kazanan()
{
            Lcd_gotoxy(1,1);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
            Lcd_gotoxy(1,2);
            Lcd_putc("x     PLAYER       x");
            Lcd_gotoxy(1,3);
            Lcd_putc("x     KAZANDI      x");
            Lcd_gotoxy(1,4);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
   if(pul_sayisi_1 == 0)
   int kazanan = 2;
   if(pul_sayisi_2 == 0)
   kazanan = 1;
   Lcd_gotoxy(14,2);
   printf(lcd_putc,"%d",kazanan);
            
}
void play_piano()
{
   Lcd_gotoxy(1,1);
   Lcd_putc("    #       #       ");
   Lcd_gotoxy(1,2);
   Lcd_putc("   ##   ##  #  #  # ");
   Lcd_gotoxy(1,3);
   Lcd_putc(" # ##   ##  ## #  # ");
   Lcd_gotoxy(1,4);
   Lcd_putc("## ## # ### ##### # ");
   delay_ms(100);
   Lcd_gotoxy(1,1);
   Lcd_putc("     #    # #       ");
   Lcd_gotoxy(1,2);
   Lcd_putc("#   ## #  ###      #");
   Lcd_gotoxy(1,3);
   Lcd_putc("##  ## ## ### # # ##");
   Lcd_gotoxy(1,4);
   Lcd_putc("### ##### ### ### ##");
   delay_ms(100);
   Lcd_gotoxy(1,1);
   Lcd_putc("    #       #       ");
   Lcd_gotoxy(1,2);
   Lcd_putc("   ##   ##  #  #  # ");
   Lcd_gotoxy(1,3);
   Lcd_putc(" # ##   ##  ## #  # ");
   Lcd_gotoxy(1,4);
   Lcd_putc("## ## # ### ##### # ");
   delay_ms(100);
   Lcd_gotoxy(1,1);
   Lcd_putc("                    ");
   Lcd_gotoxy(1,2);
   Lcd_putc("                    ");
   Lcd_gotoxy(1,3);
   Lcd_putc("     #      #     # ");
   Lcd_gotoxy(1,4);
   Lcd_putc("### ## #  ### ##  ##");
   delay_ms(100);                  
}
#priority rda,timer0

#int_rda
void seri_kesme()
{
  
   imu = getc();
   if(4 == lcdState)
   restart_wdt();
   RCIF = 0;
   GIE = 1;
}

#int_timer0
void kesme_timer0()
{
   INTCON = 0x65;
   count_timer0++;
   if(2 == count_timer0)
   {
   SET_ADC = 0; 
   delay_us(20);
   adc_value_x = read_adc();
   delay_us(20);
   SET_ADC = 1; 
   delay_us(20);
   adc_value_y = read_adc();
   delay_us(20);

   count_timer0=0;
  
    if(lcdState == 4)
    {
      if(adc_value_x < 50)
      { 
      zar_timer=0;
      loop =0;
      lcdState = 2;
      
      }
    }     
   }        
   TOIF = 0;
   GIE  = 1;
}
void main()
{
   lcd_init();
   
   ADCON0 = 0xC5;
   ADCON1 = 0x04;

   OPTION_REG = OPTION_REG && 0xD7;
   TIMER0 = 0;
  
   T1CON = T1CON || 0x35;
   set_timer1(0);
      
   INTCON = 0xE1;
   PIE1 = 0x20;
   setup_wdt(WDT_2304MS);

   /*
    *    0 = ayarlar
    *    1 = ana menu
    *    2 = oyun
    *    3 = piano
    *    4 = oyun basla
   */
   
   while(TRUE)
   {
       switch(lcdState)
      {
       case 0: //ayarlar
         {
         int choose = 1, pul_count = 0, cursor = 0, loop = 1, writeLCD = 1;
            Lcd_gotoxy(1,1);
            Lcd_putc("     <AYARLAR>      ");
            Lcd_gotoxy(1,2);
            Lcd_putc(" PUL SAYISI =       ");
            Lcd_gotoxy(1,3);
            Lcd_putc(" BLINK -->        ");
            Lcd_gotoxy(1,4);
            Lcd_putc("<Cikis     Degistir>");
            while (loop)
                {restart_wdt();
                    if (writeLCD)
                    {
                        writeLCD = 0;
                        if (choose == 1)
                        {
                            Lcd_gotoxy(1,2);
                            Lcd_putc(">");
                            Lcd_gotoxy(12,2);
                            if (pul_count)
                            {                           
                               printf(lcd_putc," = %d     ",pul_sayisi);
                            }
                            else
                            {                           
                               printf(lcd_putc," = %d     ",pul_sayisi);
                            }
                            Lcd_gotoxy(1,3);
                            Lcd_putc(" ");
                            Lcd_gotoxy(11,3);
                            if (cursor) Lcd_putc("KAPALI    ");
                            else Lcd_putc("ACIK      ");
                            Lcd_gotoxy(1,2);
                        }
                        if (choose == 2)
                        {
                            Lcd_gotoxy(1,2);
                            Lcd_putc(" ");
                            Lcd_gotoxy(12,2);
                            if (pul_count) printf(lcd_putc," = %d     ",pul_sayisi);
                            else printf(lcd_putc," = %d     ",pul_sayisi);
                            Lcd_gotoxy(1,3);
                            Lcd_putc(">");
                            Lcd_gotoxy(11,3);
                            if (cursor) Lcd_putc("KAPALI    ");
                            else Lcd_putc("ACIK      ");
                            Lcd_gotoxy(1,3);
                        }
                    }
                    if (adc_value_x > 200)
                    {
                        while(adc_value_x > 200)
                        {
                           delay_ms(10);
                        }
                        if (choose == 1) choose = 2;
                        else choose = 1;
                        writeLCD = 1;
                    }

                    if (adc_value_y < 50)
                    {
                        while(adc_value_y < 50)
                        {
                            delay_ms(10);
                        }                
                     
                        if (choose == 1)
                        {
                            if (pul_count)
                            {
                                pul_count = 0;
                                if(pul_sayisi<=20)
                                pul_sayisi++;
                                if(pul_sayisi>20)
                                pul_sayisi=5;
                            }
                            else
                            {
                                pul_count = 1;

                            }
                        }
                        if (choose == 2)
                        {
                            if (cursor) 
                            {
                                cursor = 0;
                                lcd_setcursor_vb(TRUE, TRUE);
                            }
                            else
                            {
                                cursor = 1;
                                lcd_setcursor_vb(FALSE, FALSE);
                            }
                        }
                        writeLCD = 1;
                    }

                    if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        writeLCD = 0;
                        lcdState = 1;
                        
                    }
                    write_eeprom(0,pul_sayisi);
                }
                break;
         } //case0
         case 1: //ANA MENU
         {
            short loop = 1;
            Lcd_gotoxy(1,1);
            Lcd_putc("     <ANA MENU>     ");
            Lcd_gotoxy(1,2);
            Lcd_putc("Menuleri gezmek icin");
            Lcd_gotoxy(1,3);
            Lcd_putc("Joystigi  kullanin. ");
            Lcd_gotoxy(1,4);
            Lcd_putc("<AYARLAR       OYUN>");
            while (loop)
                {
                    if (adc_value_x > 200)
                    {
                        while(adc_value_x > 200)
                        {
                            delay_ms(10);
                        }
                        loop = 0;
                        lcdState = 2;
                    }
                    if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        lcdState = 0;
                    }
                }
                break;           
         } //case 1
         case 2:
         {
         int choose = 1, loop = 1, writeLCD = 1, enter =0;
            Lcd_gotoxy(1,1);
            Lcd_putc("    <BARBUT GAME>    ");
            Lcd_gotoxy(1,2);
            Lcd_putc(" YENI OYUN           ");
            Lcd_gotoxy(1,3);
            Lcd_putc(" DEVAM ET            ");
            Lcd_gotoxy(1,4);
            Lcd_putc("<ANA MENU     PIANO>");
             while (loop){
             restart_wdt();
             if(0 == Cik)
             {
                    if (adc_value_x > 200)
                    {
                        while(adc_value_x > 200)
                        {
                            delay_ms(10);
                        }
                        loop = 0;
                        lcdState = 3;
                    }

                    if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        lcdState = 1;                        
                    }
                    if(adc_value_y < 50)
                    {
                     Cik =1;
                     enter = 1;
                    }
             }
             if(Cik == 1)
             {
             if (writeLCD)
                    {
                        writeLCD = 0;
                        if (choose == 1)
                        {
                            Lcd_gotoxy(1,2);
                            Lcd_putc(">YENI OYUN           ");
                            Lcd_gotoxy(1,3);
                            Lcd_putc(" DEVAM ET            ");
                            Lcd_gotoxy(1,2);
                        }
                        if (choose == 2)
                        {
                            Lcd_gotoxy(1,2);
                            Lcd_putc(" YENI OYUN           ");
                            Lcd_gotoxy(1,3);
                            Lcd_putc(">DEVAM ET            ");
                            Lcd_gotoxy(1,3);
                        }
                        /*if(choose == 3)
                        {
                           Lcd_gotoxy(1,2);
                           Lcd_putc(">AYARLAR             ");
                           Lcd_gotoxy(1,3);
                           Lcd_putc("                     ");                     
                        }*/
                    }
                    if (adc_value_x > 200)
                    {
                        while(adc_value_x > 200)
                        {
                           delay_ms(10);
                        }
                        /*if (choose == 1) choose = 2;
                        else choose = 1;*/
                        choose++;
                        if(3 == choose)
                        choose =1;
                        writeLCD = 1;
                        enter = 0;
                    }
                    if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        writeLCD = 0;
                        lcdState = 2;
                        Cik = 0;
                    }
                    if(adc_value_y < 50 && enter == 0)
                    {
                        while(adc_value_y < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        writeLCD = 1;
                        Cik = 0;
                        /*if(3 == choose)
                        lcdState = 7;*/
                        if(2 == choose)
                        devamet=1;
                        else devamet=0;
                        lcdState = 4;
                    }
                      }
                        }
                break;
         } //case2
         case 3:   //piano
         {restart_wdt();
                  int loop = 1, writeLCD = 1, Cik =0;
            Lcd_gotoxy(1,1);
            Lcd_putc("       <PIANO>       ");
            Lcd_gotoxy(1,2);
            Lcd_putc(" CALMAK ICIN       ");
            Lcd_gotoxy(1,3);
            Lcd_putc(" GIRIS YAPINIZ.     ");
            Lcd_gotoxy(1,4);
            Lcd_putc("<OYUN               ");
             while (loop){
             restart_wdt();

             if(0 == Cik)
             {
                    if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        lcdState = 2;                        
                    }
                    if(adc_value_y < 50)
                    {   
                     Cik =1;
                    }
             }
             if(Cik == 1)
             {
              play_piano();
              putc(1);
             if (writeLCD)
                    {
                        writeLCD = 0;                      
                    }
             
                    if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        writeLCD = 0;
                        lcdState = 3;
                        Cik = 0;
                        putc(0);
                    }
                      }
                        }
                break;
         }
            case 4: // oyuna basla
            {restart_wdt();
             set_tris_d(0x00);
             output_d(0);
             //int choose = 1, pul_count = 0, cursor = 0, loop = 1, writeLCD = 1;
             int oyuncu=1,game=0;
             loop =1,zar_timer=1;
             if(devamet ==0 || pul_sayisi_1 ==0 || pul_sayisi_2 ==0)
             {
             pul_sayisi_1 = read_eeprom(0);
            pul_sayisi_2 = read_eeprom(0);  
             }
             else if(devamet == 1)
             {
            pul_sayisi_1 = read_eeprom(1);
            pul_sayisi_2 = read_eeprom(2);
            oyuncu       = read_eeprom(3);
            if(oyuncu == 2) oyuncu = 1;
            else oyuncu = 2;
             
             }
            Lcd_gotoxy(1,1);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
            Lcd_gotoxy(1,2);
            Lcd_putc("x      BARBUT      x");
            Lcd_gotoxy(1,3);
            Lcd_putc("x       GAME       x");
            Lcd_gotoxy(1,4);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
            for(int i =0;i<8;i++)
            {
            if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        lcdState = 2;  
                       
                    }
            Lcd_gotoxy(1,1);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
            Lcd_gotoxy(1,4);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
            delay_ms(50);
            Lcd_gotoxy(1,1);
            Lcd_putc("////////////////////");
            Lcd_gotoxy(1,4);
            Lcd_putc("////////////////////");
            delay_ms(50);
            restart_wdt();
            }
            Lcd_gotoxy(1,1);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
            Lcd_gotoxy(2,2);
            Lcd_putc("1 PLAYER   2 PLAYER");
            Lcd_gotoxy(1,3);
            printf(lcd_putc,"x   %d          %d    x",pul_sayisi_1,pul_sayisi_2);
            Lcd_gotoxy(1,4);
            Lcd_putc("xxxxxxxxxxxxxxxxxxxx");
            
                while (loop)
                {
                  //game function
                  restart_wdt();
                  Lcd_gotoxy(1,3);
                  printf(lcd_putc,"    %d          %d     ",pul_sayisi_1,pul_sayisi_2);
                  
                  if(oyuncu == 1)
                  {
                  lcd_gotoxy(2,3);
                  Lcd_putc(">");
                  lcd_gotoxy(14,3);
                  Lcd_putc(" ");
                  }
                  else
                  {
                  lcd_gotoxy(14,3);
                  Lcd_putc(">");
                  lcd_gotoxy(2,3);
                  Lcd_putc(" ");
                  }
                  
                 
                    if (adc_value_x < 50)
                    {
                        while(adc_value_x < 50)
                        {
                           delay_ms(10);
                        }
                        loop = 0;
                        lcdState = 2;                        
                    }
                  if(imu >= 15)//imu)
                  {  
                     
                     zar1 = (int)(TIMER0/50) + 1;
                     zar2 = (int)(get_timer1()/13100) + 1;
                        
                     if(1 == oyuncu)
                     {
                        if(zar1 == zar2)
                        {
                           if(zar1 == 1 || zar1 == 2 || zar1 == 4)
                           {
                           pul_sayisi_2 += pul_sayisi_1;
                           pul_sayisi_1 =0;
                           }
                           else
                           {    
                           pul_sayisi_1 += pul_sayisi_2;
                           pul_sayisi_2 =0;
                           }
                           } // eþit zar
                         else
                         { 
                           if((zar1 + zar2)% 2==1)
                           {
                           pul_sayisi_1++;
                           pul_sayisi_2--;}
                           else
                           {
                           pul_sayisi_1--;
                           pul_sayisi_2++;}
                         }                           
                        } //player 1
                        if(2 == oyuncu)
                     {
                        if(zar1 == zar2)
                        {
                           if(zar1 == 1 || zar1 == 2 || zar1 == 4)
                           {
                           pul_sayisi_1 += pul_sayisi_2;
                           pul_sayisi_2 =0;
                           }
                           else
                           {    
                           pul_sayisi_2 += pul_sayisi_1;
                           pul_sayisi_1 =0;
                           }
                           } // eþit zar
                           else
                         { 
                           if((zar1 + zar2)% 2==1)
                           {
                           pul_sayisi_2++;
                           pul_sayisi_1--;}
                           else
                           {
                           pul_sayisi_2--;
                           pul_sayisi_1++;}
                         }
                        
                           
                        } //player 2
                        for(int r =0;r<18;r++)
                     {
                     output_high(zarbir);
                     output_d(0xFF);
                     delay_ms(20);
                     output_low(zarbir);
                     
                     output_high(zariki);
                     output_d(0xFF);
                     delay_ms(20);
                     output_low(zariki);
                     
                     output_high(zarbir);
                     output_d(0x00);
                     delay_ms(20);
                     output_low(zarbir);
                     
                     output_high(zariki);
                     output_d(0x00);
                     delay_ms(20);
                     output_low(zariki);
                     restart_wdt();
                     if(0 == zar_timer)
                     break;
                     }
                     
                     for(r =0;r<75;r++)
                     {
                     output_high(zarbir);
                     output_d(zar_tablo[zar1]);
                     delay_ms(20);
                     output_low(zarbir);
                     
                     output_high(zariki);
                     output_d(zar_tablo[zar2]);
                     delay_ms(20);
                     output_low(zariki);
                     imu =0;
                     if(oyuncu == 1)
                     oyuncu = 2;
                     else oyuncu = 1;
                     restart_wdt();
                     if(0 == zar_timer)
                     break;
                     } 
                     write_eeprom(1,pul_sayisi_1);
                     write_eeprom(2,pul_sayisi_2);
                     write_eeprom(3,oyuncu);
                     } //imu
                     
                   if(pul_sayisi_1 == 0 || pul_sayisi_2 == 0)
                  {
                     if(0 == zar_timer)
                     break;
                     kazanan();
                     for(int u =0;u<30;u++)
                     {
                     restart_wdt();
                     delay_ms(10);
                     }
                     loop = 0;
                     lcdState = 2; 
                     game=1;
                     Cik = 1;
                  }
                  }        
            break;
            }
   } //switch
   }
}
