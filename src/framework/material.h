#ifndef material_h
#define material_h

#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
#include "image.h"
#include "texture.h"
#include "shader.h"

typedef struct _Light{
    Vector3 position, Id, Is;
} sLight;

class Material
{
public:
    Shader* shader;
    Texture* colorTexture;
    Texture* normalTexture;
    Vector3 Ka, Kd, Ks;
    float shiness;
    
    Material();
    
    void Enable();
    void Disable();
};


#endif /* material_h */
