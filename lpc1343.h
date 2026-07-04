#include <LPC13xx.h>

#define LCD_RS_PIN (1 << 0)
#define LCD_RW_PIN (1 << 1)
#define LCD_EN_PIN (1 << 2)
#define LCD_D4_PIN (1 << 4)
#define LCD_D5_PIN (1 << 5)
#define LCD_D6_PIN (1 << 6)
#define LCD_D7_PIN (1 << 7)

#define LED_PIN (1 << 7)      /* P0.7 */

#define ALERT_THRESHOLD 2460
#define SECURE_THRESHOLD 1630

void delay_ms(unsigned int ms)
{
    unsigned int i, j;

    for(i = 0; i < ms; i++)
        for(j = 0; j < 3000; j++);
}

void lcd_set_rs(int val)
{
    if(val)
        LPC_GPIO1->DATA |= LCD_RS_PIN;
    else
        LPC_GPIO1->DATA &= ~LCD_RS_PIN;
}

void lcd_set_en(int val)
{
    if(val)
        LPC_GPIO1->DATA |= LCD_EN_PIN;
    else
        LPC_GPIO1->DATA &= ~LCD_EN_PIN;
}

void lcd_send_nibble(unsigned char nibble)
{
    LPC_GPIO1->DATA &= ~(LCD_D4_PIN | LCD_D5_PIN |
                         LCD_D6_PIN | LCD_D7_PIN);

    if(nibble & 0x01) LPC_GPIO1->DATA |= LCD_D4_PIN;
    if(nibble & 0x02) LPC_GPIO1->DATA |= LCD_D5_PIN;
    if(nibble & 0x04) LPC_GPIO1->DATA |= LCD_D6_PIN;
    if(nibble & 0x08) LPC_GPIO1->DATA |= LCD_D7_PIN;

    lcd_set_en(1);
    delay_ms(2);
    lcd_set_en(0);
    delay_ms(2);
}

void lcd_cmd(unsigned char cmd)
{
    lcd_set_rs(0);
    LPC_GPIO1->DATA &= ~LCD_RW_PIN;

    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);

    delay_ms(2);
}

void lcd_data(unsigned char dat)
{
    lcd_set_rs(1);
    LPC_GPIO1->DATA &= ~LCD_RW_PIN;

    lcd_send_nibble(dat >> 4);
    lcd_send_nibble(dat & 0x0F);

    delay_ms(2);
}

void lcd_string(char *str)
{
    while(*str)
        lcd_data(*str++);
}

void lcd_init(void)
{
    LPC_IOCON->JTAG_TMS_PIO1_0 = 0x01;
    LPC_IOCON->JTAG_TDO_PIO1_1 = 0x01;
    LPC_IOCON->JTAG_nTRST_PIO1_2 = 0x01;
    LPC_IOCON->ARM_SWDIO_PIO1_3 = 0x01;

    LPC_IOCON->PIO1_4 = 0x00;
    LPC_IOCON->PIO1_5 = 0x00;
    LPC_IOCON->PIO1_6 = 0x00;
    LPC_IOCON->PIO1_7 = 0x00;

    LPC_GPIO1->DIR |= (LCD_RS_PIN |
                       LCD_RW_PIN |
                       LCD_EN_PIN |
                       LCD_D4_PIN |
                       LCD_D5_PIN |
                       LCD_D6_PIN |
                       LCD_D7_PIN);

    delay_ms(20);

    lcd_set_rs(0);
    LPC_GPIO1->DATA &= ~LCD_RW_PIN;

    lcd_send_nibble(0x03);
    delay_ms(5);

    lcd_send_nibble(0x03);
    delay_ms(1);

    lcd_send_nibble(0x03);
    delay_ms(1);

    lcd_send_nibble(0x02);
    delay_ms(1);

    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    delay_ms(5);
    lcd_cmd(0x06);
}

void adc_init(void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 13);

    LPC_IOCON->JTAG_TDI_PIO0_11 = 0x02;

    LPC_ADC->CR = (1 << 0) |
                  (15 << 8) |
                  (0 << 16) |
                  (1 << 21);
}

unsigned int read_adc(void)
{
    unsigned long sum = 0;
    unsigned char i;

    for(i = 0; i < 10; i++)
    {
        LPC_ADC->CR &= ~(0x07 << 24);
        LPC_ADC->CR |= (0x01 << 24);

        while(!(LPC_ADC->DR0 & (1UL << 31)));

        sum += (LPC_ADC->DR0 >> 4) & 0xFFF;

        delay_ms(1);
    }

    return (unsigned int)(sum / 10);
}

void show_secure(void)
{
    lcd_cmd(0x01);
    delay_ms(5);

    lcd_cmd(0x80);
    lcd_string("SECURE");

    lcd_cmd(0xC0);
    lcd_string("NO INTRUDERS");

    LPC_GPIO0->MASKED_ACCESS[LED_PIN] = 0;
}

void show_alert(void)
{
    lcd_cmd(0x01);
    delay_ms(5);

    lcd_cmd(0x80);
    lcd_string("!! ALERT !!");

    lcd_cmd(0xC0);
    lcd_string("INTRUDERS DETECT");

    LPC_GPIO0->MASKED_ACCESS[LED_PIN] = LED_PIN;
}

int main(void)
{
    unsigned int adc_value;

    unsigned char state = 0;
    unsigned char prev_state = 1;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6) | (1 << 13);

    LPC_IOCON->PIO0_7 = 0x00;

    LPC_GPIO0->DIR |= LED_PIN;
    LPC_GPIO0->MASKED_ACCESS[LED_PIN] = 0;

    adc_init();
    lcd_init();

    while(1)
    {
        adc_value = read_adc();

        if(adc_value > ALERT_THRESHOLD)
            state = 1;
        else if(adc_value < SECURE_THRESHOLD)
            state = 0;

        if(state != prev_state)
        {
            if(state == 1)
                show_alert();
            else
                show_secure();

            prev_state = state;
        }

        delay_ms(100);
    }
}