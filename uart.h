void UART0_INIT(void);
void UART0_TX(unsigned char);
void UART0_SendString(unsigned char *str);
unsigned char UART0_RX(void);
#define THRE 5
#define RDR 0

unsigned char UART0_RX(void){
 while(!((U0LSR >> RDR)&1));
 return U0RBR;
}

void UART0_TX(unsigned char c)
{
    while (!(U0LSR & 0x20));
    U0THR = c;
}

void UART0_INIT(void){
PINSEL0 |= 0X05;
U0LCR = 0X83;
U0DLL = 97;
U0LCR = 0X03;
}
void UART0_SendString(unsigned char *str)
{
    while(*str)
    {
        UART0_TX(*str);
        str++;
    }
}
