#include "globals.h"
BOOLEAN done=FALSE;
unsigned char gamestate=STATE_INGAME;
unsigned int entitycount=0;

int highestmoney=512;
int ch=0;

int entrypoint=EP_NIL;
int preventrypoint=EP_NIL;

int animationticks=0;

int examinex=0;
int examiney=0;
char examinename[128]="NO NAME";
char examinedescription[128]="NO DESCRIPTION";
char examinealt[128]="";
time_t seed=-1;

//Ncurses global color definitions
int COL_WH  ;
int COL_RD  ;
int COL_RDBK;
int COL_RDGR;
int COL_RDYL;
int COL_GRBL;
int COL_GR  ;
int COL_GRWH;
int COL_YL  ;
int COL_YLBK;
int COL_BL  ;
int COL_BLWH;
//Game Objects
struct dll* datalist=NULL;
struct dll* looklist=NULL;

//These pointers are only ment to maintain a copy of the struct that is held in a dll.
struct room* currentroom=NULL;
struct map* currentmap=NULL;

struct actor* player;

//Global Loop Function pointers
void (*ProcessInput)()=NULL;
void (*Update)()=NULL;
void (*Render)()=NULL;
