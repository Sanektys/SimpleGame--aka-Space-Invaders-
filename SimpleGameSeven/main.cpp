#include "game.h"


///////////////////////
// Точка входа
int main()
{
	Game* game = new Game();

	do {
		game->setupSystem();
		game->initialize();

		while (game->frame());

	} while (!game->shutdown());

	delete game;

	return 0;
}