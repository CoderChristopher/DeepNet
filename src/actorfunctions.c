#include "actorfunctions.h"
void CalculateDamages(struct actor* a1,struct actor* a2){
	SetEntryPoint(EP_CALCULATEDAMAGES);
	if(!a1||!a2){
		ResetEntryPoint();
		return;
	}
	int result1=0;
	int result2=0;
	if(a1->attack&&(rand()%(140-a2->reaction)))
		result1 =a1->attack->value;
	if(a2->attack&&(rand()%(140-a1->reaction)))
		result2 =a2->attack->value;
	if(result2>=a1->shield){
		result2-=a1->shield;
		a1->shield=0;
		a1->health-=result2;
	}else{
		a1->shield-=result2;
	}
	if(result1>=a2->shield){
		result1-=a2->shield;
		a2->shield=0;
		a2->health-=result1;
	}else{
		a2->shield-=result1;
	}
	if(!a1->apperance||!a2->apperance){
		ResetEntryPoint();
		return;
	}
	if(a1->apperance->character=='@'||a2->apperance->character=='@'){
		a1->seen=TRUE;
		a2->seen=TRUE;
	}
	ResetEntryPoint();
	return;
}
void DefaultCollisionBehavior(struct actor* self,struct actor* opponent){
	SetEntryPoint(EP_DEFAULTACTBEHAVIOR);
	switch(self->action){
		case ACTION_MOVE_LEFT:
			self->x++;
			break;
		case ACTION_MOVE_RIGHT:
			self->x--;
			break;
		case ACTION_MOVE_UP:
			self->y++;
			break;
		case ACTION_MOVE_DOWN:
			self->y--;
			break;
	}
	CalculateDamages(self,opponent);
	ResetEntryPoint();
	return;
}
void DefaultActBehavior(struct actor* self){
	SetEntryPoint(EP_DEFAULTACTBEHAVIOR);
	switch(self->action){
		case ACTION_NONE:
			if(self->defense)
				if(self->shield<self->defense->value)
					self->shield++;
			break;
		case ACTION_MOVE_LEFT:
			if(self->x<=0||!CheckPassable(self->x-1,self->y,self->currentroom))
				break;
			self->x--;
			break;
		case ACTION_MOVE_RIGHT:
			if(self->x>=self->currentroom->sizex-1||!CheckPassable(self->x+1,self->y,self->currentroom))
				break;
			self->x++;
			break;
		case ACTION_MOVE_UP:
			if(self->y<=0||!CheckPassable(self->x,self->y-1,self->currentroom))
				break;
			self->y--;
			break;
		case ACTION_MOVE_DOWN:
			if(self->y>=self->currentroom->sizey-1||!CheckPassable(self->x,self->y+1,self->currentroom))
				break;
			self->y++;
			break;
		case ACTION_GRAB:;
			struct dll* interactables=self->currentroom->interactables;
			do{
				if(!interactables->current)
					break;
				struct interactable* inter=interactables->current;
				if(self->x!=inter->x||self->y!=inter->y)
					continue;
				if(!inter->apperance)
					break;
				if(inter->apperance->character='%'){
					SoftwareCollision(inter,self);
					//This is kinda sus
					free(inter);
				}
			}while((interactables=NextDll(interactables)));
			break;
	}
	ResetEntryPoint();
	return;
}
struct actor* NewActor(struct look* look,int x,int y,struct room* rm,void* adf,void* cr,void* action){
	SetEntryPoint(EP_NEWACTOR);
	struct actor* act=malloc(sizeof(struct actor));
	if(!act||!rm){
		ResetEntryPoint();
		return NULL;
	}
	AppendDll(rm->actors,act);
	act->apperance=look;
	act->x=x;
	act->y=y;
	act->actiondecisionfunction=adf;
	act->collisionresolver=cr;
	act->act=action;
	act->action=ACTION_NONE;
	act->currentroom=rm;
	act->health=10;
	act->reaction=1;
	act->stealth=1;
	act->shield=0;
	act->data=NULL;
	act->attack=NULL;
	act->defense=NULL;
	act->seen=FALSE;
	act->speed=1;
	act->speedcounter=1;
	memset(act->stack,0,12);
	ResetEntryPoint();
	return act;
}
void RemoveActor(struct actor* remove){
	SetEntryPoint(EP_REMOVEACTOR);
	if(!remove){
		ResetEntryPoint();
		return;
	}
	if(!remove->currentroom){
		ResetEntryPoint();
		return;
	}
	RemoveDll(&remove->currentroom->actors,remove);
	ResetEntryPoint();
	return;
}
void GiveAttack(struct actor* act,const char name[8],unsigned int value){
	SetEntryPoint(EP_GIVEATTACK);
	if(!act){
		ResetEntryPoint();
		return;
	}
	act->attack=malloc(sizeof(struct software));
	strncpy(act->attack->name,name,8);
	act->attack->value=value;
	act->attack->type=TYPE_ATTACK;
	ResetEntryPoint();
	return;
}
void GiveDefense(struct actor* act,const char name[8],int value){
	SetEntryPoint(EP_GIVEDEFENSE);
	if(!act){
		ResetEntryPoint();
		return;
	}
	act->defense=malloc(sizeof(struct software));
	strncpy(act->defense->name,name,8);
	act->defense->value=value;
	act->shield=act->defense->value;
	act->defense->type=TYPE_DEFENSE;
	ResetEntryPoint();
}
void KillerInput(struct actor* self){
	if(!self)
		return;
	SetEntryPoint(EP_KILLERINPUT);
	self->speedcounter--;
	if(self->speedcounter!=0){
		ResetEntryPoint();
		return;
	}
	self->speedcounter=self->speed;
	char choice=rand()%4;
	switch(self->stack[9]){
		case 0:
			switch(choice){
				case 0:
					self->action=ACTION_MOVE_LEFT;
					break;
				case 1:
					self->action=ACTION_MOVE_RIGHT;
					break;
				case 2:
					self->action=ACTION_MOVE_UP;
					break;
				case 3:
					self->action=ACTION_MOVE_DOWN;
					break;
			}
			break;
		case 1:
			if((int)self->stack[0]>self->x)
				self->action=ACTION_MOVE_RIGHT;
			else if((int)self->stack[0]<self->x)
				self->action=ACTION_MOVE_LEFT;
			if((int)self->stack[5]>self->y)
				self->action=ACTION_MOVE_DOWN;
			else if((int)self->stack[5]<self->y)
				self->action=ACTION_MOVE_UP;
			break;	
	}
	ResetEntryPoint();
	return;
}
void KillerAct(struct actor* self){
	SetEntryPoint(EP_KILLERACT);
	switch(self->action){
		case ACTION_NONE:
			if(self->defense)
				if(self->shield<self->defense->value)
					self->shield++;
			break;
		case ACTION_MOVE_LEFT:
			if(self->x<=0||!CheckPassable(self->x-1,self->y,self->currentroom)){
				self->stack[9]=0;
				break;
			}
			self->x--;
			break;
		case ACTION_MOVE_RIGHT:
			if(self->x>=self->currentroom->sizex-1||!CheckPassable(self->x+1,self->y,self->currentroom)){
				self->stack[9]=0;
				break;
			}
			self->x++;
			break;
		case ACTION_MOVE_UP:
			if(self->y<=0||!CheckPassable(self->x,self->y-1,self->currentroom)){
				self->stack[9]=0;
				break;
			}
			self->y--;
			break;
		case ACTION_MOVE_DOWN:
			if(self->y>=self->currentroom->sizey-1||!CheckPassable(self->x,self->y+1,self->currentroom)){
				self->stack[9]=0;
				break;
			}
			self->y++;
			break;
	}
	if((int)self->stack[0]==self->x&&(int)self->stack[5]==self->y)
		self->stack[9]=0;
	struct dll* actors=self->currentroom->actors;
	do{
		if(!actors->current)
			break;
		struct actor* act=actors->current;
		if(act==self)
			continue;
		if(!act->apperance)
			continue;
		if(act->apperance->character=='@'){
			if(Distance(act->x-self->x,act->y-self->y)<5&&LineOfSight(act->x,act->y,self->x,self->y,self->currentroom)){
				memcpy(&self->stack[0],&act->x,sizeof(int));
				memcpy(&self->stack[5],&act->y,sizeof(int));
				self->stack[9]=1;
				PlayerSetDeltaTrace(PlayerGetDeltaTrace()*1.8f);
			}
		}
			
		if(self->x!=act->x||self->y!=act->y)
			continue;
		if(self->collisionresolver)
			self->collisionresolver(self,act);
	}while((actors=NextDll(actors)));
	struct dll* interactables=self->currentroom->interactables;
	do{
		if(!interactables->current)
			break;
		struct interactable* inter=interactables->current;
		if(self->x!=inter->x||self->y!=inter->y)
			continue;
		if(inter->collisionresolver)
			inter->collisionresolver(inter,self);
	}while((interactables=NextDll(interactables)));
	self->action=ACTION_NONE;		
	if(self->health<=0){
		if(self->attack){
			for(int i=0;i<255;i++){
				int x=self->x+rand()%2*RandomSign();
				int y=self->y+rand()%2*RandomSign();
				if(!CheckPassable(x,y,self->currentroom))
					continue;
				struct interactable* inter=NewInteractable(FindLook("ECM Software"),x,y,self->currentroom,NULL,BuildSoftwareStruct(self->attack));
				inter->type=TYPE_ATTACK;
				free(self->attack);
				break;
			}
		}
		if(self->defense){
			for(int i=0;i<255;i++){
				int x=self->x+rand()%2*RandomSign();
				int y=self->y+rand()%2*RandomSign();
				if(!CheckPassable(x,y,self->currentroom))
					continue;
				struct interactable* inter=NewInteractable(FindLook("ECM Software"),x,y,self->currentroom,NULL,BuildSoftwareStruct(self->defense));
				inter->type=TYPE_DEFENSE;
				free(self->defense);
				break;
			}
		}
		struct dll* data=self->data;
		while(data){
			if(!data->current)
				break;
			for(int i=0;i<255;i++){
				int x=self->x+rand()%2*RandomSign();
				int y=self->y+rand()%2*RandomSign();
				if(!CheckPassable(x,y,self->currentroom))
					continue;
				NewInteractable(FindLook("Data Fragment"),x,y,self->currentroom,&DataCollision,BuildDataStruct(data->current));
				break;
			}
			data=NextDll(data);
		}
		RemoveDll(&self->currentroom->actors,self);	
	}
	ResetEntryPoint();
	return;
}
void KillerCollision(struct actor* self,struct actor* opponent){
	SetEntryPoint(EP_KILLERCOLLISION);
	if(!opponent){
		ResetEntryPoint();
		return;
	}
	if(!opponent->apperance){
		ResetEntryPoint();
		return;
	}
	switch(opponent->apperance->character){
		case '@':
			DefaultCollisionBehavior(self,opponent);
			break;
		case 'A':
			DefaultCollisionBehavior(self,opponent);
			break;
		case 'K':
			DefaultCollisionBehavior(self,opponent);
			break;
	}
	ResetEntryPoint();
	return;
}
void SysAdminInput(struct actor* self){
	SetEntryPoint(EP_SYSADMININPUT);
	if(!self){
		ResetEntryPoint();
		return;
	}
	self->speedcounter--;
	if(self->speedcounter!=0)
		return;
	self->speedcounter=self->speed;
	char choice=random()%4;
	switch(self->stack[9]){
		case 0:
			switch(choice){
				case 0:
					self->action=ACTION_MOVE_LEFT;
					break;
				case 1:
					self->action=ACTION_MOVE_RIGHT;
					break;
				case 2:
					self->action=ACTION_MOVE_UP;
					break;
				case 3:
					self->action=ACTION_MOVE_DOWN;
					break;
			}
			break;
		case 1:
			if((int)self->stack[0]>self->x)
				self->action=ACTION_MOVE_RIGHT;
			else if((int)self->stack[0]<self->x)
				self->action=ACTION_MOVE_LEFT;
			if((int)self->stack[5]>self->y)
				self->action=ACTION_MOVE_DOWN;
			else if((int)self->stack[5]<self->y)
				self->action=ACTION_MOVE_UP;
			break;	
	}
	ResetEntryPoint();
	return;
}
void SysAdminAct(struct actor* self){
	SetEntryPoint(EP_SYSADMINACT);
	if(!self){
		ResetEntryPoint();
		return;
	}
	DefaultActBehavior(self);
	struct dll* actors=self->currentroom->actors;
	do{
		if(!actors->current)
			break;
		struct actor* act=actors->current;
		if(act==self)
			continue;
		if(!act->apperance)
			continue;
		if(act->apperance->character=='@'){
			if(Distance(act->x-self->x,act->y-self->y)<10&&LineOfSight(act->x,act->y,self->x,self->y,self->currentroom)){
				memcpy(&self->stack[0],&act->x,sizeof(int));
				memcpy(&self->stack[5],&act->y,sizeof(int));
				self->stack[9]=1;

				PlayerSetDeltaTrace(PlayerGetDeltaTrace()*3.0f);
			}else if(self->x==(int)self->stack[0]&&self->y==(int)self->stack[5]){
				self->stack[9]=0;
			}
		}
			
		if(self->x!=act->x||self->y!=act->y)
			continue;
		if(self->collisionresolver)
			self->collisionresolver(self,act);
	}while((actors=NextDll(actors)));
	struct dll* interactables=self->currentroom->interactables;
	do{
		if(!interactables->current)
			break;
		struct interactable* inter=interactables->current;
		if(self->x!=inter->x||self->y!=inter->y)
			continue;
		if(inter->collisionresolver)
			inter->collisionresolver(inter,self);
	}while((interactables=NextDll(interactables)));
	self->action=ACTION_NONE;		
	if(self->health<=0){
		if(self->attack){
			for(int i=0;i<255;i++){
				int x=self->x+rand()%2*RandomSign();
				int y=self->y+rand()%2*RandomSign();
				if(!CheckPassable(x,y,self->currentroom))
					continue;
				struct interactable* inter=NewInteractable(FindLook("ECM Software"),x,y,self->currentroom,NULL,BuildSoftwareStruct(self->attack));
				inter->type=TYPE_ATTACK;
				free(self->attack);
				break;
			}
		}
		if(self->defense){
			for(int i=0;i<255;i++){
				int x=self->x+rand()%2*RandomSign();
				int y=self->y+rand()%2*RandomSign();
				if(!CheckPassable(x,y,self->currentroom))
					continue;
				struct interactable* inter=NewInteractable(FindLook("ECM Software"),x,y,self->currentroom,NULL,BuildSoftwareStruct(self->defense));
				inter->type=TYPE_DEFENSE;
				free(self->defense);
				break;
			}
		}
		struct dll* data=self->data;
		while(data){
			if(!data->current)
				break;
			for(int i=0;i<255;i++){
				int x=self->x+rand()%2*RandomSign();
				int y=self->y+rand()%2*RandomSign();
				if(!CheckPassable(x,y,self->currentroom))
					continue;
				NewInteractable(FindLook("Data Fragment"),x,y,self->currentroom,&DataCollision,BuildDataStruct(data->current));
				break;
			}
			data=NextDll(data);
		}
		RemoveDll(&self->currentroom->actors,self);	
	}
	ResetEntryPoint();
	return;
}
float PlayerGetTrace(){
	return (float)player->stack[0];
}
void PlayerIncrementTrace(){
	player->stack[0]+=(float)player->stack[4];
}
void PlayerSetDeltaTrace(float dt){
	memcpy(&player->stack[4],&dt,sizeof(float));
	player->stack[4]=dt;
}
float PlayerGetDeltaTrace(){
	return (float)player->stack[4];
}
void PlayerSetMoney(int money){
	player->stack[8]=money;
	
}
int PlayerGetMoney(){
	return (float)player->stack[8];
}
void PlayerInput(struct actor* player){
	SetEntryPoint(EP_PLAYERINPUT);
	if(!player){
		ResetEntryPoint();
		return;
	}
	player->speedcounter--;
	if(player->speedcounter!=0)
		return;
	player->speedcounter=player->speed;
	switch(ch){
		case 'h':case 'a':
			player->action=ACTION_MOVE_LEFT;		
			break;
		case 'j':case 's':
			player->action=ACTION_MOVE_DOWN;		
			break;
		case 'k':case 'w':
			player->action=ACTION_MOVE_UP;		
			break;
		case 'l':case 'd':
			player->action=ACTION_MOVE_RIGHT;		
			break;
		case 'g':
			player->action=ACTION_GRAB;		
			break;
		default:
			break;
	}
	ResetEntryPoint();
	return;
}
void PlayerAct(struct actor* self){
	SetEntryPoint(EP_PLAYERACT);	
	if(!self){
		ResetEntryPoint();
		return;
	}
	DefaultActBehavior(self);
	struct dll* actors=self->currentroom->actors;
	do{
		if(!actors->current)
			break;
		struct actor* act=actors->current;
		if(act==self||self->x!=act->x||self->y!=act->y)
			continue;
		if(self->collisionresolver)
			self->collisionresolver(self,act);
	}while((actors=NextDll(actors)));
	struct dll* interactables=self->currentroom->interactables;
	do{
		if(!interactables->current)
			break;
		struct interactable* inter=interactables->current;
		if(self->x!=inter->x||self->y!=inter->y)
			continue;
		if(inter->collisionresolver)
			inter->collisionresolver(inter,self);
	}while((interactables=NextDll(interactables)));
	self->action=ACTION_NONE;		
	if(self->health<=0)
		ChangeGameState(STATE_GAMEOVER);
	ResetEntryPoint();
	return;
}
void PlayerCollision(struct actor* self,struct actor* opponent){
	SetEntryPoint(EP_PLAYERCOLLISION);
	if(!opponent||!self){
		ResetEntryPoint();
		return;
	}
	if(!opponent->apperance){
		ResetEntryPoint();
		return;
	}
	switch(opponent->apperance->character){
		case 'A':
			DefaultCollisionBehavior(self,opponent);
			break;
		case 'K':
			DefaultCollisionBehavior(self,opponent);
			break;
	}
	ResetEntryPoint();
	return;
}
void SysAdminCollision(struct actor* self,struct actor* opponent){
	SetEntryPoint(EP_SYSADMINCOLLISION);
	if(!opponent||!self){
		ResetEntryPoint();
		return;
	}
	if(!opponent->apperance){
		ResetEntryPoint();
		return;
	}
	switch(opponent->apperance->character){
		case '@':
			DefaultCollisionBehavior(self,opponent);
			if(self->health>=0)
				currentmap->missionsuccess=TRUE;
			break;
	}
	ResetEntryPoint();
	return;
}
