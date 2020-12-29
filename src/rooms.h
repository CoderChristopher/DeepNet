#ifndef __ROOMS_H__
#define __ROOMS_H__
#include "includes.h"
BOOLEAN CheckPassable(int x,int y,struct room* rm);
BOOLEAN LineOfSight(int x1,int y1,int x2,int y2,struct room* rm);
void RoomsEntry(struct room* rm);
void RandomRoom(struct room* rm);
void Pillars(struct room* rm);
void RectangularRoom(struct room* rm);
void CircleRoom(struct room* rm);
struct room* NewRoom(struct map* map,void (*generatorfunction)(struct room*),int sizex,int sizey);
void TransferActorToRoom(struct room* to,struct actor* move);
#endif
