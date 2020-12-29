#include "interactables.h"
struct interactable* NewInteractable(struct look* look,int x,int y,struct room* rm,void* col,void* stackinit){
	if(!rm)
		return NULL;

	SetEntryPoint(EP_NEWINTERACTABLE);
	struct interactable* inter=malloc(sizeof(struct interactable));
	AppendDll(rm->interactables,inter);
	inter->apperance=look;
	inter->x=x;
	inter->y=y;
	inter->collisionresolver=col;
	inter->type=TYPE_OTHER;
	if(stackinit){
		for(char i=0;i<INTERACTABLE_STACKSIZE;i++)
			inter->stack[i]=((char*)stackinit)[i];
	}else{
		memset(&inter->stack[0],0,INTERACTABLE_STACKSIZE);
	}

	ResetEntryPoint();
	return inter;
}
//WARNING:Any function that works with this should not wield the pointer
//But rather should memcpy the contents of the pointer into a
//New stack. This is because on sucsessive runs of BuildWarpStruct
//it will be changing the same memory and so that old pointer will
//point to invalid memory
char* BuildWarpStruct(struct room* dest,int x,int y){
	if(!dest)
		return NULL;
	SetEntryPoint(EP_BUILDWARPSTRUCT);

	static char stack[INTERACTABLE_STACKSIZE];
	memcpy(stack,&dest,sizeof(void*));
	memcpy(&stack[sizeof(void*)],&x,sizeof(int));
	memcpy(&stack[sizeof(void*)+sizeof(int)],&y,sizeof(int));
	
	ResetEntryPoint();
	return stack;
}
char* BuildSoftwareStruct(struct software* soft){
	if(!soft)
		return NULL;
	SetEntryPoint(EP_BUILDSOFTWARESTRUCT);

	static char stack[INTERACTABLE_STACKSIZE];
	memcpy(stack,soft,sizeof(struct software));
	
	ResetEntryPoint();
	return stack;
}
char* BuildMoneyStruct(int value){
	SetEntryPoint(EP_BUILDMONEYSTRUCT);

	static char stack[INTERACTABLE_STACKSIZE];
	memcpy(stack,&value,sizeof(int));

	ResetEntryPoint();
	return stack;
}
char* BuildDataStruct(struct data* dest){
	if(!dest)
		return NULL;
	SetEntryPoint(EP_BUILDDATASTRUCT);

	static char stack[INTERACTABLE_STACKSIZE];
	memcpy(stack,&dest,sizeof(void*));

	ResetEntryPoint();
	return stack;
}
void SoftwareCollision(struct interactable* self,struct actor* collision){
	if(!collision||!self)
		return;
	SetEntryPoint(EP_SOFTWARECOLLISION);
	
	if(!collision->data)
		NewDll(&collision->data);
	struct software holder;
	memcpy(&holder,self->stack,sizeof(struct software));
	switch(holder.type){
		case TYPE_ATTACK:
			if(collision->attack)
				free(collision->attack);
			collision->attack=malloc(sizeof(struct software));
			memcpy(collision->attack,&holder,sizeof(struct software));
			break;
		case TYPE_DEFENSE:
			if(collision->defense)
				free(collision->defense);
			collision->defense=malloc(sizeof(struct software));
			memcpy(collision->defense,&holder,sizeof(struct software));
			break;
	}

	RemoveDll(&collision->currentroom->interactables,self);
	
	ResetEntryPoint();
	return;
}
void DataCollision(struct interactable* self,struct actor* collision){
	if(!collision||!self)
		return;
	SetEntryPoint(EP_DATACOLLISION);

	if(!collision->data)
		NewDll(&collision->data);
	struct data* datum=NULL;
	memcpy(&datum,self->stack,sizeof(void*));
	AppendDll(collision->data,datum);
	RemoveDll(&collision->currentroom->interactables,self);

	ResetEntryPoint();
	return;
}
void MoneyCollision(struct interactable* self,struct actor* collision){
	if(!collision||!self)
		return;
	if(!collision->apperance)
		return;
	SetEntryPoint(EP_MONEYCOLLISION);

	if(collision->apperance->character!='@')
		return;
	int value;
	memcpy(&value,&self->stack,sizeof(int));
	PlayerSetMoney(PlayerGetMoney()+value);
	RemoveDll(&collision->currentroom->interactables,self);

	ResetEntryPoint();
	return;
}
void WarpCollision(struct interactable* self,struct actor* collision){
	if(!collision)
		return;
	SetEntryPoint(EP_WARPCOLLISION);

	//Let the voodoo begin
	
	struct room* gotoroom;
	memcpy(&gotoroom,self->stack,sizeof(void*));
	if(!gotoroom){
		ResetEntryPoint();
		return;
	}
	//Alright so a interactable's stack is 16 bytes long
	//I use the first part of the stack [0] to hold the pointer for
	//the destination room
	//I use the next availble memory segment for the x position of type int
	//I use the last availble memory segment for the y position of type int
	//In order to reference each of these within the stack I use sizeof(*)
	//for the previous type in the above list to get the offset of the current
	//sought variable
	memcpy(&collision->x,&self->stack[sizeof(void*)],sizeof(int));
	memcpy(&collision->y,&self->stack[sizeof(void*)+sizeof(int)],sizeof(int));
	switch(collision->action){
		case ACTION_MOVE_LEFT:
			collision->x--;
			break;
		case ACTION_MOVE_RIGHT:
			collision->x++;
			break;
		case ACTION_MOVE_UP:
			collision->y--;
			break;
		case ACTION_MOVE_DOWN:
			collision->y++;
			break;
	}
	TransferActorToRoom(gotoroom,collision);
	if(!collision->apperance){
		ResetEntryPoint();
		return;
	}
	if(collision->apperance->character=='@')
		currentroom=collision->currentroom;

	ResetEntryPoint();
	return;
}
void CheckPointCollision(struct interactable* self,struct actor* collision){
	if(!self||!collision)
		return;
	if(!collision->apperance)
		return;
	SetEntryPoint(EP_CHECKPOINTCOLLISION);
	
	if(collision->apperance->character=='@'){
		if(!(rand()%collision->stealth)){
			PlayerSetDeltaTrace(PlayerGetDeltaTrace()+2.0f);
		}
	}
	ResetEntryPoint();
	return;
}
void DisconnectCollision(struct interactable* self,struct actor* collision){
	if(!self||!collision)
		return;
	if(!collision->apperance)
		return;
	SetEntryPoint(EP_DISCONNECTCOLLISION);

	if(collision->apperance->character=='@'){
		if(currentmap->missionsuccess)
			PlayerSetMoney(PlayerGetMoney()+512);
		ChangeGameState(STATE_ESCAPE);
	}
	ResetEntryPoint();
	return;
}
