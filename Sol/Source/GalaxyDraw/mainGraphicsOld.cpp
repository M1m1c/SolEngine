#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Model.h"
#include "ModelBuilder.h"
#include "Renderer.h"

#pragma region quadMesh

GLfloat quadVertices[] =
{ //     COORDS       /     UV_COORDS    //
	-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
	+1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
	+1.0f, +1.0f, 0.0f,     1.0f, 1.0f,
	-1.0f, +1.0f, 0.0f,     0.0f, 1.0f
};

GLuint quadIndices[]
{
	0, 1, 2,
	0, 2, 3
};

#pragma endregion

#pragma region cubeMesh

GLfloat cubeVertices[] =
{ //     COORDINATES     //
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f
};

GLuint cubeIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

#pragma endregion

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void ExitOnEsc(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
#pragma region window and camera setup

	glfwInit();

	// window hints, information necessary to create wwindow
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window - notice the distinction between Window and Monitor - 
	GLFWmonitor* fullscreen = NULL;
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "I am a window, my name is Doorothy", fullscreen, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Window exists, now make it the current context
	glfwMakeContextCurrent(window);

	// glad loads the openGL functions
	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	// Fixed that thing that Unreal does, where it flips textures upside down
	stbi_set_flip_vertically_on_load(true);

	// viewport size, as in, where we want to render at
	GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	// Enables Depth test, prioritizing closer objects (menmonic: LESS distant)
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.3f, 3.0f));

#pragma endregion

