#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	expon = 0.0f;
	linear = 0.0f;
	constant = 1.0f;
}


PointLight::PointLight(	GLfloat red, GLfloat green, GLfloat blue, 
						GLfloat aIntensity, GLfloat dInstensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat exp, GLfloat lin, GLfloat con) : Light(1024, 1024, red, green, blue, aIntensity, dInstensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	expon = exp;
	linear = lin;
	constant = con;
}

void PointLight::UseLight(	GLuint ambientIntensityLocation, GLuint colourLocation, GLuint diffuseIntensityLocation, GLuint positionLocation,
							GLuint exponentLocation, GLuint linearLocation, GLuint constantLocation)
{
	__super::UseLight(ambientIntensityLocation, colourLocation, diffuseIntensityLocation);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(exponentLocation, expon);
	glUniform1f(linearLocation, linear);
	glUniform1f(constantLocation, constant);
}

PointLight::~PointLight()
{

}