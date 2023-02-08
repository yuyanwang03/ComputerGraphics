
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

Entity::Entity(const char* path){
    Mesh tempMsh{Mesh()};
    int status = tempMsh.LoadOBJ(path);
    if (status) {entityMesh = tempMsh; std::cout << "Mesh correctly set" << std::endl;}
}

Entity::Entity(const Entity& e){
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
}

Entity& Entity::operator = (const Entity& e)
{
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
    
    return *this;
}

void Entity::SetMatrix(Matrix44 matx) {this->modelMatrix = matx;}

void Entity::SetMesh(Mesh msh) {this->entityMesh = msh;}

Entity::~Entity(){
    // if (modelMatrix) delete modelMatrix;
    // if (entityMesh) delete entityMesh;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c){
    Vector3 tmp0, tmp1, tmp2;
    bool neg0, neg1, neg2;
    // Iterate through the vertices of the mesh of the entity (3 by 3)
    for (int i=0; i<this->entityMesh.vertices.size(); i=i+3){
         
        // Get the vertices of the world space (3D)
        tmp0 = this->modelMatrix*entityMesh.vertices[i];
        tmp1 = this->modelMatrix*entityMesh.vertices[i+1];
        tmp2 = this->modelMatrix*entityMesh.vertices[i+2];

        // Project world space (3D) to clip space (2D)
        tmp0 = camera->ProjectVector(tmp0, neg0);
        tmp1 = camera->ProjectVector(tmp1, neg1);
        tmp2 = camera->ProjectVector(tmp2, neg2);
        
        // If any of the triangle projected vertices is outside the camera, don't draw the triangle
        if (neg0 || neg1 || neg2) {continue;}
        
        // Convert clip space (2D vector in range [-1, 1]) to screenspace (2D vector in range [0, width] || [0, height])
        // Essentially, that is to convert the clipspace range from [-1,1] to [0,1] and multiply the values by the width and height respectively
        tmp0.Set((tmp0.x/2+0.5)*framebuffer->width, (tmp0.y/2+0.5)*framebuffer->height, tmp0.z);
        tmp1.Set((tmp1.x/2+0.5)*framebuffer->width, (tmp1.y/2+0.5)*framebuffer->height, tmp1.z);
        tmp2.Set((tmp2.x/2+0.5)*framebuffer->width, (tmp2.y/2+0.5)*framebuffer->height, tmp2.z);
        
        // Draw the lines of the triangle on the framebuffer
        framebuffer->DrawLineBresenham(tmp0.x, tmp0.y, tmp1.x, tmp1.y, c);
        framebuffer->DrawLineBresenham(tmp0.x, tmp0.y, tmp2.x, tmp2.y, c);
        framebuffer->DrawLineBresenham(tmp1.x, tmp1.y, tmp2.x, tmp2.y, c);
    }
    return;
}

void Entity::Update(float seconds_elapsed){
    modelMatrix.Rotate(seconds_elapsed*PI/180*6, Vector3(0,1,0));
    return;
}
