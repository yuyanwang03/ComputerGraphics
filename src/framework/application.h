/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once


#include "main/includes.h"
#include "framework.h"
#include "image.h"

enum mouse_state{
    default_free = 0,
    left_click = 1,
    right_click = 2
};

enum buttonId{
    create = 1,
    save = 2,
    black = 3,
    red = 4,
    green = 5,
    navy_blue = 6,
    yellow = 7,
    purple = 8,
    sky_blue = 9,
    white = 10
};

class Application
{
public:

	// Window

	SDL_Window* window;
	int window_width;
	int window_height;
    // Set a attibute to keep track of the mouse color
    Color mouse_color;
    Image empty;
    bool toolbar_top;

	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
    Vector2 mouse_prev;

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
