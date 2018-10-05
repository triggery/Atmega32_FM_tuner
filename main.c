#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd_lib.h"

#define PORT_CE           PORTD
#define PIN_CE            PD0

#define PORT_CL           PORTD
#define PIN_CL            PD1

#define PORT_DATA         PORTD
#define PIN_DATA          PD2

#define CE_EN_OFF         PORT_CE &= ~(_BV(PIN_CE)); _delay_ms(1);
#define CL_OFF            PORT_CL &= ~(_BV(PIN_CE)); _delay_ms(1);

#define IF                10700000   //Intermediate Frequency = 10.7MHz
#define STEP100           100000     //Step Frequency

#define START_FREQ        87500000
#define END_FREQ          108000000

#define PIN_BUTTON        PIND

#define PIN_FREQ_UP       0b00010000
#define PIN_FREQ_DOWN     0b00100000
#define PIN_FREQ_ANY      0b00110000

char frequency_buffer[14];

void send_0_byte (void)
{
  PORT_DATA &= ~(_BV(PIN_DATA));
  _delay_us(10);
  PORT_CL |= _BV(PIN_CL);
  _delay_us(10);
  PORT_CL &= ~(_BV(PIN_CL));
  _delay_us(10);
}

void send_1_byte (void)
{
  PORT_DATA |= _BV(PIN_DATA); 
  _delay_us(10);
  PORT_CL |= _BV(PIN_CL); 
  _delay_us(10); 
  PORT_CL &= ~(_BV(PIN_CL));
   _delay_us(10);
  PORT_DATA &= ~(_BV(PIN_DATA));
}

void sendtoLM7001(void) {
  int  i;
  PORT_CE |= _BV(PIN_CE); 
  _delay_ms(1);

  for (i = 0; i < 14; i++) {
    if (frequency_buffer [i] == '0')
      send_0_byte ();
    else
      send_1_byte ();
  }

  send_0_byte();  //T0
  send_0_byte();  //T1
  send_0_byte();  //B0
  send_0_byte();  //B1
  send_0_byte();  //B2
  send_0_byte();  //TB
  send_0_byte();  //R0
  send_0_byte();  //R1
  send_0_byte();  //R2
  send_1_byte();  //FM

  PORT_CE &= ~(_BV(PIN_CE));
  _delay_ms(1);
}

void process_frequency(unsigned long freq1)
{
  int counter;
  for (counter = 0; counter < 14; counter++) {
    if (freq1 & ((unsigned long)1 << counter))
      frequency_buffer[counter] = '1';
    else
      frequency_buffer[counter] = '0';
  }
}

void change_freq(unsigned long freq)
{
  unsigned long freqs, freq_get;
  freqs = (freq + IF);
  freq_get = freqs / STEP100;
  process_frequency(freq_get);
}

//char numbers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char num_buff[14];

int main(void)
{
  unsigned long ff = START_FREQ;

  // LCD
  PORTA = 0xFF;
  DDRA = 0xFF;

  // LM7001J
  PORTD = 0xFF;
  DDRD = 0x0F;

  CE_EN_OFF;
  CL_OFF;

  lcd_init();
  lcd_goto_xy(2, 0);
  lcd_puts((unsigned char *)"hd44780 LCD");
  lcd_goto_xy(1, 1);
  //lcd_puts((unsigned char *)"Hello, Dima!!!");

  itoa((int)(ff/100000), &num_buff[0], 10);
  lcd_puts((unsigned char *)&num_buff);

  _delay_ms(100);

  change_freq(ff);
  sendtoLM7001();

  while(1)
  {
        /*if(PIN_BUTTON & PIN_FREQ_UP) {

          ff += STEP100;
          if(ff > END_FREQ)
            ff = START_FREQ;

          _delay_ms(200);

          lcd_clear();
          lcd_goto_xy(2, 0);
          lcd_puts((unsigned char *)"hd44780 LCD");
          lcd_goto_xy(1, 1);
          itoa((int)(ff/100000), &num_buff[0], 10);
          lcd_puts((unsigned char *)&num_buff);

          change_freq(ff);
          sendtoLM7001();
        }*/

      /*if( !(PIN_BUTTON & PIN_FREQ_ANY) ) {
        _delay_ms(200);*/
        
        if( !(PIN_BUTTON & PIN_FREQ_UP) ) {
          if(ff < END_FREQ)
            ff += STEP100;

          _delay_ms(200);
          
          lcd_clear();
          lcd_goto_xy(2, 0);
          lcd_puts((unsigned char *)"FM receiver:");
          lcd_goto_xy(3, 1);
          itoa((int)(ff/STEP100), &num_buff[0], 10);
          lcd_puts((unsigned char *)&num_buff);
          lcd_puts((unsigned char *)" ");
          lcd_puts((unsigned char *)"MHz");

          change_freq(ff);
          sendtoLM7001(); 
        }
        else if( !(PIN_BUTTON & PIN_FREQ_DOWN) ) {
          if(ff > START_FREQ)
            ff -= STEP100;

          _delay_ms(200);

          lcd_clear();
          lcd_goto_xy(2, 0);
          lcd_puts((unsigned char *)"FM receiver:");
          lcd_goto_xy(3, 1);
          itoa((int)(ff/STEP100), &num_buff[0], 10);
          lcd_puts((unsigned char *)&num_buff);
          lcd_puts((unsigned char *)" ");
          lcd_puts((unsigned char *)"MHz");

          change_freq(ff);
          sendtoLM7001(); 
        }


      //}


  }
}