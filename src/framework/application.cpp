#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "entity.h"

Application::Application(const char* caption, int width, int height) : animation(this)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->mouse_state = 0;
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);

    entityColor = Color::CYAN;
    this->camera = new Camera(); // Pointer to avoid initialization process
    this->entity = Entity("../res/meshes/anna.obj");
        
    this->framebuffer.Resize(w, h);
}

Application::~Application()
{
    if (camera) delete this->camera; // Free memory
    SDL_DestroyWindow(window);
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    
    // Code testing
    
    entity.Render(&this->framebuffer, this->camera, entityColor);

}

// Render one frame
void Application::Render(void)
{
    // ...
    
    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    // If inside the animation section, update the animation
    if (currentSection == section3_4) {this->animation.Update(seconds_elapsed);}
}

// Sets the application to starting conditions, which includes painting the displayed framebuffer with black
void Application::SetToDefault(void){
    this->framebuffer.Fill(Color::BLACK);
    mouse_prev.set(-1, -1);
    return;
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_4: {
            this->SetToDefault();
            this->currentSection = section3_4;
            std::cout<< "Section 3.4 Animated Version" << std::endl;
            // Initializes the animation
            this->animation.Init();
            this->animation.Render();
            break;
        }
        case SDLK_o: {
            std::cout << "Ortographic" << std::endl;
            this->SetToDefault();
            camera->SetOrthographic(-1,1,1,-1,-1,1);
            entity.Render(&this->framebuffer, this->camera, entityColor);
            break;
        }
        case SDLK_p: {
            std::cout << "Perspective" << std::endl;
            this->SetToDefault();
            camera->SetPerspective(45, window_width/window_height, 0.01, 100);
            entity.Render(&this->framebuffer, this->camera, entityColor);
            break;
        }
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = left_click;
    } else if (event.button == SDL_BUTTON_RIGHT){
        std::cout<<"rightc"<<std::endl;
        mouse_state = right_click;
        mouse_prev = mouse_position;
    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = default_free;
    } else if (event.button == SDL_BUTTON_RIGHT){
        std::cout<<"rightfree"<<std::endl;
        // this->camera->Move(Vector3(mouse_prev.x-mouse_position.x, mouse_prev.y-mouse_position.y, 0));
        entity.Render(&this->framebuffer, this->camera, Color::GREEN);
        this->Render();
        mouse_state = default_free;
        mouse_prev.set(-1,-1);
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{

}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}
