
#ifndef entity_h // Or put #pragma once
#define entity_h

#include <limits>
#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
#include "image.h"
#include "texture.h"
#include "shader.h"
#include "material.h"

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
    Material entityMaterial;
    /*
    Texture* texture;
    Shader* shader;*/
    Camera* camera;
    
    // Constructors
    Entity();
    Entity(Matrix44 matx, Mesh msh, Material mat);
    Entity(Matrix44 matx);
    Entity(Mesh msh);
    Entity(Material mat);
    Entity(const char* path);
    Entity(const Entity& e);
    Entity& operator = (const Entity& c); // Assign operator

    // Destructor
    ~Entity();

    void SetMatrix(Matrix44 matx);
    void SetMesh(Mesh msh);
    void SetMaterial(Material mat);
    
    void LoadColorTexture(const char* path);
    void LoadNormalTexture(const char* path);
    void SetShader(const char* vsf, const char* psf, const char* macros);
    void SetCamera(Camera* cam);
    
    void Render(void);
    void Render(sUniformData uniformData);
};

#endif /* entity_h */
