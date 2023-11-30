#ifndef _CAMERA_H
#define _CAMERA_H


#include "matrix4x4.h"

class Camera {
public:

	enum EMode
	{
		Orthographic,
	};

	Camera(float n, float f, float s, float asp)
		:near(n), far(f), size(s), aspect(asp)
	{
		SetProjectMode(EMode::Orthographic);
	}

	void SetProjectMode(EMode mode) {
		if (mode == EMode::Orthographic) {

			//正交视角  
			//	立方体视锥转换到NDC空间

			//平移
			//x:0
			//y:0
			//z:-(n+f)/2
			Matrix4x4 matT;
			matT.m11 = 1.0f; matT.m12 = 0.0f; matT.m13 = 0.0f; matT.m14 = 0.0f;
			matT.m21 = 0.0f; matT.m22 = 1.0f; matT.m23 = 0.0f; matT.m24 = 0.0f;
			matT.m31 = 0.0f; matT.m32 = 0.0f; matT.m33 = 1.0f; matT.m34 = -(near + far) / 2;
			matT.m41 = 0.0f; matT.m42 = 0.0f; matT.m43 = 0.0f; matT.m44 = 1.0f;

			matT.Print("matT");

			float height = size * 2;
			float width = height * aspect;

			std::cout << "width: " << width << ", height: " << height << "\n";

			//缩放						    
			//x:[-w/2,w/2] -> [-1,1] => *2/w
			//y:[-h/2,h/2] -> [-1,1] => *2/h  
			//z:[n-(n+f)/2,f-(n+f)/2] -> [-(f-n)/2,(f-n)/2] -> [-1,1] => *2/(f-n)	  		
			Matrix4x4 matS;
			matS.m11 = 2 / width; matS.m12 = 0.0f; matS.m13 = 0.0f; matS.m14 = 0.0f;
			matS.m21 = 0.0f; matS.m22 = 2 / height; matS.m23 = 0.0f; matS.m24 = 0.0f;
			matS.m31 = 0.0f; matS.m32 = 0.0f; matS.m33 = 2 / (far - near); matS.m34 = 0.0f;
			matS.m41 = 0.0f; matS.m42 = 0.0f; matS.m43 = 0.0f; matS.m44 = 1.0f;

			matS.Print("matS");

			matProject = matS * matT;

			matProject.Print("matProject");
		}
	}

	Matrix4x4& GetMatProject() {
		return matProject;
	}

private:

	float near;
	float far;
	float size;
	float aspect;

	EMode mode;
	Matrix4x4 matProject;

};


#endif // !_CAMERA_H