
#ifndef entity_h // Or put #pragma once
#define entity_h

#include "main/includes.h"
#include "framework.h"
#include "mesh.h"

class Entity
{
public:
    Matrix44 modelMatrix;
    Mesh entityMesh;
    
    // Constructors
    Entity();
    Entity(Matrix44 matx, Mesh msh);
    Entity(const Entity& e);
    Entity& operator = (const Entity& c); // Assign operator

    // Destructor
    ~Entity();

    void Render();
    
};

#endif /* entity_h */
