#include "Game.h"
int main() 
{
	Game game; 
	bool init = game.Initialize(); 
	if (init) 
	{
		game.RunGame(); 
	}

	game.ShutDown(); 
	return 0;
}