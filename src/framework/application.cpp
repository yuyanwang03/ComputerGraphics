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
    this->shader = new Shader();
    this->shaderType = 1.0;
    this->useTexture = false;
    this->useQuad = true;
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
    
    // Load Quad shader
    shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
    this->shaderTexture.Load("images/fruits.png");
    quad.CreateQuad();
    
}

// Render one frame
void Application::Render(void)
{
    if (useQuad){ // Render Quad
        shader->Enable();
        shader->SetFloat("shaderType", shaderType);
        shader->SetTexture("u_texture", &shaderTexture);
        quad.Render(GL_TRIANGLES);
        shader->Disable();
    } else{
        entity.Render();
    }
    
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    // shader->ReloadAll();
    // camera->Orbit(0.2*seconds_elapsed, 0.0);
    // entity.viewMatrix = camera->view_matrix;
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_a: {useQuad = true; useTexture = false; break;}
        case SDLK_b: {useQuad = true; useTexture = true; this->shaderType = 1.5; break;}
        case SDLK_c: {
            useQuad = false;
            // Set camera
            camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
            camera->SetPerspective(50, window_width/window_height, 0.01, 100);
            // Load entity
            entity = Entity("../res/meshes/anna.obj");
            entity.modelMatrix.Rotate(160, Vector3(0, 1, 0));
            entity.SetShader("shaders/simple.vs", "shaders/simple.fs", "");
            entity.SetCamera(this->camera);
            entity.LoadTexture("../res/textures/anna_color_specular.tga");
            break;
        }
        case SDLK_1: {shaderType= useTexture ? 1.5: 1.0; break;}
        case SDLK_2: {shaderType= useTexture ? 2.5: 2.0; break;}
        case SDLK_3: {shaderType= useTexture ? 3.5: 3.0; break;}
        case SDLK_4: {shaderType= useTexture ? 4.5: 4.0; break;}
        case SDLK_5: {shaderType= useTexture ? 5.5: 5.0; break;}
        case SDLK_6: {shaderType= useTexture ? 6.5: 6.0; break;}
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
        case SDLK_z:
        {
            this->entity.texture = nullptr;
            break;
        }
        case SDLK_t:
        {
            this->entity.LoadTexture("../res/textures/anna_color_specular.tga");
            break;
        }
        case SDLK_w: {this->entity.renderMode = Entity::eRenderMode::WIREFRAME; break;}
        case SDLK_d: {this->entity.renderMode = Entity::eRenderMode::POINTCLOUD; break;}
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
