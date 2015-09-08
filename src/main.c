#include "main.h"

int main()
{
	uint32_t data;
	
	SYS_TICK_init();
	
	data = SYS_TICK_timeOut(0,0);
	while (SYS_TICK_timeOut(1, data) < 400);
	
	// Keep power supply
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;            // Clock for GPIOA
	GPIO_config(0x0A, 0, GPIO_MODE_GP, GPIO_PULL_Floating, GPIO_TYPE_Pushpull, GPIO_SPEED_400k, 0);
	GPIOA->BSRRL |= GPIO_BSRR_BS_0;               // Output 1
	
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

	_LED_set_color_list(MENU_LED_Menu, 0);
	_LED_set_color_list(MENU_LED_SYG1, 1);
	_LED_set_color_list(MENU_LED_SYG2, 2);
	_LED_set_color_list(MENU_LED_SYG3, 3);
	_LED_set_color_list(MENU_LED_SYG4, 4);


	for(;;) {
		_actual->menu_fun(GetKeys());
		
		if (GPIOA->IDR & GPIO_IDR_IDR_1) {
			data = SYS_TICK_timeOut(0, 0);
			while (GPIOA->IDR & GPIO_IDR_IDR_1) {
				// Keep blinking
				if (_actual == _Syg_1_menu || _actual->par == _Syg_1_menu)
					_LED_blink_on(MENU_LED_SYG1);
				if (_actual == _Syg_2_menu || _actual->par == _Syg_2_menu)
					_LED_blink_on(MENU_LED_SYG2);
				if (_actual == _Syg_3_menu || _actual->par == _Syg_3_menu)
					_LED_blink_on(MENU_LED_SYG3);
				if (_actual == _Syg_4_menu || _actual->par == _Syg_4_menu)
					_LED_blink_on(MENU_LED_SYG4);
				
				if (SYS_TICK_timeOut(1, data) > TurnOffTime) {
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