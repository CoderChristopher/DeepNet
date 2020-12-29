#include "input.h"
#include "globals.h"
void INGAME_ProcessInput(){
	SetEntryPoint(EP_INGAME_PROCESSINPUT);
	timeout(150);	
	while((ch=getch())==ERR)
		Render();//Just run animations while waiting flushinp();
	switch(ch){
		case 'q':
			done=TRUE;
			ResetEntryPoint();
			return;
		case '?':
			ChangeGameState(STATE_HELP);
			ResetEntryPoint();
			return;	
		case 'e':
			ChangeGameState(STATE_EXAMINE);
			ResetEntryPoint();
			return;	
		default:;

	}
	if(!currentmap||!currentmap->rooms){
		ResetEntryPoint();
		return;
	}
	struct dll* croom=currentmap->rooms;
	do{
		if(!croom)
			break;
		if(!croom->current)
			break;
		struct dll* actors=((struct room*)croom->current)->actors;
		if(!actors)
			continue;
		while(actors){
			if(!actors->current)
				break;
			struct actor* act=actors->current;
			if(act)
			if(act->actiondecisionfunction)
				act->actiondecisionfunction(act);
			actors=NextDll(actors);
		}
	}while((croom=NextDll(croom)));
	ResetEntryPoint();
	return;
}
void MENU_ProcessInput(){
	SetEntryPoint(EP_MENU_PROCESSINPUT);
	timeout(150);	
	while((ch=getch())==ERR)
		Render();//Just run animations while waiting
	flushinp();
	switch(ch){
		case 'q':
			done=TRUE;
			ResetEntryPoint();
			return;
		default:
			ChangeGameState(STATE_INGAME);
			ResetEntryPoint();
			return;
	}
	ResetEntryPoint();
	return;
}
void DISCONNECT_ProcessInput(){
	SetEntryPoint(EP_DISCONNECT_PROCESSINPUT);
	timeout(150);	
	while((ch=getch())==ERR)
		Render();//Just run animations while waiting
	flushinp();
	switch(ch){
		default:
			done=TRUE;
			ResetEntryPoint();
			return;
	}
	ResetEntryPoint();
	return;
}
void GAMEOVER_ProcessInput(){
	SetEntryPoint(EP_GAMEOVER_PROCESSINPUT);
	timeout(150);	
	while((ch=getch())==ERR)
		Render();//Just run animations while waiting
	flushinp();
	switch(ch){
		case 'q':
			done=TRUE;
			ResetEntryPoint();
			return;
	}
	ResetEntryPoint();
	return;
}
void ESCAPE_ProcessInput(){
	SetEntryPoint(EP_GAMEOVER_PROCESSINPUT);
	timeout(15);	
	while((ch=getch())==ERR)
		Render();//Just run animations while waiting
	flushinp();
	switch(ch){
		case 'q':case 'Q':
			done=TRUE;
			ResetEntryPoint();
			return;
	}
	ResetEntryPoint();
	return;
}
void HELP_ProcessInput(){
	SetEntryPoint(EP_GAMEOVER_PROCESSINPUT);
	while((ch=getch())==ERR){
		Render();//Just run animations while waiting
		if(animationticks<=19)
			timeout(100);
		else
			timeout(2500);	
	}
	flushinp();
	switch(ch){
		case 'q':
			ChangeGameState(STATE_INGAME);	
			break;
	}
	ResetEntryPoint();
	return;
}
void EXAMINE_ProcessInput(){
	SetEntryPoint(EP_EXAMINE_PROCESSINPUT);
	timeout(150);	
	while((ch=getch())==ERR)
		Render();//Just run animations while waiting
	flushinp();
	switch(ch){
		case 'q':
			ChangeGameState(STATE_INGAME);
			break;
		case 'h':case 'a':
			if(examinex>0)
				examinex--;
			break;
		case 'l':case 'd':
			if(examinex<currentroom->sizex-1)
				examinex++;
			break;
		case 'k':case 'w':
			if(examiney>0)
				examiney--;
			break;
		case 'j':case 's':
			if(examiney<currentroom->sizey-1)
				examiney++;
			break;
	}
	ResetEntryPoint();
	return;
}
