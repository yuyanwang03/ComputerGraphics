#  Labs Overview

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

Access the Lab 2 Branch in Github: [GitHub Lab 2](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab2)

## Lab 3

Access the Lab 3 Branch in Github: [GitHub Lab 3](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab3)

## Lab 4 *

Access the Lab 4 Branch in Github: [GitHub Lab 4](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab4)

Just after running the program, the user should see a windows displaying a transition of colors with blue at the left side and red at the right side. This is mainly the effect asked for section 3.1.a in this lab.

Following we will explain in what consists the User Interface.

### **Key character 'A'**

This will correspond to section 3.1 of the lab; it starts displaying the one that is 3.1.a's formula. The user will be able to switch between the drawing formulas by pressing key numbers [1,6].

### **Key character 'B'**

This will correspond to section 3.2 of the lab; it starts displaying the grayscale image. The user will be able to switch between the image effects by pressing key numbers [1,6].

### **Key character 'C'**

This will correspond to section 3.3 of the lab; the windows shows some transformation on the same image as in section 3.2. The program will provide 2 different animations, and these will be switched with every click on the key character 'C'.

### **Key character 'D'**

This will correspond to section 3.4 of the lab; the entity uses a given texture. The program displays on screen an entity that is rotating.

### **Key character '1'**

Changes (formula || image effect) when user is in section 3.1 (A) or 3.2 (B).

### **Key character '2'**

Changes (formula || image effect) when user is in section 3.1 (A) or 3.2 (B).

### **Key character '3'**

Changes (formula || image effect) when user is in section 3.1 (A) or 3.2 (B).

### **Key character '4'**

Changes (formula || image effect) when user is in section 3.1 (A) or 3.2 (B).

### **Key character '5'**

Changes (formula || image effect) when user is in section 3.1 (A) or 3.2 (B).

### **Key character '6'**

Changes (formula || image effect) when user is in section 3.1 (A) or 3.2 (B).

---

Following are the shared keys funcionalities with Lab 2 and Lab 3

### **Mouse wheel**

It allows the user to zoom in or to zoom out.

### **Mouse left-click + mouse move**

It will allow the user to orbit around the center point of the camera by changing the eye vector of the camera. However, it has a different implementation if the user is on section "change_view".

### **Key character 'V'**

After typing 'V', the user is now in the "change_view" section, which allows the user to modify the coordinates of the center point, that is, where the camera is looking at.

This can by done by a mouse left-click and the center point will change according to the mouse move that the user does while having the mouse hold on the click.

### **Key character 'Q'**

It sets the current working section to default, which is the same as the one when first running the program. 

User will not visually see any change after typing this key. Mainly, this key is used to exit the "change_view" section that is accessed when typing the character 'V'. Hence, if the user does a mouse left-click and moves the mouse, the program will be displaying an rotation (more precisely orbit) around the center point.

### **Key character 'N'**

The current working section will be switched to "change_near", and it is used to change the value of the near_plane by '0.15'(meters). This can be done by either typing '+' to increase the value or '-' to decrease it.

Note that the value of the near_plane will be at most the same as the value of the far_plane since it makes no sense to allow this value to surpass the far_plane value.

### **Key character 'F'**

The current working section will be switched to "change_far", and it is used to change the value of the far_plane by '0.15'(meters). This can be done by either typing '+' to increase the value or '-' to decrease it.

Note that the value of the far_plane will be at least the same as the value of the near_plane since it makes no sense to allow this value to be less than the near_plane.

### **Key character '+'**

If the camera has a perspective projection type, it will allow the user to increase the value of the camera fov by '5' degrees. Visually, it has similar effect as zooming out.

However, it will only modify the values for far_plane or near_plane if the user is in the corresponding working sections.

Remember, user can make use of the key 'Q' to get back to the default working section.

### **Key character '-'**

If the camera has a perspective projection type, it will allow the user to decrease the value of the camera fov by '5' degrees. Visually, it has similar effect as zooming in.

However, it will only modify the values for far_plane or near_plane if the user is in the corresponding working sections.

Remember, user can make use of the key 'Q' to get back to the default working section.

### **Key "esc"**

The windows created when running the program will be closed; that is equivalent to the end of execution.

> #### Résumé
> List of keys that will work in this program: 'esc', 'A', 'B', 'C', 'D', '1', '2', '3', '4', '5', '6', 'F', 'N', 'Q', 'V', '+', '-'. Mouse actions: "mouse wheel", "holding left-click and moving the mouse".

### Remarks

If there are some file path issues in your computer (which does not happen in our case), please change the path inside the following functions manually:

```c++
void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    // Load Quad shader
    shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
    this->shaderTexture.Load("images/fruits.png");
    quad.CreateQuad();
    
}

void Application::OnKeyPressed(SDL_KeyboardEvent event){
    ...
    case SDLK_d: // Section 3.4
        {
            ...
            // Load entity
            entity = Entity("../res/meshes/anna.obj");
            entity.modelMatrix.Rotate(160, Vector3(0, 1, 0));
            entity.SetShader("shaders/simple.vs", "shaders/simple.fs", "");
            entity.SetCamera(this->camera);
            entity.LoadTexture("../res/textures/anna_color_specular.tga");
            break;
        }
    ...
}
```

Mouse actions may not work with a very high probability, that is why the entity is set to be rotating by itself regardless of the user's action. The code has been mainly tested on Mac, and taking into consideration that we have had similar problems previously in other labs (rotation and orbit don't work in some specific cases with mac even though there are no coding errors), we decided to ignore this part.

## Lab 5

Access the Lab 5 Branch in Github: [GitHub Lab 5](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab5)