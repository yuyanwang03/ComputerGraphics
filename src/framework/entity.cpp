
#include "entity.h"

Entity::Entity(){
    modelMatrix = Matrix44();
    entityMesh = Mesh();
    entityColor.Random();
    texture = nullptr;
    shader = nullptr;
    camera = nullptr;
}

Entity::Entity(Matrix44 matx, Mesh msh){
    modelMatrix = matx;
    entityMesh = msh;
    entityColor.Random();
    texture = nullptr;
    shader = nullptr;
}

Entity::Entity(Matrix44 matx) {modelMatrix = matx; entityMesh = Mesh(); entityColor.Random(); texture=nullptr; shader = nullptr; camera = nullptr;}

Entity::Entity(Mesh msh) {entityMesh = msh; modelMatrix = Matrix44(); entityColor.Random(); texture=nullptr; shader = nullptr; camera = nullptr;}

Entity::Entity(const char* path){
    entityColor = Color(147, 112, 219);
    texture = nullptr;
    shader = nullptr;
    camera = nullptr;
    Mesh tempMsh{Mesh()};
    int status = tempMsh.LoadOBJ(path);
    if (status) {entityMesh = tempMsh; std::cout << "Mesh correctly set" << std::endl;}
}

Entity::Entity(const Entity& e){
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
    texture = e.texture;
    entityColor = e.entityColor;
    shader = e.shader;
    camera = e.camera;
}

Entity& Entity::operator = (const Entity& e)
{
    modelMatrix = e.modelMatrix;
    entityMesh = e.entityMesh;
    texture = e.texture;
    entityColor = e.entityColor;
    shader = e.shader;
    camera = e.camera;
    return *this;
}

void Entity::SetMatrix(Matrix44 matx) {this->modelMatrix = matx;}

void Entity::SetMesh(Mesh msh) {this->entityMesh = msh;}

void Entity::LoadTexture(const char* path){
    texture = new Texture();
    bool status = this->texture->Load(path);
    if (status) {std::cout << "Texture loaded" << std::endl;}
}

void Entity::SetShader(const char* vsf, const char* psf, const char* macros){
    shader = Shader::Get(vsf, psf, macros);
    // if (shader!=nullptr) {std::cout << "Shader loaded" << std::endl;}
}

void Entity::SetCamera(Camera* cam){
    this->camera = cam;
}

Entity::~Entity(){
    // if (modelMatrix) delete modelMatrix;
    // if (entityMesh) delete entityMesh;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c){
    Vector3 tmp0, tmp1, tmp2;
    bool neg0, neg1, neg2;
    // Iterate through the vertices of the mesh of the entity (3 by 3)
    for (int i=0; i<this->entityMesh.GetVertices().size(); i=i+3){
         
        // Get the vertices of the world space (3D)
        tmp0 = this->modelMatrix*entityMesh.GetVertices()[i];
        tmp1 = this->modelMatrix*entityMesh.GetVertices()[i+1];
        tmp2 = this->modelMatrix*entityMesh.GetVertices()[i+2];

        // Project world space (3D) to clip space (2D)
        tmp0 = camera->ProjectVector(tmp0, neg0);
        tmp1 = camera->ProjectVector(tmp1, neg1);
        tmp2 = camera->ProjectVector(tmp2, neg2);
        
        // If any of the triangle projected vertices is outside the camera, don't draw the triangle
        if (neg0 || neg1 || neg2) {continue;}
        
        // Convert clip space (2D vector in range [-1, 1]) to screenspace (2D vector in range [0, width-1] || [0, height-1])
        // Essentially, that is to convert the clipspace range from [-1,1] to [0,1] and multiply the values by the width and height respectively
        tmp0.Set((tmp0.x/2+0.5)*(framebuffer->width-1), (tmp0.y/2+0.5)*(framebuffer->height-1), tmp0.z);
        tmp1.Set((tmp1.x/2+0.5)*(framebuffer->width-1), (tmp1.y/2+0.5)*(framebuffer->height-1), tmp1.z);
        tmp2.Set((tmp2.x/2+0.5)*(framebuffer->width-1), (tmp2.y/2+0.5)*(framebuffer->height-1), tmp2.z);
        
        if (renderMode==eRenderMode::POINTCLOUD) {
            framebuffer->SetPixelSafe(tmp0.x, tmp0.y, c);
            framebuffer->SetPixelSafe(tmp1.x, tmp1.y, c);
            framebuffer->SetPixelSafe(tmp2.x, tmp2.y, c);
        }
        else if (renderMode==eRenderMode::WIREFRAME){
            // Draw the lines of the triangle on the framebuffer
            framebuffer->DrawLineBresenham(tmp0.x, tmp0.y, tmp1.x, tmp1.y, c);
            framebuffer->DrawLineBresenham(tmp0.x, tmp0.y, tmp2.x, tmp2.y, c);
            framebuffer->DrawLineBresenham(tmp1.x, tmp1.y, tmp2.x, tmp2.y, c);
        }
        else if (renderMode==eRenderMode::TRIANGLES) {framebuffer->DrawTriangle(Vector2(tmp0.x, tmp0.y), Vector2(tmp1.x, tmp1.y), Vector2(tmp2.x, tmp2.y), c);}
        else if (renderMode==eRenderMode::TRIANGLES_INTERPOLATED) {framebuffer->DrawTriangleInterpolated(tmp0, tmp1, tmp2, Color::RED, Color::BLUE, Color::GREEN);}
        
    }
}