#pragma region Initializing Basic Renderers

	VAO quadVAO;
	quadVAO.Bind();

	VBO quadVBO(quadVertices, sizeof(quadVertices));
	EBO quadEBO(quadIndices, sizeof(quadIndices));

	quadVAO.LinkAttrib(quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	quadVAO.LinkAttrib(quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//const VAO& VArrayObject, const EBO& IndexBufferObject, const Shader& shader
	Renderer quadRenderer(quadVAO, quadEBO);

#pragma endregion

#pragma region mainObject shader and VAO

	// Loading model
	std::string pathString = "Models/suzanne.fbx";

	Model model = Model(pathString);

	VAO cubeVAO;
	cubeVAO.Bind();

	VBO cubeVBO(cubeVertices, sizeof(cubeVertices));
	EBO cubeEBO(cubeIndices, sizeof(cubeIndices));

	cubeVAO.LinkAttrib(cubeVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	Renderer cubeRenderer(cubeVAO, );

	cubeVAO.Unbind();
	cubeVBO.Unbind();
	cubeEBO.Unbind();

	// Generates Shader object using defualt.vert and default.frag shaders
	Shader shaderProgram("default.vert", "spotlight.frag");

#pragma endregion

#pragma region quad shader and VAO

	Shader quadShader("quadUV.vert", "quadUV.frag");

	VAO VAO2;
	VAO2.Bind();

	VBO VBO2(quadVertices, sizeof(quadVertices));
	EBO EBO2(quadIndices, 6);

	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

#pragma endregion

#pragma region lightCube shader and VAO

	// The shader for the moving light cube
	Shader lightShader("light.vert", "light.frag");

	// Generates the Vertex Array for the light cube and binds it
	VAO lightVAO;
	lightVAO.Bind();

	// Generates the vertices and indices for the light
	VBO lightVBO(cubeVertices, sizeof(cubeVertices));
	EBO lightEBO(cubeIndices, sizeof(cubeIndices));

	// Links VBO attributes to VAO (in this case, its just vertex positions)
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	// Unbind all buffers
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

#pragma endregion

#pragma region light object setup

	// Preparing Light Coloring
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightColor = lightDiffuse;

	glm::vec3 lightPos = glm::vec3(1.0f, 1.5f, 1.0f);
	glm::vec3 lightScale = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
	lightModelMatrix = glm::scale(lightModelMatrix, lightScale);
	lightModelMatrix = glm::translate(lightModelMatrix, lightPos);

	lightShader.Bind();
	lightShader.setMat4("model", lightModelMatrix);
	lightShader.setVec3("lightColor", lightColor);

#pragma endregion


#pragma region main object setup

	// Passing values to the MainObject shader
	glm::vec3 mainObjectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 mainObjectMatrix = glm::mat4(1.0f);
	mainObjectMatrix = glm::scale(mainObjectMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	mainObjectMatrix = glm::translate(mainObjectMatrix, mainObjectPos);

	shaderProgram.Bind();
	shaderProgram.setMat4("model", mainObjectMatrix);
	shaderProgram.setFloat("material.shininess", 32.0f);

	if (true) // spotlight setup
	{
		shaderProgram.setVec3("spotlight.position", lightPos);
		glm::vec3 spotlightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
		shaderProgram.setVec3("spotlight.spotDirection", spotlightDir);

		GLfloat innerCutoffValue = glm::cos(glm::radians(30.0f));
		GLfloat outerCutoffValue = glm::cos(glm::radians(60.0f));

		shaderProgram.setFloat("spotlight.innerCutoff", innerCutoffValue);
		shaderProgram.setFloat("spotlight.outerCutoff", outerCutoffValue);

		shaderProgram.setFloat("spotlight.linearFalloff", 0.003f);
		shaderProgram.setFloat("spotlight.quadraticFalloff", 0.005f);
	}

#pragma endregion

#pragma region shader setups (main object)

	shaderProgram.setVec3("spotlight.ambient", lightAmbient);
	shaderProgram.setVec3("spotlight.diffuse", lightDiffuse);
	shaderProgram.setVec3("spotlight.specular", lightSpecular);

	shaderProgram.setVec3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shaderProgram.setVec3("material.diffuse", glm::vec3(0.3f, 0.5f, 0.8f));
	shaderProgram.setVec3("material.specular", glm::vec3(0.1f, 0.1f, 0.1f));

	Texture texture("KristerSphere.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(shaderProgram, "texture_diffuse1", 0);

	float rot = 0.0f;

#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		lightShader.Bind();
		lightPos.x = 0.0f + 1.0f * (float)sin(glfwGetTime());
		lightPos.z = 0.0f + 1.0f * (float)cos(glfwGetTime());
		lightPos.y = 1.5f + 0.0f * (float)cos(glfwGetTime());
		lightModelMatrix = glm::translate(glm::mat4(1.0f), lightPos);
		lightModelMatrix = glm::scale(lightModelMatrix, lightScale);
		lightShader.setMat4("model", lightModelMatrix);

		// camera events
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// setup for rendering the main object
		shaderProgram.Bind();
		texture.Bind();
		mainObjectMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mainObjectMatrix = glm::rotate(mainObjectMatrix, rot, glm::vec3(0.0, 1.0, 1.0));
		mainObjectMatrix = glm::scale(mainObjectMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
		shaderProgram.setMat4("model", mainObjectMatrix);

		rot += 0.03f;

		// Regularly drawing the object 
		model.Draw(shaderProgram);

		//cubeVAO.Bind();
		//mainObjectMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		//shaderProgram.setMat4("model", mainObjectMatrix);
		//GLCall(glDrawElements(GL_TRIANGLES, cubeEBO.count, GL_UNSIGNED_INT, 0));

		shaderProgram.Bind();
		// Passing the camera position vector as a uniform to the object's shader file
		shaderProgram.setVec3("spotlight.position", lightPos.x, lightPos.y, lightPos.z);

		glm::vec3 spotlightDir = mainObjectPos - lightPos;
		shaderProgram.setVec3("spotlight.spotDirection", spotlightDir);
		shaderProgram.setVec3("camPos", camera.Position.x, camera.Position.y, camera.Position.z);

		// Passing the camera model * projection matrix as a uniform to the object's shader file
		camera.MatrixUniform(shaderProgram, "camMatrix");

		// setup for rendering the light cube
		lightShader.Bind();
		camera.MatrixUniform(lightShader, "camMatrix");
		lightVAO.Bind();

		const int indexCountLight = sizeof(cubeIndices) / sizeof(int);
		GLCall(glDrawElements(GL_TRIANGLES, indexCountLight, GL_UNSIGNED_INT, 0));

		// Swap render buffers
		glfwSwapBuffers(window);
		// handle window events, I think also inputs?
		glfwPollEvents();

		ExitOnEsc(window);
	}

	// Deleting the abstracted GPU objects
	shaderProgram.Delete();
	texture.Delete();

	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();
	quadShader.Delete();

	// cleaning up window reference and then glfw context (!! IN THAT ORDER !!)
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
