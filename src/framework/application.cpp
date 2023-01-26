#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height) : animation(this)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->has_toolbar = false; // Application initializes without having a toolbar
    this->toolbar_top = false; // Setting default value for the toolbar_top
    this->currentSection = section3_1; // Application initializes itself in section3.1, that is, allowing user to draw straight lines with DDA method
    this->mouse_prev.set(-1,-1); // Set the mouse_prev position to (-1,-1) to indicate that it has no record stored in it
    this->mouse_state = default_free; // Set the mouse_state to default_free, indicating that the user is not clicking on the mouse
    this->mouse_color = Color(255, 255, 255); // Set default mouse color to white
    
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
    if (currentSection == section3_5) {this->animation.Update(seconds_elapsed);}
}

// Loads the toolbar to the framebuffer, return true if success, false otherwise
bool Application::LoadToolbar(void){
    Image toolbar{Image()};
    int status = toolbar.LoadPNG("../res/images/toolbar.png");
    // Status check
    if (status) {this->framebuffer.DrawImagePixels(toolbar, 0, 0, this->toolbar_top); return true;}
    return false;
}

// Gets the ID of the button and proceeds a change (if the button exists) through the application
void Application::ProceedToolbarFunction(int ButtonID){
    switch(ButtonID){
        case create: // Create a new file; that is, setting fill the current framebuffer black
        {
            this->has_toolbar = false;
            this->framebuffer.Fill(Color::BLACK);
            // Adding the toolbar to the framebuffer
            has_toolbar = this->LoadToolbar();
            break;
        }
        case save: // Save the file (drawing) with name "SavedDocument.tga" locally
        {
            char tgaFileName[20] = "SavedDocument.tga";
            if (this->framebuffer.SaveTGA(tgaFileName)) {std::cout << "TGA file sucessfully saved as " << tgaFileName << std:: endl; }
            else {std::cout << "Error saving TGA file" << std::endl;}
            break;
        }
        // Change current mouse_color
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
    return;
}

void Application::SetToDefault(void){
    this->framebuffer.Fill(Color::BLACK);
    mouse_prev.set(-1, -1);
    this->has_toolbar = false;
    return;
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
        case SDLK_ESCAPE: {exit(0); break;} // ESC key, kill the app
        case SDLK_1: // key 1, execute section 3.1
        {
            std::cout << "Section 3.1" << std::endl;
            if (currentSection == section3_5) {this->SetToDefault();}
            currentSection = section3_1;
            mouse_prev.set(-1, -1);
            break;
        }
        case SDLK_2: // key 2, execute section 3.2
        {
            std::cout << "Section 3.2" << std::endl;
            if (currentSection == section3_5) {this->SetToDefault();}
            currentSection = section3_2;
            mouse_prev.set(-1, -1);
            break;
        }
        case SDLK_3: // key 3, execute section 3.3
        {
            std::cout << "Section 3.3" << std::endl;
            if (currentSection == section3_5) {this->SetToDefault();}
            currentSection = section3_3;
            mouse_prev.set(-1, -1);
            break;
        }
        case SDLK_4: // key 4, execute section 3.4
        {
            std::cout << "Section 3.4" << std::endl;
            if (currentSection == section3_5) {this->SetToDefault();}
            currentSection = section3_4;
            mouse_prev.set(-1, -1);
            break;
        }
        case SDLK_5: // key 5, execute section 3.5
        {
            std::cout << "Section 3.5" << std::endl;
            currentSection = section3_5;
            this->framebuffer.Fill(Color::BLACK);
            animation.Init(300);
            animation.Render();
            break;
        }
        case SDLK_t: // t meaning top
        {
            if (this->currentSection == section3_4) {
                this->toolbar_top = true;
                // Clean the frame buffer
                this->framebuffer.Fill(Color::BLACK);
                // Adding the toolbar to the framebuffer
                has_toolbar = this->LoadToolbar();
            }
            break;
        }
        case SDLK_b: // b meaning bottom
        {
            if (this->currentSection == section3_4) {
                this->toolbar_top = false;
                // Clean the frame buffer
                this->framebuffer.Fill(Color::BLACK);
                // Adding the toolbar to the framebuffer
                has_toolbar = this->LoadToolbar();
            }
            break;
        }
        case SDLK_c: {this->SetToDefault();}
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = left_click;
        switch(currentSection)
        {
            case section3_1:
            {
                // Prevent the user from drawing on the toolbar if the program has one && proceed the toolbar corresponding funcionality if pressed on its icons
                if (has_toolbar) {
                    std::pair<int, int> bound = toolbar_top ? std::make_pair(0, this->framebuffer.height-64) : std::make_pair(64, this->framebuffer.height);
                    if ((mouse_position.y < bound.first || mouse_position.y > bound.second)) {
                        int buttonId = std::floor(mouse_position.x/50)+1;
                        this->ProceedToolbarFunction(buttonId);
                        break;
                    }
                }
                if (mouse_prev.x == -1 || mouse_prev.y == -1) {mouse_prev.set(mouse_position.x, mouse_position.y);}
                else{
                    this->framebuffer.DrawLineDDA(mouse_prev.x, mouse_prev.y, mouse_position.x, mouse_position.y, mouse_color);
                    mouse_prev.set(-1, -1);
                }
                break;
            }
            case section3_2:
            {
                // Prevent the user from drawing on the toolbar if the program has one && proceed the toolbar corresponding funcionality if pressed on its icons
                if (has_toolbar) {
                    std::pair<int, int> bound = toolbar_top ? std::make_pair(0, this->framebuffer.height-64) : std::make_pair(64, this->framebuffer.height);
                    if ((mouse_position.y < bound.first || mouse_position.y > bound.second)) {
                        int buttonId = std::floor(mouse_position.x/50)+1;
                        this->ProceedToolbarFunction(buttonId);
                        break;
                    }
                }
                if (mouse_prev.x == -1 || mouse_prev.y == -1) {mouse_prev.set(mouse_position.x, mouse_position.y);}
                else{
                    this->framebuffer.DrawLineBresenham(mouse_prev.x, mouse_prev.y, mouse_position.x, mouse_position.y, mouse_color);
                    mouse_prev.set(-1, -1);
                }
                break;
            }
            case section3_3:
            {
                // To fill
                break;
            }
            case section3_4:
            {
                if (has_toolbar) {
                    std::pair<int, int> bound = toolbar_top ? std::make_pair(0, this->framebuffer.height-64) : std::make_pair(64, this->framebuffer.height);
                    if (!(mouse_position.y < bound.first || mouse_position.y > bound.second)) {mouse_prev.set(mouse_position.x, mouse_position.y);}
                    else {
                        int buttonId = std::floor(mouse_position.x/50)+1;
                        this->ProceedToolbarFunction(buttonId);
                    }
                } else {mouse_prev.set(mouse_position.x, mouse_position.y);}
            }
            case section3_5:
            {
                break;
            }
        }
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        mouse_state = default_free;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if (mouse_state == left_click && event.button == SDL_BUTTON_LEFT){
        if (currentSection==section3_4){
            // Prevent the user to draw on the toolbar
            if (has_toolbar){
                std::pair<int, int> bound = toolbar_top ? std::make_pair(0, this->framebuffer.height-64) : std::make_pair(64, this->framebuffer.height);
                if (!(mouse_position.y < bound.first || mouse_position.y > bound.second)) {
                    this->framebuffer.DrawLineDDA(mouse_prev.x, mouse_prev.y, mouse_position.x, mouse_position.y, mouse_color);
                    mouse_prev.set(mouse_position.x, mouse_position.y);
                }
            } else{
                this->framebuffer.DrawLineDDA(mouse_prev.x, mouse_prev.y, mouse_position.x, mouse_position.y, mouse_color);
                mouse_prev.set(mouse_position.x, mouse_position.y);
            }
        }
    }
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
