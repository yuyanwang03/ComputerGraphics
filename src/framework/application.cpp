#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "entity.h"

Application::Application(const char* caption, int width, int height)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->mouse_state = 0;
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);

    this->camera = new Camera(); // Pointer to avoid initialization process
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
    Entity temp{Entity()};
    Mesh tempMsh{Mesh()};
    int status = tempMsh.LoadOBJ("../res/meshes/anna.obj");
    // std::cout << status << std::endl;
    if (status) {temp.SetMesh(tempMsh); std::cout << "Mesh correctly set" << std::endl;}
    
    this->camera->UpdateViewMatrix();
    this->camera->UpdateProjectionMatrix();
    temp.Render(&this->framebuffer, this->camera, Color::BLUE);
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

}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_o: {break;}
        case SDLK_p: {break;}
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {

    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {

    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}
