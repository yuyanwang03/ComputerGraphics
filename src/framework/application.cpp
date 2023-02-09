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
    
    // Code testing
    this->animation.Init();
    this->animation.Render();

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
    this->animation.Update(seconds_elapsed);
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
        case SDLK_o:
        { // Set to orthographic projection
            std::cout << "Orthographic" << std::endl;
            this->SetToDefault();
            camera->view_matrix.SetIdentity();
            camera->SetOrthographic(-1,1,1,-1,-1,1);
            entity.Render(&this->framebuffer, this->camera, entityColor);
            break;
        }
        case SDLK_p:
        { // Set to perspective projection
            std::cout << "Perspective" << std::endl;
            this->SetToDefault();
            camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
            camera->SetPerspective(45, window_width/window_height, 0.01, 100);
            entity.Render(&this->framebuffer, this->camera, entityColor);
            this->animation.Render();
            break;
        }
        case SDLK_q:
        { // Go back to initial state; can be used to exit animation
            currentSection = default_section;
            SetToDefault();
            camera->type = Camera::ORTHOGRAPHIC;
            camera->view_matrix.SetIdentity();
            camera->SetOrthographic(-1,1,1,-1,-1,1);
            entity.Render(&this->framebuffer, this->camera, entityColor);
            break;
        }
        case SDLK_c:
        { // Change color of the entity
            animation.ChangeColor();
            break;
        }
        case SDLK_n: {currentSection = change_near; break;}
        case SDLK_f: {currentSection = change_far; break;}
        case SDLK_PLUS:
        { // Increase values for near_plane, far_plane or fov
            std::cout <<"plus"<<std::endl;
            if (currentSection==change_far) {camera->far_plane += 0.15; std::cout <<"here"<<std::endl;camera->UpdateProjectionMatrix();}
            if (currentSection==change_near) {camera->near_plane += 0.15; camera->UpdateProjectionMatrix();}
            if (currentSection==perspective) {
                std::cout <<"here"<<std::endl;
                camera->fov += 5;
                camera->UpdateProjectionMatrix();
                this->animation.Render();
            }
            break;
        }
        case SDLK_MINUS:
        { // Decrease values for near_plane, far_plane or fov
            if (currentSection==change_far) {camera->far_plane -= 0.15; camera->UpdateProjectionMatrix();}
            if (currentSection==change_near) {camera->near_plane -= 0.15; camera->UpdateProjectionMatrix();}
            if (currentSection==perspective) {
                camera->fov -= 5;
                camera->UpdateProjectionMatrix();
                this->animation.Render();
            }
            break;
        }
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = left_click;
        mouse_prev = mouse_position;
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
    float dx = event.preciseX;
    /*
    camera->MoveEye(dx, dy);
     */
    
    camera->eye = camera->eye+Vector3(0,0,dy);
    camera->UpdateViewMatrix();
    framebuffer.Fill(Color::BLACK);
    entity.Render(&this->framebuffer, this->camera, entityColor);
    this->Render();
    
    std::cout << dx << " " << dy<<std::endl;
    
	// ...
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}
