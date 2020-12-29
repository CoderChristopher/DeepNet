#include "ninit.h"
void NCursesInit(){
	SetEntryPoint(EP_NCURSESINIT);

	initscr();
	start_color();
	raw();
	noecho();
	cbreak();

	ResetEntryPoint();
	return;
}

void NColorPairs(){
	SetEntryPoint(EP_NCOLORPAIRS);

	init_pair( 1,COLOR_WHITE ,COLOR_BLACK );
	COL_WH  =COLOR_PAIR( 1);
	init_pair( 2,COLOR_RED   ,COLOR_BLACK );
	COL_RD  =COLOR_PAIR( 2);
	init_pair( 3,COLOR_RED   ,COLOR_YELLOW);
	COL_RDYL=COLOR_PAIR( 3);
	init_pair( 4,COLOR_GREEN ,COLOR_BLUE  );
	COL_GRBL=COLOR_PAIR( 4);
	init_pair( 5,COLOR_GREEN ,COLOR_BLACK );
	COL_GR  =COLOR_PAIR( 5);
	init_pair( 6,COLOR_GREEN ,COLOR_WHITE);
	COL_GRWH=COLOR_PAIR( 6);
	init_pair( 7,COLOR_RED   ,COLOR_GREEN );
	COL_RDGR=COLOR_PAIR( 7);
	init_pair( 8,COLOR_YELLOW,COLOR_BLACK );
	COL_YL  =COLOR_PAIR( 8);
	init_pair( 9,COLOR_BLUE  ,COLOR_BLACK );
	COL_BL  =COLOR_PAIR( 9);
	init_pair(10,COLOR_BLUE  ,COLOR_WHITE );
	COL_BLWH=COLOR_PAIR(10);
	init_pair(11,COLOR_BLACK ,COLOR_RED );
	COL_RDBK=COLOR_PAIR(11);
	init_pair(12,COLOR_YELLOW,COLOR_BLACK);
	COL_YLBK=COLOR_PAIR(12);

	ResetEntryPoint();
	return;
}
