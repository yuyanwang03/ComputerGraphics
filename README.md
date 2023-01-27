#  Lab description

Access the Github Repository of the Lab: [GitHub Repo Here](https://github.com/yuyanwang03/ComputerGraphics)

| Student name | Student NIA | Student email |
| --- | --- | --- |
| Ivan Hernández Gómez | 253616 | ivan.hernandez04@estudiant.upf.edu |
| Yuyan Wang | 254745 | yuyan.wang01@estudiant.upf.edu |

## Lab 1 *

Access the Lab1 branch code: [Lab1 branch](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab1)

Just after running the program, it will only react to keyboard types of "esc" (to exit the program), 'c' (to clear the windows) and numbers '1', '2', '3', '4' and '5'. Each of the numbers corresponds to an assignment section (section3.1, section3.2, section3.3, section3.4 and section3.5 respectively).

The program is set by default to initialize itself in section3_1.

Following we will explain what happens when the user types/presses the above mentioned keys.

### **Key number 1 (SDLK_1)**

The program will allow the user to draw straight lines inside the application windows. As the assignment demands, these lines will be generated with the DDA method. The lines will be drawn from 2 points given by the user using the mouse left-clicks.

If there has been loaded a toolbar, the user will not be allowed to draw on top of the toolbar.

### **Key number 2 (SDLK_2)**

Same definition as for the previous section but instead of using DDA, the program will use the bresenham method.

If there has been loaded a toolbar, the user will not be allowed to draw on top of the toolbar.

### **Key number 3 (SDLK_3)**

The program will allow the user draw circles on the windows; this can be done with 2 left-mouse-click on any part that belongs to the windows and that does not belong to the toolbar (if there is a toolbar). The former will refer to the circle center and the latter will be the radius of the circle.

Within this section, that is, every time the user types on keyboard number 3 and does not type on any other number from 1 and 5, the user can choose to draw a filled/unfilled circle. The program is set to be drawing unfilled circles; but the user can modify this by typing 'f' key if he/she wants the circle to be drawn filled or 'u' key if unfilled. 

The user has ilimited times to switch between these two modes.

### **Key number 4 (SDLK_4)**

The user will be able to paint while having the left mouse botton held on and moving the cursor on the application windows. This allows a more flexible drawing. 

Within this section, that is, every time the user types on keyboard number 4 and does not type on any other number from 1 and 5, the user can choose to create (paint) a toolbar to the windows. This can be done by typing 'b' if the user wants the toolbar to be situated at the bottom; or typing 't' if the user wants the toolbar on top of the windows.

The user will have ilimited times to click on 'b' or 't', that is, being able to freely change the position of the toolbar whenever he/she wants. But the only thing to take into consideration is that all previously painted primitives will be erased; in other words, after typing 'b' or 't', the user will start with a whole new background he/she will be able to paint onto.

 Following is a picture of the toolbar:

 ![Image](res/images/toolbar.png "toolbar.png")

>**Functionalities and effects of the toolbar**: 
>
> First thing to mention is that, when the toolbar is created, the user will not be able to paint over it, this will reduce the range of pixel that the user is allowed to work on.
>
> Each icon within the toolbar is considered as a button (only theorically), where each has a different functionality. To interact with them, the user has to left-click on them. Following are their descriptions (from left to right):
> * Cleans the windows (keeping the toolbar at its position), that is, to delete everything that has been drawn by the user so far
> * Save the drawing of the user into a TGA file named "SavedDocument.tga"
> * Change painting color to black
> * Change painting color to red
> * Change painting color to green
> * Change painting color to blue
> * Change painting color to yellow
> * Change painting color to purple
> * Change painting color to cyan
> * Change painting color to white

After loading the toolbar, the user can switch between previous sections (section3_1, section3_2, section3_3 and section3_4) to draw lines, circles or whatever. These switches will not erase/delete the loaded toolbar. If the user somehow wants the toolbar deleted, he/she can press the 'c' (clear) button.

### **Key number 5 (SDLK_5)**

After the user types 5, the windows will show an animation that consists of particles moving from the left side to the right side of the windows.

User can use the keys "Left" (Left arrow key) and "Right" (Right arrow key) to manage the direction of the particles. The program is set by default to have particles moving to the right, but the user can use the "Left" key to make them move towards the left. This switch of direction can be done as many times as the user wants.

Keep in mind that the toolbar will not exist in this section, and that the user is not allowed to draw on the windows. What's more, the key character 'c' will not clean the animation

### **Key character 'c'**

The program will react to this character type no matter the current section it is working on. This will erase all the modifications done by the user to the windows (drawing on it or loading a toolbar). Its functionality is similar to the first button of the toolbar but the difference is that this clean can be done in any time and that it erases the toolbar.

However, it will not clean the animation displayed if the user is in the section3_5.

### **Key "esc"**

The windows created when running the program will be closed; that is equivalent to the end of execution.

> #### Résumé
> List of keys that will work in this program: 'esc', 'c', 'f', 'u', 't', 'b', '1', '2', '3', '4', '5', "Left" (the Left arrow key (navigation keypad)) and "Right" (the Right arrow key (navigation keypad)).

### Remarks

If there are some file path issues in your computer (which does not happen in our case), please change the path inside the following function manually:

```c++
bool Application::LoadToolbar(void){
    Image toolbar{Image()};
    int status = toolbar.LoadPNG("../res/images/toolbar.png"); // Change file path if there's been some error loading it
    if (status) {this->framebuffer.DrawImagePixels(toolbar, 0, 0, this->toolbar_top); return true;}
    return false;
}
```

This code has not been tested for windows resize. Hence, it can have some problems with the animated version's particles position and the toolbar position if the user modifies the windows size. This is because their positions are not set to be relative/proportional to the windows size.

We are also aware that in section3_3, if the first left click is close enough to the toolbar and the second click is far enough, the user will draw on the toolbar. This is related to the fact that the toolbar has ben set to be a part of the framebuffer but not as another component of the application. But, even if the toolbar has been overwritten, the user can still access to the toolbar funcionalities by clicking at the location where the toolbar buttons are supposed to be. If it gets messy, we recomend the user to make use of the key 'c' and start over again.