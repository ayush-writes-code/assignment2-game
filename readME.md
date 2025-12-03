🚀 SPACE WAR

A terminal-based arcade game where you pilot a chunky ASCII spaceship through a storm of chaos at ludicrous speed.
Built in C using ncurses, powered by pure stubbornness and keyboard reflexes.

If you crash… you crash gloriously.

--------------------------------------------------------------------------------------------------------------------

🎮 WHAT IS SPACE WAR?

SPACE WAR is a fast-paced ASCII space-dodging game where:

You pilot a spaceship through 15 lanes of danger

Dodging obstacles (X) flying toward you

The track is HUGE. The speed is REAL.

Your goal: survive as long as possible and collect that sweet score

It’s like if the terminal had an arcade cabinet.

--------------------------------------------------------------------------------------------------------------------

🕹️ CONTROLS
Key	Action
W / ↑	Move Up
S / ↓	Move Down
A / ←	Move Left
D / →	Move Right
Q	Quit to Menu

--------------------------------------------------------------------------------------------------------------------

⚙️ FEATURES

✅ Full Menu System
✅ Difficulty Settings (Easy, Medium, Hard)
✅ Speed Settings (Slow, Fast, Ultra)
✅ Continue Game Support
✅ ASCII Spaceship (with wings 😏)
✅ Expanding score system
✅ Wide track (150 chars wide)
✅ Tons of obstacles
✅ ncurses-powered smooth gameplay

--------------------------------------------------------------------------------------------------------------------


🧠 DIFFICULTY LEVELS
Mode	Obstacles
Easy	3
Medium	8
Hard	15

--------------------------------------------------------------------------------------------------------------------

⚡ SPEED MODES
Mode	FPS Feel
Slow	Chill cruising
Fast	React or perish
Ultra	Terminal becomes a warzone

--------------------------------------------------------------------------------------------------------------------

🛠️ HOW TO COMPILE & RUN
Linux / MacOS:

Make sure you have ncurses installed.

gcc game.c -o spacewarp -lncurses
./spacewarp

If <ncurses.h> doesn’t work:

Change it to:

#include <curses.h>

--------------------------------------------------------------------------------------------------------------------


💾 SAVE SYSTEM

When you quit using Q, your game state is saved so you can Continue Game from the menu.
Crash though? Sorry… that ship is space dust ☠️

--------------------------------------------------------------------------------------------------------------------

🧑‍🚀 TIPS FOR PILOTS

Central lanes are NOT safe lanes.

Ultra speed does not care about your feelings.

Zigzagging is your friend.

Pretend you’re cool when you crash.

--------------------------------------------------------------------------------------------------------------------

📁 FILE STRUCTURE
game.c    -> Main game source
README.md -> This glorious document

--------------------------------------------------------------------------------------------------------------------


Thanks for reading!