
#ifndef entity_h // Or put #pragma once
#define entity_h

#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
#include "image.h"

class Entity
{
public:
    Matrix44 modelMatrix;
    Mesh entityMesh;
    
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
};

#endif /* entity_h */
