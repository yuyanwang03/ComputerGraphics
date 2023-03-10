#include "material.h"

Material::Material(){
    shader = nullptr;
    colorTexture = nullptr;
    normalTexture = nullptr;
    Ka = Vector3(); Kd = Vector3(); Ks = Vector3();
    shiness = 0.0;
}

void Material::Enable(){
    shader->Enable();
}

void Material::Disable(){
    shader->Disable();
}
