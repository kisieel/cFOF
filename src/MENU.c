#include <stdlib.h>
#include "main.h"

uint8_t Syg_qnt = 4;

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

void menu_par(MENU *);

void menu_par(MENU *parent)
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
	_Syg_1_menu->sub = _1_menu;
	_Syg_1_menu->menu_fun = menu_Syg_1_fun;
	
	_Syg_2_menu->next = _Syg_3_menu;
	_Syg_2_menu->sub = _1_menu;
	_Syg_2_menu->menu_fun = menu_Syg_2_fun;
	
	_Syg_3_menu->next = _Syg_4_menu;
	_Syg_3_menu->sub = _1_menu;
	_Syg_3_menu->menu_fun = menu_Syg_3_fun;
	
	_Syg_4_menu->next = _Syg_NONE_menu;
	_Syg_4_menu->sub = _1_menu;
	_Syg_4_menu->menu_fun = menu_Syg_4_fun;
	
	// 1 level
	
	_1_menu->next = _2_menu;
	_1_menu->par = NULL;
	_1_menu->menu_fun = menu_1_fun;
	
	_2_menu->next = _3_menu;
	_2_menu->par = NULL;
	_2_menu->menu_fun = menu_2_fun;
	
	_3_menu->next = _4_menu;
	_3_menu->par = NULL;
	_3_menu->menu_fun = menu_3_fun;
	
	_4_menu->next = _5_menu;
	_4_menu->par = NULL;
	_4_menu->menu_fun = menu_4_fun;
	
	_5_menu->next = _1_menu;
	_5_menu->par = NULL;
	_5_menu->menu_fun = menu_5_fun;
	
	_actual = _Syg_NONE_menu;
}

void menu_Syg_NONE_fun(unsigned int key)
{
	switch (key) {
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
			if (Syg_qnt > 0)
				_actual = _actual->next;
			else
				_actual = _Syg_NONE_menu;
			
			break;
	}
}

void menu_Syg_1_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG1);
	
	switch (key) {
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
			menu_par(_actual);
			_actual = _actual->sub;
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
			if (Syg_qnt > 1)
				_actual = _actual->next;
			else
				_actual = _Syg_NONE_menu;
			
			_LED_blink_off(MENU_LED_SYG1);
			
			break;
	}
}

void menu_Syg_2_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG2);
	
	switch (key) {
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
			menu_par(_actual);
			_actual = _actual->sub;
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
			if (Syg_qnt > 2)
				_actual = _actual->next;
			else
				_actual = _Syg_NONE_menu;
			
			_LED_blink_off(MENU_LED_SYG2);
			
			break;
	}
}

void menu_Syg_3_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG3);
	
	switch (key) {
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
			menu_par(_actual);
			_actual = _actual->sub;
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
			if (Syg_qnt > 3)
				_actual = _actual->next;
			else
				_actual = _Syg_NONE_menu;
			
			_LED_blink_off(MENU_LED_SYG3);
			
			break;
	}
}

void menu_Syg_4_fun(unsigned int key)
{
	_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
			menu_par(_actual);
			_actual = _actual->sub;
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
			_actual = _actual->next;
			
			_LED_blink_off(MENU_LED_SYG4);
			
			break;
	}
}

void menu_1_fun(unsigned int key)
{
	if (_actual->par == _Syg_1_menu)
		_LED_blink_on(MENU_LED_SYG1);
	if (_actual->par == _Syg_2_menu)
		_LED_blink_on(MENU_LED_SYG2);
	if (_actual->par == _Syg_3_menu)
		_LED_blink_on(MENU_LED_SYG3);
	if (_actual->par == _Syg_4_menu)
		_LED_blink_on(MENU_LED_SYG4);
	
	switch (key) {
		case (KEY_2):
			ClrKeyb( KBD_LOCK );
			break;
		case (KEY_1):
			ClrKeyb( KBD_LOCK );
			break;
	}
}

void menu_2_fun(unsigned int key)
{
	
}

void menu_3_fun(unsigned int key)
{
	
}

void menu_4_fun(unsigned int key)
{
	
}

void menu_5_fun(unsigned int key)
{
	
}