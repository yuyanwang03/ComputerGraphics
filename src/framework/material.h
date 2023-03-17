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

typedef struct _UniformData{
    Matrix44 view_proj = Matrix44();
    Matrix44 model = Matrix44();
    Vector3 Ia = Vector3(0.0);
    sLight light;
} sUniformData;

class Material
{
public:
    Shader* shader;
    Texture* colorTexture;
    Texture* normalTexture;
    Vector3 Ka, Kd, Ks;
    float shiness;
    
    Material();
    Material(Texture* cTexture, Texture* nTexture, Vector3 a, Vector3 d, Vector3 s, float shn);
    Material(Vector3 a, Vector3 d, Vector3 s, float shn);
    Material& operator = (const Material& m); // Assign operator
    
    void LoadColorTexture(const char* path);
    void LoadNormalTexture(const char *path);
    void SetViewProjection(Camera* cam);
    void SetShader(const char* vsf, const char* psf, const char* macros);
    void Enable();
    void Enable(const sUniformData& uniformData);
    void Disable();
};


#endif /* material_h */
