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
    
	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
    Vector2 mouse_prev;

    bool LoadToolbar(void);
    void ProceedToolbarFunction(int ButtonID);
    void SetToDefault(void);
	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;
    
    struct Particle{
        Vector2 position;
        Vector2 direction;
        int size;
        float velocity;
        Color color;
    };
    
    class ParticleSystem{
        int numParticles;
        bool dirRight;
        Particle* particles;
        Application* app;
        public:
        ParticleSystem(Application* a) {app = a;}
        void Init(int n) {
            numParticles = n; particles = new Particle[numParticles]; dirRight = true;
            for (int i=0; i<numParticles; i++){
                particles[i].position.set(rand()%app->window_width, rand()%app->window_height);
                particles[i].direction = Vector2(1, 0);
                particles[i].color.Random();
                particles[i].size = 1 + rand()%6;
                particles[i].velocity = 30 + rand()%60;
            }
            return;
        }
        void Update(float t) {
            for (int i=0; i<numParticles; i++){
                particles[i].position.x += particles[i].direction.x * particles[i].velocity * t;
                if (dirRight) {
                    if (particles[i].position.x > app->window_width || particles[i].position.y > app->window_height) {particles[i].position.set(1, rand()%app->window_height);}
                } else{
                    if (particles[i].position.x < 0 || particles[i].position.y > app->window_height) {particles[i].position.set(app->window_width-1, rand()%app->window_height);}
                }
            }
            this->Render();
            return;
        }
        void Render(){
            app->framebuffer.Fill(Color::BLACK);
            for (int i=0; i<numParticles; i++){
                app->framebuffer.DrawCircle(particles[i].position.x, particles[i].position.y, particles[i].size, particles[i].color, true);
            }
            return;
        }
        void ChangeDirection(bool right){
            if (dirRight==right){return;}
            for (int i=0; i<numParticles; i++){
                if (right) {particles[i].direction.x = 1; dirRight=true;}
                else {particles[i].direction.x = -1; dirRight = false;}
            }
        }
    };
    
    ParticleSystem animation;
    
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
        this->framebuffer.Resize(width, height);
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
