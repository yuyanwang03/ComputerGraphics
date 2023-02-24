#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "entity.h"
#include "shader.h"

Application::Application(const char* caption, int width, int height) // : animation(this)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);
    this->currentSection = default_section;

    this->camera = new Camera(); // Pointer to avoid initialization process
    this->entity.renderMode = Entity::eRenderMode::TRIANGLES;
    this->useZbuffer = false;
    this->shader = new Shader();
    
}

Application::~Application()
{
    if (camera) delete this->camera; // Free memory
    if (shader) delete this->shader;
    SDL_DestroyWindow(window);
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
    camera->SetPerspective(50, window_width/window_height, 0.01, 100);
    entity = Entity("../res/meshes/anna.obj");
    shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
    std::cout << (shader == NULL) << std::endl;
    quad.CreateQuad();
}

// Render one frame
void Application::Render(void)
{
    // ...
    shader->Enable();
    quad.Render(GL_TRIANGLES);
    shader->Disable();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    // camera->Orbit(seconds_elapsed*10, 0);
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
            camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
            camera->SetOrthographic(-1,1,1,-1,-1,1);
            break;
        }
        case SDLK_p:
        { // Set to perspective projection
            std::cout << "Perspective View" << std::endl;
            camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
            camera->SetPerspective(45, window_width/window_height, 0.01, 100);
            break;
        }
        case SDLK_v:
        {
            std::cout << "Change center" << std::endl;
            currentSection = change_center;
            break;
        }
        case SDLK_q: {currentSection = default_section; break;}
        case SDLK_c: {
            this->useZbuffer = false;
            this->entity.renderMode = (entity.renderMode==Entity::eRenderMode::TRIANGLES) ? Entity::eRenderMode::TRIANGLES_INTERPOLATED : Entity::eRenderMode::TRIANGLES;
            break;
        }
        case SDLK_z:
        {
            this->useZbuffer = true;
            this->entity.texture = nullptr;
            break;
        }
        case SDLK_t:
        {
            this->useZbuffer = true;
            this->entity.LoadTexture("../res/textures/anna_color_specular.tga");
            break;
        }
        case SDLK_w: {this->useZbuffer = false; this->entity.renderMode = Entity::eRenderMode::WIREFRAME; break;}
        case SDLK_d: {this->useZbuffer = false; this->entity.renderMode = Entity::eRenderMode::POINTCLOUD; break;}
        case SDLK_r: {entity.entityColor.Random(); break;}
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
        
    } else if (event.button == SDL_BUTTON_RIGHT){
        
    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {

    } else if (event.button == SDL_BUTTON_RIGHT){

    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event) // Orbiting
{
    // std::cout<<"here"<<std::endl;
    if (event.button == SDL_BUTTON_LEFT){
        if (currentSection == change_center){
            camera->center = Vector3(camera->center.x+mouse_delta.x/12.0, camera->center.y+mouse_delta.y/12.0, camera->center.z);
            camera->UpdateViewMatrix();
        }
        else {camera->Orbit(mouse_delta.x, mouse_delta.y);}
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
