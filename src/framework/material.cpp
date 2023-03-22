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

void Material::Enable(const sUniformData& uniformData){
    this->shader->Enable();
    this->shader->SetTexture("u_colorTexture", this->colorTexture);
    this->shader->SetTexture("u_normalTexture", this->normalTexture);
    this->shader->SetMatrix44("u_viewprojection", uniformData.view_proj);
    this->shader->SetMatrix44("u_model", uniformData.model);
    this->shader->SetVector3("u_eye", uniformData.cameraEye);
    this->shader->SetVector3("u_lightPosition", uniformData.lights[0].position);
    this->shader->SetVector3("u_Ia", uniformData.Ia);
    this->shader->SetVector3("u_Id", uniformData.lights[0].Id);
    this->shader->SetVector3("u_Is", uniformData.lights[0].Is);
    this->shader->SetVector3("u_Ka", Ka);
    this->shader->SetVector3("u_Kd", Kd);
    this->shader->SetVector3("u_Ks", Ks);
    this->shader->SetFloat("u_alfa", shiness);
}

// For each light
void Material::Enable(const sUniformData& uniformData, int light_index){
    this->shader->Enable();
    this->shader->SetTexture("u_colorTexture", this->colorTexture);
    this->shader->SetTexture("u_normalTexture", this->normalTexture);
    this->shader->SetMatrix44("u_viewprojection", uniformData.view_proj);
    this->shader->SetMatrix44("u_model", uniformData.model);
    this->shader->SetVector3("u_eye", uniformData.cameraEye);
    this->shader->SetVector3("u_lightPosition", uniformData.lights[light_index].position);
    this->shader->SetVector3("u_Ia", uniformData.Ia);
    this->shader->SetVector3("u_Id", uniformData.lights[light_index].Id);
    this->shader->SetVector3("u_Is", uniformData.lights[light_index].Is);
    this->shader->SetVector3("u_Ka", Ka);
    this->shader->SetVector3("u_Kd", Kd);
    this->shader->SetVector3("u_Ks", Ks);
    this->shader->SetFloat("u_alfa", shiness);
    this->shader->SetFloat("u_addAmbient", light_index==0 ? 1.0 : 0.0);
}

void Material::Disable(){
    shader->Disable();
}
