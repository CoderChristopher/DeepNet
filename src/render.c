#include "render.h"
void INGAME_Render(){
	SetEntryPoint(EP_INGAME_RENDER);
	erase();
	//If current room is not set, just run away, nothing good will happen if you proceed
	if(!currentroom){
		ResetEntryPoint();
		return;
	}
	//These coordinates are used for the world camera, basically centers around the
	//player and displays the tiles immediately around
	int leftcoordinate=player->x-10;
	int rightcoordinate=leftcoordinate+21;
	int topcoordinate=player->y-10;
	int bottomcoordinate=topcoordinate+21;
	//Render all the static tiles
	for(int i=topcoordinate;i<bottomcoordinate;i++){
		for(int j=leftcoordinate;j<rightcoordinate;j++){
			//If tile is outside the game world render a 'static' background
			if(i<0||j<0||i>currentroom->sizey-1||j>currentroom->sizex-1){
				attron(COL_RD);
				addch((const char)(rand()%32+32));
			}else{//Otherwise if it is a good coordinate then render the given tile
				if(!currentroom->tiles[i*currentroom->sizex+j].seen){
					if(!currentmap->fogofwar||LineOfSight(player->x,player->y,j,i,player->currentroom)){
						if(!currentroom->tiles[i*currentroom->sizex+j].apperance)
							continue;
						attron(currentroom->tiles[i*currentroom->sizex+j].apperance->nattribute);
						printw("%c",currentroom->tiles[i*currentroom->sizex+j].apperance->character);
						currentroom->tiles[i*currentroom->sizex+j].seen=TRUE;
					}else{
						attron(COL_RD);
						addch((const char)(rand()%32+32));
					}
				}else{
					if(!currentroom->tiles[i*currentroom->sizex+j].apperance)
						continue;
					attron(currentroom->tiles[i*currentroom->sizex+j].apperance->nattribute);
					printw("%c",currentroom->tiles[i*currentroom->sizex+j].apperance->character);
				}
			}
		}
		printw("\n");
	}
	//Run through Dynamic list of entities and render each if they are on screen
	struct dll* interactables=currentroom->interactables;
	if(interactables){
		do{
			if(!interactables->current)
				break;
			struct interactable* inter=interactables->current;
			if(inter->x<leftcoordinate||inter->x>rightcoordinate-1||inter->y<topcoordinate||inter->y>bottomcoordinate-1)
				continue;
			if(!inter->apperance)
				continue;
			attron(inter->apperance->nattribute);
			mvaddch(inter->y-topcoordinate,inter->x-leftcoordinate,inter->apperance->character);
		}while((interactables=NextDll(interactables)));
	}
	struct dll* actors=currentroom->actors;
	if(actors){
		do{
			if(!actors->current)
				break;
			struct actor* act=actors->current;
			if(act->x<leftcoordinate||act->x>rightcoordinate-1||act->y<topcoordinate||act->y>bottomcoordinate-1)
				continue;
			if(!act->seen){
				if(!LineOfSight(player->x,player->y,act->x,act->y,player->currentroom))
					continue;	
				if((rand()%act->stealth))
					continue;
				act->seen=true;
			}
			if(!act->apperance)
				continue;
			attron(act->apperance->nattribute);
			mvaddch(act->y-topcoordinate,act->x-leftcoordinate,act->apperance->character);
		}while((actors=NextDll(actors)));
	}
	//Render UI to the right of the screen
	attron(COL_WH);
	if(player->attack)
		mvprintw(1,23,"Attack  :%s (%i)-%i",player->attack->name,player->attack->value,player->attack->type);
	if(player->defense)
		mvprintw(2,23,"Defense :%s (%i)-%i",player->defense->name,player->defense->value,player->defense->type);
	mvprintw(3,23,"Health  :%i",player->health);
	mvprintw(4,23,"Shield  :%i",player->shield);
	mvprintw(5,23,"Reaction:%i",player->reaction);
	mvprintw(6,23,"Stealth :%i",player->stealth);
	mvprintw(7,23,"Money   :%i",PlayerGetMoney());

	//Render a really cool tracer indicator
	static BOOLEAN tick=TRUE;//Used to create blinking effect
	attron(COL_GR);//First Select a color based on what percentage of the trace is complete
	//closer to red means more bad
	if(PlayerGetTrace()>50)
		attron(COL_WH);
	if(PlayerGetTrace()>75)
		attron(COL_YL);
	if(PlayerGetTrace()>90)
		attron(COL_RD);
	if(PlayerGetTrace()>95)
		tick=!tick;//Oscilate the blinking indicator if player is in the danger zone

	//Render text with numeric reading...(will remove numeric readout in later development, maybe make it an unlockable upgrade)
	//mvprintw(9,23,"Trace:%.2f",tracepercentage);
	mvprintw(9,23,"Trace:");
	//Render the top half of the bar
	mvprintw(10,23,"############");
	//Render the middle
	mvaddch (11,23,'#');
	for(int i=1;i<=10;i++){//Iterate over every slot of the bar and fill the appropriate slot, where each slot equals 10%
		if(PlayerGetTrace()>=i*10){
			if(tick)//Create the oscilation if the player is in the danger zone
				addch('=');
			else
				addch('-');
		}else
			addch(' ');
	}
	addch('#');
	//Render the bottom
	mvprintw(12,23,"############");

	attron(COL_WH);
	struct dll* item=player->data;
	if(item){
		int i=0;
		do{
			if(!item->current)
				break;
			if(!((struct data*)item->current)->name)
				break;
			mvprintw(13+i,23,":%s-%d\n",((struct data*)item->current)->name,((struct data*)item->current)->value);
			i++;
		}while((item=NextDll(item)));
	}
	move(23,0);
	refresh();
	ResetEntryPoint();
	return;
}
void MENU_Render(){
	SetEntryPoint(EP_MENU_RENDER);
	erase();
	//Give a 'random' static background
	attron(COL_RD);
	for(int i=0;i<21;i++){
		for(int j=0;j<43;j++){
			//Pick a random non alpha ascii character, this intentionally 
			//includes the control character at 31 because breaks the 
			//display in an interesting way that sometimes makes it look
			//longer, thereby adding to the glitch asthetic
			addch((const char)(rand()%33+31));
		}
		addch('\n');
	}
	attron(COL_WH);
	//The weighted coin flip function creates the glitch visual effect.
	//Basically a coin flip can be either heads(return value of 1) or tails (return 
	//value of 0)
	//By adding a weight to the coin flip I decrease the probabiliy that tails will
	//land. But since the most likely value is an offset of 1(heads) I invert the
	//output of the function with ! operator to make it zero
	PrintNoSpace(1 ,4+!WeightedCoinFlip(16),"#####    ########  ########  ######");
	PrintNoSpace(2 ,4+!WeightedCoinFlip(10),"#    #   #         #         #     #");
	PrintNoSpace(3 ,4+!WeightedCoinFlip(16),"#    #   #         #         #     #");
	PrintNoSpace(4 ,4+!WeightedCoinFlip(20),"#    #   #######   #######   #     #");
	PrintNoSpace(5 ,4+!WeightedCoinFlip(16),"#    #   #         #         ###### ");
	PrintNoSpace(6 ,4+!WeightedCoinFlip(10),"#    #   #         #         #");
	PrintNoSpace(7 ,4+!WeightedCoinFlip(16),"#####    #######   #######   #");
	PrintNoSpace(9 ,9+!WeightedCoinFlip(32),"##   #   ########  #######");
	PrintNoSpace(10,9+!WeightedCoinFlip(16),"# #  #   #            #");
	PrintNoSpace(11,9+!WeightedCoinFlip(32),"# #  #   #            #");
	PrintNoSpace(12,9+!WeightedCoinFlip(16),"# #  #   #######      #");
	PrintNoSpace(13,9+!WeightedCoinFlip(20),"#  # #   #            #");
	PrintNoSpace(14,9+!WeightedCoinFlip(16),"#  # #   #            #");
	PrintNoSpace(15,9+!WeightedCoinFlip(16),"#   ##   #######      #");
	mvprintw(17,6,"Developed by Christopher Copeland");
	//mmm.... flavor text. More to come as this project gets... deeper
	mvprintw(19,7,"<PRESS ENTER TO START THE DIVE>");
	refresh();
	ResetEntryPoint();
	return;
}
void GAMEOVER_Render(){
	SetEntryPoint(EP_GAMEOVER_RENDER);
	erase();
	//Render all the static tiles
	for(int i=0;i<16;i++){
		for(int j=0;j<16;j++){
			//select a random color
			switch(rand()%5){
				case 0:
					attron(COL_RD);
					break;
				case 1:
					attron(COL_RD);
					break;
				case 2:
					attron(COL_RD);
					break;
				case 3:
					attron(COL_GR);
					break;
				case 4:
					attron(COL_BL);
					break;
			}
			addch((const char)(rand()%32+32));
		}
		printw("\n");
	}
	attron(COL_WH);
	mvprintw(4,4,"GAMEOVER");
	mvprintw(16,2,"(q to exit)");
	refresh();
	ResetEntryPoint();
	return;
}
void DISCONNECT_Render(){
	SetEntryPoint(EP_DISCONNECT_RENDER);
	static char tick=0;//an easily static toggle-able variable
	tick=!tick;//Flip the toggle
	erase();
	//Render a bacgkround of either pluses or minuses based on toggle
	for(int i=0;i<16;i++){
		for(int j=0;j<17;j++){
			attron(COL_WH);
			if(tick)
				printw("-");
			else
				printw("+");
		}
		printw("\n");
	}
	//Flavor text
	attron(COL_RD);
	mvprintw(1 ,1,"YOUR CONNECTION");
	attron(COL_WH);
	mvprintw(2 ,6,"-----");
	attron(COL_RD);
	mvprintw(3 ,1,"HAS BEEN TRACED");
	attron(COL_WH);
	mvprintw(4 ,6,"-----");
	attron(COL_RD);
	mvprintw(5 ,2,"PLEASE REMAIN");
	attron(COL_WH);
	mvprintw(6 ,6,"-----");
	attron(COL_RD);
	mvprintw(7 ,5,"AT YOUR");
	attron(COL_WH);
	mvprintw(8 ,6,"-----");
	attron(COL_RD);
	mvprintw(9 ,1,"TERMINAL  UNTIL");
	attron(COL_WH);
	mvprintw(10,6,"-----");
	attron(COL_RD);
	mvprintw(11,1,"THE AUTHORITIES");
	attron(COL_WH);
	mvprintw(12,6,"-----");
	attron(COL_RD);
	mvprintw(13,5,"ARRIVE");
	refresh();
	ResetEntryPoint();
	return;
}
void ESCAPE_Render(){
	static short progress=0;
	SetEntryPoint(EP_ESCAPE_RENDER);
	erase();
	attron(COL_RD);
	//Give a 'random' static background
	for(int i=0;i<20;i++){
		for(int j=0;j<20;j++){
			//Pick a random non alpha ascii character, this intentionally 
			//includes the control character at 31 because breaks the 
			//display in an interesting way that sometimes makes it look
			//longer, thereby adding to the glitch asthetic
			addch((const char)(rand()%33+32));
		}
		addch('\n');
	}
	//Render a bacgkround of either pluses or minuses based on toggle
	short tick=0;
	char x=0;
	char y=0;
	char cycle=0;
	short count=0;
	short limit=18;
	
	if(progress<215){
		attron(COL_YL);
	}else
		attron(COL_GR);
	while(tick<progress){
		while(count<=limit&&tick<progress){
			mvprintw(y ,x,"*");
			x++;
			count++;
			tick++;
		}
		limit--;
		if(tick>=progress)
			break;
		count=0;
		while(count<=limit&&tick<progress){
			mvprintw(y ,x,"*");
			y++;
			count++;
			tick++;
		}
		limit--;
		if(tick>=progress)
			break;
		count=0;
		while(count<=limit&&tick<progress){
			mvprintw(y ,x,"*");
			x--;
			count++;
			tick++;
		}
		limit--;
		if(tick>=progress)
			break;
		count=0;
		while(count<=limit&&tick<progress){
			mvprintw(y ,x,"*");
			y--;
			count++;
			tick++;
		}
		limit--;
		if(tick>=progress)
			break;
		count=0;
		tick++;
	}
	//Flavor text
	if(progress<215){
		attron(COL_RD);
		mvprintw(20,0,"***ROUTING ESCAPE***\n");
	}else{
		attron(COL_GR);
		mvprintw(20,0,"***ESCAPE PLOTTED***");
		mvprintw(21,0,"****Q TO EXECUTE****\n");
	}
	refresh();
	progress++;
	ResetEntryPoint();
	return;
}
void HELP_Render(){
	static int tick=TRUE;
	tick=!tick;
	SetEntryPoint(EP_HELP_RENDER);
	erase();
	//Give a 'random' static background
	attron(COL_RD);
	for(int i=0;i<20;i++){
		for(int j=0;j<29;j++){
			//Pick a random non alpha ascii character, this intentionally 
			//includes the control character at 31 because breaks the 
			//display in an interesting way that sometimes makes it look
			//longer, thereby adding to the glitch asthetic
			addch((const char)(rand()%33+32));
		}
		addch('\n');
	}

	if(tick||animationticks<16)
		mvprintw( 0,0,"+----Ono Sendai Man Page----+");
	else
		mvprintw( 0,0,".----Ono Sendai Man Page----.");
	for(int i=1;i<=fmin(animationticks,18);i++)
		mvprintw(i,0,"|                           |");
	if(tick)
		mvprintw(fmin(animationticks,19),0,"+-----Press q to Return-----+");
	else

		mvprintw(fmin(animationticks,19),0,".-----Press q to Return-----.");

	if(animationticks>=2){
		attron(COL_RD);
		mvprintw(1,1,"-----Console Interface-----");
	}
	attron(COL_WH);
	if(animationticks>=3)		
		mvprintw(2,1,"h,a - phase left");
	if(animationticks>=4)		
		mvprintw(3,1,"j,s - phase down");
	if(animationticks>=5)		
		mvprintw(4,1,"k,w - phase up");
	if(animationticks>=6)		
		mvprintw(5,1,"l,d - phase right");
	if(animationticks>=7)		
		mvprintw(6,1,"g   - copy item locally");
	if(animationticks>=8)		
		mvprintw(7,1,"x   - examine entity");
	if(animationticks>=9)		
		mvprintw(8,1,"q   - terminate connection");

	if(animationticks>=10){
		attron(COL_RD);
		mvprintw( 9,1,"---Entity Identification---");
	}
	attron(COL_WH);
	if(animationticks>=11){
		attron(COL_GR);
		mvaddch(10,1,'@');
		attron(COL_WH);
		mvprintw(10,2,"   - Phase Locator");
	}
	if(animationticks>=12){
		attron(COL_GR);
		mvaddch(11,1,'!');
		attron(COL_WH);
		mvprintw(11,2,"   - Data");
	}
	if(animationticks>=13){
		attron(COL_GR);
		mvaddch(12,1,'$');
		attron(COL_WH);
		mvprintw(12,2,"   - CryptoCredit Wallet");
	}
	if(animationticks>=14){
		attron(COL_RD);
		mvaddch(13,1,'%');
		attron(COL_WH);
		mvprintw(13,2,"   - ECM Software");
	}
	if(animationticks>=15){
		attron(COL_YL);
		mvaddch(14,1,'*');
		attron(COL_WH);
		mvprintw(14,2,"   - Secure Entry Exit");
	}
	if(animationticks>=16){
		attron(COL_YL);
		mvaddch(15,1,'=');
		attron(COL_WH);
		mvprintw(15,2,"   - Plane Connector");
	}
	if(animationticks>=17){		
		attron(COL_RD);
		mvaddch(16,1,'+');
		attron(COL_WH);
		mvprintw(16,2,"   - Access Gate");
	}
	if(animationticks>=18){
		attron(COL_RD);
		mvaddch(17,1,'K');
		attron(COL_WH);
		mvprintw(17,2,"   - Killer ICE");
	}
	if(animationticks>=19){
		attron(COL_RD);
		mvaddch(18,1,'@');
		attron(COL_WH);
		mvprintw(18,2,"   - System Admin");
	}

	refresh();
	animationticks++;
	ResetEntryPoint();
	return;
}
void EXAMINE_Render(){
	SetEntryPoint(EP_EXAMINE_RENDER);
	erase();
	//If current room is not set, just run away, nothing good will happen if you proceed
	if(!currentroom){
		ResetEntryPoint();
		return;
	}
	//These coordinates are used for the world camera, basically centers around the
	//player and displays the tiles immediately around
	int leftcoordinate=examinex-10;
	int rightcoordinate=leftcoordinate+21;
	int topcoordinate=examiney-10;
	int bottomcoordinate=topcoordinate+21;
	//Render all the static tiles
	for(int i=topcoordinate;i<bottomcoordinate;i++){
		for(int j=leftcoordinate;j<rightcoordinate;j++){
			//If tile is outside the game world render a 'static' background
			if(i<0||j<0||i>currentroom->sizey-1||j>currentroom->sizex-1){
				attron(COL_RD);
				addch((const char)(rand()%32+32));
			}else{//Otherwise if it is a good coordinate then render the given tile
				if(!currentroom->tiles[i*currentroom->sizex+j].seen){
					if(!currentmap->fogofwar||LineOfSight(player->x,player->y,j,i,player->currentroom)){
						if(!currentroom->tiles[i*currentroom->sizex+j].apperance)
							continue;
						attron(currentroom->tiles[i*currentroom->sizex+j].apperance->nattribute);
						printw("%c",currentroom->tiles[i*currentroom->sizex+j].apperance->character);
						currentroom->tiles[i*currentroom->sizex+j].seen=TRUE;
					}else{
						attron(COL_RD);
						addch((const char)(rand()%32+32));
					}
				}else{
					if(!currentroom->tiles[i*currentroom->sizex+j].apperance)
						continue;
					attron(currentroom->tiles[i*currentroom->sizex+j].apperance->nattribute);
					printw("%c",currentroom->tiles[i*currentroom->sizex+j].apperance->character);
				}
			}
		}
		printw("\n");
	}
	//Run through Dynamic list of entities and render each if they are on screen
	struct dll* interactables=currentroom->interactables;
	if(interactables){
		do{
			if(!interactables->current)
				break;
			struct interactable* inter=interactables->current;
			if(inter->x<leftcoordinate||inter->x>rightcoordinate-1||inter->y<topcoordinate||inter->y>bottomcoordinate-1)
				continue;
			if(!inter->apperance)
				continue;
			attron(inter->apperance->nattribute);
			mvaddch(inter->y-topcoordinate,inter->x-leftcoordinate,inter->apperance->character);
		}while((interactables=NextDll(interactables)));
	}
	struct dll* actors=currentroom->actors;
	if(actors){
		do{
			if(!actors->current)
				break;
			struct actor* act=actors->current;
			if(act->x<leftcoordinate||act->x>rightcoordinate-1||act->y<topcoordinate||act->y>bottomcoordinate-1)
				continue;
			if(!act->seen){
				if(!LineOfSight(player->x,player->y,act->x,act->y,player->currentroom))
					continue;	
				if((rand()%act->stealth))
					continue;
				act->seen=true;
			}
			if(!act->apperance)
				continue;
			attron(act->apperance->nattribute);
			mvaddch(act->y-topcoordinate,act->x-leftcoordinate,act->apperance->character);
		}while((actors=NextDll(actors)));
	}
	attron(COL_RD);
	mvaddch(examiney-topcoordinate,examinex-leftcoordinate,'X');
	//Render UI to the right of the screen
	attron(COL_WH);
	if(player->attack)
		mvprintw(1,23,"Attack  :%s (%i)-%i",player->attack->name,player->attack->value,player->attack->type);
	if(player->defense)
		mvprintw(2,23,"Defense :%s (%i)-%i",player->defense->name,player->defense->value,player->defense->type);
	mvprintw(3,23,"Health  :%i",player->health);
	mvprintw(4,23,"Shield  :%i",player->shield);
	mvprintw(5,23,"Reaction:%i",player->reaction);
	mvprintw(6,23,"Stealth :%i",player->stealth);
	mvprintw(7,23,"Money   :%i",PlayerGetMoney());

	//Render a really cool tracer indicator
	static BOOLEAN tick=TRUE;//Used to create blinking effect
	attron(COL_GR);//First Select a color based on what percentage of the trace is complete
	//closer to red means more bad
	if(PlayerGetTrace()>50)
		attron(COL_WH);
	if(PlayerGetTrace()>75)
		attron(COL_YL);
	if(PlayerGetTrace()>90)
		attron(COL_RD);
	if(PlayerGetTrace()>95)
		tick=!tick;//Oscilate the blinking indicator if player is in the danger zone

	//Render text with numeric reading...(will remove numeric readout in later development, maybe make it an unlockable upgrade)
	//mvprintw(9,23,"Trace:%.2f",tracepercentage);
	mvprintw(9,23,"Trace:");
	//Render the top half of the bar
	mvprintw(10,23,"############");
	//Render the middle
	mvaddch (11,23,'#');
	for(int i=1;i<=10;i++){//Iterate over every slot of the bar and fill the appropriate slot, where each slot equals 10%
		if(PlayerGetTrace()>=i*10){
			if(tick)//Create the oscilation if the player is in the danger zone
				addch('=');
			else
				addch('-');
		}else
			addch(' ');
	}
	addch('#');
	//Render the bottom
	mvprintw(12,23,"############");

	attron(COL_WH);
	struct dll* item=player->data;
	if(item){
		int i=0;
		do{
			if(!item->current)
				break;
			if(!((struct data*)item->current)->name)
				break;
			mvprintw(13+i,23,":%s-%d\n",((struct data*)item->current)->name,((struct data*)item->current)->value);
			i++;
		}while((item=NextDll(item)));
	}

	attron(COL_WH);
	mvprintw(21,0,"Press q to close the scanner");
	mvprintw(23,0,"%s",examinename);
	mvprintw(24,0,"%s",examinedescription);
	if(strlen(examinealt))
		mvprintw(25,0,"%s",examinealt);

	move(26,0);
	refresh();
	ResetEntryPoint();
	return;
}
