#include "camera.h"

#include "main/includes.h"
#include <iostream>

Camera::Camera()
{
	view_matrix.SetIdentity();
	SetOrthographic(-1,1,1,-1,-1,1);
}

Vector3 Camera::GetLocalVector(const Vector3& v)
{
	Matrix44 iV = view_matrix;
	if (iV.Inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.RotateVector(v);
	return result;
}

Vector3 Camera::ProjectVector(Vector3 pos, bool& negZ)
{
	Vector4 pos4 = Vector4(pos.x, pos.y, pos.z, 1.0);
	Vector4 result = viewprojection_matrix * pos4;
	negZ = result.z < 0;
	return result.GetVector3() / result.w;
}

void Camera::Rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.SetRotation(angle, axis);
	Vector3 new_front = R * (center - eye);
	center = eye + new_front;
	UpdateViewMatrix();
}

void Camera::Move(Vector3 delta)
{
	Vector3 localDelta = GetLocalVector(delta);
	eye = eye - localDelta;
	center = center - localDelta;
	UpdateViewMatrix();
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::SetPerspective(float fov, float aspect, float near_plane, float far_plane)
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	// Reset Matrix (Identity)
	view_matrix.SetIdentity();
    
    // View matrix rotation
    view_matrix.SetFrontAndOrthonormalize(this->eye-this->center);
    
    // Transpose the matrix. This line was obtained by comparing the code with GluLookAt code https://www.khronos.org/opengl/wiki/GluLookAt_code
    view_matrix.Transpose();
    
    // Get final view matrix by multiplyting to the translation matrix
    view_matrix.TranslateLocal(-eye.x, -eye.y, -eye.z);
    
    // Comment this line to create your own projection matrix!
    // SetExampleViewMatrix();
    
	UpdateViewProjectionMatrix();
}

// Create a projection matrix
void Camera::UpdateProjectionMatrix()
{
	// Reset Matrix (Identity)
	projection_matrix.SetIdentity();
	
	if (type == PERSPECTIVE) {
        projection_matrix.m[0] = 2/aspect;                                          // M[0][0]
        projection_matrix.m[5] = 1.0/(std::tan(fov*(PI/180)/2.0));                  // M[1][1]
        projection_matrix.m[10] = (far_plane+near_plane)/(far_plane-near_plane);    // M[2][2]
        projection_matrix.m[11] = -1;                                               // M[2][3]
        projection_matrix.m[14] = 2*(far_plane*near_plane)/(far_plane-near_plane);  // M[3][2]
        projection_matrix.m[15] = 0;                                                // M[3][3]
	} else if (type == ORTHOGRAPHIC) {
        projection_matrix.SetTranslation(-(right+left)/(right-left), -(top+bottom)/(top-bottom), -(far_plane+near_plane)/(far_plane-near_plane));
        projection_matrix.M[0][0] = 2/(right-left);             // m[0]
        projection_matrix.M[1][1] = 2/(top-bottom);             // m[5]
        projection_matrix.M[2][2] = -2/(far_plane-near_plane);  // m[10]
	}
    
    /*
    // Comment this line to create your own projection matrix!
    SetExampleProjectionMatrix();
    */

	UpdateViewProjectionMatrix();
}

void Camera::UpdateViewProjectionMatrix()
{
	viewprojection_matrix = view_matrix * projection_matrix;
}

Matrix44 Camera::GetViewProjectionMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	return viewprojection_matrix;
}

// The following methods have been created for testing.
// Do not modify them.

void Camera::SetExampleViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.m );
}

void Camera::SetExampleProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (type == PERSPECTIVE)
		gluPerspective(fov, aspect, near_plane, far_plane);
	else
		glOrtho(left,right,bottom,top,near_plane,far_plane);

	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.m );
	glMatrixMode(GL_MODELVIEW);
}

