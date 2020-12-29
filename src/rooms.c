#include "rooms.h"
//A helper function to check if there are any units at a location
BOOLEAN CheckPassable(int x,int y,struct room* rm){
	SetEntryPoint(EP_CHECKPASSABLE);

	if(x<0||y<0||x>rm->sizex-1||y>rm->sizey-1||!rm)
		return FALSE;

	ResetEntryPoint();
	return rm->tiles[y*rm->sizex+x].passable;
}
BOOLEAN LineOfSight(int x1,int y1,int x2,int y2,struct room* rm){ 
	SetEntryPoint(EP_LINEOFSIGHT);
	int distx=x2-x1;
	int disty=y2-y1;
	int sign=0;
	if(distx!=0){
		float angle=atan2(disty,distx);
		if(distx>0)
			sign=1;
		else
			sign=-1;
		for(float k=(float)abs(distx)-0.1f;k>=0.0f;k-=.2f){
			float newy=(k*sign)*tan(angle);
			if(round(k*sign)==distx&&round(newy)==disty)
				continue;
			if(!CheckPassable(x1+round(k*sign),y1+round(newy),rm)){
				ResetEntryPoint();
				return FALSE;
			}
		}
	}else{
		if(disty>0)
			sign=1;
		else
			sign=-1;
		for(float k=(float)abs(disty)-1.0f;k>=0.0f;k-=1.0f)
			if(!CheckPassable(x1,y1+k*sign,rm)){
				ResetEntryPoint();
				return FALSE;
			}
	}

	ResetEntryPoint();
	return TRUE;
}
void RoomsEntry(struct room* rm){
	int remainingwidth=rm->sizex;
	int x=0;
	int y=0;
	int newheight=rm->sizey/2-1;
	while(remainingwidth>=5){
		int newwidth=(rand()%(remainingwidth-4))+4;
		for(int i=x;i<x+newwidth;i++){
			for(int j=y;j<y+newheight;j++){
				if(i!=x&&i!=x+newwidth-1&&j!=y&&j!=y+newheight-1)
					continue;
				NewTile(FindLook("Data Wall"),i,j,rm,FALSE);
			}
		}
		int doorlocation=rand()%(newwidth-2)+x+1;
		NewTile(FindLook("Void"),doorlocation,y+newheight-1,rm,TRUE);
		NewInteractable(FindLook("Access Gate"),doorlocation,y+newheight-1,rm,&CheckPointCollision,NULL);
		newwidth+=rand()%2;
		x+=newwidth;
		remainingwidth-=newwidth;
	}
	remainingwidth=rm->sizex;
	x=0;
	y=rm->sizey/2+1;
	newheight=rm->sizey/2;
	while(remainingwidth>=5){
		int newwidth=(rand()%(remainingwidth-4))+4;
		for(int i=x;i<x+newwidth;i++){
			for(int j=y;j<y+newheight;j++){
				if(i!=x&&i!=x+newwidth-1&&j!=y&&j!=y+newheight-1)
					continue;
				NewTile(FindLook("Data Wall"),i,j,rm,FALSE);
			}
		}
		int doorlocation=rand()%(newwidth-2)+x+1;
		NewTile(FindLook("Void"),doorlocation,y,rm,TRUE);
		NewInteractable(FindLook("Access Gate"),doorlocation,y,rm,&CheckPointCollision,NULL);
		newwidth+=rand()%2;
		x+=newwidth;
		remainingwidth-=newwidth;
	}
}
void RandomRoom(struct room* rm){
	SetEntryPoint(EP_RANDOMROOM);
	if(!rm){
		ResetEntryPoint();
		return;
	}
	for(int i=0;i<rm->sizey;i++)
		for(int j=0;j<rm->sizex;j++){
			if(CoinFlip())
				NewTile(FindLook("Void"),i,j,rm,TRUE);
			else
				NewTile(FindLook("Data Wall"),i,j,rm,FALSE);
		}

	ResetEntryPoint();
	return;
}
void Pillars(struct room* rm){
	SetEntryPoint(EP_PILLARS);

	for(int i=0;i<(rm->sizex-2)/3;i++){
		for(int j=0;j<(rm->sizey-2)/3;j++){
			int x=i*4+2;
			int y=j*4+2;
			NewTile(FindLook("Data Wall"),x  ,y  ,rm,FALSE);
			NewTile(FindLook("Data Wall"),x+1,y  ,rm,FALSE);
			NewTile(FindLook("Data Wall"),x+1,y+1,rm,FALSE);
			NewTile(FindLook("Data Wall"),x  ,y+1,rm,FALSE);
		}
	}

	ResetEntryPoint();
	return;
}
void RectangularRoom(struct room* rm){
	SetEntryPoint(EP_RECTANGULARROOM);
	if(!rm){
		ResetEntryPoint();
		return;
	}
	for(int i=0;i<rm->sizey;i++)
		for(int j=0;j<rm->sizex;j++){
			if(i!=0&&i!=rm->sizey-1&&j!=0&&j!=rm->sizex-1)
				NewTile(FindLook("Void"),j,i,rm,TRUE);
			else
				NewTile(FindLook("Data Wall"),j,i,rm,FALSE);
		}

	ResetEntryPoint();
	return;
}
void CircleRoom(struct room* rm){
	SetEntryPoint(EP_CIRCLEROOM);
	if(!rm){
		ResetEntryPoint();
		return;
	}
	for(int i=0;i<rm->sizey;i++)
		for(int j=0;j<rm->sizex;j++){
			if(Distance((rm->sizex/2)-i,(rm->sizey/2)-j)<rm->sizex/2&&j!=rm->sizex-1&&i!=rm->sizey-1)
				NewTile(FindLook("Void"),j,i,rm,TRUE);
			else
				NewTile(FindLook("Data Wall"),j,i,rm,FALSE);
		}

	ResetEntryPoint();
	return;
}
struct room* NewRoom(struct map* map,void (*generatorfunction)(struct room*),int sizex,int sizey){
	SetEntryPoint(EP_NEWROOM);
	if(!currentmap){
		ResetEntryPoint();
		return NULL;
	}
	if(!currentmap->rooms){
		NewDll(&currentmap->rooms);
	}
	struct room* rm=malloc(sizeof(struct room));
	AppendDll(map->rooms,rm);
	NewDll(&rm->actors);		
	NewDll(&rm->interactables);		
	rm->tiles=malloc(sizex*sizey*sizeof(struct tile));
	rm->sizex=sizex;
	rm->sizey=sizey;
	if(!generatorfunction){
		for(int i=0;i<sizex;i++)
			for(int j=0;j<sizey;j++)
				NewTile(FindLook("Void"),i,j,rm,TRUE);
	}else	
		generatorfunction(rm);
	ResetEntryPoint();
	return rm;
}
void TransferActorToRoom(struct room* to,struct actor* move){
	SetEntryPoint(EP_TRANSFERACTORTOROOM);

	if(!to||!move){
		ResetEntryPoint();
		return;
	}
	RemoveDll(&move->currentroom->actors,move);
	AppendDll(to->actors,move);
	move->currentroom=to;

	ResetEntryPoint();
}
