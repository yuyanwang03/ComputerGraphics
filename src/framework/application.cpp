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
    // 1 mesh
    Entity temp{Entity()};
    Mesh tempMsh{Mesh()};
    int status = tempMsh.LoadOBJ("../res/meshes/anna.obj");
    // std::cout << status << std::endl;
    if (status) {temp.SetMesh(tempMsh); std::cout << "Mesh correctly set" << std::endl;}
    
    // 2 mesh
    Entity temp2{Entity()};
    Mesh tempMsh2{Mesh()};
    Camera tempCam = Camera();
    int status2 = tempMsh2.LoadOBJ("../res/meshes/cleo.obj");
    // std::cout << status << std::endl;
    if (status2) {temp2.SetMesh(tempMsh2); std::cout << "Mesh correctly set" << std::endl;}
    
    // Camera change view
    this->camera->LookAt(Vector3(1, 1, 1), Vector3(0, 0.3, 0), Vector3::UP);
    temp.Render(&this->framebuffer, this->camera, Color::BLUE);
    // this is rendering the 2 mesh with a different camera
    temp2.Render(&this->framebuffer, &tempCam, Color::WHITE);
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
            this->animation.Init(); // Function to be filled!!
            this->animation.Render();
            break;
        }
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

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}
