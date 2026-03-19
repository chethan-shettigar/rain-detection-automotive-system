#ifndef LCD_HEADER_H_
#define LCD_HEADER_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define lcdport PORTA
#define RS_HIGH PORTD |= (1 << 0)
#define RS_LOW PORTD &= ~(1 << 0)
#define EN_HIGH PORTD |= (1 << 1)
#define EN_LOW PORTD &= ~(1 << 1)

void lcd_cmd(unsigned char cmd)
{
    lcdport = cmd;
    RS_LOW;
    EN_LOW;
    _delay_ms(1);
    EN_HIGH;
}

void lcd_data(unsigned char data)
{
    lcdport = data;
    RS_HIGH;
    EN_LOW;
    _delay_ms(1);
    EN_HIGH;
}

void lcd_inti()
{
    DDRA = 0XFF;
    DDRD |= (1 << 1);
    DDRD |= (1 << 0);
    lcd_cmd(0X33);
    lcd_cmd(0X02);
    lcd_cmd(0X01);
    lcd_cmd(0X0C);
    lcd_cmd(0X06);
    lcd_cmd(0X80);
}

void lcd_str(char *ptr)
{
    int i;
    for (i = 0; ptr[i] != 0; i++)
    {
        lcd_data(ptr[i]);
    }
}

void lcd_num(unsigned long long int num)
{
    if (num)
    {
        lcd_num(num / 10);
        lcd_data(num % 10 + 0X30);
    }
}

#endif /* LCD_HEADER_H_ */
