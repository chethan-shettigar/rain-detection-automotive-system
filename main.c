#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

int flag, flag1 = 0, flag2 = 0, flag3 = 0, o = 0;

void delay(void) // timer of 4 sec FOR MOTOR
{
    long int i;
    for (i = 0; i <= 1; i++) // 4SEC ON [612]
    {
        TCCR0 = 0X04;
        TCNT0 = 255;
        while ((TIFR & (1 << TOV0)) == 0)
            ;
        TCCR0 = 0X00;
        TIFR = 0X01;
    }
}

void delay1(void) // timer of 4 sec FOR wiper MOTOR
{
    long int j;

    for (j = 0; j <= 1; j++) // 4SEC ON [612]
    {
        TCCR2 = 0X04;
        TCNT2 = 150;
        while ((TIFR & (1 << TOV2)) == 0)
            ;
        TCCR2 = 0X00;
        TIFR = 0X01;
    }
}

void motor(void) // motor pgm
{
    lcd_cmd(0X01); // lcd clear cmd
    lcd_str("<Window_Closing>");
    _delay_ms(100);
    PORTC |= (1 << 6);
    delay();
    lcd_str("<Window_Closing>");
    _delay_ms(3000);
    lcd_cmd(0X01);
    // window closed
    lcd_str(" <Window_Closed>");
    _delay_ms(2000);
    PORTC |= (1 << 5);
    PORTC &= ~(1 << 7);
    flag = 1;
}

void manual(void) // manual mode
{
    PORTC &= ~(1 << 6);
    lcd_cmd(0X01);
    lcd_str(" <Manual_Mode>");
    _delay_ms(1000);
}

void rain_detect(void) // rain detect
{
    lcd_cmd(0X01);
    lcd_str(" <Rain Detected>");
    _delay_ms(900);
}

void automatic(void) // automatic mode
{
    PORTC |= (1 << 7);
    PORTC &= ~(1 << 6);
    PORTC &= ~(1 << 5);
    flag = 0;
    lcd_cmd(0X01);
    lcd_str("<Automatic_Mode>");
    lcd_cmd(0xAA);
    lcd_str("  NO_Rain");
    _delay_ms(1000);
}

void wiper(void) // wiper
{
    lcd_cmd(0X01);
    lcd_str("   <Wiper ON>");
    _delay_ms(2000);
}

void temp(void) // temp
{
    SFIOR = 0;
    ACSR = 0;
    if ((ACSR & (1 << ACO)))
    {
        lcd_cmd(0X01);
        lcd_str("  <normal temp>");
        _delay_ms(2000);
    }
    else
    {
        lcd_cmd(0X01);
        lcd_str("  <temp raised>");
        _delay_ms(2000);
        lcd_cmd(0XAA);
        lcd_str("<increase AC>");
        _delay_ms(2000);
    }
}

int main(void) // main pgm
{
    lcd_inti();
    DDRB |= (3 << 5);
    DDRB &= ~(3 << 6);
    PORTB |= (3 << 6);

    DDRC |= (15 << 4);
    DDRC &= ~(15 << 0);
    PORTC |= (15 << 0);

    SFIOR = 0;
    ACSR = 0;
    while (1)
    {
        if ((PINB & (1 << 7)) == 0)
        {
            if (o == 0)
            {
                for (o = 0; o <= 1; o++)
                {
                    lcd_cmd(0X01);
                    lcd_str("  <Engine ON>");
                    _delay_ms(2000);
                }
            }
            o = 1;
            if ((PINC & (1 << 1)) == 0) // Manula window open/close
            {
                PORTC &= ~(1 << 4);
                manual();

                PORTB &= ~(1 << 5);
                if ((PINC & (1 << 0)) == 0)
                {
                    PORTB ^= (1 << 5);
                    delay();
                    wiper();
                    temp();
                }
                if ((PINC & (1 << 2)) == 0) // manual window close switch
                {
                    flag3 = 1;
                    if (flag == 0)
                    {
                        motor();
                    }
                    else
                    {
                        PORTC &= ~(1 << 6);
                        PORTC &= ~(1 << 7);
                        PORTC |= (1 << 5);
                        lcd_cmd(0X01);
                        lcd_str(" <window status>");
                        _delay_ms(1000);
                        lcd_cmd(0XAA);
                        lcd_str("   Closed");
                        _delay_ms(2000);
                        if (flag3 == 1)
                        {
                            if ((ACSR & (1 << ACO)))
                            {
                                lcd_cmd(0X01);
                                lcd_str("  <normal temp>");
                                _delay_ms(2000);
                            }
                            else
                            {
                                lcd_cmd(0X01);
                                lcd_str("  <temp raised>");
                                _delay_ms(2000);
                                lcd_cmd(0XAA);
                                lcd_str("<increase AC>");
                                _delay_ms(2000);
                            }
                        }
                        else
                        {
                            manual();
                        }
                    }
                }
                else
                {
                    lcd_cmd(0X01);
                    flag = 0;
                    PORTC &= ~(1 << 6);
                    PORTC |= (1 << 7);
                    PORTC &= ~(1 << 5);
                    flag3 = 0;
                    // window status open
                }
            }
            else // auto
            {
                PORTB &= ~(1 << 5);
                PORTC |= (1 << 4);
                _delay_ms(1000);
                lcd_cmd(0X01);
                lcd_str("<Automatic_Mode>");
                _delay_ms(1000);
                if ((PINC & (1 << 0)) == 0) // Automatic window open/close
                {
                    PORTB ^= (1 << 5);
                    delay1();
                    if (flag == 0)
                    {
                        rain_detect();
                        motor();
                        wiper();
                        flag = 1;
                    }
                    else
                    {
                        PORTC &= ~(1 << 6);
                        PORTC &= ~(1 << 7);
                        PORTC |= (1 << 5);
                        wiper();
                        temp();
                        lcd_cmd(0X01);
                        lcd_str(" <window status>");
                        _delay_ms(1000);
                        lcd_cmd(0XAA);
                        lcd_str("   Closed");
                        _delay_ms(2000);
                        flag1 = 1;
                    }
                }
                else
                {
                    if (flag1 == 1)
                    {
                        lcd_cmd(0X01);
                        lcd_str(" <window status>");
                        _delay_ms(1000);
                        lcd_cmd(0XAA);
                        lcd_str("   Closed");
                        _delay_ms(2000);
                        temp();
                        if ((PINC & (1 << 3)) == 0) // open manual after rain gone
                        {
                            flag = 0;
                            flag1 = 0;
                            PORTC |= (1 << 7);
                            PORTC &= ~(1 << 5);
                            lcd_cmd(0X01);
                            lcd_str(" <window status>");
                            _delay_ms(1000);
                            lcd_cmd(0XAA);
                            lcd_str("   open");
                            _delay_ms(2000);
                        }
                        else
                        {
                            PORTC &= ~(1 << 7);
                            PORTC |= (1 << 5);
                        }
                    }
                    else
                    {
                        flag1 = 0;
                        automatic();
                    }
                }
            }
        }
        else
        {
            PORTB &= ~(1 << 5);
            PORTC &= ~(1 << 4);
            PORTC &= ~(1 << 5);
            PORTC &= ~(1 << 6);
            PORTC &= ~(1 << 7);
            lcd_cmd(0X01);
            lcd_str("  <Engine OFF>");
            _delay_ms(2000);
            o = 0;
        }
    }
}
