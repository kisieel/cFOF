#include "main.h"

volatile SystemType System[4];

int main()
{
	uint32_t data1, data2;
	
	SYS_TICK_init();
	
	data1 = SYS_TICK_timeOut(0,0);
	while (SYS_TICK_timeOut(1, data1) < 1000);
	
	// Keep power supply
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;            // Clock for GPIOA
	GPIO_config(0x0A, 3, GPIO_MODE_GP, GPIO_PULL_Floating, GPIO_TYPE_Pushpull, GPIO_SPEED_400k, 0);
	PowerOn;
	
	KEY_init();
	MENU_init();
	USART_init();
	
#ifdef USART_debug
	USART_send("Peripherals initialized.\n");
#endif
	
	RFM69W_init();
	_BUZZER_init();
	_LED_init();
	_LED_off();

#ifdef USART_debug
	USART_send("External devices initialized.\n");
#endif

	_LED_change_brightness_limit_list(0);

	_LED_set_color_list(MENU_LED_Menu, 0);
	_LED_set_color_list(MENU_LED_SYG1, 1);
	_LED_set_color_list(MENU_LED_SYG2, 2);
	_LED_set_color_list(MENU_LED_SYG3, 3);
	_LED_set_color_list(MENU_LED_SYG4, 4);
	_LED_on();

	// Block device while the power button is still pressed
	while(GPIOA->IDR & GPIO_IDR_IDR_0);

	for(;;) {
		_actual->menu_fun(GetKeys());
		
		// Holding key for GoUpTime will make menu to go up, to level 0
		if (GPIOA->IDR & GPIO_IDR_IDR_2) {
			data2 = SYS_TICK_timeOut(0, 0);
			while (GPIOA->IDR & GPIO_IDR_IDR_2) {
				// Keep blinking
				if (_actual == _Syg_1_menu || _actual->par == _Syg_1_menu)
					_LED_blink_on(MENU_LED_SYG1);
				if (_actual == _Syg_2_menu || _actual->par == _Syg_2_menu)
					_LED_blink_on(MENU_LED_SYG2);
				if (_actual == _Syg_3_menu || _actual->par == _Syg_3_menu)
					_LED_blink_on(MENU_LED_SYG3);
				if (_actual == _Syg_4_menu || _actual->par == _Syg_4_menu)
					_LED_blink_on(MENU_LED_SYG4);
				
				if (SYS_TICK_timeOut(1, data2) > GoUpTime) {
					_LED_set_color(MENU_LED_Menu, 0, 0, 0);
					_LED_on();
	#ifdef USART_debug
					USART_send("Going up to level 0.\n");
	#endif
					_actual = _actual->par;
			}
		}
	}
		
		// Holding key for TurnOffTime will make device to turn off
		if (GPIOA->IDR & GPIO_IDR_IDR_0) {
			data1 = SYS_TICK_timeOut(0, 0);
			while (GPIOA->IDR & GPIO_IDR_IDR_0) {
				// Keep blinking
				if (_actual == _Syg_1_menu || _actual->par == _Syg_1_menu)
					_LED_blink_on(MENU_LED_SYG1);
				if (_actual == _Syg_2_menu || _actual->par == _Syg_2_menu)
					_LED_blink_on(MENU_LED_SYG2);
				if (_actual == _Syg_3_menu || _actual->par == _Syg_3_menu)
					_LED_blink_on(MENU_LED_SYG3);
				if (_actual == _Syg_4_menu || _actual->par == _Syg_4_menu)
					_LED_blink_on(MENU_LED_SYG4);
				
				if (SYS_TICK_timeOut(1, data1) > TurnOffTime) {
					EEPROM_SystemBackup();
					_LED_off();
#ifdef USART_debug
					USART_send("Power off detected. System backed up. Switching off.\n");
#endif
					PowerOff;
				}
			}
		}
	}
}