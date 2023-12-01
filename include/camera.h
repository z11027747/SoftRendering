#ifndef _CAMERA_H
#define _CAMERA_H

#include "matrix4x4.h"

class Camera {
public:

	float near;
	float far;
	float aspect;
	float baseSize;

	Camera(float n, float f, float asp, float bs)
		:near(n), far(f), aspect(asp), baseSize(bs)
	{
		matProject.Identity();
	}

	//正交视角  
	//	立方体视锥转换到NDC空间
	void SetOrthographicMode(float size) {

		//平移
		//x:0
		//y:0
		//z:-(n+f)/2
		Matrix4x4 matT;
		matT.m11 = 1.0f; matT.m12 = 0.0f; matT.m13 = 0.0f; matT.m14 = 0.0f;
		matT.m21 = 0.0f; matT.m22 = 1.0f; matT.m23 = 0.0f; matT.m24 = 0.0f;
		matT.m31 = 0.0f; matT.m32 = 0.0f; matT.m33 = 1.0f; matT.m34 = -(near + far) / 2;
		matT.m41 = 0.0f; matT.m42 = 0.0f; matT.m43 = 0.0f; matT.m44 = 1.0f;

		//matT.Print("matT");

		float height = baseSize * size * 2;
		float width = height * aspect;

		//std::cout << "width: " << width << ", height: " << height << "\n";

		//缩放						    
		//x:[-w/2,w/2] -> [-1,1] => *2/w
		//y:[-h/2,h/2] -> [-1,1] => *2/h  
		//z:[n-(n+f)/2,f-(n+f)/2] -> [-(f-n)/2,(f-n)/2] -> [-1,1] => *2/(f-n)	  		
		Matrix4x4 matS;
		matS.m11 = 2 / width; matS.m12 = 0.0f; matS.m13 = 0.0f; matS.m14 = 0.0f;
		matS.m21 = 0.0f; matS.m22 = 2 / height; matS.m23 = 0.0f; matS.m24 = 0.0f;
		matS.m31 = 0.0f; matS.m32 = 0.0f; matS.m33 = 2 / (far - near); matS.m34 = 0.0f;
		matS.m41 = 0.0f; matS.m42 = 0.0f; matS.m43 = 0.0f; matS.m44 = 1.0f;

		//matS.Print("matS");

		matProject = matS * matT;

		matProject.Print("matProject (Orthographic)");
	}

	//透视视角
	//	先压扁，再走转换到正交视角的推导
	void SetPerspectiveMode(float fieldOfView) {

		//压扁，z不知道，xy可以根据相似三角形求出
		// (x,y,z) -> ( (n/z)*x, (n/z)*y, ? ) 

		//推算矩阵
		// n   0   0  0       x
		// 0   n   0  0   =>  y    =>  ( n*x, n*y, (a*z+b), z ) 
		// 0   0   a  b   =>  z					↓
		// 0   0   1  0       1      ( (n/z)*x, (n/z)*y, (a*z+b)/z, 1 ) 

		//求 a 和 b

		//z为n或f时，保持不变
		// (a*f+b)/f = f   =>  a = n+f
		// (a*n+b)/n = n 	   b = nf

		float rad = ((fieldOfView / 2.0f) * 3.14f) / 180.0f;
		float size = std::tan(rad) * near;

		//std::cout << "size: " << size;
		//std::cout << "\n";

		SetOrthographicMode(size);

		Matrix4x4 matPersp;
		matPersp.m11 = near; matPersp.m12 = 0.0f; matPersp.m13 = 0.0f; matPersp.m14 = 0.0f;
		matPersp.m21 = 0.0f; matPersp.m22 = near; matPersp.m23 = 0.0f; matPersp.m24 = 0.0f;
		matPersp.m31 = 0.0f; matPersp.m32 = 0.0f; matPersp.m33 = near + far; matPersp.m34 = near * far;
		matPersp.m41 = 0.0f; matPersp.m42 = 0.0f; matPersp.m43 = 1; matPersp.m44 = 0.0f;

		matProject = matProject * matPersp;

		matProject.Print("matProject (Perspective)");

	}

	Matrix4x4& GetMatProject() {
		return matProject;
	}

private:

	Matrix4x4 matProject;

};


#endif // !_CAMERA_H