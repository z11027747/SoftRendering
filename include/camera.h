#ifndef _CAMERA_H
#define _CAMERA_H

#include "all_define.h"
#include "static_helper.h"
#include "matrix4x4.h"

class Camera {
public:

	Camera() {
	}

	Camera(float n, float f, float asp)
	{
		frustum.near = n;
		frustum.far = f;
		frustum.aspect = asp;
		frustum.fov = 60.0f;
	}

	//p  没有v 相机在(0,0)点，看向z正方向
	Matrix4x4 matProject;

	//正交视角  
	//	立方体视锥转换到NDC空间
	void SetOrthographicMode(float size);

	//透视视角
	//	先压扁，再走转换到正交视角的推导
	void SetPerspectiveMode(float fieldOfView);

	//视锥
	Frustum frustum;

	bool IsInFrustum(Vector4<float>& p) const;
};


#endif // !_CAMERA_H