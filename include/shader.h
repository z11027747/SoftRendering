#ifndef _SHADER_H
#define _SHADER_H

#include "vector.h"
#include "color.h"
#include "all_define.h"

struct Uniforms {
	Texture texture;
};

class Shader
{
public:
	Uniforms uniforms;

	virtual Vector4<float> vertChanging(Vertex&) const = 0;
	virtual Color fragChanging(Vertex&) const = 0;
};

class DefaultShader : public Shader {

	Vector4<float> vertChanging(Vertex& i) const override {
		return Vector4<float>(i.position.x, i.position.y, i.position.z, 1.0f);
	}

	Color fragChanging(Vertex& v) const override {
		Color texCol = uniforms.texture.Tex(v.uv);
		return texCol * v.color;
	}

};


#endif 
