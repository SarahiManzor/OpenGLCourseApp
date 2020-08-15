#pragma once
#include "Light.h"
class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dInstensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat exp, GLfloat lin, GLfloat con);

	void UseLight(	GLuint ambientIntensityLocation, GLuint colourLocation, GLuint diffuseIntensityLocation, GLuint positionLocation,
					GLuint exponentLocation, GLuint linearLocation, GLuint constantLocation);

	~PointLight();

protected:
	glm::vec3 position;
	GLfloat expon;
	GLfloat linear;
	GLfloat constant;
};

