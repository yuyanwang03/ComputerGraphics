#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->toolbar_top = false;
    this->has_toolbar = false;
    this->currentSection = section3_1;
    this->mouse_state = 0;
    // Set default mouse color to white
    this->mouse_color = Color(255, 255, 255);
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);

    this->framebuffer.Resize(w, h);
    this->empty = Image(this->framebuffer);
}

Application::~Application()
{
    SDL_DestroyWindow(window);
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
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
        case SDLK_ESCAPE: {exit(0); break;} // ESC key, kill the app
        case SDLK_1:
        {
            std::cout << "Section 3.1" << std::endl;
            currentSection = section3_1;
            break; // 1 key, execute section 3.1
        }
        case SDLK_2:
        {
            std::cout << "Section 3.2" << std::endl;
            currentSection = section3_2;
            break;
        }
        case SDLK_3:
        {
            std::cout << "Section 3.3" << std::endl;
            currentSection = section3_3;
            break;
        }
        case SDLK_4:
        {
            std::cout << "Section 3.4" << std::endl;
            currentSection = section3_4;
            break;
        }
        case SDLK_5:
        {
            std::cout << "Section 3.5" << std::endl;
            currentSection = section3_5;
            break;
        }
        case SDLK_t:
        {
            if (this->currentSection == section3_3) {
                this->toolbar_top = true;
                // Clean the frame buffer
                this->framebuffer = Image(this->empty);
                // Adding the toolbar to the framebuffer
                Image toolbar{Image()};
                int status = toolbar.LoadPNG("../res/images/toolbar.png");
                // Status check
                if (status) {this->framebuffer.DrawImagePixels(toolbar, 0, 0, toolbar_top); has_toolbar=true;}
                else std::cout << "There has been some error loading the toolbar" << std::endl;
            }
            break;
        }
        case SDLK_b:
        {
            if (this->currentSection == section3_3) {
                this->toolbar_top = false;
                // Clean the frame buffer
                this->framebuffer = Image(this->empty);
                // Adding the toolbar to the framebuffer
                Image toolbar{Image()};
                int status = toolbar.LoadPNG("../res/images/toolbar.png");
                // Status check
                if (status) {this->framebuffer.DrawImagePixels(toolbar, 0, 0, toolbar_top); has_toolbar=true;}
                else std::cout << "There has been some error loading the toolbar" << std::endl;
            }
            break;
        }
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = left_click;
        if (currentSection==section3_3 && has_toolbar){
            std::pair<int, int> bound = toolbar_top ? std::make_pair(0, this->framebuffer.height-64) : std::make_pair(64, this->framebuffer.height);
            if (!(mouse_position.y < bound.first || mouse_position.y > bound.second)) {mouse_prev.set(mouse_position.x, mouse_position.y);}
            else {
                int buttonId = std::floor(mouse_position.x/50)+1;
                switch(buttonId){
                    case create:
                    {
                        this->has_toolbar = false;
                        this->framebuffer = Image(this->empty);
                        // Adding the toolbar to the framebuffer
                        Image toolbar{Image()};
                        int status = toolbar.LoadPNG("../res/images/toolbar.png");
                        // Status check
                        if (status) {this->framebuffer.DrawImagePixels(toolbar, 0, 0, toolbar_top); has_toolbar = true;}
                        else std::cout << "There has been some error loading the toolbar" << std::endl;
                        break;
                    }
                    case save:
                    {
                        char tgaFileName[20] = "SavedDocument.tga";
                        if (this->framebuffer.SaveTGA(tgaFileName)) {std::cout << "TGA file sucessfully saved as " << tgaFileName << std:: endl; }
                        else {std::cout << "Error saving TGA file" << std::endl;}
                        break;
                    }
                    case black: {mouse_color = Color::BLACK; break;}
                    case red: {mouse_color = Color::RED; break;}
                    case green: {mouse_color = Color::GREEN; break;}
                    case navy_blue: {mouse_color = Color::BLUE; break;}
                    case yellow: {mouse_color = Color::YELLOW; break;}
                    case purple: {mouse_color = Color::PURPLE; break;}
                    case sky_blue: {mouse_color = Color::CYAN; break;}
                    case white: {mouse_color = Color::WHITE; break;}
                    default: {break;}
                }
            }
        }
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = default_free;
        // std::cout << "mouse state: "<< mouse_state<<std::endl;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if (currentSection==section3_3 && has_toolbar){
        std::pair<int, int> bound = toolbar_top ? std::make_pair(0, this->framebuffer.height-64) : std::make_pair(64, this->framebuffer.height);
        if (mouse_state == left_click && event.button == SDL_BUTTON_LEFT){
            // Avoid drawing on top of the toolbar
            if (!(mouse_position.y < bound.first || mouse_position.y > bound.second)) {
                this->framebuffer.DrawLineDDA(mouse_prev.x, mouse_prev.y, mouse_position.x, mouse_position.y, mouse_color);
                mouse_prev.set(mouse_position.x, mouse_position.y);
            }
        }
    }
    /*
    // draw hover effect
    if (mouse_position.y < bound.first || mouse_position.y > bound.second){
        int buttonId = std::floor(mouse_position.x/50)+1;
        if (buttonId<=10) {
            // Adding border lines
            int margin(5),iconSize(50), numButtons(10);
            this->framebuffer.DrawRect(margin+(buttonId-1)*iconSize, this->framebuffer.height-margin-iconSize, iconSize, iconSize, Color::WHITE);
        }
    }
    */
    
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
