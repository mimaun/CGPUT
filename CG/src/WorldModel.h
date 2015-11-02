#ifndef __MODEL
#define __MODEL

#include <vector>
#include "glm/glm.hpp"
#include "PlanetModel.h"
using namespace std; //makes using vectors easy

/*
--------------------------------------------------------------------------------------------------
This is the world model. It can contain multiple objects.
--------------------------------------------------------------------------------------------------
*/

class WorldModel
{
public:
	WorldModel()
	{
		sun = PlanetModel();
		mercury = PlanetModel(0.39, 0.382, glm::vec3(1.0, 0.0, 0.0), "simple");
		venus = PlanetModel(0.72, 0.949, glm::vec3(1.0, 0.0, 0.0), "simple");
		earth = PlanetModel(1.0, 1.0, glm::vec3(1.0, 0.0, 0.0), "earth");
		mars = PlanetModel(1.52, 0.532, glm::vec3(0.0, 1.0, 0.0), "mars");
		jupiter = PlanetModel(5.2, 11.209, glm::vec3(1.0, 0.0, 0.0), "simple");
		saturn = PlanetModel(9.54, 9.449, glm::vec3(1.0, 0.0, 0.0), "simple");
		uranus = PlanetModel(19.22, 4.007, glm::vec3(1.0, 0.0, 0.0), "simple");
		neptune = PlanetModel(30.06, 3.883, glm::vec3(1.0, 0.0, 0.0), "simple");
	}

	void setUpBuffers()
	{
		sun.setUpBuffers();
		mercury.setUpBuffers();
		venus.setUpBuffers();
		earth.setUpBuffers();
		mars.setUpBuffers();
		jupiter.setUpBuffers();
		saturn.setUpBuffers();
		uranus.setUpBuffers();
		neptune.setUpBuffers();
	}

	void setUpShader()
	{
		sun.setUpShader();
		mercury.setUpShader();
		venus.setUpShader();
		earth.setUpShader();
		mars.setUpShader();
		jupiter.setUpShader();
		saturn.setUpShader();
		uranus.setUpShader();
		neptune.setUpShader();
	}

	void draw(glm::mat4 P, glm::mat4 C, glm::mat4 currentModelTransform)
	{
		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sun.draw(P, C, currentModelTransform);
		mercury.draw(P, C, currentModelTransform);
		venus.draw(P, C, currentModelTransform);
		earth.draw(P, C, currentModelTransform);
		mars.draw(P, C, currentModelTransform);
		jupiter.draw(P, C, currentModelTransform);
		saturn.draw(P, C, currentModelTransform);
		uranus.draw(P, C, currentModelTransform);
		neptune.draw(P, C, currentModelTransform);

		//cleanup
		glBindVertexArray(0);
		glUseProgram(0);
	}

private:
	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<GLuint> elements;
	size_t objectCount;

	PlanetModel sun;
	PlanetModel mercury;
	PlanetModel	venus;
	PlanetModel earth;
	PlanetModel mars;
	PlanetModel jupiter;
	PlanetModel saturn;
	PlanetModel uranus;
	PlanetModel neptune;
};

#endif