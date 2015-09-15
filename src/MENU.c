#include <stdlib.h>
#include "main.h"

uint8_t Syg_qnt = 0;

MENU *_Syg_NONE_menu;
MENU *_Syg_1_menu;
MENU *_Syg_2_menu;
MENU *_Syg_3_menu;
MENU *_Syg_4_menu;

MENU *_1_menu;
MENU *_2_menu;
MENU *_3_menu;
MENU *_4_menu;
MENU *_5_menu;

MENU *_actual;

// 0 level
void menu_Syg_NONE_fun(unsigned int);
void menu_Syg_1_fun(unsigned int);
void menu_Syg_2_fun(unsigned int);
void menu_Syg_3_fun(unsigned int);
void menu_Syg_4_fun(unsigned int);

// 1 level
void menu_1_fun(unsigned int);
void menu_2_fun(unsigned int);
void menu_3_fun(unsigned int);
void menu_4_fun(unsigned int);
void menu_5_fun(unsigned int);

void MENU_par(MENU *);

void MENU_SystemFill(uint8_t syg)
{
	uint32_t buffer;
	
	System[syg].ActAddress    = RFM69W_Data.SenderID;
	System[syg].ActAnimation  = (0x0F & RFM69W_Data.Data[1]) >> 0;
	System[syg].ActColor      = (0xF0 & RFM69W_Data.Data[1]) >> 4;
	System[syg].ActAlarmTone  = (0x0F & RFM69W_Data.Data[2]) >> 0;
	System[syg].ActAlarmVol   = (0xF0 & RFM69W_Data.Data[2]) >> 4;
	System[syg].ActAlarmTempo = (0x0F & RFM69W_Data.Data[3]) >> 0;
	System[syg].ActBrightness = (0xF0 & RFM69W_Data.Data[3]) >> 4;
	System[syg].ActMusic      = (0x0F & RFM69W_Data.Data[4]) >> 0;
	
	EEPROM_SystemBackup(syg);
}

void MENU_SystemFillReset(uint8_t syg)
{
	uint32_t buffer;
	
	System[syg].ActAddress    = 0;
	System[syg].ActAnimation  = 0;
	System[syg].ActColor      = 0;
	System[syg].ActAlarmTone  = 0;
	System[syg].ActAlarmVol   = 0;
	System[syg].ActAlarmTempo = 0;
	System[syg].ActBrightness = 0;
	System[syg].ActMusic      = 0;
	
	EEPROM_SystemBackup(syg);
}

void MENU_PacketInterpreter(void)
{
	switch (RFM69W_Data.Data[0]) {
		case (MENU_RF_Recruit):
			// Check if there are no maximum ammount of sygnallers
			if (Syg_qnt < 4) {
				// Check if there is no already this address in database
				// TO JEST SLISKIE W HUJ BO CO JAK WYLACZE I WLACZE PO CHAMSKU? ODSWIEZANIE?
				if (!((RFM69W_Data.SenderID == System[0].ActAddress) || 
					    (RFM69W_Data.SenderID == System[1].ActAddress) || 
				      (RFM69W_Data.SenderID == System[2].ActAddress) || 
				      (RFM69W_Data.SenderID == System[3].ActAddress))) {
					MENU_SystemFill(Syg_qnt);
					_LED_set_color_list(MENU_LED_SYG1, System[Syg_qnt].ActColor);
					_LED_on();
					Syg_qnt++;
				}
			}
			break;
		case (MENU_RF_HallAlarmStop):
			_LED_blink_off(MENU_LED_SYG1);
			_BUZZER_alarm_stop();
			_actual = _Syg_NONE_menu;
			break;
		case (MENU_RF_HallAlarm):
			_BUZZER_alarm_start();
			if (RFM69W_Data.SenderID == System[0].ActAddress) {
				_actual = _Syg_1_menu;
				_LED_set_color(MENU_LED_Menu, 0, 0, 0);
			} else {
				if (RFM69W_Data.SenderID == System[1].ActAddress) {
					_actual = _Syg_2_menu;
				} else {
					if (RFM69W_Data.SenderID == System[2].ActAddress) {
						_actual = _Syg_3_menu;
					} else {
						if (RFM69W_Data.SenderID == System[3].ActAddress) {
							_actual = _Syg_4_menu;
						}
					}
				}
			}
			break;
		case (MENU_RF_SendInfo):
			MENU_SystemFill(RFM69W_Data.SenderID - 1);
			_LED_set_color_list(MENU_LED_SYG1, System[RFM69W_Data.SenderID - 1].ActColor);
			_LED_on();
			break;
		case (MENU_RF_LogOff):
			if ((RFM69W_Data.SenderID == System[0].ActAddress) || 
				  (RFM69W_Data.SenderID == System[1].ActAddress) || 
			    (RFM69W_Data.SenderID == System[2].ActAddress) || 
			    (RFM69W_Data.SenderID == System[3].ActAddress)) {
				_LED_set_color(MENU_LED_SYG1, 0, 0, 0);
				_LED_on();
				MENU_SystemFillReset(Syg_qnt-1);
				Syg_qnt--;
			}
			break;
	}
}

