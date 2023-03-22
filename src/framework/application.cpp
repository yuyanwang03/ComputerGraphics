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
    
}

Application::~Application()
{
    if (camera) delete this->camera; // Free memory
    // if (shader) delete this->shader;
    SDL_DestroyWindow(window);
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    // Set camera
    camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
    camera->SetPerspective(50, window_width/window_height, 0.01, 100);
    camera->Orbit(90.0, 0);
    // Load entity
    entity = Entity("../res/meshes/lee.obj");
    entity.modelMatrix.Rotate(160, Vector3(0, 1, 0));
    
    this->Ia = Vector3(0.2, 0.2, 0.2);
    uData.Ia = this->Ia;
    
    // Arbitrary values for light
    sLight l1, l2;
    // l1.position = Vector3(20.0, 20.0, 10.0);
    l1.position = Vector3(-3, 2, -1);
    l1.Id = Vector3(0.5, 0.5, 0.5);
    l1.Is = Vector3(0.5, 0.5, 0.5);
    
    l2.position = Vector3(5, 2, -1);
    l2.Id = Vector3(0, 0.5, 0.5);
    l2.Is = Vector3(0, 0.5, 0.5);
    
    this->lights.push_back(l1);
    this->lights.push_back(l2);
    
    // Add 1rt light
    uData.lights = this->lights;
    uData.numLights = (int) this->lights.size();
    
    entity.entityMaterial.Ka = Vector3(1.0, 1.0, 1.0);
    entity.entityMaterial.Kd = Vector3(1.0, 1.0, 1.0);
    entity.entityMaterial.Ks = Vector3(0.8, 0.8, 0.8);
    entity.entityMaterial.shiness = 10.0;
    
    uData.view_proj = camera->viewprojection_matrix;
    
    entity.SetCamera(this->camera);
    // Gouraud
    // entity.SetShader("shaders/gouraud.vs", "shaders/gouraud.fs", "");
    // Phong
    // entity.SetShader("shaders/phong.vs", "shaders/phong.fs", "");
    // Phong texture
    // entity.SetShader("shaders/phongTexture.vs", "shaders/phongTexture.fs", "");
    // Multipass
    entity.SetShader("shaders/multipass.vs", "shaders/multipass.fs", "");
    
    // entity.entityMaterial.SetViewProjection(this->camera);
    entity.LoadColorTexture("../res/textures/lee_color_specular.tga");
    entity.LoadNormalTexture("../res/textures/lee_normal.tga");
    
}

// Render one frame
void Application::Render(void)
{
    uData.view_proj = camera->viewprojection_matrix;
    uData.cameraEye = this->camera->eye;
    
    entity.Render(uData);
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    time += seconds_elapsed;
    // std::cout<<seconds_elapsed<<std::endl;
    // camera->Orbit(25*seconds_elapsed, 0.0);
    entity.modelMatrix.Rotate(seconds_elapsed, Vector3(0,1,0));
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_v:
        {
            std::cout << "Change center" << std::endl;
            currentSection = change_center;
            break;
        }
        case SDLK_q: {currentSection = default_section; break;}
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
            camera->center = Vector3(camera->center.x+mouse_delta.x/3.0, camera->center.y+mouse_delta.y/3.0, camera->center.z);
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
