#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "entity.h"

Application::Application(const char* caption, int width, int height) // : animation(this)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->mouse_state = default_free;
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);
    this->currentSection = default_section;

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
    camera->LookAt(Vector3(0,1.5,1.5), Vector3(0,0,0), Vector3::UP);
    camera->SetPerspective(50, window_width/window_height, 0.01, 100);
    entity = Entity("../res/meshes/anna.obj");
    // entity.modelMatrix.Translate(0, 0.6, 0.1);
}

// Render one frame
void Application::Render(void)
{
    // ...
    framebuffer.Fill(Color::BLACK);
    entity.Render(&framebuffer, camera, Color::RED);
    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

// Sets the application to starting conditions, which includes painting the displayed framebuffer with black
void Application::SetToDefault(void){
    camera->LookAt(Vector3(0.5,0.5,0.5), Vector3(0,0.3,0), Vector3(0,1,0));
    currentSection = default_section;
    return;
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_o:
        { // Set to orthographic projection
            std::cout << "Orthographic View" << std::endl;
            SetToDefault();
            camera->SetOrthographic(-1,1,1,-1,-1,1);
            break;
        }
        case SDLK_p:
        { // Set to perspective projection
            std::cout << "Perspective View" << std::endl;
            this->SetToDefault();
            camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
            camera->SetPerspective(45, window_width/window_height, 0.01, 30);
            break;
        }
        case SDLK_v:
        {
            std::cout << "Change center" << std::endl;
            currentSection = change_center;
            break;
        }
        case SDLK_q:
        {
            currentSection = default_section;
            /*
            // To see the effects of far_plane and near_plane values
            SetToDefault();
            camera->view_matrix.SetIdentity(); // near far
            camera->SetOrthographic(-1,1,1,-1,-1,1);
            */
            break;
        }
        case SDLK_c:
        { // Change color of the entity
            // animation.ChangeColor();
            break;
        }
        case SDLK_n: {currentSection = change_near; break;}
        case SDLK_f: {currentSection = change_far; break;}
        case SDLK_PLUS:
        { // Increase values for near_plane, far_plane or fov
            if (currentSection==change_far) {camera->far_plane += 0.15;}
            else if (currentSection==change_near) {camera->near_plane += 0.15; camera->near_plane = std::min(camera->near_plane, camera->far_plane);}
            else if (camera->type==Camera::PERSPECTIVE) {camera->fov += 5; camera->fov = std::min((int)camera->fov, 180);}
            else {break;}
            camera->UpdateProjectionMatrix();
            std::cout << "Increased value" << std::endl;
            break;
        }
        case SDLK_MINUS:
        { // Decrease values for near_plane, far_plane or fov
            if (currentSection==change_far) {camera->far_plane -= 0.15; camera->far_plane = std::max(camera->near_plane, camera->far_plane);}
            else if (currentSection==change_near) {camera->near_plane -= 0.15;}
            else if (camera->type==Camera::PERSPECTIVE) {camera->fov -= 5; camera->fov = std::max((int)camera->fov, 0);}
            else {break;}
            camera->UpdateProjectionMatrix();
            std::cout << "Decreased value" << std::endl;
            break;
        }
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = left_click;
    } else if (event.button == SDL_BUTTON_RIGHT){
        mouse_state = right_click;
    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = default_free;
    } else if (event.button == SDL_BUTTON_RIGHT){
        mouse_state = default_free;
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event) // Orbiting
{
    while (mouse_state==left_click){
        if (currentSection == change_center){
            camera->center = Vector3(camera->center.x+mouse_delta.x/6.0, camera->center.y+mouse_delta.y/6.0, camera->center.z);
            camera->UpdateViewMatrix();
        }
        else {camera->Orbit(mouse_delta.x, mouse_delta.y);}
        break;
    }
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    // Zoom in; Zoom out
	float dy = event.preciseY;
    camera->Zoom(dy);
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}