void MENU_par(MENU *parent)
{
	_1_menu->par = parent;
	_2_menu->par = parent;
	_3_menu->par = parent;
	_4_menu->par = parent;
	_5_menu->par = parent;
}

void MENU_init()
{
	_Syg_NONE_menu = malloc(sizeof(MENU));
	_Syg_1_menu = malloc(sizeof(MENU));
	_Syg_2_menu = malloc(sizeof(MENU));
	_Syg_3_menu = malloc(sizeof(MENU));
	_Syg_4_menu = malloc(sizeof(MENU));
	
	_1_menu = malloc(sizeof(MENU));
	_2_menu = malloc(sizeof(MENU));
	_3_menu = malloc(sizeof(MENU));
	_4_menu = malloc(sizeof(MENU));
	_5_menu = malloc(sizeof(MENU));
	
	// 0 level
	
	_Syg_NONE_menu->next = _Syg_1_menu;
	_Syg_NONE_menu->menu_fun = menu_Syg_NONE_fun;
	
	_Syg_1_menu->next = _Syg_2_menu;
	_Syg_1_menu->par = _Syg_1_menu;
	_Syg_1_menu->sub = _1_menu; 
	_Syg_1_menu->menu_fun = menu_Syg_1_fun;
	_Syg_1_menu->LED_number = MENU_LED_SYG1;
	
	_Syg_2_menu->next = _Syg_3_menu;
	_Syg_2_menu->par = _Syg_2_menu;
	_Syg_2_menu->sub = _1_menu;
	_Syg_2_menu->menu_fun = menu_Syg_2_fun;
	_Syg_2_menu->LED_number = MENU_LED_SYG2;
	
	_Syg_3_menu->next = _Syg_4_menu;
	_Syg_3_menu->par = _Syg_3_menu;
	_Syg_3_menu->sub = _1_menu;
	_Syg_3_menu->menu_fun = menu_Syg_3_fun;
	_Syg_3_menu->LED_number = MENU_LED_SYG3;
	
	_Syg_4_menu->next = _Syg_NONE_menu;
	_Syg_4_menu->par = _Syg_4_menu;
	_Syg_4_menu->sub = _1_menu;
	_Syg_4_menu->menu_fun = menu_Syg_4_fun;
	_Syg_4_menu->LED_number = MENU_LED_SYG4;
	
	// 1 level
	
	_1_menu->next = _2_menu;
	_1_menu->par = NULL;
	_1_menu->menu_fun = menu_1_fun;
	_1_menu->LED_number = MENU_LED_Menu;
	_1_menu->LED_color = MENU_1_LED_Color;
	
	_2_menu->next = _3_menu;
	_2_menu->par = NULL;
	_2_menu->menu_fun = menu_2_fun;
	_1_menu->LED_color = MENU_1_LED_Color;
	_2_menu->LED_number = MENU_LED_Menu;
	_2_menu->LED_color = MENU_2_LED_Color;
	
	_3_menu->next = _4_menu;
	_3_menu->par = NULL;
	_3_menu->menu_fun = menu_3_fun;
	_3_menu->LED_number = MENU_LED_Menu;
	_3_menu->LED_color = MENU_3_LED_Color;	
	
	_4_menu->next = _5_menu;
	_4_menu->par = NULL;
	_4_menu->menu_fun = menu_4_fun;
	_4_menu->LED_number = MENU_LED_Menu;
	_4_menu->LED_color = MENU_4_LED_Color;
	
	_5_menu->next = _1_menu;
	_5_menu->par = NULL;
	_5_menu->menu_fun = menu_5_fun;
	_5_menu->LED_number = MENU_LED_Menu;
	_5_menu->LED_color = MENU_5_LED_Color;
	
	_actual = _Syg_NONE_menu;
}

