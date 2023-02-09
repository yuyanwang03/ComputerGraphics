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

enum workingSection{
    default_section = 1,
    orthographic = 2,
    perspective = 3,
    change_far = 4,
    change_near = 5,
};

class Application
{
public:

	// Window

	SDL_Window* window;
	int window_width;
	int window_height;
    // Set a attibute to keep track of the mouse color
    int currentSection;
	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

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
        Entity entities[3];
        int numEntities;
        Color entitiesColor[3];
        public:
        EntitySystem(Application* a) {app = a; numEntities=0;}
        void Init(){
            app->camera->SetOrthographic(-1,1,1,-1,-1,1);
            // Load specific meshes here
            app->camera->LookAt(Vector3(0.5, 0.5, 0.5), Vector3(0, 0.3, 0), Vector3::UP);
            entities[0] = Entity("../res/meshes/anna.obj");
            entities[0].modelMatrix.Translate(0, 0.6, 0.1);
            entities[1] = Entity("../res/meshes/cleo.obj");
            entities[1].modelMatrix.Translate(0, 0, 0.4);
            entities[2] = Entity("../res/meshes/lee.obj");
            entities[2].modelMatrix.Translate(0, -0.4, -0.3);
            numEntities = 3;
            for (int i=0; i<numEntities; i++) {
                Color temp = Color();
                temp.Random();
                entitiesColor[i] = temp;
            }
            return;
        }
        void Update(float t) {
            for (int i=0; i<numEntities; i++) entities[i].Update(t);
            this->Render();
            return;
        }
        void Render(){
            app->framebuffer.Fill(Color::BLACK);
            for (int i=0; i<numEntities; i++) {entities[i].Render(&app->framebuffer, app->camera, entitiesColor[i]);}
            return;
        }
        void ChangeColor(){
            for (int i=0; i<numEntities; i++) {
                Color temp = Color();
                temp.Random();
                entitiesColor[i] = temp;
            }
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
        this->framebuffer.Fill(Color::BLACK);
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
