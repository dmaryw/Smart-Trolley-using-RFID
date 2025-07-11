#include <LPC21xx.H>
#include <string.h>
#include <stdio.h>
#include "lcd.h"
#include "uart.h"

unsigned int pen_amount = 0;
unsigned int book_amount = 0;

unsigned char penID[] =  "060067849F7A";
unsigned char bookID[] = "06006783BB59";

void display_amount(const char *type, unsigned int amount);
void get_RFID_UID(unsigned char *uid);


int main(void)
{
    char buf[20];
    unsigned char cardID[13];
    int i = 0;
    static int removeMode = 0;

    UART0_INIT();
    lcd_init();
    delay_milliseconds(100);

    lcd_cmd(0x80);
    lcd_str("SMART TROLLEY");
    lcd_cmd(0xC0);
    lcd_str("Initializing...");
    delay_seconds(2);

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    IODIR0 &= ~(SWITCH1 | SWITCH2);
    lcd_str("RFID Ready");

    while (1)
    {
        if ((IOPIN0 & SWITCH2) == 0)
        {
            delay_milliseconds(20);
            if ((IOPIN0 & SWITCH2) == 0)
            {
                removeMode = 1;
                lcd_cmd(0x01);
                lcd_str("Remove Mode ON");
                delay_milliseconds(1000);
                lcd_cmd(0x01);
                lcd_str("Scan to Remove");

                while ((IOPIN0 & SWITCH2) == 0);
                delay_milliseconds(200);
            }
        }

        if ((IOPIN0 & SWITCH1) == 0)
        {
            delay_milliseconds(20);
            if ((IOPIN0 & SWITCH1) == 0)
            {
                lcd_cmd(0x01);
                lcd_cmd(0x80);
                lcd_str("Total BOOK:");
                lcd_cmd(0xC0);
                sprintf(buf, "Rs %d", book_amount);
                lcd_str((unsigned char *)buf);
                delay_milliseconds(2000);

                lcd_cmd(0x01);
                lcd_cmd(0x80);
                lcd_str("Total PEN:");
                lcd_cmd(0xC0);
                sprintf(buf, "Rs %d", pen_amount);
                lcd_str((unsigned char *)buf);
                delay_milliseconds(2000);

                while ((IOPIN0 & SWITCH1) == 0);
                delay_milliseconds(200);

                lcd_cmd(0x01);
                lcd_str("Ready...");
            }
        }

        if (U0LSR & 0x01)
        {
            cardID[i++] = U0RBR;
            if (i >= 12)
            {
                cardID[12] = '\0';

                if (strcmp((char *)cardID, (char *)penID) == 0)
                {
                    if (removeMode)
                    {
                        if (pen_amount >= 50)
                            pen_amount -= 50;
                        lcd_cmd(0x01);
                        lcd_str("PEN Removed");
                        removeMode = 0;
                    }
                    else
                    {
                        pen_amount += 50;
                        lcd_cmd(0x01);
                        lcd_str("PEN Added");
                    }
                }
                else if (strcmp((char *)cardID, (char *)bookID) == 0)
                {
                    if (removeMode)
                    {
                        if (book_amount >= 50)
                            book_amount -= 50;
                        lcd_cmd(0x01);
                        lcd_str("BOOK Removed");
                        removeMode = 0;
                    }
                    else
                    {
                        book_amount += 50;
                        lcd_cmd(0x01);
                        lcd_str("BOOK Added");
                    }
                }
                else
                {
                    lcd_cmd(0x01);
                    lcd_str("Unknown Card");
                    removeMode = 0;
                }

                delay_milliseconds(1000);
                lcd_cmd(0x01);
                lcd_str("Ready...");
                i = 0; 
            }
        }
    }
}

void get_RFID_UID(unsigned char *uid)
{
    int i = 0;
    while (i < 12)
    {
        while (!(U0LSR & 0x01));  // Wait until data is received
        uid[i] = U0RBR;           // Read received character
        i++;
    }
    uid[12] = '\0';
}
void display_amount(const char *type, unsigned int amount)
{
    char buffer[20];

    lcd_cmd(0x01);  // Clear
    lcd_str((unsigned char *)type);
    delay_milliseconds(1000);

    lcd_cmd(0xC0);  // 2nd line
    sprintf(buffer, "Amount: Rs %d", amount);
    lcd_str((unsigned char *)buffer);
    delay_milliseconds(2000);
}