void menu_Syg_NONE_fun(unsigned int key)
{
	switch (key) {
		case (KEY_3):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			switch (Syg_qnt) {
				case 0:
#ifdef USART_debug
					USART_send("Central unit menu.\n");
#endif
				break;
				case 1:
					_actual = _Syg_1_menu;
#ifdef USART_debug
					USART_send("Sygnaller #1 menu.\n");
#endif
					break;
				case 2:
					_actual = _Syg_2_menu;
#ifdef USART_debug
					USART_send("Sygnaller #2 menu.\n");
#endif
					break;
				case 3:
					_actual = _Syg_3_menu;
#ifdef USART_debug
					USART_send("Sygnaller #3 menu.\n");
#endif
					break;
				case 4:
					_actual = _Syg_4_menu;
#ifdef USART_debug
					USART_send("Sygnaller #4 menu.\n");
#endif
					break;
			}
			break;
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
			
			break;
		case (KEY_1):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
			
			if (Syg_qnt > 0) {
				_actual = _actual->next;
#ifdef USART_debug
				USART_send("Sygnaller #1 menu.\n");
#endif
			} else {
				_actual = _Syg_NONE_menu;
#ifdef USART_debug
				USART_send("Central unit menu.\n");
#endif
			}
			break;
	}
}

void menu_Syg_1_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG1);
	
	switch (key) {
		case (KEY_3):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
			_LED_blink_off(_Syg_1_menu->LED_number);
		
#ifdef USART_debug
			USART_send("Central unit menu.\n");
#endif

			_actual = _Syg_NONE_menu;
			break;
		case (KEY_2):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			MENU_par(_actual);

#ifdef USART_debug
			USART_send("-1- LED color setting mode.\n");
#endif

			_actual = _actual->sub;
			break;
		case (KEY_1):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
			_LED_blink_off(_Syg_1_menu->LED_number);
		
			if (Syg_qnt > 1) {
				_actual = _actual->next;
#ifdef USART_debug
				USART_send("Sygnaller #2 menu.\n");
#endif
			} else {
				_actual = _Syg_NONE_menu;
#ifdef USART_debug
				USART_send("Central unit menu.\n");
#endif
			}
			break;
	}
}

void menu_Syg_2_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG2);
	
	switch (key) {
		case (KEY_3):
			ClrKeyb( KBD_LOCK );
			_BUZZER_single_beep();
			_LED_blink_off(_Syg_2_menu->LED_number);

#ifdef USART_debug
			USART_send("Sygnaller #1 menu.\n");
#endif
			
			_actual = _Syg_1_menu;
			break;
		case (KEY_2):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			MENU_par(_actual);

#ifdef USART_debug
			USART_send("-1- LED color setting mode.\n");
#endif
		
			_actual = _actual->sub;
			break;
		case (KEY_1):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
			_LED_blink_off(_Syg_2_menu->LED_number);
		
			if (Syg_qnt > 2) {
				_actual = _actual->next;
#ifdef USART_debug
				USART_send("Sygnaller #3 menu.\n");
#endif
			} else {
				_actual = _Syg_NONE_menu;
#ifdef USART_debug
				USART_send("Central unit menu.\n");
#endif
			}
			break;
	}
}

