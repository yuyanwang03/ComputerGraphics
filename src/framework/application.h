/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/
#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "camera.h"
#include "entity.h"

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

enum workingSection{
    section3_1 = 1,
    section3_2 = 2,
    section3_3 = 3,
    section3_4 = 4,
    section3_5 = 5
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
    bool circleFill;
    bool toolbar_top;
    bool has_toolbar;
    int currentSection;
    Entity entity;
    
	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
    Vector2 mouse_prev;

    void SetToDefault(void);
	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

    // Create a class for the animation
    class EntitySystem{
        Application* app;
        Entity* entities;
        int numEntities;
        Color* entitiesColor;
        public:
        EntitySystem(Application* a) {app = a; numEntities=0; entities = NULL;}
        void Init(){
            // Load specific meshes here
            return;
        }
        void Update(float t) {for (int i=0; i<numEntities; i++) entities[i].Update(t); return;}
        void Render(){
            for (int i=0; i<numEntities; i++) {entities[i].Render(&app->framebuffer, app->camera, entitiesColor[i]);}
            return;
        }
    };
    
    EntitySystem animation;
	// CPU Global framebuffer
	Image framebuffer;
    Camera* camera;
    
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
        this->framebuffer.Resize(width, height);
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
