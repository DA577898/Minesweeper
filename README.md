Name: Daniel Hernandez\
Section: 10829\
UFL email: dhernandez8@ufl.edu\
System: ASUS Laptop, Windows 11\
Compiler: MinGW\
SFML version: 2.6.x (CMakeLists.txt linked in Project 3 Specifications)\
IDE: CLion\
Other notes:\
The game is guaranteed to start at a blank tile, as long as the number of mines isn't equal to the number of cells. The
leaderboard button only shows the "tile_revealed" when in a game, because it doesn't make sense to hide the mines if 
they lost or the game if they won. All cpp files must be included. The asterisk from a new winner is removed once a new 
game is started. If the leaderboard has less than 5 players, it adds the player to the allowed position

Also added the function where, if the player clicks a tile with numbers, and the flags surrounding it are equal to this number, then it 
cleans out the square (chording).