void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer){
    Vector3 tmp0, tmp1, tmp2;
    bool neg0, neg1, neg2;
    sTriangleInfo triangle;
    triangle.colors.push_back(Color::RED); triangle.colors.push_back(Color::BLUE); triangle.colors.push_back(Color::GREEN);
    triangle.texture = this->texture;
    // Fill the zBuffer with high distance
    zBuffer->Fill((float)INT_MAX);
    
    // Iterate through the vertices of the mesh of the entity (3 by 3)
    for (int i=0; i<this->entityMesh.GetVertices().size(); i=i+3){
        // Get the vertices of the world space (3D)
        tmp0 = this->modelMatrix*entityMesh.GetVertices()[i];
        tmp1 = this->modelMatrix*entityMesh.GetVertices()[i+1];
        tmp2 = this->modelMatrix*entityMesh.GetVertices()[i+2];

        // Project world space (3D) to clip space (2D)
        tmp0 = camera->ProjectVector(tmp0, neg0);
        tmp1 = camera->ProjectVector(tmp1, neg1);
        tmp2 = camera->ProjectVector(tmp2, neg2);
        
        // If any of the triangle projected vertices is outside the camera, don't draw the triangle
        if (neg0 || neg1 || neg2) {continue;}
        
        // Convert clip space (2D vector in range [-1, 1]) to screenspace (2D vector in range [0, width-1] || [0, height-1])
        // Essentially, that is to convert the clipspace range from [-1,1] to [0,1] and multiply the values by the width and height respectively
        tmp0.Set((tmp0.x/2+0.5)*(framebuffer->width-1), (tmp0.y/2+0.5)*(framebuffer->height-1), tmp0.z);
        tmp1.Set((tmp1.x/2+0.5)*(framebuffer->width-1), (tmp1.y/2+0.5)*(framebuffer->height-1), tmp1.z);
        tmp2.Set((tmp2.x/2+0.5)*(framebuffer->width-1), (tmp2.y/2+0.5)*(framebuffer->height-1), tmp2.z);
        
        // Clear previous data
        triangle.points.clear();
        triangle.uvs.clear();
        // Add new triangle data
        triangle.points.push_back(tmp0); triangle.points.push_back(tmp1); triangle.points.push_back(tmp2);
        triangle.uvs.push_back(entityMesh.GetUVs()[i]); triangle.uvs.push_back(entityMesh.GetUVs()[i+1]); triangle.uvs.push_back(entityMesh.GetUVs()[i+2]);
        
        framebuffer->DrawTriangleInterpolated(triangle, zBuffer);
    }
}

void Entity::Render(void){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    shader->Enable();
    shader->SetTexture("u_texture", this->texture);
    shader->SetMatrix44("u_model", modelMatrix);
    shader->SetMatrix44("u_viewprojection", camera->view_matrix);
    entityMesh.Render();
    glDisable(GL_DEPTH_TEST);
    shader->Disable();
}

void Entity::Update(float seconds_elapsed){
    modelMatrix.Rotate(seconds_elapsed*PI/180*10, Vector3(0,1,0));
}
