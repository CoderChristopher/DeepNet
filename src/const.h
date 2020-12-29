#ifndef __CONST_H__
#define __CONST_H__
//List of defines
//Boolean logic
#define BOOLEAN char
#define TRUE 1
#define FALSE 0
#define PI 3.14159

#define DEFAULT_STRING_SIZE 128

//Defines actions used in the Input()
#define ACTION_NONE       0
#define ACTION_MOVE_LEFT  1
#define ACTION_MOVE_RIGHT 2
#define ACTION_MOVE_UP    3
#define ACTION_MOVE_DOWN  4
#define ACTION_GRAB       5

#define STATE_INGAME      0
#define STATE_GAMEOVER    1
#define STATE_DISCONNECT  2
#define STATE_ESCAPE      3
#define STATE_HELP        4
#define STATE_EXAMINE     5 
#define STATE_HIGHEST     5 

#define BASE_TRACE_DELTA  0.1f

#define INTERACTABLE_STACKSIZE 16

#define TYPE_DEFENSE 0
#define TYPE_ATTACK  1
#define TYPE_OTHER   2
//EntryPoints
#define EP_TEST                    -3
#define EP_NIL                     -2
#define EP_MAIN                    -1

//Rendering
#define EP_INGAME_RENDER            0
#define EP_MENU_RENDER              1
#define EP_GAMEOVER_RENDER          2
#define EP_DISCONNECT_RENDER        3
#define EP_ESCAPE_RENDER            4
#define EP_HELP_RENDER              5
#define EP_EXAMINE_RENDER           6

//Input
#define EP_INGAME_PROCESSINPUT      10
#define EP_GAMEOVER_PROCESSINPUT    11
#define EP_MENU_PROCESSINPUT        12
#define EP_DISCONNECT_PROCESSINPUT  13
#define EP_ESCAPE_PROCESSINPUT      14
#define EP_HELP_PROCESSINPUT        15
#define EP_EXAMINE_PROCESSINPUT     16

//Update
#define EP_INGAME_UPDATE            20
#define EP_MENU_UPDATE              21
#define EP_DISCONNECT_UPDATE        22
#define EP_GAMEOVER_UPDATE          23
#define EP_ESCAPE_UPDATE            24
#define EP_HELP_UPDATE              25
#define EP_EXAMINE_UPDATE           26

//Actor Functions
#define EP_CALCULATEDAMAGES         30
#define EP_DEFAULTCOLLISIONBEHAVIOR 31
#define EP_DEFAULTACTBEHAVIOR       32
#define EP_NEWACTOR                 33
#define EP_REMOVEACTOR              34
#define EP_GIVEATTACK               35
#define EP_GIVEDEFENSE              36
#define EP_KILLERINPUT              37
#define EP_KILLERACT                38
#define EP_KILLERCOLLISION          39
#define EP_SYSADMININPUT            40
#define EP_SYSADMINCOLLISION        41 
#define EP_SYSADMINACT              42
#define EP_PLAYERINPUT              43
#define EP_PLAYERACT                44
#define EP_PLAYERCOLLISION          45

//Data
#define EP_NEWDATA                  50

//Dll
#define EP_NEWDLL                   60
#define EP_APPENDDLL                61
#define EP_REMOVEDLL                62
#define EP_NEXTDLL                  63
#define EP_FREEDLL		    64

//Gamestate
#define EP_CHANGEGAMESTATE          70

//Interactables
#define EP_NEWINTERACTABLE          80
#define EP_BUILDWARPSTRUCT          81
#define EP_BUILDSOFTWARESTRUCT      82
#define EP_BUILDMONEYSTRUCT         83
#define EP_BUILDDATASTRUCT          84
#define EP_SOFTWARECOLLISION        85
#define EP_DATACOLLISION            86
#define EP_MONEYCOLLISION           87
#define EP_WARPCOLLISION            88
#define EP_CHECKPOINTCOLLISION      89
#define EP_DISCONNECTCOLLISION      90

//Map
#define EP_GENERATEMAP              100

//ncursesinit
#define EP_NCURSESINIT              110
#define EP_NCOLORPAIRS              111

//Rooms
#define EP_CHECKPASSABLE            120
#define EP_LINEOFSIGHT              121
#define EP_RANDOMROOM               122
#define EP_PILLARS                  123
#define EP_RECTANGULARROOM          124
#define EP_CIRCLEROOM               125
#define EP_NEWROOM                  126
#define EP_TRANSFERACTORTOROOM      127

//Tiles
#define EP_NEWTILE                  140

//Utility
#define EP_CALCULATESCORE           150

//LOOK
#define EP_NEWLOOK                  160
#endif