void menu_Syg_3_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG3);
	
	switch (key) {
		case (KEY_3):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
			_LED_blink_off(_Syg_3_menu->LED_number);
		
#ifdef USART_debug
			USART_send("Sygnaller #2 menu.\n");
#endif

			_actual = _Syg_2_menu;
			break;
		case (KEY_2):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			MENU_par(_actual);

#ifdef USART_debug
			USART_send("-1- LED color setting mode.\n");
#endif
		
			_actual = _actual->sub;
			break;
		case (KEY_1):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			if (Syg_qnt > 3) {
				_actual = _actual->next;
#ifdef USART_debug
				USART_send("Sygnaller #4 menu.\n");
#endif
			} else {
				_actual = _Syg_NONE_menu;
#ifdef USART_debug
				USART_send("Central unit menu.\n");
#endif
			}
			
			_LED_blink_off(_Syg_3_menu->LED_number);
			break;
	}
}

void menu_Syg_4_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_3):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
			_LED_blink_off(_Syg_4_menu->LED_number);

#ifdef USART_debug
			USART_send("Sygnaller #3 menu.\n");
#endif
		
			_actual = _Syg_3_menu;
			break;
		case (KEY_2):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			MENU_par(_actual);

#ifdef USART_debug
			USART_send("-1- LED color setting mode.\n");
#endif
		
			_actual = _actual->sub;
			break;
		case (KEY_1):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
			_LED_blink_off(_Syg_4_menu->LED_number);

#ifdef USART_debug
			USART_send("Central unit menu.\n");
#endif

			_actual = _actual->next;			
			break;
	}
}

// Regulacja koloru diody
void menu_1_fun(unsigned int key)
{
	uint8_t buffer_send[2];
	
	_LED_set_color_list(MENU_LED_Menu, MENU_1_LED_Color);
	
	if (_actual->par == _Syg_1_menu)
		_LED_blink_on(MENU_LED_SYG1);
	if (_actual->par == _Syg_2_menu)
		_LED_blink_on(MENU_LED_SYG2);
	if (_actual->par == _Syg_3_menu)
		_LED_blink_on(MENU_LED_SYG3);
	if (_actual->par == _Syg_4_menu)
		_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_3):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActColor == led_colors_qnt - 1)
					System[0].ActColor = 0;
				else
					System[0].ActColor++;
				_LED_set_color_list(MENU_LED_SYG1, System[0].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[0].ActColor, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActColor == led_colors_qnt - 1)
					System[1].ActColor = 0;
				else
					System[1].ActColor++;
				_LED_set_color_list(MENU_LED_SYG2, System[1].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[1].ActColor, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActColor == led_colors_qnt - 1)
					System[2].ActColor = 0;
				else
					System[2].ActColor++;
				_LED_set_color_list(MENU_LED_SYG3, System[2].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[2].ActColor, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActColor == led_colors_qnt - 1)
					System[3].ActColor = 0;
				else
					System[3].ActColor++;
				_LED_set_color_list(MENU_LED_SYG4, System[3].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[3].ActColor, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
		
			break;
		case (KEY_2):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			buffer_send[0] = MENU_RF_ActColor;
			if (_actual->par == _Syg_1_menu) {
				buffer_send[1] = System[0].ActColor;
				RFM69W_sendWithRetry(System[0].ActAddress, buffer_send, 2, 15, 10);
			}
			if (_actual->par == _Syg_2_menu) {
				buffer_send[1] = System[1].ActColor;
				RFM69W_sendWithRetry(System[1].ActAddress, buffer_send, 2, 15, 10);
			}
			if (_actual->par == _Syg_3_menu) {
				buffer_send[1] = System[2].ActColor;
				RFM69W_sendWithRetry(System[2].ActAddress, buffer_send, 2, 15, 10);
			}
			if (_actual->par == _Syg_4_menu) {
				buffer_send[1] = System[3].ActColor;
				RFM69W_sendWithRetry(System[3].ActAddress, buffer_send, 2, 15, 10);
			}
		
#ifdef USART_debug
			USART_send("-2- LED brightness setting mode.\n");
#endif

			_actual = _actual->next;
			break;
		case (KEY_1):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActColor == 0)
					System[0].ActColor = led_colors_qnt - 1;
				else
					System[0].ActColor--;
				_LED_set_color_list(MENU_LED_SYG1, System[0].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[0].ActColor, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActColor == 0)
					System[1].ActColor = led_colors_qnt - 1;
				else
					System[1].ActColor--;
				_LED_set_color_list(MENU_LED_SYG2, System[1].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[1].ActColor, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActColor == 0)
					System[2].ActColor = led_colors_qnt - 1;
				else
					System[2].ActColor--;
				_LED_set_color_list(MENU_LED_SYG3, System[2].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[2].ActColor, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActColor == 0)
					System[3].ActColor = led_colors_qnt - 1;
				else
					System[3].ActColor--;
				_LED_set_color_list(MENU_LED_SYG4, System[3].ActColor);

