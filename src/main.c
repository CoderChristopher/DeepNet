#include "globals.h"
#include "const.h"
#include "structs.h"
#include "utility.h"
#include "ninit.h"
#include "dll.h"
#include "actorfunctions.h"
#include "tiles.h"
#include "rooms.h"
#include "map.h"
#include "interactables.h"
#include "gamestate.h"

void CalculateScore(struct actor* player){
	SetEntryPoint(EP_CALCULATESCORE);
	if(!player){
		ResetEntryPoint();
		return;
	}
	int tally=0;
	struct dll* datum=player->data;
	while(datum){
		if(!datum->current)
			break;
		tally+=((struct data*)datum->current)->value;
		datum=NextDll(datum);
	}
	tally+=PlayerGetMoney();
	if(currentmap->missionsuccess)
		printf("***You completed the mission so you recieved a bonus from your client of $512***\n");
	printf("Score is %i.\n",tally);
	int percentage=(float)tally/(float)highestmoney*100.0f;
	printf("Your rank is %i / 100\n",percentage);
	ResetEntryPoint();
	return;
}
void SegmentationFaultCatch(int signo){
	endwin();
	printf("There was segmentation fault in the program at %i,%i.\n",entrypoint,preventrypoint);
	printf("Please email this above error to copelandwebdesign@gmail.com .\n",entrypoint,preventrypoint);
	exit(1);
}
void CloseCatch(int signo){
	endwin();
	CalculateScore(player);
	exit(1);
}

int main(int argc,void* argv[]){
	BOOLEAN seedset=FALSE;
	time_t seed=-1;
	if(argc>1){
		if(strstr(argv[1],"-s")){
			char* val=strtok("-s",argv[1]);
			if(val){
				seed=atoi(val);
			}else{
				if(argc>=3){
					seed=atoi(argv[2]);
					seedset=TRUE;
				}
			}
		}
	}
	done=FALSE;

	//Signals to catch various mischief
	signal(SIGSEGV,SegmentationFaultCatch);
	signal(SIGINT ,CloseCatch);
	signal(SIGTERM,CloseCatch);
	signal(SIGQUIT,CloseCatch);

	SetEntryPoint(EP_MAIN);

	//Setup Ncurses
	NCursesInit();
	NColorPairs();

	//Setup Functions for the main loop
	Render=&MENU_Render;
	ProcessInput=&MENU_ProcessInput;
	Update=&MENU_Update;

	//RNG Init
	if(!seedset)
		seed=time(NULL);
	srand(seed);

	//Setup looks
	NewLook('@',COL_GR,"Entity Locator","Indicates the location of outside entity.","");
	NewLook('!',COL_GR,"Data Fragment","This is a collectable data fragment.","Usually these award some Crypto Cred");
	NewLook('$',COL_GR,"Crypto Cred","Crypto creds is money that can be used to buy things.","");
	NewLook('%',COL_RD,"ECM Software","This is entity counter measure software that may be equipped for attack or defense.","");
	NewLook('+',COL_RD,"Access Gate","Access gates monitor for unathorized user access and will trace any suspicious activity.","");
	NewLook('*',COL_YL,"Secure Entry Exit","A point at which new entities may enter and exit a system.","");
	NewLook('=',COL_YL,"Plane Connector","A warp that connects two planes.","");
	NewLook('K',COL_RD,"Killer ICE","A Intrusion Countering Entity that will attack percieved hostile threats.","It is rumored that the default aggression settings are so high that they will attack anything that moves, even other killers.");
	NewLook('A',COL_RD,"System Admin","A special user that manages a system.","");
	NewLook('#',COL_WH,"Data Wall","A plane wall that resists the transfer of system entities.","");
	NewLook('.',COL_WH,"Void","An empty space in a plane that has the potential to carry data.","");

	currentroom=GenerateMap(10);
	NewInteractable(FindLook("Secure Entry Exit"),2,1,currentroom,&DisconnectCollision,NULL);
	player=NewActor(FindLook("Entity Locator"),2,2,currentroom,&PlayerInput,&PlayerCollision,&PlayerAct);
	player->seen=TRUE;
	player->health=12;
	GiveAttack( player,"Hammer",3);
	GiveDefense(player,"Shield",6);

	//Main Game Loop
	Render();
	do{
		ProcessInput();
		Update();
		Render();//Render the world
	}while(!done);//Check to see if quit condition is called, if not continue on

	//clean things up
	endwin();
	
	FreeDll(&looklist);
	FreeDll(&datalist);
	struct dll* room=currentmap->rooms;
	while(room&&room->current){
		FreeDll(&(((struct room*)room->current)->actors));
		FreeDll(&(((struct room*)room->current)->interactables));
		room=room->next;
	}
	FreeDll(&currentmap->rooms);

	CalculateScore(player);
	printf("\nWorld Seed: %i\n",seed);

	return 0;
}
