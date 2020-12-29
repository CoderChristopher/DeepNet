#include "gamestate.h"
void ChangeGameState(unsigned char state){
	if(state>STATE_HIGHEST)
		return;
	SetEntryPoint(EP_CHANGEGAMESTATE);
	switch(state){
		case STATE_INGAME:
			Update=&INGAME_Update;
			Render=&INGAME_Render;
			ProcessInput=&INGAME_ProcessInput;
			break;
		case STATE_GAMEOVER:
			Update=&GAMEOVER_Update;
			Render=&GAMEOVER_Render;
			ProcessInput=&GAMEOVER_ProcessInput;
			break;
		case STATE_DISCONNECT:
			Update=&DISCONNECT_Update;
			Render=&DISCONNECT_Render;
			ProcessInput=&DISCONNECT_ProcessInput;
			break;
		case STATE_ESCAPE:
			Update=&ESCAPE_Update;
			Render=&ESCAPE_Render;
			ProcessInput=&ESCAPE_ProcessInput;
			break;
		case STATE_HELP:
			animationticks=0;

			Update=&HELP_Update;
			Render=&HELP_Render;
			ProcessInput=&HELP_ProcessInput;
			break;
		case STATE_EXAMINE:
			examinex=player->x;
			examiney=player->y;

			Update=&EXAMINE_Update;
			Render=&EXAMINE_Render;
			ProcessInput=&EXAMINE_ProcessInput;
			break;
	}
	gamestate=state;
	ResetEntryPoint();
	return;
}
