#ifndef __MENU_H
#define __MENU_H

#define MENU_LED_Menu    1
#define MENU_LED_SYG1    5
#define MENU_LED_SYG2    4
#define MENU_LED_SYG3    3
#define MENU_LED_SYG4    2

#define MENU_1_LED_Color 1
#define MENU_2_LED_Color 2
#define MENU_3_LED_Color 3
#define MENU_4_LED_Color 4
#define MENU_5_LED_Color 5

#define MENU_RF_SendInfo       0x1F
#define MENU_RF_Recruit        0x2F
#define MENU_RF_GetInfo        0x3F
#define MENU_RF_HallAlarm      0x4F
#define MENU_RF_LogOff         0x5F

#define MENU_RF_ActColor       3
#define MENU_RF_ActBrightness  4
#define MENU_RF_ActAlarmTone   5
#define MENU_RF_ActAlarmVolume 6
#define MENU_RF_ActAlarmTempo  7

typedef struct _menuitem
{
	struct _menuitem *next;
	struct _menuitem *prev;
	struct _menuitem *sub;
	struct _menuitem *par;
	uint8_t LED_number;
	uint8_t LED_color;
	void (*menu_fun)(unsigned int);
} MENU;

extern MENU *_actual;

void MENU_init(void);
void MENU_PacketInterpreter(void);

extern MENU *_Syg_1_menu;
extern MENU *_Syg_2_menu;
extern MENU *_Syg_3_menu;
extern MENU *_Syg_4_menu;

#endif
