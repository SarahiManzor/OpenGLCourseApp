#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CommonValues.h"
#include "Material.h"
#include "Model.h"

const float toRadians = 3.1415926535f / 180.0f;

Window* MainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Shader* directionalShadowShader;
Camera* camera;

Texture* brickTexture;
const char* brickLoc = "Textures/brick.png";
const char* dirtLoc = "Textures/dirt.png";
const char* plainLoc = "Textures/plain.png";
Texture* dirtTexture;
Texture* plainTexture;

Material* shinyMaterial;
Material* dullMaterial;

Model monkey;

DirectionalLight* mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLuint uniformModel;

double deltaTime;
double lastTime = 0.0f;

int pointLightCount = 0;
int spotLightCount = 0;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CalculateAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat * vertices, unsigned int vertexCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indicesCount; i += 3)
	{
		unsigned int ind0 = indices[i] * vLength;
		unsigned int ind1 = indices[i + 1] * vLength;
		unsigned int ind2 = indices[i + 2] * vLength;
		
		glm::vec3 vert0 = glm::vec3(vertices[ind0], vertices[ind0 + 1], vertices[ind0 + 2]);
		glm::vec3 vert1 = glm::vec3(vertices[ind1], vertices[ind1 + 1], vertices[ind1 + 2]);
		glm::vec3 vert2 = glm::vec3(vertices[ind2], vertices[ind2 + 1], vertices[ind2 + 2]);

		glm::vec3 ev1 = vert1 - vert0;
		glm::vec3 ev2 = vert2 - vert0;

		glm::vec3 normal = glm::cross(ev1, ev2);
		normal = glm::normalize(normal);

		ind0 += normalOffset;
		ind1 += normalOffset;
		ind2 += normalOffset;

		vertices[ind0] += normal.x;
		vertices[ind0 + 1] += normal.y;
		vertices[ind0 + 2] += normal.z;

		vertices[ind1] += normal.x;
		vertices[ind1 + 1] += normal.y;
		vertices[ind1 + 2] += normal.z;

		vertices[ind2] += normal.x;
		vertices[ind2 + 1] += normal.y;
		vertices[ind2 + 2] += normal.z;
	}

	for (size_t i = 0; i < vertexCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec = glm::vec3(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset+2]);
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObject()
{
	unsigned int pyramidIndices[] = 
	{
		0, 1, 2, // Bottom back
		0, 3, 1, // Bottom front 
		0, 2, 4, 
		2, 1, 4,
		1, 3, 4,
		3, 0, 4,
	};

	GLfloat pyramidVertices[] =
	{// x		y		z			s     t			nx    ny    nz
		-10.0f, 0.0f,	0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		10.0f,	0.0f,	0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f,	0.0f,	-10.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f,	0.0f,	10.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f,	10.0f,	0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	unsigned int floorIndices[] = 
	{
		0, 1, 2,
		0, 3, 2
	};

	GLfloat floorVertices[] =
	{// x			y				z			s     t			nx    ny    nz
		-100.0f,	0.0f,		-100.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-100.0f,	0.0f,		100.0f,			0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		100.0f,		0.0f,		100.0f,			10.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		100.0f,		0.0f,		-100.0f,		10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
	};

	CalculateAverageNormals(pyramidIndices, 18, pyramidVertices, 40, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(pyramidVertices, pyramidIndices, 40, 18);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(pyramidVertices, pyramidIndices, 40, 18);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);

	directionalShadowShader = new Shader();
	directionalShadowShader->CreateFromFiles("Shaders/directionalShadowMap.vert", "Shaders/directionalShadowMap.frag");
}

void RenderScene()
{
	// Draw first pyramid
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
	uniformModel = shaderList[0]->GetModelLocation();

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture->UseTexture();
	shinyMaterial->UseMaterial(shaderList[0]->GetSpecularIntensityLocation(), shaderList[0]->GetShininessLocation());
	meshList[0]->RenderMesh();

	// Draw second pyramid
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture->UseTexture();
	dullMaterial->UseMaterial(shaderList[0]->GetSpecularIntensityLocation(), shaderList[0]->GetShininessLocation());
	meshList[1]->RenderMesh();

	// Draw second pyramid
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture->UseTexture();
	shinyMaterial->UseMaterial(shaderList[0]->GetSpecularIntensityLocation(), shaderList[0]->GetShininessLocation());
	meshList[2]->RenderMesh();

	// Draw Monkey
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 40.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

	glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
	//plainTexture->UseTexture();
	shinyMaterial->UseMaterial(shaderList[0]->GetSpecularIntensityLocation(), shaderList[0]->GetShininessLocation());
	monkey.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader->UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader->GetModelLocation();
	directionalShadowShader->SetDirectionalLightTransform(&light->CalculateLightTransform());

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix)
{
	shaderList[0]->UseShader();

	// Clear Window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1466, 768);

	// Set scene uniforms
	glUniformMatrix4fv(shaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shaderList[0]->GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera->CalculateViewMatrix()));
	glUniform3f(shaderList[0]->GetEyePositionLocation(), camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);

	// Set flashlight
	glm::vec3 cameraPosition = camera->GetCameraPosition();
	spotLights[0].SetFlash(camera->GetCameraDirection(), cameraPosition);

	// set light uniforms
	shaderList[0]->SetDirectionalLight(mainLight);
	shaderList[0]->SetPointLights(pointLights, pointLightCount);
	shaderList[0]->SetSpotLights(spotLights, spotLightCount);
	shaderList[0]->SetDirectionalLightTransform(&mainLight->CalculateLightTransform());

	mainLight->GetShadowMap()->Read(GL_TEXTURE1);
	shaderList[0]->SetTexture(0);
	shaderList[0]->SetDirectionalShadowMap(1);

	RenderScene();
}

