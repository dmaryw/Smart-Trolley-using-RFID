#include<LPC21XX.H>
#include"delay.h"
#define LCD_D 0xf<<20
#define RS 1<<17
#define RW 1<<18
#define E 1<<19
#define SWITCH1 1<<14
#define SWITCH2 1<<15

void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_str(unsigned char *s);

void lcd_init(void){
IODIR1 = LCD_D|RS|RW|E;
IOCLR1 = RW;
lcd_cmd(0x01);
lcd_cmd(0x02);
lcd_cmd(0x0c);
lcd_cmd(0x28);
lcd_cmd(0x80);
}

void lcd_cmd(unsigned char cmd){
IOCLR1 = LCD_D;
IOSET1 = (cmd&0xf0)<<16;
IOCLR1 = RS;
IOSET1 = E;
delay_milliseconds(2);
IOCLR1 =  E;
IOCLR1 = LCD_D;
IOSET1 = (cmd&0x0f)<<20;
IOCLR1 = RS;
IOSET1 = E;
delay_milliseconds(2);
IOCLR1 =  E;
		   
}

void lcd_data(unsigned char data){
IOCLR1 = LCD_D;
IOSET1 = (data&0xf0)<<16; 

IOSET1 = RS;
IOSET1 = E;
delay_milliseconds(2);
IOCLR1 =  E;
IOCLR1 = LCD_D;
IOSET1 = (data&0x0f)<<20;
IOSET1 = RS;
IOSET1 = E;
delay_milliseconds(2);
IOCLR1 =  E;
}

void lcd_str(unsigned char *s){
while(*s)
lcd_data(*s++);
}
