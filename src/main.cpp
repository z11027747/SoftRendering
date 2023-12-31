#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "cpu_renderer.h"

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

unsigned int VBO, VAO;
void initRenderData();

unsigned int colorTextureID;
void initTexture();

unsigned int shaderProgram;
void initShaderProgram();

void draw();
void clear();

CPURenderer* cpuRenderer;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "SoftRendering Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	initRenderData();
	initTexture();
	initShaderProgram();

	cpuRenderer = new CPURenderer(WIN_WIDTH, WIN_HEIGHT);

	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		cpuRenderer->Render(currentTime, deltaTime);

		draw();
		glfwSwapBuffers(window);
	}

	clear();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void initRenderData()
{
	float vertices[] = {
		0.8f,  0.8f, 0.0f, 1.0f, 1.0f, // 右上
		0.8f, -0.8f, 0.0f, 1.0f, 0.0f, // 右下
	   -0.8f, -0.8f, 0.0f, 0.0f, 0.0f, // 左下
	   -0.8f,  0.8f, 0.0f, 0.0f, 1.0f  // 左上
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initTexture()
{
	glGenTextures(1, &colorTextureID);
	glBindTexture(GL_TEXTURE_2D, colorTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_POINT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_POINT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void initShaderProgram()
{
	const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 positionOS;
    layout (location = 1) in vec2 texcoord;
    out vec2 uv;
    void main() {
        gl_Position = vec4(positionOS, 1.0);
        uv = texcoord;
    }
)";

	const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 uv;
    out vec4 FragColor;
    uniform sampler2D colorTexture;
    void main() {
        FragColor = texture(colorTexture, uv);
    }
)";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "colorTexture"), 0);
}

void draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glBindTexture(GL_TEXTURE_2D, colorTextureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, cpuRenderer->colorAttachment);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void clear()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &colorTextureID);
	glDeleteProgram(shaderProgram);
}
