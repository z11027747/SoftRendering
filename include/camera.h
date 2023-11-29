#ifndef _CAMERA_H
#define _CAMERA_H

//视锥，无远平面
struct Frustum {
	float near;
	float fov;
	float aspect;
};

class Camera {
public:
	Frustum frustum;

private:

};


#endif // !_CAMERA_H