#ifdef USART_debug
				USART_send("\tLED color #");
				USART_write_buf(System[3].ActColor, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
			
			break;
	}
}

// Wybor jasnosci diod
void menu_2_fun(unsigned int key)
{
	uint8_t buffer_send[2];
	
	_LED_set_color_list(MENU_LED_Menu, MENU_2_LED_Color);
	
	if (_actual->par == _Syg_1_menu)
		_LED_blink_on(MENU_LED_SYG1);
	if (_actual->par == _Syg_2_menu)
		_LED_blink_on(MENU_LED_SYG2);
	if (_actual->par == _Syg_3_menu)
		_LED_blink_on(MENU_LED_SYG3);
	if (_actual->par == _Syg_4_menu)
		_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_3):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActBrightness == led_brightness_qnt - 1)
					System[0].ActBrightness = 0;
				else
					System[0].ActBrightness++;
				_LED_change_brightness_limit_list(System[0].ActBrightness);

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[0].ActBrightness, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActBrightness == led_brightness_qnt - 1)
					System[1].ActBrightness = 0;
				else
					System[1].ActBrightness++;
				_LED_change_brightness_limit_list(System[1].ActBrightness);

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[1].ActBrightness, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActBrightness == led_brightness_qnt - 1)
					System[2].ActBrightness = 0;
				else
					System[2].ActBrightness++;
				_LED_change_brightness_limit_list(System[2].ActBrightness);
			}

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[2].ActBrightness, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActBrightness == led_brightness_qnt - 1)
					System[3].ActBrightness = 0;
				else
					System[3].ActBrightness++;
				_LED_change_brightness_limit_list(System[3].ActBrightness);

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[3].ActBrightness, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
		
			break;
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
		
			buffer_send[0] = MENU_RF_ActBrightness;
			if (_actual->par == _Syg_1_menu) {
				buffer_send[1] = System[0].ActBrightness;
				RFM69W_sendWithRetry(System[0].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_2_menu) {
				buffer_send[1] = System[1].ActBrightness;
				RFM69W_sendWithRetry(System[1].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_3_menu) {
				buffer_send[1] = System[2].ActBrightness;
				RFM69W_sendWithRetry(System[2].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_4_menu) {
				buffer_send[1] = System[3].ActBrightness;
				RFM69W_sendWithRetry(System[3].ActAddress, buffer_send, 2, 15, 15);
			}
		
#ifdef USART_debug
			USART_send("-3- Alarm tone setting mode.\n");
#endif

			_actual = _actual->next;
			break;
		case (KEY_1):
			_BUZZER_single_beep();
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActBrightness == 0)
					System[0].ActBrightness = led_colors_qnt - 1;
				else
					System[0].ActBrightness--;
				_LED_change_brightness_limit_list(System[0].ActBrightness);

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[0].ActBrightness, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActBrightness == 0)
					System[1].ActBrightness = led_colors_qnt - 1;
				else
					System[1].ActBrightness--;
				_LED_change_brightness_limit_list(System[1].ActBrightness);

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[1].ActBrightness, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActBrightness == 0)
					System[2].ActBrightness = led_colors_qnt - 1;
				else
					System[2].ActBrightness--;
				_LED_change_brightness_limit_list(System[2].ActBrightness);

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[2].ActBrightness, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActBrightness == 0)
					System[3].ActBrightness = led_colors_qnt - 1;
				else
					System[3].ActBrightness--;
				_LED_change_brightness_limit_list(System[3].ActBrightness);

