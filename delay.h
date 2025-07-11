#include<LPC21XX.H>

typedef unsigned int U32;

void delay_seconds(U32);
void delay_milliseconds(U32);
void delay_microseconds(U32);

void delay_seconds(U32 seconds)
{
  T0PR  = 15000000 - 1;
  T0TCR = 0X01;
  while(T0TC<seconds);//waiting for  seconds Delay	
  T0TCR = 0X03;//reset the counters
  T0TCR = 0X00;//disable the counters
}

void delay_milliseconds(U32 ms)
{
  T0PR  = 15000 - 1;
  T0TCR = 0X01;
  while(T0TC<ms);//waiting for  milliseconds Delay	
  T0TCR = 0X03;//reset the counters
  T0TCR = 0X00;//disable the counters
}

void delay_microseconds(U32 us)
{
  T0PR  = 15 - 1;
  T0TCR = 0X01;
  while(T0TC<us);//waiting for  microseconds Delay	
  T0TCR = 0X03;//reset the counters
  T0TCR = 0X00;//disable the counters
}
