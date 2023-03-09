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

Just after running the program, the user should see a one-color-painted entity situated at the middle of the windows. These are the default setting of the program: projection type is the perspective; the entity render mode is "TRIANGLES" and the boolean useZbuffer is set to be false (meaning that the program is not using zBuffer yet to render the entity).

This lab is very similar to Lab 2 so there are shared keys that do the same for both labs.

Following we will explain in what consists the User Interface.

### **Key character 'D'**

It sets the program to the "POINTCLOUD" render mode, which basically displays only the vertices (dots) of the mesh on screen. It will set the boolean useZbuffer to false and hence the program will paint all the points regardless of if they are nearer or farer to the camera.

### **Key character 'W'**

It sets the program to the "WIREFRAME" render mode, which basically displays only the lines of the triangles of the mesh on screen; that is, the same display mode as in previous lab. It will set the boolean useZbuffer to false and hence the program will paint all the edges regardless of if they are nearer or farer to the camera.

### **Key character 'C'**

It will set the boolean useZbuffer to false and the program will switch the render mode between "TRIANGLES" and "TRIANGLES_INTERPOLATED". The former render mode paints mesh's triangles with only one color and the latter paints the triangles with the barycentric interpolation method having the vertices' colors being red, blue and green. This character mainly displays the tasks asked for sections 3.1 and 3.2 of the lab.

### **Key character 'Z'**

The program will display a mesh using a zBuffer (that is, taking into consideration the distances to the camera) and barycentric interpolation; it does the job demanded for section 3.3. If observed with detail, the entity should slightly be different than the one without using zBuffer.

### **Key character 'T'**

The program will load a texture to the entity and using the zBuffer (section 3.4). It will use the UV coordinates to get the correct colors.

If the texture is not correctly loaded, the program will proceed to render the mesh in the same way as when typing key "Z". 

### **Key character 'R'**

The program will randomly change the color of the rendered mesh. Bear in mind that there's a chance that the rendered mesh becomes black (same as the background color) and so it seems to disappear. In this case, just press the 'R' key again.

The user can change the colors of the meshes whenever he/she wants and for unlimited number of times.

---

Following are the shared keys funcionalities with Lab 2

### **Mouse wheel**

It allows the user to zoom in or to zoom out.

### **Mouse left-click + mouse move**

It will allow the user to orbit around the center point of the camera by changing the eye vector of the camera. However, it has a different implementation if the user is on section "change_view".

### **Key character 'O'**

The program will change the camera projection type to Orthographic. It will reset the values of the camera; so it is good to use it if the user somehow gets lost and does not find the meshes.

Keep in mind that, because of the properties of the orthographic view, the use of the *mouse wheel* may not have any change to the displayed windows.

### **Key character 'P'**

The program will change the camera projection type to Perspective. It will reset the values of the camera; so it is good to use it if the user somehow gets lost and does not find the meshes.

### **Key character 'V'**

After typing 'V', the user is now in the "change_view" section, which allows the user to modify the coordinates of the center point, that is, where the camera is looking at.s

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
> List of keys that will work in this program: 'esc', 'C', 'D', 'F', 'N', 'O', 'P', 'Q', 'R', 'T', 'V', 'W', 'Z', '+', '-'. Mouse actions: "mouse wheel", "holding left-click and moving the mouse".

### Remarks

If there are some file path issues in your computer (which does not happen in our case), please change the path inside the following function manually:

```c++
void Application::Init(void){
    std::cout << "Initiating app..." << std::endl;
    camera->LookAt(Vector3(0,0.4,1.5), Vector3(0,0,0), Vector3::UP);
    camera->SetPerspective(50, window_width/window_height, 0.01, 100);
    entity = Entity("../res/meshes/anna.obj");
}

void Application::OnKeyPressed(SDL_KeyboardEvent event){
    ...
    case SDLK_t:
    {
        this->useZbuffer = true;
        this->entity.LoadTexture("../res/textures/anna_color_specular.tga");
        break;
    }
    ...
}
```

We have tested the program under various circumstances and situations and it works without having strange behaviours, so quality can be assured. However, we are aware that our program may have some little bugs (that we could not find), but it fulfills perfectly the basic needs.

We recommend the user to work with the perspective view since it resemble more the real world. It will better approximate the views.

The change of values for near_plane and far_plane can not allways have visual effects since they only modify the size of the frustrum. If the meshes are still inside the frustrum we shall see them without errors. In the same manner, if the user modifies the center of the camera up to some point, he/she will not see any mesh rendered on the screen or will observe that the meshes are partially rendered. 

## Lab 5

Access the Lab 5 Branch in Github: [GitHub Lab 5](https://github.com/yuyanwang03/ComputerGraphics/tree/Lab5)