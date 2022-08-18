#include "Game.h"

int main()
{
	Game* engine = new Game();
	engine->Run();
	delete engine;
}