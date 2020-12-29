#ifndef __UTILITY__H__
#define __UTILITY__H__
#include "includes.h"
void SetEntryPoint(int ep);
void ResetEntryPoint();
int Distance(int x,int y);
int RandomSign();
char WeightedCoinFlip(int weight);
char CoinFlip();
void PrintNoSpace(int y,int x,const char* message);
#endif
