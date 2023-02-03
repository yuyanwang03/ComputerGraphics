
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

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c){
    // Iterate through the vertices of the mesh of the entity
    for (Vector3 vert : this->entityMesh.vertices){
        // Set a Vector4 from the vertice to make the matrix multiplication
        Vector4 tempWorldSpace(vert.x, vert.y, vert.z, 1.0);
        // Multiply the vertice by the modelMatrix to get the World Space coordinates
        tempWorldSpace = this->modelMatrix*tempWorldSpace;
        // Get the positions (x,y,z) to be drawn on screen
        Vector3 WorldSpace = tempWorldSpace.GetVector3();
        // To be modified
        // Check if one of the triangle projected vertices is outside the camera frustum
        bool negZ = 1 ? true : false;
        // Project the vector with the camera
        camera->ProjectVector(WorldSpace, negZ);
    }
    return;
}

