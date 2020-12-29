#include "update.h"
void INGAME_Update(){
	SetEntryPoint(EP_INGAME_UPDATE);	
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
		do{
			if(!actors->current)
				break;
			struct actor* act=actors->current;
			if(act->act)
				act->act(act);
			if(act->health<=0)
				RemoveActor(act);
			
		}while((actors=NextDll(actors)));
	}while((croom=NextDll(croom)));
	PlayerIncrementTrace();
	PlayerSetDeltaTrace(BASE_TRACE_DELTA);
	if(PlayerGetTrace()>=100.0)
		ChangeGameState(STATE_DISCONNECT);
	ResetEntryPoint();
	return;
}
void MENU_Update(){
	SetEntryPoint(EP_MENU_UPDATE);
	//Really in case I ever add code here...
	ResetEntryPoint();
	return;
}
void DISCONNECT_Update(){
	SetEntryPoint(EP_DISCONNECT_UPDATE);
	//Really in case I ever add code here...
	ResetEntryPoint();
	return;
}
void GAMEOVER_Update(){
	SetEntryPoint(EP_GAMEOVER_UPDATE);
	//Really in case I ever add code here...
	ResetEntryPoint();
	return;
}
void ESCAPE_Update(){
	SetEntryPoint(EP_GAMEOVER_UPDATE);
	//Really in case I ever add code here...
	ResetEntryPoint();
	return;
}
void HELP_Update(){
	SetEntryPoint(EP_GAMEOVER_UPDATE);
	//Really in case I ever add code here...
	ResetEntryPoint();
	return;
}
void EXAMINE_Update(){
	SetEntryPoint(EP_EXAMINE_UPDATE);	
	if(!currentmap||!currentmap->rooms){
		ResetEntryPoint();
		return;
	}
	struct dll* actors=currentroom->actors;
	do{
		if(!actors->current)
			break;
		struct actor* act=actors->current;
		if(act->x==examinex&&act->y==examiney){
			strncpy(examinename,act->apperance->name,128);		
			strncpy(examinedescription,act->apperance->description,128);		
			strncpy(examinealt,act->apperance->flavor,128);		
			return;
		}
	}while((actors=NextDll(actors)));
	struct dll* interactables=currentroom->interactables;
	do{
		if(!interactables)
			break;
		if(!interactables->current)
			break;
		struct interactable* inter=interactables->current;
		if(inter->x==examinex&&inter->y==examiney){
			strncpy(examinename,inter->apperance->name,128);		
			strncpy(examinedescription,inter->apperance->description,128);		
			switch(inter->type){
				case TYPE_DEFENSE:;
					char flavor[128];
					sprintf(flavor,"Name:%s, Defense Rating:%i",((struct software*)inter->stack)->name,((struct software*)inter->stack)->value);
					strncpy(examinealt,flavor,128);		
					break;
				case TYPE_ATTACK:
					sprintf(flavor,"Name:%s, Attack Rating:%i",((struct software*)inter->stack)->name,((struct software*)inter->stack)->value);
					strncpy(examinealt,flavor,128);		
					break;
				case TYPE_OTHER:
					examinealt[0]=0;
					break;
			}
			return;
		}
	}while((interactables=NextDll(interactables)));
	ResetEntryPoint();
	return;
}
