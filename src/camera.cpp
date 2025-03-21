#include "camera.h"

Camera::Camera(vec3 position, vec3 front, float fov, float speed, float directionSpeed)
: position(position), front(front), fov(fov), speed(speed), directionSpeed(directionSpeed)
{
    up = vec3(0.0f, 1.0f, 0.0f);
	right = front.cross(up);

    view = mat4::lookAt(position, position + front, up);
}

void Camera::rotate(quaternion rotation)
{
	front = quaternion::rotateVec3(rotation, front);
	right = quaternion::rotateVec3(rotation, right);
	up = quaternion::rotateVec3(rotation, up);
	view = mat4::lookAt(position, position + front, up);
}

void Camera::incPosition(vec3 position)
{ 
    this->position += position; 
    view = mat4::lookAt(this->position, this->position + front, up);
}