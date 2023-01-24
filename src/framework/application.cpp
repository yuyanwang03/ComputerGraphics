#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

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

    this->framebuffer.Resize(w, h);
}

Application::~Application()
{
    SDL_DestroyWindow(window);
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    
    Image toolbar{Image()};
    int status = toolbar.LoadPNG("../res/images/toolbar.png");
    std::cout << "load PNG: " << status << std::endl;
    this->framebuffer.DrawImagePixels(toolbar, 0, 0, true);
    
    this->framebuffer.Render();
}

// Render one frame
void Application::Render(void)
{
	// ...
    // Testing the code
    
    /*
    int centerx = this->window_width / 2;
    int centery = this->window_height / 2;
    framebuffer.Fill(Color::BLACK);
    framebuffer.DrawLineBresenham(centerx, centery, centerx + 200 * cos(time), centery + 200 * sin(time), Color::WHITE);
    */
    
    /*
    // 1 octant correct
    framebuffer.DrawLineBresenham(200, 200, 400, 300, Color(244, 85, 128));
    framebuffer.DrawLineDDA(200, 200, 400, 300, Color(244, 85, 128));
    
    // 8 octant correct
    framebuffer.DrawLineBresenham(200, 200, 400, 100, Color(10, 70, 80));
    framebuffer.DrawLineDDA(200, 200, 400, 100, Color(10, 70, 80));
    
    // 5 octant correct
    framebuffer.DrawLineBresenham(200, 200, 0, 100, Color(244, 85, 128));
    framebuffer.DrawLineDDA(200, 200, 0, 100, Color(244, 85, 128));
    
    // 4 octant correct
    framebuffer.DrawLineBresenham(200, 200, 0, 300, Color(80, 85, 128));
    framebuffer.DrawLineDDA(200, 200, 0, 300, Color(80, 85, 128));
     
    // 2 octant incorrect
    framebuffer.DrawLineBresenham(200, 200, 300, 400, Color(80, 85, 10));
    framebuffer.DrawLineDDA(200, 200, 300, 400, Color(80, 85, 10));
    
    // 3 octant incorrect
    framebuffer.DrawLineBresenham(200, 200, 100, 400, Color(120, 85, 128));
    framebuffer.DrawLineDDA(200, 200, 100, 400, Color(120, 85, 128));
    
    // 5 octant incorrect
    framebuffer.DrawLineBresenham(200, 200, 100, 0, Color(50, 10, 128));
    framebuffer.DrawLineDDA(200, 200, 100, 0, Color(50, 10, 128));
    
    // 7 octant incorrect
    framebuffer.DrawLineBresenham(200, 200, 300, 0, Color(120, 200, 128));
    framebuffer.DrawLineDDA(200, 200, 300, 0, Color(120, 200, 128));
    */
    
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
