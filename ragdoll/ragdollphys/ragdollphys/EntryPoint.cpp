#include <vector>
#include <iostream>
#include <string>

#include "RenderWindow.h"


int main() 
{
	bool isfullscreen = false;
	//int width = 1980;
	//int height = 1080;
	int width = 800;
	int height = 600;
	RenderWindow* window = new RenderWindow(width, height, new std::string("Physics Library - Gerallt Franke"), isfullscreen);
	window->render_loop();

	delete window;

	return EXIT_SUCCESS;
}