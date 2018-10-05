
#define LCD_PORT  PORTA             // port for LCD
#define LCD_PORTU PORTA             // port for LCD óïðàâëåíèå
#define LCD_RS    PA4               // pin RS for LCD
#define LCD_EN    PA5               // pin EN for LCD
#define LCD_TIME  10                // wait for LCD

#define LINE0 0x00
#define LINE1 0x40

#define LCD_CLR                 0	// clear display
#define LCD_HOME                1	// return to home position
#define LCD_ENTRY_MODE          2	// set entry mode
#define LCD_ENTRY_INC           1	// increment
#define LCD_ENTRY_SHIFT         0	// shift
#define LCD_ON_CTRL             3	// turn lcd/cursor on
#define LCD_ON_DISPLAY          2	// turn display on
#define LCD_ON_CURSOR           1	// turn cursor on
#define LCD_ON_BLINK            0	// blinking cursor
#define LCD_MOVE                4	// move cursor/display
#define LCD_MOVE_DISP           3	// move display (0-> move cursor)
#define LCD_MOVE_RIGHT          2	// move right (0-> left)
#define LCD_FUNCTION            5	// function set
#define LCD_CGRAM               6	// set CG RAM address
#define LCD_DDRAM               7	// set DD RAM address
#define LCD_LINES			    1	// visible lines
#define LCD_LINE_LENGTH		    16	// line length (in characters)
#define PROGRESSPIXELS_PER_CHAR	6

void lcd(unsigned char p);
void lcd_init(void);
void lcd_com(unsigned char p);
void lcd_col(unsigned char p);
void lcd_dat(unsigned char p);
void lcd_puts(unsigned char *data);
void lcd_clear(void);
void lcd_space(unsigned char p);
void lcd_home(void);
void lcd_cursor_ON(void); 
void lcd_cursor_ON_blink(void);
void lcd_cursor_OFF(void);
void lcd_shift_right(void);
void lcd_shift_left(void);
void lcd_goto_xy(uint8_t, uint8_t);       //Устанавливаем курсор в X, Y позицию
