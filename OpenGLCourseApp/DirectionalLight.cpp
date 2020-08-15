#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(	GLfloat red, GLfloat green, GLfloat blue, 
									GLfloat aIntensity, GLfloat dInstensity, 
									GLfloat xDir, GLfloat yDir, GLfloat zDir, 
									GLfloat shadowWidth, GLfloat shadowHeight) : Light(red, green, blue, aIntensity, dInstensity, shadowWidth, shadowHeight)
{
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 250.0f);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint colourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	__super::UseLight(ambientIntensityLocation, colourLocation, diffuseIntensityLocation);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction * 200.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{

}