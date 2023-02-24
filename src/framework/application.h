/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/
#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "camera.h"
#include "entity.h"
#include "shader.h"

enum workingSection{
    default_section = 1,
    orthographic = 2,
    perspective = 3,
    change_far = 4,
    change_near = 5,
    change_center = 6
};

class Application
{
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;
    int currentSection;
	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
    bool useZbuffer;

    void SetToDefault(void);
	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	// Image framebuffer;
    Camera* camera;
    Entity entity;
    Shader* shader;
    Mesh quad;
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
        // Update framebuffer size
        // Update aspect ratio
        this->camera->SetAspectRatio((float)width/height);
        // Update projection matrix, that may use the aspect ratio value
        this->camera->UpdateProjectionMatrix();
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
