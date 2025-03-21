#ifndef CAMERA_H
#define CAMERA_H

#include "maths/maths.h"

class Camera
{
    public:
        Camera(vec3 position, vec3 direction, float fov, float speed, float directionSpeed);
        void rotate(quaternion q);

        mat4 getView() const { return view; }
        mat4 getProjection() const { return projection; }

        vec3 getPosition() const { return position; }
        vec3 getFront() const { return front; }
        vec3 getUp() const { return up; }
        vec3 getRight() const { return right; }

        float getSpeed() const { return speed; }
        float getDirectionSpeed() const { return directionSpeed; }

        void setPosition(vec3 position) { this->position = position; }
        void incPosition(vec3 position);

    private:

        mat4 view;
        mat4 projection;

        vec3 position;   
        vec3 front;
        vec3 up;    
        vec3 right;

        float fov;
        float speed;
        float directionSpeed;
};

#endif