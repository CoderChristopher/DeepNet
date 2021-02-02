# DeepNet
Deep Net is a terminal based ascii rogue like game written in C and using the NCurses for rendering text to the console. The idea behind the game is that you play as a cracker in a cyberpunk world breaking into computer systems, stealing valuble data, and fighting various programs and SysAdmins that try and stop you.

# Current State
The game is currently in a playable state. You can walk around a digital world, there is loot to collect, baddies to fight, difficulties to overcome, and a rudementry scoring system in place. While all all this content is there a playable, it is not a particularly deep game at the moment. All of the mechanics are shallow need further development.

# System Requirements
Unforetunately this game is only availible for systems that support ncurses, so Windows is out of the questions right now unless you are will to using WSL or something like Cygwin.  For OSX, Linux, or any other operating systems that support ncurses things should work with minimal effort. Minimum resolution is a screen that is 30x30 characters and anything built within the last three decades should likely run this with no issue.  
# Build Requirements
The requirements for building the game fairly minimal, and for the most part are already contained on most base linux systems:
	
*C compiler: I use gcc and positively confirm it works, but your milege may vary on other compilers

*make program: I use basic makefiles for invoking the toolchain. The project is not very complex at the moment so something more robust is not neccary

*NCurses Library: Very easy to install on most linux distrios. Just check your local repository for ncurses or libcurses.

# Build Instructions:
1. CD to the where you have downloaded the repo.
2. In the base folder (that is the folder that contains this README, a makefile, and the
src directory) type: make
3. Once the compile is done a binary will be created called deepnet. To  run the binary 
type: ./deepnet

#Code Tour
	main.c: Contains the scoring function, functions to catch various signals (segfaults mostly), and the main function. The main function is used to initalize various systems like NCurses, the signal catchers, the game state, the function pointers for the main loop, and setup various objects in the game. To understand the game loop in is import to note that the function calls in main() for ProcessInput(), Update(), and Render() are actually function pointers. This means that though they are called like normal functions actual function they call may be arbitarily changed during run time. This is used so that in various game states ( like in a menu, in game, showing the game over screen) the game loop stays exactly the same, but actual functions called by the main loop change based upon a set global variable for the game's state.
	A special word should also be said for how I handle the signal segfault. Basically, I have setup in every single function call in the game a special flag that is at the top of a function call and reset to its previous value when a function exits. I did this because segmentation faults are unfortunately very common during runs of the program. I am nor are my players always running GDB when they run the game. So to make tracking down bugs easier when the segfault signal is called the game will read the value of the entry point flag to determine what function caused the segmentation fault. The game will then report this value before exiting to the console.

	includes.h: Defines all included files in the project from stdlib and project files in their correct orders.

	dll.*: These files contain the implementation for my generic dynamic linked list implementation.

	const.h: This file contains a whole bunch of hard definitions used for the code and the game. There are definitions of booleans, PI, various entity state values, and all the definitions for the various entrypoints.

	globals.*: Defines various global variables used in the game state, of special note are the function pointer that are utilized by the main game loop 

	gamestate.*:defines the function for changing the Global Game State. (This is the function that switches all the game loop functions pointers I described above in main.c)

	utility.*: Defines various function that are used almost universally for doing specifc tasks: rng, entrypoint mangement, distance calculation, etc

	ninit.*: Functions for initalizing Ncurses for the project

	structs.h: Defines various data structures used in the project from Dynamic linked lists to actor objects.

	interactbles.*: Defines various functions for manipulating and managing interactable objects in the game

	actorfunctions.*: Defines various function for manipulating and managing actor objects in the game

	data.* : Defines functions for manipulating and managing data objects in the game

	look.* : Defines functions for manipulating and managing look data structures in the game

	rooms.* : Defines functions for manipulating and managing room objects in the game
	
	map.* : Defines functions for manipulating and managing map objects in the game

	tiles.* : Defines functions for manipulating and managing tile objects in the game

	input.* : Defines the functions that can be pointed to by the Input() function pointer, very closely tied to game state. Basically checks for player input

	update.* : Defines the functions that can be pointed to by the Update() function pointer, very closely tied to game state. Basically makes all game objects respond
	
	render.* : Defines the functions that can be pointed to by the Render() function pointer, very closely tied to game state. Basically displays everything

# FAQ

Q. What are your future plans with this project?
A. Originally, I just wanted to build a minimum playable rogue like with a 'hacking' asthetic. While I would by no means call this prototype finished or anything near fun, I do thing that I have built something that at the very least can be qualified as a rogue like game. From this point there are multiple features and things I would like to add/ expand including: map generation, enemy types, combat system, collectabiles, other interactables, a larger 'overworld' context. These are just some of the things I am interested in doing, but obviously them actually depends on my schedule/willingness to work on them.

Q. Will you support auto generating makefiles using something like a configure mechanism?
A. Perhaps in the future if this project takes off and the complexity invovled grows past what basic makefiles can do on their own.

Q. I found a bug in the game, where do I report it?
A. Thank you for taking the time to be considerate and want to report it. Please email me at cmc232@zips.uakron.edu with DeepNet-BUG in the subject line and be sure to include a description of the bug and/or any error codes thrown. For serious bugs that crash the game I have put in a system that gives very specific error codes, please copy and paste those in your email to make the process of bug revision much faster.
