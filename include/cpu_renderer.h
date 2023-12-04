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
	int colorLen;

	//深度缓冲
	float* depthAttachment;
	int depthLen;

	//贴图
	Texture texture;

	//相机
	Camera camera;

	CPURenderer(int w, int h)
	{
		viewport = Viewport(0, 0, w, h);

		colorLen = colorLen = w * h * 3;
		colorAttachment = new unsigned char[colorLen];

		depthLen = depthLen = w * h;
		depthAttachment = new float[depthLen];

		camera = Camera(1.0f, 10.0f, ((float)w / (float)h), 2.0f);
		//camera.SetOrthographicMode(1.0f);
		camera.SetPerspectiveMode(30.0f);

		texture = Helper::LoadImage("ColoredNumberAtlas.png");
	}

	~CPURenderer() {
		delete[] colorAttachment;
		delete[] depthAttachment;
	}

	void Render(float currentTime, float deltaTime) {

		clearColor();
		clearDepth();

		//std::cout << "===========================> RenderBegin \n";

		Matrix4x4 matModel;
		matModel.Identity();
		matModel.SetT(0.0f, 0.0f, 5.0f);
		matModel.SetR_Y(Helper::Rad2Deg(currentTime * 100.0f));
		//matModel.Print("matModel");

		//Cube
		std::vector<Vertex> vertices = {
			// position                  ---                  color      ---        uv
			Vertex(Vector3<float>(-0.5f, -0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,  -0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,   0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f,   0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(-0.5f,  0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(-0.5f, -0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 0.0f)),

			Vertex(Vector3<float>(-0.5f, -0.5f, 0.5f),  Color(0, 255, 0),  Vector2<float>(0.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,  -0.5f, 0.5f),  Color(0, 255, 0),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,   0.5f, 0.5f),  Color(0, 255, 0),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f,   0.5f, 0.5f),  Color(0, 255, 0),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(-0.5f,  0.5f, 0.5f),  Color(0, 255, 0),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(-0.5f, -0.5f, 0.5f),  Color(0, 255, 0),  Vector2<float>(0.0f, 0.0f)),

			Vertex(Vector3<float>(-0.5f,  0.5f,  0.5f), Color(0, 0, 255),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(-0.5f,  0.5f, -0.5f), Color(0, 0, 255),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(-0.5f, -0.5f, -0.5f), Color(0, 0, 255),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(-0.5f, -0.5f, -0.5f), Color(0, 0, 255),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(-0.5f, -0.5f,  0.5f), Color(0, 0, 255),  Vector2<float>(0.0f, 0.0f)),
			Vertex(Vector3<float>(-0.5f,  0.5f,  0.5f), Color(0, 0, 255),  Vector2<float>(1.0f, 0.0f)),

			Vertex(Vector3<float>(0.5f,  0.5f,  0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,  0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f, -0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f, -0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f, -0.5f,  0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,  0.5f,  0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 0.0f)),

			Vertex(Vector3<float>(-0.5f, -0.5f, -0.5f),  Color(0, 255, 0),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f,  -0.5f, -0.5f),  Color(0, 255, 0),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f,  -0.5f,  0.5f),  Color(0, 255, 0),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,  -0.5f,  0.5f),  Color(0, 255, 0),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(-0.5f, -0.5f,  0.5f),  Color(0, 255, 0),  Vector2<float>(0.0f, 0.0f)),
			Vertex(Vector3<float>(-0.5f, -0.5f, -0.5f),  Color(0, 255, 0),  Vector2<float>(0.0f, 1.0f)),

			Vertex(Vector3<float>(-0.5f, 0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f,  0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 1.0f)),
			Vertex(Vector3<float>(0.5f,  0.5f,  0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(0.5f,  0.5f,  0.5f),  Color(255, 0, 0),  Vector2<float>(1.0f, 0.0f)),
			Vertex(Vector3<float>(-0.5f, 0.5f,  0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 0.0f)),
			Vertex(Vector3<float>(-0.5f, 0.5f, -0.5f),  Color(255, 0, 0),  Vector2<float>(0.0f, 1.0f))
		};

		for (int i = 0; i < vertices.size(); i += 3) {
			DrawTriangle(vertices[i + 0], vertices[i + 1], vertices[i + 2],
				matModel);
		}
	}

	void DrawTriangle(Vertex& v1, Vertex& v2, Vertex& v3,
		const Matrix4x4& matModel)//mat model
	{
		//m变化
		v1.position = matModel * v1.position;
		v2.position = matModel * v2.position;
		v3.position = matModel * v3.position;

		//背面剔除 索引逆时针为正面
		if (!needCullFace(v1, v2, v3, Vector3<float>(0, 0, 1))) {
			return;
		}

		//p变化
		Matrix4x4& matProject = camera.GetMatProject();
		v1.position = matProject * v1.position;
		v2.position = matProject * v2.position;
		v3.position = matProject * v3.position;

		//还原z
		v1.position.z = v1.position.w;
		v2.position.z = v2.position.w;
		v3.position.z = v3.position.w;

		//透视除法
		v1.position.x /= v1.position.w;
		v1.position.y /= v1.position.w;
		v1.position.w = 1.0f;
		v2.position.x /= v2.position.w;
		v2.position.y /= v2.position.w;
		v2.position.w = 1.0f;
		v3.position.x /= v3.position.w;
		v3.position.y /= v3.position.w;
		v3.position.w = 1.0f;

		//视口变化
		v1.position.x = (v1.position.x + 1.0f) * ((viewport.w - 1) / 2.0f);
		v1.position.y = (v1.position.y + 1.0f) * ((viewport.h - 1) / 2.0f);
		v2.position.x = (v2.position.x + 1.0f) * ((viewport.w - 1) / 2.0f);
		v2.position.y = (v2.position.y + 1.0f) * ((viewport.h - 1) / 2.0f);
		v3.position.x = (v3.position.x + 1.0f) * ((viewport.w - 1) / 2.0f);
		v3.position.y = (v3.position.y + 1.0f) * ((viewport.h - 1) / 2.0f);

		//填色
		Trapezoid trapezoids[2];
		int trapezoidCount = splitTrapezoids(v1, v2, v3,
			trapezoids);
		for (int i = 0; i < trapezoidCount; i++) {
			drawTrapezoid(trapezoids[i]);
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

	//设置颜色
	void clearColor();
	void setColor(int x, int y, const Color& color) const;

	//设置深度
	void clearDepth();
	void setDepth(int x, int y, float z) const;
	float getDepth(int x, int y) const;

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

	//背面剔除 索引顺时针
	bool needCullFace(const Vertex& v1, const Vertex& v2, const Vertex& v3,
		const Vector3<float>& dirView) const;

};

#endif 
