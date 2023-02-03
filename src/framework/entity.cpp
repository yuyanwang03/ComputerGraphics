
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
    Vector4 temp0, temp1, temp2;
    Vector3 tmp0, tmp1, tmp2;
    bool neg0, neg1, neg2;
    // Iterate through the vertices of the mesh of the entity (3 by 3)
    for (int i=0; i<this->entityMesh.vertices.size(); i=i+3){
        // Set Vector4 from the vertices to make the matrix multiplication
        temp0.Set(entityMesh.vertices[i].x, entityMesh.vertices[i].y, entityMesh.vertices[i].z, 1.0);
        temp1.Set(entityMesh.vertices[i+1].x, entityMesh.vertices[i+1].y, entityMesh.vertices[i+1].z, 1.0);
        temp2.Set(entityMesh.vertices[i+2].x, entityMesh.vertices[i+2].y, entityMesh.vertices[i+2].z, 1.0);
        // Multiply the vertices by the modelMatrix to get the World Space coordinates
        temp0 = this->modelMatrix*temp0;
        temp1 = this->modelMatrix*temp1;
        temp2 = this->modelMatrix*temp2;
        // Get the positions (x,y,z) to be drawn on screen
        tmp0 = temp0.GetVector3();
        tmp1 = temp1.GetVector3();
        tmp2 = temp2.GetVector3();
        
        camera->ProjectVector(tmp0, neg0);
        camera->ProjectVector(tmp1, neg1);
        camera->ProjectVector(tmp2, neg2);
        
        if (neg0 || neg1 || neg2) {continue;}
        
        tmp0.Set((tmp0.x/2+0.5)*framebuffer->width, (tmp0.y/2+0.5)*framebuffer->height, tmp0.z);
        tmp1.Set((tmp1.x/2+0.5)*framebuffer->width, (tmp1.y/2+0.5)*framebuffer->height, tmp1.z);
        tmp2.Set((tmp2.x/2+0.5)*framebuffer->width, (tmp2.y/2+0.5)*framebuffer->height, tmp2.z);
        
        framebuffer->DrawLineBresenham(tmp0.x, tmp0.y, tmp1.x, tmp1.y, c);
        framebuffer->DrawLineBresenham(tmp0.x, tmp0.y, tmp2.x, tmp2.y, c);
        framebuffer->DrawLineBresenham(tmp1.x, tmp1.y, tmp2.x, tmp2.y, c);
    }
    return;
}

