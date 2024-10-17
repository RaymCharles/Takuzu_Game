<<<<<<< HEAD
# Takuzu_Game
=======
# takuzu-10a
##### Project status
Our project still contains some potential bugs that we don't know how solve.

##### Project's name
Takuzu game, a single-player logic puzzle

##### Description
This project proposes to you to develop a computer program in order to obtain a game (takuzu) which can be played without problem.

# Takuzu's initial rules
The game Takuzu can be played with the following rules considered as a first version of the game (V1).
→ Given a grid of squares, you must fill it with zeros and ones. Some squares are initially provided as clues and are immutable. You must complete the remaining empty squares, according to the following rules :
• Each row and column must contain the same number of zeros and ones.
• No row or column may contain three consecutive squares of the same number.
We limit our game to square grid of size 6x6.

→ By convention, the square in the i-th row and j-th column of the grid is referred to as the coordinate (i,j), and the coordinate (0,0) corresponds to the top left corner (like in matrices).
The squares are stored as an C array in memory using row-major storage.

→ In summary, the game uses 5 different square states, that can be printed in a terminal as a single character:
• empty (`' '),
• zero/white ('w'),
• one/black ('b'),
• immutable zero/white ('W'),
• immutable one/black ('B').
Note that immutable squares are represented with an uppercase character.

# Takuzu's advanced rules
This project contains advanced rules considered as a new version of the game (V2), which is characterized by the following changes :
• The grid can now be rectangular and have an arbitrary (but still even) number of rows and columns.
• The game grid has the option of having a "toric" topology (wrapping option) : the rightmost column is adjacent to the leftmost column and the highest row is adjacent to the lowest row. In order to properly take this option into account, it was necessary to adapt the code of the game_next_square() and game_next_number() functions and to rely on it to implement the other impacted functions such as game_is_over(), etc.
• Furthermore, we add the unique option, which modifies the rules of the game and indicates that two rows must not be identical, nor two columns. (This option is found in several web versions of the game.)

# Takuzu with the graphic interface
To play in better conditions, we have created a graphical interface using the SDL library.
To access it, run the file.
From Ubuntu, you need to install the library if you haven't already.
Here are the commands to type from your terminal :
sudo apt install net-tools
>route -n (to retrieve your IP address)
>export DISPLAY=<gateway>:0 (replacing <gateway> with your IP address)
>glxgears
>ESCAPE (to close the window that just opened)
>sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
>sudo apt install git gcc cmake
>git clone git@gitlab.emi.u-bordeaux.fr:pt2/teams/tm10/takuzu-10a.git
>cd takuzu-10a
>mkdir build
>cd build
>cmake..
>make
>./game_sdl
To play, click one, two or three times on the square to get Panpan, Bambi or an empty square.
If you try to play a forbidden move, a sad Panpan or Bambi appears in the square.
If you won, a message is displayed.
On the right, you have the possibility to :
- cancel a move : the arrow to the left
- replay this move : the arrow to the right
- reset the game : the restart button
- quit the game : the rocket

# A few more variations
This puzzle is known under several names including Takuzu, Tohu-Wa-Vohu, Binairo or Unruly. There are some variations in the symbols used for this game, as for instance X and O letters, white and black colours, though 0 and 1 are the most common symbols used.

# About the files
Most of the functions for playing are in the game.c file.
However, you can find the game extension corresponding to the V2 version in the game_ext.c file and the game_test.c file aims to test all functions and show you their effectiveness.
Here is a link you can consult for more details :
https://pt2.gitlabpages.inria.fr/support/doc/v2/html/index.html

##### Installation
You can install packages with Ubuntu :
https://packages.ubuntu.com/
# Requirements
The project only runs in a specific context. You must have :
• a particular programming language C version : C99
• a particular operating system : Linux. You can install Ubuntu : https://www.ubuntu-fr.org/
• particular dependencies as stdbool.h, stdio.h, stdlib.h, string.h, or assert.h. You can use Yarn for example :
https://yarnpkg.com/

##### Usage
Some information...
# To test our project
• open a terminal Linux,
• create a temporary build folder with the following command : mkdir build
• enter in this folder using the following command: cd build
• then, compile using the following commands :
cmake..
make
make test
→ You should have no problem because our project is compilable

# To play
• open a terminal Linux,
• open a game using the following command : ./game
   012345
   ------
0 | BW   |
1 |      |
2 | W  W |
3 | WB   |
4 |  B  W|
5 |     W|
   ------
• enter a character ("b" for black or "w" for white)
• entre a move ("row index" space "line index")
• click enter
→ example : b and 0 3
   012345
   ------
0 | BWb  |
1 |      |
2 | W  W |
3 | WB   |
4 |  B  W|
5 |     W|
   ------

• if you choose an incorrect move, a message will instruct you to choose another

• if you need help, you can press "h" and click enter :
action : help
- press 'w <i> <j>' to put a zero/white at square (i,j)
- press 'b <i> <j>' to put a one/black at square (i,j)
- press 'e <i> <j>' to empty square (i,j)
- press 'r' to restart
- press 'q' to quit
- press 'z' to undo a move
- press 'y' to redo a move

• the game evolves gradually and a message will be displayed depending on the final result : "shame" or "congratulation"

##### Support
If you need help, you can :
• contact us at the following email addresses :
  - salome.rodriguez@etu.u-bordeaux.fr
  - baptiste.ruberte@etu.u-bordeaux.fr
  - charles.raymondiere@etu.u-bordeaux.fr
• use rocket chat :
  https://chat.u-bordeaux.fr/group/PT2

## Roadmap
We could imagine a three-dimensional version of the game.

##### Contributing
We are not open to contributions.

##### Authors and acknowledgment
We are three to have coded the functions of this game : Baptiste RUBERTE, Charles RAYMONDIERE and Salomé RODRIGUEZ, but we would like to thank our teachers who are at the origin of this project, in particular Aurélien ESNARD, Nicolas BONICHON and Alexis BANDET because without their help , we could not have advanced in the same way.

##### License
Our project is not an open source projects, it is private.
>>>>>>> 96cf8c7 (Initial commit for Takuzu_Game)
