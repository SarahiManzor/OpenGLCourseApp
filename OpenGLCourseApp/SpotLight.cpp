#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cos(glm::radians(edge));
}


SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dInstensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLfloat exp, GLfloat lin, GLfloat con, GLfloat ed) : PointLight(red, green, blue, aIntensity, dInstensity, xPos, yPos, zPos, exp, lin, con)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	edge = ed;
	procEdge = cos(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint colourLocation, GLuint diffuseIntensityLocation, GLuint positionLocation,
	GLuint exponentLocation, GLuint linearLocation, GLuint constantLocation, GLuint directionLocation, GLuint edgeLocation)
{
	__super::UseLight(ambientIntensityLocation, colourLocation, diffuseIntensityLocation, positionLocation, exponentLocation, linearLocation, constantLocation);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 dir, glm::vec3 pos)
{
	direction = dir;
	position = pos;
}

SpotLight::~SpotLight()
{

}