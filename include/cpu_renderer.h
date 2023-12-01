#ifndef _CPU_RENDERER_H
#define _CPU_RENDERER_H

#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>

#include "color.h"
#include "vector.h"
#include "camera.h"
#include "matrix4x4.h"
#include "all_define.h"

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
		camera = new Camera(1.0f, 10.0f, ((float)width / (float)height), 2.0f);
		//camera->SetOrthographicMode(1.0f);
		camera->SetPerspectiveMode(60.0f);
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
		matModel.SetT(0.0f, 0.0f, 5.0f);
		matModel.SetR_Y((3.14f * 100 * currentTime) / 180);
		//matModel.SetS(2.0f, 1.0f, 1.0f);

		//matModel.Print("matModel");

		DrawTriangle(Vector3(-3, -3, 2), Vector3(2, 0, 2), Vector3(0, 3, 2),
			matModel,
			Color::blue);
	}

	//绘制三角形
	void DrawTriangle(
		Vector3 p1, Vector3 p2, Vector3 p3,//p123
		const Matrix4x4& matModel, //mat model
		const Color& color) //color
	{
		//p1.Print("p1_model");

		//1.坐标转换 mvp
		Vector4 p1_world = (matModel * Vector4(p1, 1.0f));
		Vector4 p2_world = (matModel * Vector4(p2, 1.0f));
		Vector4 p3_world = (matModel * Vector4(p3, 1.0f));

		//p1_world.Print("p1_world");

		Matrix4x4& matProject = camera->GetMatProject(); //暂时没有matView  相机固定远点 视角看z负方向

		Vector4 p1_project = matProject * p1_world;
		Vector4 p2_project = matProject * p2_world;
		Vector4 p3_project = matProject * p3_world;

		//p1_project.Print("p1_project");

		Vector4 p1_ndc = p1_project / p1_project.w;
		Vector4 p2_ndc = p2_project / p2_project.w;
		Vector4 p3_ndc = p3_project / p3_project.w;

		//p1_ndc.Print("p1_ndc");

		//2.视口变化
		// x [-1,1] -> [0,w-1] => +1、*(w-1)/2
		// y [-1,1] => [0,h-1] => +1、*(h-1)/2
		Vector2 p1_view = Vector2(
			(int)std::floor((p1_ndc.x + 1.0f) * ((w - 1) / 2.0f)),
			(int)std::floor((p1_ndc.y + 1.0f) * ((h - 1) / 2.0f)));
		Vector2 p2_view = Vector2(
			(int)std::floor((p2_ndc.x + 1.0f) * ((w - 1) / 2.0f)),
			(int)std::floor((p2_ndc.y + 1.0f) * ((h - 1) / 2.0f)));
		Vector2 p3_view = Vector2(
			(int)std::floor((p3_ndc.x + 1.0f) * ((w - 1) / 2.0f)),
			(int)std::floor((p3_ndc.y + 1.0f) * ((h - 1) / 2.0f)));

		//p1_view.Print("p1_view");
		//p2_view.Print("p2_view");
		//p3_view.Print("p3_view");

		//3.画线框
		//DrawLine(p1_view, p2_view, color);
		//DrawLine(p2_view, p3_view, color);
		//DrawLine(p1_view, p3_view, color);

		//3.光栅化填色
		Trapezoid trapezoid1, trapezoid2;
		int trapezoidCount = splitTrapezoids(p1_view, p2_view, p3_view,
			trapezoid1, trapezoid2);

		//std::cout << "trapezoidCount: " << trapezoidCount << "\n";

		if (trapezoidCount == 1) {
			//trapezoid1.Print("trapezoid1");

			drawTrapezoid(trapezoid1,
				color);
		}
		else if (trapezoidCount == 2) {
			//trapezoid1.Print("trapezoid1");
			//trapezoid2.Print("trapezoid2");

			drawTrapezoid(trapezoid1,
				color);
			drawTrapezoid(trapezoid2,
				color);
		}
	}

	//绘制直线
	void DrawLine(Vector2 start, Vector2 end, const Color& color) {
		bool clipResult = cohenSutherlandLineClip(start, end, Vector2(w - 1, h - 1));
		if (clipResult) {
			bresenmanDrawLine(start, end, color);
		}
	}

private:

	//设置像素颜色
	void setColor(int x, int y, const Color& color) const;
	void clearColor();

	//bresenman直线绘制
	void bresenmanDrawLine(const Vector2& start, const Vector2& end,
		const Color& color) const;

	//cohenSutherland直线裁剪
	bool cohenSutherlandLineClip(Vector2& v0, Vector2& v1, const Vector2& max) const;
	int cohenSutherlandLineComputeOutCode(const Vector2& v, const Vector2& max) const;

	//trapezoid划分
	int splitTrapezoids(const Vector2& v1, const Vector2& v2, const Vector2& v3,
		Trapezoid& trapezoid1, Trapezoid& trapezoid2) const;
	void drawTrapezoid(const Trapezoid& trapezoid,
		const Color& color) const;

	//scanline扫描线
	Scanline genScanline(const Trapezoid& trapezoid, int beginY) const;

};

#endif 
