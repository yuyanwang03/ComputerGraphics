#include "material.h"

Material::Material(){
    shader = nullptr;
    colorTexture = nullptr;
    normalTexture = nullptr;
    Ka = Vector3(); Kd = Vector3(); Ks = Vector3();
    shiness = 0.0;
}

Material::Material(Texture* cTexture, Texture* nTexture, Vector3 a, Vector3 d, Vector3 s, float shn){
    shader = nullptr;
    colorTexture = cTexture;
    normalTexture = nTexture;
    Ka = a; Kd = d; Ks = s;
    shiness = shn;
}

Material::Material(Vector3 a, Vector3 d, Vector3 s, float shn){
    shader = nullptr;
    colorTexture = nullptr;
    normalTexture = nullptr;
    Ka = a; Kd = d; Ks = s;
    shiness = shn;
}

Material& Material::operator = (const Material& m){
    shader = m.shader;
    colorTexture = m.colorTexture;
    normalTexture = m.normalTexture;
    Ka = m.Ka; Kd = m.Kd; Ks = m.Ks;
    shiness = m.shiness;
}

void Material::LoadColorTexture(const char *path){
    colorTexture = new Texture();
    bool status = this->colorTexture->Load(path);
    if (status) {std::cout << "Color texture loaded" << std::endl;}
    else{std::cout << "Color texture NOT loaded" << std::endl;}
}

void Material::LoadNormalTexture(const char *path){
    normalTexture = new Texture();
    bool status = this->normalTexture->Load(path);
    if (status) {std::cout << "Normal texture loaded" << std::endl;}
}

void Material::SetShader(const char* vsf, const char* psf, const char* macros){
    shader = Shader::Get(vsf, psf, macros);
}

void Material::SetViewProjection(Camera* cam){
    shader->SetMatrix44("u_viewprojection", cam->viewprojection_matrix);
}

void Material::Enable(){
    shader->Enable();
}

void Material::Disable(){
    shader->Disable();
}
