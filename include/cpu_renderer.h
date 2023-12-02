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
	//视口
	Viewport viewport;

	//颜色缓冲
	unsigned char* colorAttachment;
	unsigned int colorLen;

	//相机
	Camera camera;

	CPURenderer(int w, int h)
	{
		viewport = Viewport(0, 0, w, h);

		colorLen = w * h * 3;
		colorAttachment = new unsigned char[colorLen];

		camera = Camera(1.0f, 10.0f, ((float)w / (float)h), 2.0f);
		//camera.SetOrthographicMode(1.0f);
		camera.SetPerspectiveMode(60.0f);
	}

	~CPURenderer() {
		delete[] colorAttachment;
	}

	void Render(float currentTime, float deltaTime) {

		//std::cout << "===========================> RenderBegin \n";

		clearColor();

		//DrawLine(Vector2<int>(200, 200), Vector2<int>(400, 400), Color::red);
		//DrawLine(Vector2<int>(400, 400), Vector2<int>(600, 200), Color::red);
		//DrawLine(Vector2<int>(200, 200), Vector2<int>(600, 200), Color::red);

		Matrix4x4 matModel;
		matModel.Identity();
		matModel.SetT(0.0f, 0.0f, 5.0f);
		matModel.SetR_Y(Mathf::Rad2Deg(currentTime * 100.0f));
		//matModel.SetS(2.0f, 1.0f, 1.0f);
		//matModel.Print("matModel");

		//DrawTriangle(Vector3<float>(-3, -3, 2), Vector3<float>(2, 0, 2), Vector3<float>(0, 3, 2),
		//	matModel,
		//	Color::blue);

		std::vector<Vertex> vertices = {
			Vertex(Vector3<float>(-3,3,2), Vector3<char>(255,0,0)),
			Vertex(Vector3<float>(-3,3,2), Vector3<char>(255,0,0)),
			Vertex(Vector3<float>(-3,3,2), Vector3<char>(255,0,0))
		};
		DrawArray(vertices,
			matModel);
	}

	void DrawArray(std::vector<Vertex> vertices,
		const Matrix4x4& matModel)//mat model
	{
		//mv变化
		for (auto& vertex : vertices) {
			vertex.position = matModel * vertex.position;
		}

		//剔除 TODO

		//p变化
		Matrix4x4& matProject = camera.GetMatProject();
		for (auto& vertex : vertices) {
			vertex.position = matProject * vertex.position;
		}

		//透视除法
		for (auto& vertex : vertices) {
			vertex.position.x /= vertex.position.w;
			vertex.position.y /= vertex.position.w;
			vertex.position.w = 1.0f;
		}

		//视口变化


	}

	//绘制三角形
	void DrawTriangle(Vector3<float> p1, Vector3<float> p2, Vector3<float> p3,//p123
		const Matrix4x4& matModel, //mat model
		const Color& color) //color
	{
		//p1.Print("p1_model");

		//1.坐标转换 
		Vector4<float> p1_world = (matModel * Vector4<float>(p1, 1.0f));
		Vector4<float> p2_world = (matModel * Vector4<float>(p2, 1.0f));
		Vector4<float> p3_world = (matModel * Vector4<float>(p3, 1.0f));

		//p1_world.Print("p1_world");

		//暂时没有matView  相机固定远点 视角看z负方向

		Matrix4x4& matProject = camera.GetMatProject();

		Vector4<float> p1_project = matProject * p1_world;
		Vector4<float> p2_project = matProject * p2_world;
		Vector4<float> p3_project = matProject * p3_world;

		//p1_project.Print("p1_project");

		Vector4<float> p1_ndc = p1_project / p1_project.w;
		Vector4<float> p2_ndc = p2_project / p2_project.w;
		Vector4<float> p3_ndc = p3_project / p3_project.w;

		//p1_ndc.Print("p1_ndc");

		//2.视口变化
		// x [-1,1] -> [0,w-1] => +1、*(w-1)/2
		// y [-1,1] => [0,h-1] => +1、*(h-1)/2
		Vector2<int> p1_view = Vector2<int>(
			(int)std::floor((p1_ndc.x + 1.0f) * ((viewport.w - 1) / 2.0f)),
			(int)std::floor((p1_ndc.y + 1.0f) * ((viewport.h - 1) / 2.0f)));
		Vector2<int> p2_view = Vector2<int>(
			(int)std::floor((p2_ndc.x + 1.0f) * ((viewport.w - 1) / 2.0f)),
			(int)std::floor((p2_ndc.y + 1.0f) * ((viewport.h - 1) / 2.0f)));
		Vector2<int> p3_view = Vector2<int>(
			(int)std::floor((p3_ndc.x + 1.0f) * ((viewport.w - 1) / 2.0f)),
			(int)std::floor((p3_ndc.y + 1.0f) * ((viewport.h - 1) / 2.0f)));

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
	void DrawLine(Vector2<int> start, Vector2<int> end,
		const Color& color)
	{
		bool clipResult = cohenSutherlandLineClip(start, end, Vector2<int>(viewport.w - 1, viewport.h - 1));
		if (clipResult) {
			bresenmanDrawLine(start, end, color);
		}
	}

private:

	//设置像素颜色
	void clearColor();
	void setColor(int x, int y, const Color& color) const;

	//bresenman直线绘制
	void bresenmanDrawLine(const Vector2<int>& start, const Vector2<int>& end,
		const Color& color) const;

	//cohenSutherland直线裁剪
	bool cohenSutherlandLineClip(Vector2<int>& v0, Vector2<int>& v1, const Vector2<int>& max) const;
	int cohenSutherlandLineComputeOutCode(const Vector2<int>& v, const Vector2<int>& max) const;

	//trapezoid划分
	int splitTrapezoids(const Vector2<int>& v1, const Vector2<int>& v2, const Vector2<int>& v3,
		Trapezoid& trapezoid1, Trapezoid& trapezoid2) const;
	void drawTrapezoid(const Trapezoid& trapezoid,
		const Color& color) const;

	//scanline扫描线
	Scanline genScanline(const Trapezoid& trapezoid, int beginY) const;

};

#endif 
