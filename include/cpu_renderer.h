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
#include "static_helper.h"

class CPURenderer {

public:
	//视口
	Viewport viewport;

	//颜色缓冲
	unsigned char* colorAttachment;
	unsigned int colorLen;

	//贴图
	Texture texture;

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

		texture = Helper::LoadImage("ColoredNumberAtlas.png");
	}

	~CPURenderer() {
		delete[] colorAttachment;
	}

	void Render(float currentTime, float deltaTime) {

		//std::cout << "===========================> RenderBegin \n";

		clearColor();

		Matrix4x4 matModel;
		matModel.Identity();
		matModel.SetT(0.0f, 0.0f, 5.0f);
		matModel.SetR_Y(Helper::Rad2Deg(currentTime * 100.0f));
		//matModel.Print("matModel");

		std::vector<Vertex> vertices = {
			// position           ---           color           ---           uv
			//1
			Vertex(Vector3<float>(-2, -2, 2), Color(255, 0, 0), Vector2<float>(0.0f, 0.0f)),
			Vertex(Vector3<float>(-2, 2, 2), Color(0, 255, 0), Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(2, -2, 2), Color(0, 255, 0), Vector2<float>(1.0f, 0.0f)),
			//2
			Vertex(Vector3<float>(-2, 2, 2), Color(0, 255, 0), Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(2, 2, 2), Color(0, 0, 255), Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(2, -2, 2), Color(0, 255, 0), Vector2<float>(1.0f, 0.0f))
		};
		DrawTriangle(vertices,
			matModel);
	}

	void DrawTriangle(std::vector<Vertex> vertices,
		const Matrix4x4& matModel)//mat model
	{
		//m变化
		for (auto& vertex : vertices) {
			vertex.position = matModel * vertex.position;
			//vertex.position.Print("vertex.position===>Model");
		}

		//剔除 TODO

		//p变化
		Matrix4x4& matProject = camera.GetMatProject();
		for (auto& vertex : vertices) {
			vertex.position = matProject * vertex.position;
			//vertex.position.Print("vertex.position===>Project");
		}

		//还原z
		for (auto& vertex : vertices) {
			vertex.position.z = vertex.position.w;
			//vertex.position.Print("vertex.position===>RevertZ");
		}

		//透视除法
		for (auto& vertex : vertices) {
			vertex.position.x /= vertex.position.w;
			vertex.position.y /= vertex.position.w;
			vertex.position.w = 1.0f;
			//vertex.position.Print("vertex.position===>PerspectiveDivice");
		}

		//视口变化
		for (auto& vertex : vertices) {
			vertex.position.x = (vertex.position.x + 1.0f) * ((viewport.w - 1) / 2.0f);
			vertex.position.y = (vertex.position.y + 1.0f) * ((viewport.h - 1) / 2.0f);
			//vertex.position.Print("vertex.position===>ViewPort");
		}

		//光栅化三角形
		for (int i = 0; i < vertices.size(); i += 3) {
			Trapezoid trapezoids[2];
			int trapezoidCount = splitTrapezoids(vertices[i + 0], vertices[i + 1], vertices[i + 2],
				trapezoids);
			for (int i = 0; i < trapezoidCount; i++) {
				drawTrapezoid(trapezoids[i]);
			}
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
	int splitTrapezoids(const Vertex& v1, const Vertex& v2, const Vertex& v3,
		Trapezoid* trapezoids) const;
	void drawTrapezoid(Trapezoid& trapezoid) const;

	//scanline扫描线
	Scanline genScanline(const Trapezoid& trapezoid, float y) const;

};

#endif 
