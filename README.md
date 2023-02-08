#  Labs Overview *

Access the Github Repository of the Lab: [GitHub Repo Here](https://github.com/yuyanwang03/ComputerGraphics)

| Student name | Student NIA | Student email |
| --- | --- | --- |
| Ivan Hernández Gómez | 253616 | ivan.hernandez04@estudiant.upf.edu |
| Yuyan Wang | 254745 | yuyan.wang01@estudiant.upf.edu |

**Each lab will have assigned a different branch. You can access to the code (and the description) of every lab in their corresponding branches.**

If you get lost somehow, just go back to the main branch, there will be direct links to access the code of each lab.

## Lab 1

Access the Lab 1 Branch in Github: [GitHub Lab 1](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab1)

## Lab 2

Access the Lab 2 Branch in Github: [GitHub Lab 2]


# To be completed...
Following we will explain what happens when the user types/presses the above mentioned keys.

### **Key number 4 (SDLK_4)**

After the user types 5, the windows will show an animation that consists of 

In order to simplify things, the user will not be allowed to interact with the displayed animation. If he/she wants to exit the animation, the user can press on the key character 'Q'.

### **Key character 'Q'**



### **Key character 'O'**

The program will react to this character type when 

### **Key character 'P'**

The program will react to this character type when 

### **Key character 'C'**

The program will react to this character type when 

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