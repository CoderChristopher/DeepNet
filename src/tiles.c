#include "tiles.h"
void NewTile(struct look* look,unsigned int x,unsigned int y,struct room* rm,BOOLEAN passable){
	SetEntryPoint(EP_NEWTILE);

	if(x<0||y<0||x>rm->sizex-1||y>rm->sizey-1||!rm){
		ResetEntryPoint();
		return; 
	}

	rm->tiles[y*rm->sizex+x].apperance=look;
	rm->tiles[y*rm->sizex+x].passable=passable;
	rm->tiles[y*rm->sizex+x].seen=FALSE;

	ResetEntryPoint();
	return;
}