#ifdef USART_debug
				USART_send("\tLED brightness #");
				USART_write_buf(System[3].ActBrightness, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
			
			break;
	}
}

// Wybor tonu alarmu
void menu_3_fun(unsigned int key)
{
	uint8_t buffer_send[2];
	
	_LED_set_color_list(MENU_LED_Menu, MENU_3_LED_Color);
	_BUZZER_alarm_start();
	
	if (_actual->par == _Syg_1_menu)
		_LED_blink_on(MENU_LED_SYG1);
	if (_actual->par == _Syg_2_menu)
		_LED_blink_on(MENU_LED_SYG2);
	if (_actual->par == _Syg_3_menu)
		_LED_blink_on(MENU_LED_SYG3);
	if (_actual->par == _Syg_4_menu)
		_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_3):
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActAlarmTone == buzzer_tones_qnt - 1)
					System[0].ActAlarmTone = 0;
				else
					System[0].ActAlarmTone++;
				_BUZZER_alarm_set_tone_list(System[0].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[0].ActAlarmTone, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActAlarmTone == buzzer_tones_qnt - 1)
					System[1].ActAlarmTone = 0;
				else
					System[1].ActAlarmTone++;
				_BUZZER_alarm_set_tone_list(System[1].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[1].ActAlarmTone, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActAlarmTone == buzzer_tones_qnt - 1)
					System[2].ActAlarmTone = 0;
				else
					System[2].ActAlarmTone++;
				_BUZZER_alarm_set_tone_list(System[2].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[2].ActAlarmTone, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActAlarmTone == buzzer_tones_qnt - 1)
					System[3].ActAlarmTone = 0;
				else
					System[3].ActAlarmTone++;
				_BUZZER_alarm_set_tone_list(System[3].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[3].ActAlarmTone, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
		
			break;
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
		
			_BUZZER_alarm_stop();
		
			buffer_send[0] = MENU_RF_ActAlarmTone;
			if (_actual->par == _Syg_1_menu) {
				buffer_send[1] = System[0].ActAlarmTone;
				RFM69W_sendWithRetry(System[0].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_2_menu) {
				buffer_send[1] = System[1].ActAlarmTone;
				RFM69W_sendWithRetry(System[1].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_3_menu) {
				buffer_send[1] = System[2].ActAlarmTone;
				RFM69W_sendWithRetry(System[2].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_4_menu) {
				buffer_send[1] = System[3].ActAlarmTone;
				RFM69W_sendWithRetry(System[3].ActAddress, buffer_send, 2, 15, 15);
			}

#ifdef USART_debug
			USART_send("-4- Alarm volume setting mode.\n");
#endif
		
			_actual = _actual->next;
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActAlarmTone == 0)
					System[0].ActAlarmTone = buzzer_tones_qnt - 1;
				else
					System[0].ActAlarmTone--;
				_BUZZER_alarm_set_tone_list(System[0].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[0].ActAlarmTone, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActAlarmTone == 0)
					System[1].ActAlarmTone = buzzer_tones_qnt - 1;
				else
					System[1].ActAlarmTone--;
				_BUZZER_alarm_set_tone_list(System[1].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[1].ActAlarmTone, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActAlarmTone == 0)
					System[2].ActAlarmTone = buzzer_tones_qnt - 1;
				else
					System[2].ActAlarmTone--;
				_BUZZER_alarm_set_tone_list(System[2].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[2].ActAlarmTone, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActAlarmTone == 0)
					System[3].ActAlarmTone = buzzer_tones_qnt - 1;
				else
					System[3].ActAlarmTone--;
				_BUZZER_alarm_set_tone_list(System[3].ActAlarmTone);

#ifdef USART_debug
				USART_send("\tAlarm tone #");
				USART_write_buf(System[3].ActAlarmTone, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
			
			break;
	}
}

// Wybor glososci alarmu
void menu_4_fun(unsigned int key)
{
	uint8_t buffer_send[2];
	
	_LED_set_color_list(MENU_LED_Menu, MENU_4_LED_Color);
	_BUZZER_alarm_start();
	
	if (_actual->par == _Syg_1_menu)
		_LED_blink_on(MENU_LED_SYG1);
	if (_actual->par == _Syg_2_menu)
		_LED_blink_on(MENU_LED_SYG2);
	if (_actual->par == _Syg_3_menu)
		_LED_blink_on(MENU_LED_SYG3);
	if (_actual->par == _Syg_4_menu)
		_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_3):
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActAlarmVol == buzzer_vols_qnt - 1)
					System[0].ActAlarmVol = 0;
				else
					System[0].ActAlarmVol++;
				_BUZZER_alarm_set_vol_list(System[0].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[0].ActAlarmVol, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActAlarmVol == buzzer_vols_qnt - 1)
					System[1].ActAlarmVol = 0;
				else
					System[1].ActAlarmVol++;
				_BUZZER_alarm_set_vol_list(System[1].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[1].ActAlarmVol, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActAlarmVol == buzzer_vols_qnt - 1)
					System[2].ActAlarmVol = 0;
				else
					System[2].ActAlarmVol++;
				_BUZZER_alarm_set_vol_list(System[2].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[2].ActAlarmVol, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActAlarmVol == buzzer_vols_qnt - 1)
					System[3].ActAlarmVol = 0;
				else
					System[3].ActAlarmVol++;
				_BUZZER_alarm_set_vol_list(System[3].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[3].ActAlarmVol, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
			
			break;
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
		
			_BUZZER_alarm_stop();
		
			buffer_send[0] = MENU_RF_ActAlarmVolume;
			if (_actual->par == _Syg_1_menu) {
				buffer_send[1] = System[0].ActAlarmVol;
				RFM69W_sendWithRetry(System[0].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_2_menu) {
				buffer_send[1] = System[1].ActAlarmVol;
				RFM69W_sendWithRetry(System[1].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_3_menu) {
				buffer_send[1] = System[2].ActAlarmVol;
				RFM69W_sendWithRetry(System[2].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_4_menu) {
				buffer_send[1] = System[3].ActAlarmVol;
				RFM69W_sendWithRetry(System[3].ActAddress, buffer_send, 2, 15, 15);
			}
		
#ifdef USART_debug
			USART_send("-5- Alarm tempo setting mode.\n");
#endif

			_actual = _actual->next;
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActAlarmVol == 0)
					System[0].ActAlarmVol = buzzer_vols_qnt - 1;
				else
					System[0].ActAlarmVol--;
				_BUZZER_alarm_set_vol_list(System[0].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[0].ActAlarmVol, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActAlarmVol == 0)
					System[1].ActAlarmVol = buzzer_vols_qnt - 1;
				else
					System[1].ActAlarmVol--;
				_BUZZER_alarm_set_vol_list(System[1].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[1].ActAlarmVol, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActAlarmVol == 0)
					System[2].ActAlarmVol = buzzer_vols_qnt - 1;
				else
					System[2].ActAlarmVol--;
				_BUZZER_alarm_set_vol_list(System[2].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[2].ActAlarmVol, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActAlarmVol == 0)
					System[3].ActAlarmVol = buzzer_vols_qnt - 1;
				else
					System[3].ActAlarmVol--;
				_BUZZER_alarm_set_vol_list(System[3].ActAlarmVol);

#ifdef USART_debug
				USART_send("\tAlarm volume #");
				USART_write_buf(System[3].ActAlarmVol, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
			
			break;
	}
}

// Wybor tempa alarmu
void menu_5_fun(unsigned int key)
{
	uint8_t buffer_send[2];
	
	_LED_set_color_list(MENU_LED_Menu, MENU_5_LED_Color);
	_BUZZER_alarm_start();
	
	if (_actual->par == _Syg_1_menu)
		_LED_blink_on(MENU_LED_SYG1);
	if (_actual->par == _Syg_2_menu)
		_LED_blink_on(MENU_LED_SYG2);
	if (_actual->par == _Syg_3_menu)
		_LED_blink_on(MENU_LED_SYG3);
	if (_actual->par == _Syg_4_menu)
		_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_3):
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActAlarmTempo == buzzer_tempos_qnt - 1)
					System[0].ActAlarmTempo = 0;
				else
					System[0].ActAlarmTempo++;
				_BUZZER_alarm_set_tempo_list(System[0].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[0].ActAlarmTempo, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActAlarmTempo == buzzer_tempos_qnt - 1)
					System[1].ActAlarmTempo = 0;
				else
					System[1].ActAlarmTempo++;
				_BUZZER_alarm_set_tempo_list(System[1].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[1].ActAlarmTempo, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActAlarmTempo == buzzer_tempos_qnt - 1)
					System[2].ActAlarmTempo = 0;
				else
					System[2].ActAlarmTempo++;
				_BUZZER_alarm_set_tempo_list(System[2].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[2].ActAlarmTempo, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActAlarmTempo == buzzer_tempos_qnt - 1)
					System[3].ActAlarmTempo = 0;
				else
					System[3].ActAlarmTempo++;
				_BUZZER_alarm_set_tempo_list(System[3].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[3].ActAlarmTempo, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
			
			break;
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
		
			_BUZZER_alarm_stop();
		
			buffer_send[0] = MENU_RF_ActAlarmTempo;
			if (_actual->par == _Syg_1_menu) {
				buffer_send[1] = System[0].ActAlarmTempo;
				RFM69W_sendWithRetry(System[0].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_2_menu) {
				buffer_send[1] = System[1].ActAlarmTempo;
				RFM69W_sendWithRetry(System[1].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_3_menu) {
				buffer_send[1] = System[2].ActAlarmTempo;
				RFM69W_sendWithRetry(System[2].ActAddress, buffer_send, 2, 15, 15);
			}
			if (_actual->par == _Syg_4_menu) {
				buffer_send[1] = System[3].ActAlarmTempo;
				RFM69W_sendWithRetry(System[3].ActAddress, buffer_send, 2, 15, 15);
			}
		
#ifdef USART_debug
			USART_send("-1- LED color setting mode.\n");
#endif

			_actual = _actual->next;
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
		
			if (_actual->par == _Syg_1_menu) {
				if (System[0].ActAlarmTempo == 0)
					System[0].ActAlarmTempo = buzzer_tempos_qnt - 1;
				else
					System[0].ActAlarmTempo--;
				_BUZZER_alarm_set_tempo_list(System[0].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[0].ActAlarmTempo, DEC);
				USART_send(" for #1 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_2_menu) {
				if (System[1].ActAlarmTempo == 0)
					System[1].ActAlarmTempo = buzzer_tempos_qnt - 1;
				else
					System[1].ActAlarmTempo--;
				_BUZZER_alarm_set_tempo_list(System[1].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[1].ActAlarmTempo, DEC);
				USART_send(" for #2 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_3_menu) {
				if (System[2].ActAlarmTempo == 0)
					System[2].ActAlarmTempo = buzzer_tempos_qnt - 1;
				else
					System[2].ActAlarmTempo--;
				_BUZZER_alarm_set_tempo_list(System[2].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[2].ActAlarmTempo, DEC);
				USART_send(" for #3 sygnaller.\n");
#endif
			}
			if (_actual->par == _Syg_4_menu) {
				if (System[3].ActAlarmTempo == 0)
					System[3].ActAlarmTempo = buzzer_tempos_qnt - 1;
				else
					System[3].ActAlarmTempo--;
				_BUZZER_alarm_set_tempo_list(System[3].ActAlarmTempo);

#ifdef USART_debug
				USART_send("\tAlarm tempo #");
				USART_write_buf(System[3].ActAlarmTempo, DEC);
				USART_send(" for #4 sygnaller.\n");
#endif
			}
			
			break;
	}
}
