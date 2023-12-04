#ifndef _CAMERA_H
#define _CAMERA_H

#include "all_define.h"
#include "static_helper.h"
#include "matrix4x4.h"

//视锥
struct Frustum {
	float near;
	float far;
	float aspect;
	float fov;

	bool Contains(Vector4<float>& p) const {

		bool invalidNear = (p.z <= near);
		bool invalidFar = (p.z >= far);

		float rad = Helper::Rad2Deg(fov / 2.0f);
		float hHalf = std::tan(rad) * near;
		float wHalf = hHalf * aspect;

		Vector3<float> nl = Vector3<float>(-near, 0.0f, -wHalf).Normalize();
		Vector3<float> nr = Vector3<float>(near, 0.0f, -wHalf).Normalize();
		Vector3<float> nt = Vector3<float>(0.0f, near, -hHalf).Normalize();
		Vector3<float> nd = Vector3<float>(0.0f, -near, -hHalf).Normalize();

		Vector3<float> p3 = Vector3<float>(p.x, p.y, p.z).Normalize();

		float dotnl = nl.Dot(p3);
		float dotnr = nr.Dot(p3);
		float dotnt = nt.Dot(p3);
		float dotnd = nd.Dot(p3);

		bool invalidLeft = dotnl > 0;
		bool invalidRight = dotnr > 0;
		bool invalidTop = dotnt > 0;
		bool invalidDown = dotnd > 0;

		//std::cout << "Frustum Contains=============> \n";
		//std::cout << "hHalf: " << hHalf << ", wHalf: " << wHalf << "\n";
		//nl.Print("nl");
		//nr.Print("nr");
		//p3.Print("p3");
		//std::cout << "dotnl: " << dotnl << ", dotnr: " << dotnr << "\n";
		//std::cout << "invalidNear: " << invalidNear << ", invalidFar: " << invalidFar << "\n";
		//std::cout << "invalidLeft: " << invalidLeft << ", invalidRight: " << invalidRight << "\n";
		//std::cout << "invalidTop: " << invalidTop << ", invalidDown: " << invalidDown << "\n";

		return !(invalidNear || invalidFar || invalidLeft || invalidRight || invalidTop || invalidDown);

	}
};

//(0,0)点，看向z正方向
class Camera {
public:

	Frustum frustum;
	Matrix4x4 matProject;

	Camera() {
	}

	Camera(float n, float f, float asp)
	{
		frustum.near = n;
		frustum.far = f;
		frustum.aspect = asp;
		frustum.fov = 60.0f;
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
		matT.m31 = 0.0f; matT.m32 = 0.0f; matT.m33 = 1.0f; matT.m34 = -(frustum.near + frustum.far) / 2;
		matT.m41 = 0.0f; matT.m42 = 0.0f; matT.m43 = 0.0f; matT.m44 = 1.0f;
		//matT.Print("matT");

		float h = size * 2;
		float w = h * frustum.aspect;
		float n = frustum.near;
		float f = frustum.far;

		//缩放						    
		//x:[-w/2,w/2] -> [-1,1] => *2/w
		//y:[-h/2,h/2] -> [-1,1] => *2/h  
		//z:[n-(n+f)/2,f-(n+f)/2] -> [-(f-n)/2,(f-n)/2] -> [-1,1] => *2/(f-n)	  		
		Matrix4x4 matS;
		matS.m11 = 2 / w; matS.m12 = 0.0f; matS.m13 = 0.0f; matS.m14 = 0.0f;
		matS.m21 = 0.0f; matS.m22 = 2 / h; matS.m23 = 0.0f; matS.m24 = 0.0f;
		matS.m31 = 0.0f; matS.m32 = 0.0f; matS.m33 = 2 / (f - n); matS.m34 = 0.0f;
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

		frustum.fov = fieldOfView;

		float rad = Helper::Rad2Deg(frustum.fov / 2.0f);
		float size = std::tan(rad) * frustum.near;

		//std::cout << "size: " << size;
		//std::cout << "\n";

		SetOrthographicMode(size);

		float n = frustum.near;
		float f = frustum.far;

		Matrix4x4 matPersp;
		matPersp.m11 = n; matPersp.m12 = 0.0f; matPersp.m13 = 0.0f; matPersp.m14 = 0.0f;
		matPersp.m21 = 0.0f; matPersp.m22 = n; matPersp.m23 = 0.0f; matPersp.m24 = 0.0f;
		matPersp.m31 = 0.0f; matPersp.m32 = 0.0f; matPersp.m33 = n + f; matPersp.m34 = n * f;
		matPersp.m41 = 0.0f; matPersp.m42 = 0.0f; matPersp.m43 = 1; matPersp.m44 = 0.0f;

		matProject = matProject * matPersp;

		matProject.Print("matProject (Perspective)");

	}

};


#endif // !_CAMERA_H