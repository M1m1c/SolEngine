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

#pragma region vertexMeshes

GLfloat verticesTri[] =
{ //               COORDS                        /       COLORS          //
	-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,   0.80f, 0.30f, 0.02f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,   0.80f, 0.30f, 0.02f, // Lower right corner
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   1.00f, 0.60f, 0.32f, // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,   0.00f, 0.45f, 0.77f, // Inner left
	 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,   0.00f, 0.45f, 0.77f, // Inner right
	 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,   0.20f, 0.30f, 0.80f  // Inner down
};

GLfloat verticesQuad[] =
{ //     COORDS       /     UV_COORDS    //
	-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
	+1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
	+1.0f, +1.0f, 0.0f,     1.0f, 1.0f,
	-1.0f, +1.0f, 0.0f,     0.0f, 1.0f
};

GLfloat verticesPyramid[] =
{ //    COORDS            /       COLORS       /      UV_COORDS    //
	-0.5f, +0.0f, +0.5f,     0.80f, 0.30f, 0.02f,	  0.0f, 1.0f,
	-0.5f, +0.0f, -0.5f,     0.80f, 0.30f, 0.02f,	  0.0f, 0.0f,
	+0.5f, +0.0f, -0.5f,     0.80f, 0.30f, 0.02f,	  1.0f, 0.0f,
	+0.5f, +0.0f, +0.5f,     1.00f, 0.60f, 0.32f,	  0.0f, 0.0f,
	+0.0f, +0.8f, +0.0f,     0.00f, 0.45f, 0.77f,     1.0f, 1.0f
};

GLfloat verticesPyramidLighting[] =
{ //     COORDINATES    /    TexCoord    /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.0f, 2.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     2.0f, 2.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     2.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     2.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     2.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     2.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     2.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

GLuint indicesTri[]
{
	0, 3, 5,
	3, 2, 4,
	5, 4, 1
};

GLuint indicesQuad[]
{
	0, 1, 2,
	0, 2, 3
};

GLuint indicesPyramid[]
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

GLuint indicesPyramidLighting[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

#pragma endregion

#pragma region lightMesh

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
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
const unsigned int SCREEN_HEIGHT = 800;

// // Commented due to usage somewhere else
//void ExitOnEsc(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}

int monkeyMain()
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

	// Fixed that stupid thing that Unreal does, where it flips textures upside down
	stbi_set_flip_vertically_on_load(true);

	// viewport size, as in, where we want to render at
	GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

#pragma region Depth Buffer

	GLCall(glEnable(GL_DEPTH_TEST));

	// To not write into the depth buffer, use:
	// GLCall(glDepthMask(GL_FALSE));
	// This requires enabling the depth buffer, otherwise it has no effect

	GLCall(glDepthFunc(GL_LESS));
	// The default value is GL_LESS, but:
	// Function		Description
	// GL_ALWAYS	The depth test always passes.
	// GL_NEVER		The depth test never passes.
	// GL_LESS		Passes if the fragment's depth value is less than the stored depth value.
	// GL_EQUAL		Passes if the fragment's depth value is equal to the stored depth value.
	// GL_LEQUAL	Passes if the fragment's depth value is less than or equal to the stored depth value.
	// GL_GREATER	Passes if the fragment's depth value is greater than the stored depth value.
	// GL_NOTEQUAL	Passes if the fragment's depth value is not equal to the stored depth value.
	// GL_GEQUAL	Passes if the fragment's depth value is greater than or equal to the stored depth value.
	// All of the above are valid too

	// Thinking here - GL_LESS means "objects which are closer can render", therefore....
	//GLCall(glDepthFunc(GL_LESS));
	//GLCall(glDepthFunc(GL_GREATER));

#pragma endregion

#pragma region Stencil Buffer

	GLCall(glEnable(GL_STENCIL_TEST));

	GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
	/* Parameters
Eight symbolic constants are accepted: GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, and GL_INVERT. Initial value is GL_KEEP for all

sfail (Stencil Failed)
Specifies the action to take when the stencil test fails.

dpfail (Stencil Success, Depth Fail)
Specifies the stencil action when the stencil test passes, but the depth test fails.

dppass (Stencil Success and Depth Success OR Stencil Success and Depth N/A)
Specifies the stencil action when both the stencil test and the depth test pass,
or
when the stencil test passes and either there is no depth buffer or depth testing is not enabled.
	*/

#pragma endregion

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.3f, 3.0f));

#pragma endregion

