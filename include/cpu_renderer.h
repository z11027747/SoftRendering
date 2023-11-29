#ifndef _CPU_RENDERER_H
#define _CPU_RENDERER_H

#include <iostream>
#include <cmath>
#include <math.h>

#include "color.h"
#include "vector.h"
#include "camera.h"
#include "matrix4x4.h"

class CPURenderer {

public:
	unsigned int w;
	unsigned int h;
	unsigned int channels;

	//颜色缓冲
	unsigned char* colorAttachment;

	//相机
	Camera* camera;

	CPURenderer(unsigned int width, unsigned int height)
		: w(width), h(height), channels(3)
	{
		colorAttachment = new unsigned char[width * height * channels];
		camera = new Camera(0, 10, width, height);
	}

	~CPURenderer() {
		delete[] colorAttachment;
		delete camera;
	}

	void Render(float currentTime, float deltaTime) {

		//std::cout << "===========================> RenderBegin \n";

		clearColor();

		Matrix4x4 matModel;
		matModel.Identity();
		matModel.SetT(std::sin(currentTime) * 10, std::cos(currentTime) * 10, 0.0f);
		matModel.SetR_Y((3.14f * 100 * currentTime) / 180);

		//matModel.Print("matModel");

		DrawTriangle(Vector3(-20, -20, 5), Vector3(20, -20, 5), Vector3(0, 20, 5),
			matModel,
			Color::red);
	}

	//绘制三角形
	void DrawTriangle(
		Vector3 p1, Vector3 p2, Vector3 p3,//p123
		const Matrix4x4& matModel, //mat model
		const Color& color) //color
	{
		//1.坐标转换
		//	暂时没有matView  相机固定远点 视角看z负方向
		Matrix4x4& matProject = camera->GetMatProject();

		Vector3 p1_world = (matModel * p1);
		Vector3 p2_world = (matModel * p2);
		Vector3 p3_world = (matModel * p3);

		//std::cout << "p1_world: " << p1_world.x << ", " << p1_world.y << ", " << p1_world.z << "\n";

		Vector3 p1_ndc = matProject * p1_world;
		Vector3 p2_ndc = matProject * p2_world;
		Vector3 p3_ndc = matProject * p3_world;

		//std::cout << "p1_ndc: " << p1_ndc.x << ", " << p1_ndc.y << ", " << p1_ndc.z << "\n";

		//2.视口变化
		// x [-1,1] -> [0,w] => +1、*w/2
		// y [-1,1] => [0,h] => +1、*h/2
		Vector2 p1_view = Vector2(
			(int)std::round((p1_ndc.x + 1.0f) * ((w - 1) / 2.0f)),
			(int)std::round((p1_ndc.y + 1.0f) * ((h - 1) / 2.0f)));
		Vector2 p2_view = Vector2(
			(int)std::round((p2_ndc.x + 1.0f) * ((w - 1) / 2.0f)),
			(int)std::round((p2_ndc.y + 1.0f) * ((h - 1) / 2.0f)));
		Vector2 p3_view = Vector2(
			(int)std::round((p3_ndc.x + 1.0f) * ((w - 1) / 2.0f)),
			(int)std::round((p3_ndc.y + 1.0f) * ((h - 1) / 2.0f)));

		// std::cout << "p1_view: " << p1_view.x << "," << p1_view.y << "\n";

		//3.画线
		DrawLine(p1_view, p2_view, color);
		DrawLine(p2_view, p3_view, color);
		DrawLine(p1_view, p3_view, color);
	}

	//绘制直线
	void DrawLine(Vector2 start, Vector2 end, const Color& color) {
		bool clipResult = cohenSutherlandLineClip(start, end, Vector2(w - 1, h - 1));
		if (clipResult) {
			bresenmanDrawLine(start, end, color);
		}
	}

private:

	void clearColor();

	//设置像素颜色
	void setColor(int x, int y, const Color& color) const;

	//bresenman直线绘制
	void bresenmanDrawLine(const Vector2& start, const Vector2& end, const Color& color) const;

	//cohenSutherland直线裁剪
	bool cohenSutherlandLineClip(Vector2& v0, Vector2& v1, const Vector2& max);
	int cohenSutherlandLineComputeOutCode(const Vector2& v, const Vector2& max) const;

};

#endif 