int main()
{
	MainWindow = new Window(1366, 768);
	MainWindow->Initialise();

	CreateObject();
	CreateShaders();

	camera = new Camera(glm::vec3(0.0f, 10.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 25.0f, 0.5f);

	brickTexture = new Texture(brickLoc);
	brickTexture->LoadTextureA();
	dirtTexture = new Texture(dirtLoc);
	dirtTexture->LoadTextureA();
	plainTexture = new Texture(plainLoc);
	plainTexture->LoadTextureA();

	shinyMaterial = new Material(10.0f, 256.0f);
	dullMaterial = new Material(0.3f, 4.0f);

	monkey = Model();
	monkey.LoadModel("Models/monkey.obj");

	mainLight = new DirectionalLight(1.0f, 1.0f, 1.0f, 0.2f, 0.3f, 2.0f, -1.0f, -2.0f, 1024, 1024);

	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -40.0f, 10.0f, 0.0f, 0.01f, 0.02f, 0.03f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 40.0f, 10.0f, 0.0f, 0.01f, 0.02f, 0.03f);
	pointLightCount++;
	pointLights[2] = PointLight(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 10.0f, -40.0f, 0.01f, 0.02f, 0.03f);
	pointLightCount++;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 0.4f, 0.4f, 0.0f, 10.0f, 40.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, 0.4f, 0.4f, 0.0f, 10.0f, 40.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 20.0f);
	spotLightCount++;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)MainWindow->GetBufferWidth() / (GLfloat)MainWindow->GetBufferHeight(), 0.1f, 300.0f);

	// Loop until window closed
	while (!MainWindow->GetShouldClose())
	{
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get and Handle user input events
		glfwPollEvents();

		camera->KeyControl(MainWindow->GetKeys(), deltaTime);
		camera->MouseControl(MainWindow->GetXChange(), MainWindow->GetYChange());
		printf("Camera x: %.2f y %.2f z %.2f\n", camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);

		//DirectionalShadowMapPass(mainLight);
		RenderPass(projection);

		glUseProgram(0);

		MainWindow->SwapBuffers();
	}

	return 0;
}