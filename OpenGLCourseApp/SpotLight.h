#pragma once
#include "PointLight.h"
class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dInstensity, GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat exp, GLfloat lin, GLfloat con, GLfloat ed);

	void UseLight(GLuint ambientIntensityLocation, GLuint colourLocation, GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint exponentLocation, GLuint linearLocation, GLuint constantLocation, GLuint directionLocation, GLuint edgeLocation);

	void SetFlash(glm::vec3 dir, glm::vec3 pos);

	~SpotLight();

private:
	glm::vec3 direction;
	GLfloat edge, procEdge;
};

