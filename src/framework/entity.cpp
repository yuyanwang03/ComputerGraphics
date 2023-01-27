
#include "entity.h"

Entity::Entity(){
    modelMatrix = Matrix44();
    entityMesh = Mesh();
}

Entity::Entity(Matrix44 matx, Mesh msh){
    modelMatrix = matx;
    entityMesh = msh;
}

Entity::Entity(const Entity& e){
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
}

Entity::~Entity(){
    // if (modelMatrix) delete modelMatrix;
    // if (entityMesh) delete entityMesh;
}
