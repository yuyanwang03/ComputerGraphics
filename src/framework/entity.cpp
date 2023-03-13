
#include "entity.h"

Entity::Entity(){
    modelMatrix = Matrix44();
    entityMesh = Mesh();
    // entityMaterial = Material();
}

Entity::Entity(Matrix44 matx, Mesh msh, Material mat){
    modelMatrix = matx;
    entityMesh = msh;
    entityMaterial = mat;
}

Entity::Entity(Matrix44 matx) {modelMatrix = matx; entityMesh = Mesh(); /*entityMaterial = Material();*/}

Entity::Entity(Mesh msh) {entityMesh = msh; modelMatrix = Matrix44(); /*entityMaterial = Material();*/}

Entity::Entity(Material mat) {entityMaterial = mat; modelMatrix = Matrix44(); entityMesh = Mesh();}

Entity::Entity(const char* path){
    modelMatrix = Matrix44();
    Mesh tempMsh{Mesh()};
    int status = tempMsh.LoadOBJ(path);
    if (status) {entityMesh = tempMsh; std::cout << "Mesh correctly set" << std::endl;}
    // entityMaterial = Material();
}

Entity::Entity(const Entity& e){
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
    // entityMaterial = e.entityMaterial;
}

Entity& Entity::operator = (const Entity& e)
{
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
    // entityMaterial = e.entityMaterial;
    return *this;
}

void Entity::SetMatrix(Matrix44 matx) {this->modelMatrix = matx;}

void Entity::SetMesh(Mesh msh) {this->entityMesh = msh;}

void Entity::SetMaterial(Material mat) {this->entityMaterial = mat;}

void Entity::LoadColorTexture(const char* path){
    entityMaterial.LoadColorTexture(path);
}

void Entity::SetShader(const char* vsf, const char* psf, const char* macros){
    entityMaterial.SetShader(vsf, psf, macros);
    // if (shader!=nullptr) {std::cout << "Shader loaded" << std::endl;}
}

void Entity::SetCamera(Camera* cam){
    this->camera = cam;
}

Entity::~Entity(){
    // if (modelMatrix) delete modelMatrix;
    // if (entityMesh) delete entityMesh;
}

void Entity::Render(void){
    // camera->view_matrix.Print();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    entityMaterial.Enable();
    entityMaterial.shader->SetTexture("u_texture", entityMaterial.colorTexture);
    entityMaterial.shader->SetMatrix44("u_model", modelMatrix);
    entityMaterial.shader->SetMatrix44("u_viewprojection", camera->viewprojection_matrix);
    entityMesh.Render();
    glDisable(GL_DEPTH_TEST);
    entityMaterial.Disable();
}

void Entity::Render(sUniformData uniformData){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    uniformData.model = this->modelMatrix;
    entityMaterial.Enable(uniformData);
    entityMesh.Render();
    glDisable(GL_DEPTH_TEST);
    entityMaterial.Disable();
}
