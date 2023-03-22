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

## Lab 4

Access the Lab 4 Branch in Github: [GitHub Lab 4](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab4)

## Lab 5 *

Access the Lab 5 Branch in Github: [GitHub Lab 5](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab5)

Just after running the program, the user should see a windows displaying a entity in gray rotating at the center of the windows. The default number of lights is 1 and its color is white (white light) and the illumination type is Phong. The displayed entity does not use any texture by default.

Following we will explain in what consists the User Interface.

### **Key character 'C'**

The program will toggle the usage of the color texture. That is, the entity will be displayed with some skintone (with color texture) or in gray (without color texture). This will be achieved by modifying the vectors Kd and Ka.

### **Key character 'S'**

The program will toggle the usage of the specular texture. In other words, the program will modify the vector Ks and apply its changes to the rendered mesh. This is related with the alpha component of each of the rgb colors.

### **Key character 'N'**

The program will toggle the usage of the normal texture. The user should see bumps on the skin when applying the normal texture; otherwise, he/she should see a smoother skin. 

### **Key character 'G'**

The illumination type will be switched to Gouraud. Keep in mind that the program do not allow gouraud toggle the usage of any texture.

### **Key character 'P'**

The illumination type will be switched to Phong.

### **Key character '1'**

The program will use 1 light. This light is mainly white.

### **Key character '2'**

The program will use 2 lights. The second added light has a warm tone (close to orange). This is related with section 3.6, using the multipass method to compute the final color of each texture.

---

Following are the shared keys funcionalities with Lab 2, Lab 3 and Lab 4

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

### **Key character 'F'**

The current working section will be switching from "change_near" to "change_far" or viceversa, and it is used to change the value of the (far_plane || near_plane) by '0.15'(meters). This can be done by either typing '+' to increase the value or '-' to decrease it.

Note that the value of the (far_plane || near_plane) will be (at least || at most) the same as the value of the (near_plane || far_plane) since it makes no sense to allow this value to be less than the near_plane.

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
> List of keys that will work in this program: 'esc', 'C', 'S', 'N', 'G', 'P', '1', '2', 'F', 'Q', 'V', '+', '-'. Mouse actions: "mouse wheel", "holding left-click and moving the mouse".

### Remarks

If there are some file path issues in your computer (which does not happen in our case), please change the path inside the following functions manually:

```c++
void Application::Init(void)
{
    ...
    // Set Phong as default illumination type
    entity.SetShader("shaders/phongTexture.vs", "shaders/phongTexture.fs", "");
    ...
    // Load textures
    entity.LoadColorTexture("../res/textures/lee_color_specular.tga");
    entity.LoadNormalTexture("../res/textures/lee_normal.tga");
}

void Application::OnKeyPressed(SDL_KeyboardEvent event){
    ...
    case SDLK_g: { entity.SetShader("shaders/gouraud.vs", "shaders/gouraud.fs", ""); break; }
    case SDLK_p: { entity.SetShader("shaders/phongTexture.vs", "shaders/phongTexture.fs", ""); break; }
    ...
}
```

Mouse actions may not work with a very high probability, that is why the entity is set to be rotating by itself regardless of the user's action. The code has been mainly tested on Mac, and taking into consideration that we have had similar problems previously in other labs (rotation and orbit don't work in some specific cases with mac even though there are no coding errors), we decided to ignore this part.
