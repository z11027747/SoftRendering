#include "camera.h"

void Camera::SetOrthographicMode(float size) {

	//平移
	//x:0
	//y:0
	//z:-(n+f)/2
	Matrix4x4 matT;
	matT.m11 = 1.0f; matT.m12 = 0.0f; matT.m13 = 0.0f; matT.m14 = 0.0f;
	matT.m21 = 0.0f; matT.m22 = 1.0f; matT.m23 = 0.0f; matT.m24 = 0.0f;
	matT.m31 = 0.0f; matT.m32 = 0.0f; matT.m33 = 1.0f; matT.m34 = -(frustum.near + frustum.far) / 2;
	matT.m41 = 0.0f; matT.m42 = 0.0f; matT.m43 = 0.0f; matT.m44 = 1.0f;

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

	matProject = matS * matT;
}

void Camera::SetPerspectiveMode(float fieldOfView) {

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
}

bool Camera::IsInFrustum(Vector4<float>& p) const {

	bool invalidNear = (p.z <= frustum.near);
	bool invalidFar = (p.z >= frustum.far);

	float rad = Helper::Rad2Deg(frustum.fov / 2.0f);
	float hHalf = std::tan(rad) * frustum.near;
	float wHalf = hHalf * frustum.aspect;

	Vector3<float> nl = Vector3<float>(-frustum.near, 0.0f, -wHalf).Normalize();
	Vector3<float> nr = Vector3<float>(frustum.near, 0.0f, -wHalf).Normalize();
	Vector3<float> nt = Vector3<float>(0.0f, frustum.near, -hHalf).Normalize();
	Vector3<float> nd = Vector3<float>(0.0f, -frustum.near, -hHalf).Normalize();

	Vector3<float> p3 = Vector3<float>(p.x, p.y, p.z).Normalize();

	float dotnl = nl.Dot(p3);
	float dotnr = nr.Dot(p3);
	float dotnt = nt.Dot(p3);
	float dotnd = nd.Dot(p3);

	bool invalidLeft = dotnl > 0;
	bool invalidRight = dotnr > 0;
	bool invalidTop = dotnt > 0;
	bool invalidDown = dotnd > 0;

	return !(invalidNear || invalidFar || invalidLeft || invalidRight || invalidTop || invalidDown);
}