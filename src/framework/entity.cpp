
#include "entity.h"

Entity::Entity(){
    modelMatrix = Matrix44();
    entityMesh = Mesh();
}

Entity::Entity(Matrix44 matx, Mesh msh){
    modelMatrix = matx;
    entityMesh = msh;
}

Entity::Entity(Matrix44 matx) {modelMatrix = matx;}

Entity::Entity(Mesh msh) {entityMesh = msh;}

Entity::Entity(const Entity& e){
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
}

void Entity::SetMatrix(Matrix44 matx) {this->modelMatrix = matx;}

void Entity::SetMesh(Mesh msh) {this->entityMesh = msh;}

Entity::~Entity(){
    // if (modelMatrix) delete modelMatrix;
    // if (entityMesh) delete entityMesh;
}
