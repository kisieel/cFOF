#ifndef __MENU_H
#define __MENU_H

#define MENU_LED_Menu    1
#define MENU_LED_SYG1    5
#define MENU_LED_SYG2    4
#define MENU_LED_SYG3    3
#define MENU_LED_SYG4    2

typedef struct _menuitem
{
	struct _menuitem *next;
//	struct _menuitem *prev;
	struct _menuitem *sub;
	struct _menuitem *par;
	void (*menu_fun)(unsigned int);
} MENU;

extern MENU *_actual;

void MENU_init(void);

extern MENU *_Syg_1_menu;
extern MENU *_Syg_2_menu;
extern MENU *_Syg_3_menu;
extern MENU *_Syg_4_menu;

#endif
