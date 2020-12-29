#include "map.h"
struct room* GenerateMap(int nodes){
	SetEntryPoint(EP_GENERATEMAP);

	currentmap=malloc(sizeof(struct map));
	memset(currentmap,0,sizeof(currentmap));
	currentmap->fogofwar=FALSE;
	currentmap->missionsuccess=FALSE;
	struct room* currentroom=NewRoom(currentmap,&RectangularRoom,5,5);
	struct room* prevroom=currentroom;
	char direction=0;//0=NORTH,1=EAST,2=SOUTH,3=WEST
	int datanumber=0;
	struct room* currentnode;
	for(int i=0;i<nodes-1;i++){
		currentnode=NewRoom(currentmap,&RectangularRoom,rand()%15+10,rand()%15+10);
		switch(rand()%2){
			case 0:
				RoomsEntry(currentnode);
				break;
			case 1:
				Pillars(currentnode);
				break;
		}
		//Coordinates to setup datalines (x,y are to orignating room; x2,y2 are to previous room)
		int x,y,x2,y2;
		//Reference the direction and then select appropriate coordinates
		//Aim to put a portal in the middle of the outside wall
		switch (direction){
			case 0:;
				//Coordinates for current room
				x=currentnode->sizex/2;
				y=1;
				//Coordinates for the room that is connected to
				x2=prevroom->sizex/2;
				y2=prevroom->sizey-2;
				//Currentroom
				NewTile(FindLook("Void"),x+1  ,y  ,currentnode,TRUE);
				NewTile(FindLook("Void"),x-1  ,y  ,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y+1,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y  ,currentnode,TRUE);
				//Prevroom
				NewTile(FindLook("Void"),x2+1,y2  ,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2-1,y2  ,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2-1,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2  ,   prevroom,TRUE);
				break;
			case 1:
				//Coordinates for current room
				x=1;
				y=currentnode->sizey/2;
				//Coordinates for the room that is connected to
				x2=prevroom->sizex-2;
				y2=prevroom->sizey/2;
				//Currentroom
				NewTile(FindLook("Void"),x+1  ,y  ,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y-1,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y+1,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y  ,currentnode,TRUE);
				//Prevroom
				NewTile(FindLook("Void"),x2-1,y2  ,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2-1,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2+1,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2  ,   prevroom,TRUE);
				break;
			case 2:
				//Coordinates for current room
				x=currentnode->sizex/2;
				y=currentnode->sizey-2;
				//Coordinates for the room that is connected to
				x2=prevroom->sizex/2;
				y2=1;
				//Currentroom
				NewTile(FindLook("Void"),x+1  ,y  ,currentnode,TRUE);
				NewTile(FindLook("Void"),x-1  ,y  ,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y-1,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y  ,currentnode,TRUE);
				//Prevroom
				NewTile(FindLook("Void"),x2+1,y2  ,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2-1,y2  ,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2+1,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2  ,   prevroom,TRUE);
				break;
			case 3:
				//Coordinates for current room
				x=currentnode->sizex-2;
				y=currentnode->sizey/2;
				//Coordinates for the room that is connected to
				x2=1;
				y2=prevroom->sizey/2;
				//Currentroom
				NewTile(FindLook("Void"),x    ,y-1,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y+1,currentnode,TRUE);
				NewTile(FindLook("Void"),x-1  ,y  ,currentnode,TRUE);
				NewTile(FindLook("Void"),x    ,y  ,currentnode,TRUE);
				//Prevroom
				NewTile(FindLook("Void"),x2  ,y2-1,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2+1,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2+1,y2  ,   prevroom,TRUE);
				NewTile(FindLook("Void"),x2  ,y2  ,   prevroom,TRUE);
				break;
		}
		//Take the selected coordinates and built the data lines
		NewInteractable(FindLook("Plane Connector"), x, y,currentnode,&WarpCollision,BuildWarpStruct(prevroom,x2,y2));
		NewInteractable(FindLook("Plane Connector"),x2,y2,prevroom,&WarpCollision,BuildWarpStruct(currentnode,x,y));
		for(int j=0;j<(i+1);j++){
			for(int j=0;j<255;j++){
				int x=rand()%(currentnode->sizex-3)+1;
				int y=rand()%(currentnode->sizey-3)+1;
				if(!CheckPassable(x,y,currentnode))
					continue;
				BOOLEAN found=FALSE;
				struct dll* inter=currentnode->interactables;
				do{
					if(!inter)
						break;
					if(!inter->current)
						break;
					if(((struct interactable*)inter->current)->x==x&&((struct interactable*)inter->current)->y==y){
						found=TRUE;
						break;
					}	
				}while(inter=NextDll(inter));
				if(found)
					continue;	
				struct actor* a=NewActor(FindLook("Killer ICE"),x,y,currentnode,&KillerInput,&KillerCollision,&KillerAct);
				GiveAttack( a,  "Lance",2);
				//GiveDefense(a,"Damge Mitigator",1);
				a->speed=2;
				a->health=8;
				break;
			}
		}
		for(int j=0;j<255;j++){
			int x=rand()%(currentnode->sizex-3)+1;
			int y=rand()%(currentnode->sizey-3)+1;
			if(!CheckPassable(x,y,currentnode))
				continue;
			BOOLEAN found=FALSE;
			struct dll* inter=currentnode->interactables;
			do{
				if(!inter)
					break;
				if(!inter->current)
					break;
				if(((struct interactable*)inter->current)->x==x&&((struct interactable*)inter->current)->y==y){
					found=TRUE;
					break;
				}	
			}while(inter=NextDll(inter));
			if(found)
				continue;	
			NewInteractable(FindLook("Crypto Cred"),x,y,currentnode,&MoneyCollision,BuildMoneyStruct(8));
			highestmoney+=8;
			break;
		}
		for(int j=0;j<255;j++){
			int x=rand()%(currentnode->sizex-3)+1;
			int y=rand()%(currentnode->sizey-3)+1;
			if(!CheckPassable(x,y,currentnode))
				continue;
			BOOLEAN found=FALSE;
			struct dll* inter=currentnode->interactables;
			do{
				if(!inter)
					break;
				if(!inter->current)
					break;
				if(((struct interactable*)inter->current)->x==x&&((struct interactable*)inter->current)->y==y){
					found=TRUE;
					break;
				}	
			}while(inter=NextDll(inter));
			if(found)
				continue;	
			char name[128];
			sprintf(name,"Testdata%i",datanumber);
			void* data=NewData(name,0,datanumber*16+16);
			NewInteractable(FindLook("Data Fragment"),x,y,currentnode,&DataCollision,BuildDataStruct(data));
			highestmoney+=datanumber*16+16;
			datanumber++;
			break;
		}
		prevroom=currentnode;
		direction++;
		if(direction==4)
			direction=0;
	}
	for(int j=0;j<255;j++){
		int x=rand()%(currentnode->sizex-3)+1;
		int y=rand()%(currentnode->sizey-3)+1;
		if(!CheckPassable(x,y,currentnode))
			continue;
		BOOLEAN found=FALSE;
		struct dll* inter=currentnode->interactables;
		do{
			if(!inter)
				break;
			if(!inter->current)
				break;
			if(((struct interactable*)inter->current)->x==x&&((struct interactable*)inter->current)->y==y){
				found=TRUE;
				break;
			}	
		}while(inter=NextDll(inter));
		if(found)
			continue;	
		struct actor* a=NewActor(FindLook("System Admin"),x,y,currentnode,&SysAdminInput,&SysAdminCollision,&SysAdminAct);
		GiveAttack( a,"Katana",4);
		GiveDefense(a,"Barrier",1);
		a->health=20;
		break;
	}
	ResetEntryPoint();
	return currentroom;
}
