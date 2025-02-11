#include <16f886.h>// micro
#fuses LP,NOWDT,NOPUT,NOLVP,NOPROTECT,BROWNOUT// conf
#use delay(clock=4M) //reloj si se emplea uno de mas de 4M se usa otra instruccion qno es LP
#use RS232(BAUD=9600, BITS=8,PARITY=N,XMIT=PIN_C6,RCV=PIN_C7)
#include <stdio.h>
#use fast_io(B) //se va usar el puerto B

//--------LCD-----------------------
#define LCD_DB4 PIN_A0
#define LCD_DB5 PIN_A1
#define LCD_DB6 PIN_A2
#define LCD_DB7 PIN_A3
#define LCD_RS PIN_A4
#define LCD_E PIN_A5

#include <LCD_16x2.c>


 char dato_rx;
 int optico;
 int encoder;
 int banderaopt;
 
 

 
 
 void manual();
 void automatico();
 void finalizado();
 

  
#INT_EXT
void interrupcion_rb(){
      encoder++; 
}

 
 
 
void main()
{

   port_b_pullups(true);
   
   enable_interrupts(INT_EXT);
   ext_int_edge(H_TO_L);
   enable_interrupts(GLOBAL);
   
  

   lcd_init();
   set_tris_b(0b00010001); // 0 salidas / 1 entrada 7-0
    output_b(0b00000000); // todo apagado
    
     lcd_gotoxy(6,1); //columna, fila
      lcd_putc("FINAL!!");
       lcd_gotoxy(1,2);
      lcd_putc("ELTC DE POTENCIA");
      delay_ms(1500);
      lcd_clear();
      lcd_gotoxy(3,1);
      lcd_putc("Modo de");
      lcd_gotoxy(3,2);
      lcd_putc("Funcionamiento");
      delay_ms(1500);
      
   while(true)
   {
         if(kbhit()>0){
               dato_rx=getc();
               
            if(dato_rx =='1'){
              manual();
            }
            
            if(dato_rx == '2'){
               automatico();
            }
               
        }
   
      }
      
}



void manual()
{
    lcd_clear();
    lcd_gotoxy(6,1);
    lcd_putc("Modo:");
    lcd_gotoxy(7,2);
    lcd_putc("Manual");
    delay_ms(1000);
    lcd_clear();
    optico=0;
    encoder=0;
    banderaopt=0;
    while(true)
    {
       lcd_gotoxy(1,1);
       lcd_putc("S.optico:");
       lcd_gotoxy(11,1);
       printf(lcd_putc, "%u", optico);
  
       lcd_gotoxy(1,2);
       lcd_putc("Encoder:");
       lcd_gotoxy(11,2);
       printf(lcd_putc, "%u", encoder);
       
         if(input(PIN_B4) == 1 && banderaopt == 0)
       {
         banderaopt=1;
       }
       
         if(input(PIN_B4) == 0 && banderaopt == 1)
       {
         optico++;
         
         banderaopt =0;
       }
     
       

       
       if(kbhit()>0){
               dato_rx=getc();
               
            if(dato_rx =='a'){
               output_b(0b00000000); 
            }
            
            if(dato_rx == 'b'){
               output_b(0b00000010); 
            }
            
            if(dato_rx == 'c'){
               finalizado();
         }
       }
    }
}


void automatico()
{
    output_b(0b10000000); 
}


