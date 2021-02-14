#ifndef __GLOBALS__H__
#define __GLOBALS__H__
#include "includes.h"
//Game quit flag
extern BOOLEAN done;
//Game tally variables
extern unsigned char gamestate;
extern unsigned int entitycount;

extern int highestmoney;
extern int ch;

extern int entrypoint;
extern int preventrypoint;
extern time_t seed;

extern int animationticks;
extern int examinex;
extern int examiney;
extern char examinename[128];
extern char examinedescription[128];
extern char examinealt[128];
//Ncurses global color definitions
extern int COL_WH  ;
extern int COL_RD  ;
extern int COL_RDBK;
extern int COL_RDGR;
extern int COL_RDYL;
extern int COL_GRBL;
extern int COL_GR  ;
extern int COL_GRWH;
extern int COL_YL  ;
extern int COL_YLBK;
extern int COL_BL  ;
extern int COL_BLWH;
//Game Objects
extern struct dll* datalist;
extern struct dll* looklist;
extern struct room* currentroom;
extern struct map* currentmap;

extern struct actor* player;

//Global Loop Function pointers
extern void (*ProcessInput)();
extern void (*Update)();
extern void (*Render)();

//Prototypes
void ChangeGameState(unsigned char state);
#endif
