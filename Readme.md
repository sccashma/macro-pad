# Macro Pad

This project is for the macro pad being developed by Scott Cashman

## macro definitions

The macros are stored in the "/macros.csv" file in the root of the SD card. The first line is the headers which can be anything you want (it's ignored). Each entry MUST have a unique id and MUST be between 0 and 65534.

Example macro.csv

```txt
id,	display name,	            bmp,	        macro
0,	A_FLAM-40_Flame_Sentry,     A_FLA039.bmp,	DOWN,UP,RIGHT,DOWN,UP,UP
1,	AM-23_EMS_Mortar_Sentry,    AM-23040.bmp,	DOWN,UP,RIGHT,DOWN,RIGHT
2,	Anti-Materiel_Rifle,        ANTI-041.bmp,	DOWN,LEFT,RIGHT,UP,DOWN
3,	Anti-Personnel_Minefield,   ANTI-042.bmp,	DOWN,LEFT,UP,RIGHT
4,	Arc_Thrower,                ARC_T043.bmp,	DOWN,RIGHT,DOWN,UP,LEFT,LEFT
5,	Autocannon,                 AUTOC045.bmp,	DOWN,LEFT,DOWN,UP,UP,RIGHT
6,	Autocannon_Sentry,          AUTOC044.bmp,	DOWN,UP,RIGHT,UP,LEFT,UP
7,	Ballistic_Shield_Backpack,  BALLI046.bmp,	DOWN,LEFT,DOWN,DOWN,UP,LEFT
8,	Commando,                   COMMA047.bmp,	DOWN,LEFT,UP,DOWN,RIGHT
```

**LIMITATION:** Due to some current performance optimisations, it's possible to freeze the device if there are too many macros defined, or macros too long.

## Background image

The background image must be a 24-bit bitmap. To fill the screen, it should be 240x320 in size. Call this image "bckgrnd.bmp" and place it in the root of the sd.

## icons

Icons should be 80x80 24-bit bitmap images. They must be stored in the "/icons/" directory. 
Names must not exceed 8 characters in lenght (excluding the extension ".bmp").

The "menu.bmp" image for the settings button should be placed in the icons directory also.
