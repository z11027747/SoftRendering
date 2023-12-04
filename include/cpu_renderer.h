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
#include "shader.h"

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

		camera = Camera(1.0f, 10.0f, ((float)w / (float)h));
		//camera.SetOrthographicMode(2.0f);
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

		//Cube-vertices
		std::vector<Vertex> vertices = {
			// position                  ---                  color      ---        uv
			Vertex(Vector3<float>(0.5f, -0.5f, 1.5f),  Color::green, Vector2<float>(0.0f, 0.0f)),//0
			Vertex(Vector3<float>(0.5f, 0.5f, 1.5f),   Color::green, Vector2<float>(0.0f, 1.0f)),//2
			Vertex(Vector3<float>(-0.5f, 0.5f, 1.5f),  Color::green, Vector2<float>(1.0f, 1.0f)),//3
			Vertex(Vector3<float>(0.5f, -0.5f, 1.5f),  Color::green, Vector2<float>(0.0f, 0.0f)),//0
			Vertex(Vector3<float>(-0.5f, 0.5f, 1.5f),  Color::green, Vector2<float>(1.0f, 1.0f)),//3
			Vertex(Vector3<float>(-0.5f, -0.5f, 1.5f), Color::green, Vector2<float>(1.0f, 0.0f)),//1

			Vertex(Vector3<float>(0.5f, 0.5f, 1.5f),   Color::red, Vector2<float>(0.0f, 0.0f)),//8
			Vertex(Vector3<float>(0.5f, 0.5f, 0.5f),   Color::red, Vector2<float>(0.0f, 1.0f)),//4
			Vertex(Vector3<float>(-0.5f, 0.5f, 0.5f),  Color::red, Vector2<float>(1.0f, 1.0f)),//5
			Vertex(Vector3<float>(0.5f, 0.5f, 1.5f),   Color::red, Vector2<float>(0.0f, 0.0f)),//8
			Vertex(Vector3<float>(-0.5f, 0.5f, 0.5f),  Color::red, Vector2<float>(1.0f, 1.0f)),//5
			Vertex(Vector3<float>(-0.5f, 0.5f, 1.5f),  Color::red, Vector2<float>(1.0f, 0.0f)),//9

			Vertex(Vector3<float>(0.5f, 0.5f, 0.5f),   Color::white, Vector2<float>(0.0f, 0.0f)),//10
			Vertex(Vector3<float>(0.5f, -0.5f, 0.5f),  Color::white, Vector2<float>(0.0f, 1.0f)),//6
			Vertex(Vector3<float>(-0.5f, -0.5f, 0.5f), Color::white, Vector2<float>(1.0f, 1.0f)),//7
			Vertex(Vector3<float>(0.5f, 0.5f, 0.5f),   Color::white, Vector2<float>(0.0f, 0.0f)),//10
			Vertex(Vector3<float>(-0.5f, -0.5f, 0.5f), Color::white, Vector2<float>(1.0f, 1.0f)),//7
			Vertex(Vector3<float>(-0.5f, 0.5f, 0.5f),  Color::white, Vector2<float>(1.0f, 0.0f)),//11

			Vertex(Vector3<float>(0.5f, -0.5f, 0.5f),  Color::red, Vector2<float>(0.0f, 0.0f)),//12
			Vertex(Vector3<float>(0.5f, -0.5f, 1.5f),  Color::red, Vector2<float>(0.0f, 1.0f)),//13
			Vertex(Vector3<float>(-0.5f, -0.5f, 1.5f), Color::red, Vector2<float>(1.0f, 1.0f)),//14
			Vertex(Vector3<float>(0.5f, -0.5f, 0.5f),  Color::red, Vector2<float>(0.0f, 0.0f)),//12
			Vertex(Vector3<float>(-0.5f, -0.5f, 1.5f), Color::red, Vector2<float>(1.0f, 1.0f)),//14
			Vertex(Vector3<float>(-0.5f, -0.5f, 0.5f), Color::red, Vector2<float>(1.0f, 0.0f)),//15

			Vertex(Vector3<float>(-0.5f, -0.5f, 1.5f), Color::blue, Vector2<float>(0.0f, 0.0f)),//16
			Vertex(Vector3<float>(-0.5f, 0.5f, 1.5f),  Color::blue, Vector2<float>(0.0f, 1.0f)),//17
			Vertex(Vector3<float>(-0.5f, 0.5f, 0.5f),  Color::blue, Vector2<float>(1.0f, 1.0f)),//18
			Vertex(Vector3<float>(-0.5f, -0.5f, 1.5f), Color::blue, Vector2<float>(0.0f, 0.0f)),//16
			Vertex(Vector3<float>(-0.5f, 0.5f, 0.5f),  Color::blue, Vector2<float>(1.0f, 1.0f)),//18
			Vertex(Vector3<float>(-0.5f, -0.5f, 0.5f), Color::blue, Vector2<float>(1.0f, 0.0f)),//19

			Vertex(Vector3<float>(0.5f, -0.5f, 0.5f),  Color::green, Vector2<float>(0.0f, 0.0f)),//20
			Vertex(Vector3<float>(0.5f, 0.5f, 0.5f),   Color::green, Vector2<float>(0.0f, 1.0f)),//21
			Vertex(Vector3<float>(0.5f, 0.5f, 1.5f),   Color::green, Vector2<float>(1.0f, 1.0f)),//22
			Vertex(Vector3<float>(0.5f, -0.5f, 0.5f),  Color::green, Vector2<float>(0.0f, 0.0f)),//20
			Vertex(Vector3<float>(0.5f, 0.5f, 1.5f),   Color::green, Vector2<float>(1.0f, 1.0f)),//22
			Vertex(Vector3<float>(0.5f, -0.5f, 1.5f),  Color::green, Vector2<float>(1.0f, 0.0f)),//23
		};

		DefaultShader shader;
		shader.uniforms = {
			texture = texture
		};

		Matrix4x4 matModel1;
		matModel1.Identity();
		matModel1.SetT(2.0f, 0.0f, 5.0f);
		matModel1.SetR_Y(Helper::Rad2Deg(currentTime * 100.0f));
		DrawArrays(vertices, matModel1, shader);

		Matrix4x4 matModel2;
		matModel2.Identity();
		matModel2.SetT(0.0f, 0.0f, 5.0f);
		matModel2.SetR_X(Helper::Rad2Deg(30.0f));
		DrawArrays(vertices, matModel2, shader);
	}

	void DrawArrays(std::vector<Vertex> vertices, const Matrix4x4& matModel, const Shader& shader)
	{
		for (int i = 0; i < vertices.size(); i += 3) {
			std::vector<Vertex> v3 = { vertices[i + 0], vertices[i + 1], vertices[i + 2] };
			DrawTriangle(v3,
				matModel,
				shader);
		}
	}

	void DrawTriangle(std::vector<Vertex>& v3,
		const Matrix4x4& matModel,//mat model
		const Shader& shader)
	{
		//顶点着色器
		for (auto& v : v3) {
			v.position = shader.vertChanging(v);
		}

		//m变化
		for (auto& v : v3) {
			v.position = matModel * v.position;
		}

		//视锥剔除
		if (!camera.IsInFrustum(v3[0].position)
			&& !camera.IsInFrustum(v3[1].position)
			&& !camera.IsInFrustum(v3[2].position)) {
			return;
		}

		//p变化
		for (auto& v : v3) {
			v.position = camera.matProject * v.position;
		}

		//还原z
		for (auto& v : v3) {
			v.position.z = v.position.w;
		}

		//透视除法
		for (auto& v : v3) {
			v.position.x /= v.position.w;
			v.position.y /= v.position.w;
			v.position.w = 1.0f;
		}

		//背面剔除
		if (!needCullFace(v3[0], v3[1], v3[2],
			Vector3<float>(0, 0, 1))) {
			return;
		}

		//视口变化
		for (auto& v : v3) {
			v.position.x = (v.position.x + 1.0f) * ((viewport.w - 1) / 2.0f);
			v.position.y = (v.position.y + 1.0f) * ((viewport.h - 1) / 2.0f);
		}

		//填色
		Trapezoid trapezoids[2];
		int trapezoidCount = splitTrapezoids(v3[0], v3[1], v3[2],
			trapezoids);
		for (int i = 0; i < trapezoidCount; i++) {
			drawTrapezoid(trapezoids[i],
				shader);
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

	//线框
	//直线绘制
	void bresenmanDrawLine(const Vector2<int>& start, const Vector2<int>& end,
		const Color& color) const;
	//直线裁剪
	bool cohenSutherlandLineClip(Vector2<int>& v0, Vector2<int>& v1, const Vector2<int>& max) const;
	int cohenSutherlandLineComputeOutCode(const Vector2<int>& v, const Vector2<int>& max) const;

	//填色
	//拆分平底梯形
	int splitTrapezoids(const Vertex& v1, const Vertex& v2, const Vertex& v3,
		Trapezoid* trapezoids) const;
	void drawTrapezoid(Trapezoid& trapezoid,
		const Shader& shader) const;
	//生成扫描线
	Scanline genScanline(const Trapezoid& trapezoid, float y) const;

	//背面剔除
	bool needCullFace(const Vertex& v1, const Vertex& v2, const Vertex& v3,
		const Vector3<float>& dirView) const;

};

#endif 