#pragma region mainObject shader and VAO

	// Loading model
	std::string pathString = "Models/Suzanne.fbx";

	// Testing builder pattern ----------
	ModelBuilder mBuilder;

	mBuilder.With_path(pathString);

	Model model = mBuilder.Build();

	// Generates Shader object using defualt.vert and default.frag shaders
	Shader shaderProgram("default.vert", "spotlight.frag");
	Shader outlineProgram("stencilOutline.vert", "stencilOutline.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(verticesPyramidLighting, sizeof(verticesPyramidLighting));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indicesPyramidLighting, sizeof(indicesPyramidLighting));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	// (!) As of right now in the code, its safe to assume that the VBO's are highly packed and not particularly modularized, 
	// or at least there should be a better way to structure their data 

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

#pragma endregion

#pragma region quad shader and VAO

	Shader quadShader("quadUV.vert", "quadUV.frag");

	VAO VAO2;
	VAO2.Bind();

	VBO VBO2(verticesQuad, sizeof(verticesQuad));
	EBO EBO2(indicesQuad, sizeof(indicesQuad));

	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

#pragma endregion

#pragma region lightCube shader and VAO

	// The shader for the light cube
	Shader lightShader("light.vert", "light.frag");

	// Generates the Vertex Array for the light cube and binds it
	VAO lightVAO;
	lightVAO.Bind();

	// Generates the vertices and indices for the light
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

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
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
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

	float outlineRange = 1.10f;
	glm::mat4 outlineMatrix = glm::mat4(1.0f);
	outlineMatrix = glm::scale(outlineMatrix, glm::vec3(outlineRange, outlineRange, outlineRange));
	outlineMatrix = glm::translate(outlineMatrix, mainObjectPos);

	outlineProgram.Bind();
	outlineProgram.setMat4("model", outlineMatrix);

	shaderProgram.Bind();
	shaderProgram.setMat4("model", mainObjectMatrix);
	shaderProgram.setFloat("material.shininess", 32.0f);

	if (false) // set to true for directional lighting setup
	{
		glm::vec3 lightDir = glm::vec3(0.3f, -1.0f, 0.3f);
		shaderProgram.setVec3("light.direction", lightDir);
	}

	if (false) // set to true for point lighting setup
	{
		shaderProgram.setVec3("light.position", lightPos);
		shaderProgram.setFloat("light.linearFalloff", 0.01f);
		shaderProgram.setFloat("light.quadraticFalloff", 0.032f);
	}

	if (true) // set to true for spotlight setup
	{
		shaderProgram.setVec3("spotlight.position", lightPos);
		glm::vec3 spotlightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
		shaderProgram.setVec3("spotlight.spotDirection", spotlightDir);

		GLfloat innerCutoffValue = glm::cos(glm::radians(20.0f));
		GLfloat outerCutoffValue = glm::cos(glm::radians(45.0f));

		shaderProgram.setFloat("spotlight.innerCutoff", innerCutoffValue);
		shaderProgram.setFloat("spotlight.outerCutoff", outerCutoffValue);

		shaderProgram.setFloat("spotlight.linearFalloff", 0.03f);
		shaderProgram.setFloat("spotlight.quadraticFalloff", 0.05f);
	}

#pragma endregion

#pragma region shader setups

	shaderProgram.setVec3("spotlight.ambient", lightAmbient);
	shaderProgram.setVec3("spotlight.diffuse", lightDiffuse);
	shaderProgram.setVec3("spotlight.specular", lightSpecular);

	shaderProgram.setVec3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shaderProgram.setVec3("material.diffuse", glm::vec3(0.3f, 0.5f, 0.8f));
	shaderProgram.setVec3("material.specular", glm::vec3(0.1f, 0.1f, 0.1f));

	//Texture texture("PebblesTile.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	//texture.texUnit(shaderProgram, "material.albedo", 0);
	//Texture specular("PebblesTileSpecular.png", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
	//specular.texUnit(shaderProgram, "material.specular", 1);

	quadShader.Bind();
	quadShader.setMat4("model", mainObjectMatrix);

#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		lightShader.Bind();
		lightPos.x = 0.0f + 2.0f * (float)sin(glfwGetTime());
		lightPos.z = 0.0f + 2.0f * (float)cos(glfwGetTime());
		lightPos.y = 1.0f + 0.0f * (float)cos(glfwGetTime());
		lightModelMatrix = glm::translate(glm::mat4(1.0f), lightPos);
		lightShader.setMat4("model", lightModelMatrix);

		// camera events
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// setup for rendering the main object
		shaderProgram.Bind();
		mainObjectMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		shaderProgram.setMat4("model", mainObjectMatrix);

		// Regularly drawing the object 
		GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		GLCall(glStencilMask(0xFF));
		model.Draw(shaderProgram);

		// Masking the object for the stencil effect
		outlineProgram.Bind();
		outlineMatrix = glm::mat4(1.0f);
		outlineMatrix = glm::translate(outlineMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		outlineMatrix = glm::scale(outlineMatrix, glm::vec3(outlineRange, outlineRange, outlineRange));
		outlineProgram.setMat4("model", outlineMatrix);

		GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		GLCall(glStencilMask(0x00));
		GLCall(glDisable(GL_DEPTH_TEST));

		model.Draw(outlineProgram);

		// Regularly drawing the object a second time
		shaderProgram.Bind();
		mainObjectMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f));
		shaderProgram.setMat4("model", mainObjectMatrix);

		GLCall(glEnable(GL_DEPTH_TEST));
		// Adjusting Depth test to run only if objects HAVE something in front of them 
		// Running GL_ALWAYS is incorrect here, the stencil outlining happens only while in the first monkey's outline range
		GLCall(glDepthFunc(GL_GREATER));

		// Forcefully writing into the stencil buffer, using The value within glStencilFunc(...,value,...)
		GLCall(glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE));
		GLCall(glStencilFunc(GL_ALWAYS, 0, 0xFF));
		GLCall(glStencilMask(0xFF));
		model.Draw(shaderProgram);

		// Masking the object for the stencil effect
		outlineProgram.Bind();
		outlineMatrix = glm::mat4(1.0f);
		outlineMatrix = glm::translate(outlineMatrix, glm::vec3(0.0f, 0.0f, -15.0f));
		outlineMatrix = glm::scale(outlineMatrix, glm::vec3(outlineRange, outlineRange, outlineRange));

		outlineProgram.setMat4("model", outlineMatrix);

		GLCall(glStencilFunc(GL_NOTEQUAL, 0, 0xFF));
		GLCall(glStencilMask(0x00));
		GLCall(glDisable(GL_DEPTH_TEST));
		model.Draw(outlineProgram);

		GLCall(glDepthFunc(GL_LESS));
		GLCall(glStencilMask(0xFF));
		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		GLCall(glEnable(GL_DEPTH_TEST));

		camera.MatrixUniform(outlineProgram, "camMatrix");

		shaderProgram.Bind();
		// Passing the camera position vector as a uniform to the object's shader file
		shaderProgram.setVec3("spotlight.position", lightPos.x, lightPos.y, lightPos.z);
		// GLCall(glUniform3f(glGetUniformLocation(shaderProgram.ID, "spotlight.position"), lightPos.x, lightPos.y, lightPos.z));

		glm::vec3 spotlightDir = mainObjectPos - lightPos;
		shaderProgram.setVec3("spotlight.spotDirection", spotlightDir);
		shaderProgram.setVec3("camPos", camera.Position.x, camera.Position.y, camera.Position.z);
		//GLCall(glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z));
		// Passing the camera model * projection matrix as a uniform to the object's shader file
		camera.MatrixUniform(shaderProgram, "camMatrix");

		//texture.Bind();
		//specular.Bind();
		VAO1.Bind();

		const int indexCount = sizeof(indicesPyramidLighting) / sizeof(int);
		//GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));

		// setup for rendering the light cube
		lightShader.Bind();
		camera.MatrixUniform(lightShader, "camMatrix");
		lightVAO.Bind();

		const int indexCountLight = sizeof(lightIndices) / sizeof(int);
		GLCall(glDrawElements(GL_TRIANGLES, indexCountLight, GL_UNSIGNED_INT, 0));

		//quadShader.Activate();
		//camera.MatrixUniform(quadShader, "camMatrix");
		//VAO2.Bind();
		//const int indexCountQuad = sizeof(indicesQuad) / sizeof(int);
		//GLCall(glDrawElements(GL_TRIANGLES, indexCountQuad , GL_UNSIGNED_INT, 0));

		// Swap render buffers
		glfwSwapBuffers(window);
		// handle window events
		glfwPollEvents();

		// ExitOnEsc(window);
	}

	// Deleting the abstracted GPU objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	//texture.Delete();
	//specular.Delete();
	shaderProgram.Delete();

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
