//Replace with your usernames!
#define printusers() printf("Program by heinish+klitzkd\n");

#define _CRT_SECURE_NO_WARNINGS
#define RESOLUTION 512
#define TARGET_FPS 30                // controls spin update rate
#define TIME_BETWEEN_UPDATES 0.001   // seconds between motion updates
#define PRINT_FPS_INTERVAL 2.0f


#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#endif

#define GL3W
#ifdef GLEW
#define GLEW_STATIC
#include "glew/glew.h"
#endif
#ifdef GL3W
#include "gl3w/gl3w.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <SFML/Window.hpp>
//#define _USE_MATH_DEFINES
//#include <math.h>
#define PI 3.14159f
#include "ShaderManager.h"
#include "GLHelper.h"
#include "RenderEngine.h"
#include "TrackBall.h"
#include "WorldState.h"
#include "PlanetModel.h"

/*
--------------------------------------------------------------------------------------------------
This is the main module. Here especially events are handled.
--------------------------------------------------------------------------------------------------
*/

class Program4
{
public:
	Program4()
	{
		getWindowContext();

		WorldState state;
		render.init(state);
		
		fixMousePos = glm::ivec2(RESOLUTION/2, RESOLUTION/2);
		buttonDown[0]=false;
		buttonDown[1]=false;
		buttonDown[2]=false;
		render.reshape(RESOLUTION, RESOLUTION);
		state.setSize(RESOLUTION, RESOLUTION);
		
		sf::Clock c;
		float lastFrame = c.restart().asSeconds();
		float lastPrint = lastFrame;
		float targetFrameTime = 1.0f/(float)TARGET_FPS;
		App->setMouseCursorVisible(false);
		while (state.isRunning())
		{			
			App->setActive();
			float currentTime = c.getElapsedTime().asSeconds();
			float sinceLastFrame = currentTime - lastFrame;
			float sleepTime = targetFrameTime - sinceLastFrame;
			if(sleepTime > 0)
				sf::sleep(sf::seconds(sleepTime));
			
			currentTime = c.getElapsedTime().asSeconds();
			lastFrame = currentTime;
			float sinceLastPrint = currentTime - lastPrint;
			handleEvents(state, render);
			state.timeStep(currentTime);
			if(sinceLastPrint > PRINT_FPS_INTERVAL) {
				lastPrint = currentTime;
				state.printFPS();
			}
			render.display(state);
			App->display();
		}
	}
	
private:
	sf::Window * App;
	RenderEngine render;
	
	sf::Clock timer;
	float lastUpdate;
	float motionTime;
	glm::ivec2 fixMousePos;
	bool buttonDown[3];
	int cnt = 0;

	void handleEvents(WorldState & state, RenderEngine & render)
	{
		bool shiftDown = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
		sf::Event event;
		
		while (App->pollEvent(event))
		{
			// Exit program when pressing escape or closing window
			if (event.type == sf::Event::Closed)
				state.setRunning(false);
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				state.setRunning(false);
			
			if (event.type == sf::Event::Resized) {
				fixMousePos.x = event.size.width / 2;
				fixMousePos.y = event.size.height / 2;
				render.reshape( event.size.width, event.size.height );
				state.setSize( event.size.width, event.size.height );
			}

			
			// WASD and Mouse 
			// Control translation with WASD
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W)) {
				state.updateMoveCameraForward(true);
			}
			if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::W)) {
				state.updateMoveCameraForward(false);
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S)) {
				state.updateMoveCameraBackward(true);
			}
			if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::S)) {
				state.updateMoveCameraBackward(false);
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A)) {
				state.updateMoveCameraLeft(true);
			}
			if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::A)) {
				state.updateMoveCameraLeft(false);
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::D)) {
				state.updateMoveCameraRight(true);
			}
			if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::D)) {
				state.updateMoveCameraRight(false);
			}
			
			// Mouse control
			if (event.type == sf::Event::MouseMoved) {
				bool needsUpdate = lastUpdate > TIME_BETWEEN_UPDATES;
				if (needsUpdate) {
					glm::ivec2 newPos = glm::ivec2(event.mouseMove.x, event.mouseMove.y);
					state.updateRotate(fixMousePos, newPos);
					sf::Mouse::setPosition(sf::Vector2i(fixMousePos.x, fixMousePos.y), *App);
					lastUpdate = timer.restart().asSeconds();
				}
				else {
					lastUpdate = timer.getElapsedTime().asSeconds();
				}
			}

		}
		
		
	}
	
	void getWindowContext()
	{
		sf::err().rdbuf(NULL); //hide errors
		
#ifdef __APPLE__
		int nullFD = open("/dev/null", O_WRONLY);
		int oldFD = dup(2); // Duplicate the old file descriptor, so it can be restored
		dup2(nullFD, 2); // Redirect
#endif
		
		sf::VideoMode mode(RESOLUTION, RESOLUTION, 32);
#ifdef __linux__
		sf::ContextSettings settings(32, 0, 0, 3, 3);
#else
		sf::ContextSettings settings(24, 0, 0, 3, 3, sf::ContextSettings::Core);
#endif
		App = new sf::Window(mode, "SFML application", sf::Style::Default, settings);
		
#ifdef __APPLE__
		dup2(oldFD, 2); // Redirect back
		close(oldFD); // Not needed anymore
#endif
	}
};

int main()
{
	printusers();
	Program4 prog;
	
    return EXIT_SUCCESS;
}
