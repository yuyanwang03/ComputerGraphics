
#ifndef entity_h // Or put #pragma once
#define entity_h

#include <limits>
#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
#include "image.h"
#include "texture.h"
#include "shader.h"

class Entity
{
public:
    enum class eRenderMode {
        POINTCLOUD,
        WIREFRAME,
        TRIANGLES,
        TRIANGLES_INTERPOLATED
    };
    
    Matrix44 modelMatrix;
    Mesh entityMesh;
    Color entityColor;
    eRenderMode renderMode;
    Texture* texture;
    Shader* shader;
    
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
    void LoadTexture(const char* path);
    void SetShader(const char* vsf, const char* psf, const char* macros);
    void Update(float seconds_elapsed);
    
    void Render(Image* framebuffer, Camera* camera, const Color& c);
    void Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
    void Render(void);
};

#endif /* entity_h */
