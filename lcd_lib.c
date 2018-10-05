#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <inttypes.h>
#include "lcd_lib.h"

void lcd(unsigned char p) { 
  LCD_PORT &= 0xF0;
  LCD_PORT |= (p >> 4);
  _delay_ms(1);

  LCD_PORTU &= ~(_BV(LCD_EN));
  _delay_ms(1);

  LCD_PORTU |= _BV(LCD_EN);

  LCD_PORT &= 0xF0;
  LCD_PORT |= (p & 0x0F);
  _delay_ms(1);

  LCD_PORTU &= ~(_BV(LCD_EN));
  _delay_ms(1);
}                

void lcd_com(unsigned char p) {
  LCD_PORTU &= ~(_BV(LCD_RS));
  LCD_PORTU |= _BV(LCD_EN);
  lcd(p);
}

void lcd_dat(unsigned char p) {
  LCD_PORTU |= _BV(LCD_RS) | _BV(LCD_EN);
  lcd(p);
}

void lcd_init(void) {
  _delay_ms(200);
  lcd_com(0x33);  // 0x00110011

  _delay_ms(20);
  lcd_com(0x32);  // 0x00110010

  _delay_ms(20);
  lcd_com(0x28);  // 0x00101000 - function set 4-bit,2 line 5x7 dot format

  _delay_ms(20);
  lcd_com(0x06);  // 0x00000110 - инкремент счетчика адреса, курсора - без сдвигов

  _delay_ms(2);
  lcd_com(0x0C);  // 0x00001100 - increment address, invisible cursor shift
  
  _delay_ms(1);
  lcd_com(1<<LCD_HOME); // LCD_HOME

  _delay_ms(1);
  lcd_com(0x80);  // Переключить адресацию на DDRAM и задать адрес в DDRAM
}

void lcd_shift_right(void) {
  lcd_com(0x18);
}

void lcd_shift_left(void) {
  lcd_com(0x1E);
}

void lcd_puts(unsigned char *data) {
  while(*data) {
    lcd_dat(*(data++));
  }
}

void lcd_clear(void) {
  lcd_com(1<<LCD_CLR);
  _delay_ms(100);
}

void lcd_home(void) {
  lcd_com(1<<LCD_HOME);
}

void lcd_space(unsigned char p) {
  unsigned char i;
  if (p == 0)
    p = 16;

  for (i = 1; i <= p; i++) {
    lcd_dat(' ');
  }
}

void lcd_cursor_ON(void) {
  lcd_com(0x0E);
}

void lcd_cursor_ON_blink(void) {
  lcd_com(0x0F);
}

void lcd_cursor_OFF(void) {
  lcd_com(0x0C);
}

void lcd_goto_xy(uint8_t x,uint8_t y) {
  uint8_t Address;
  
  switch(y) {
    case 0: Address = LINE0+x; 
        break;
    case 1: Address = LINE1+x; 
        break;
    default: Address = LINE0+x;
  }
  
  lcd_com(1<<7 | Address);
}