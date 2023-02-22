
#ifndef entity_h // Or put #pragma once
#define entity_h

#include <limits>
#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
#include "image.h"

class Entity
{
    enum class eRenderMode {
        POINTCLOUD,
        WIREFRAME,
        TRIANGLES,
        TRIANGLES_INTERPOLATED

    };
public:
    Matrix44 modelMatrix;
    Mesh entityMesh;
    eRenderMode renderMode;
    Image* texture;
    
    // Constructors
    Entity();
    Entity(Matrix44 matx, Mesh msh);
    Entity(Matrix44 matx);
    Entity(Mesh msh);
    Entity(const char* path);
    Entity(const Entity& e);
    Entity& operator = (const Entity& c); // Assign operator

    // Destructor
    ~Entity();

    void SetMatrix(Matrix44 matx);
    void SetMesh(Mesh msh);
    void Update(float seconds_elapsed);
    
    void Render(Image* framebuffer, Camera* camera, const Color& c);
    void Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
};

#endif /* entity_h */
