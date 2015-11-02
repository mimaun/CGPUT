#ifndef __PLANET_MODEL
#define __PLANET_MODEL

using namespace std;
#include <vector>
#include "glm/glm.hpp"
#include "objload/objLoader.h"

class PlanetModel
{
private:
	float scaleFactor = 0.1;
	float earthSemiMajorAxis = 149598023;
	float earthEquatorialRadius = 6378.1;

public:
	PlanetModel()
	{
		this->shaderName = "sun";

		float factor = scaleFactor * 50; // normal 109
		basicTransforms = glm::scale(glm::mat4(1.0), glm::vec3(factor, factor, factor)) * glm::mat4(1.0);

		loadModel(glm::vec3(1.0, 1.0, 0.0));
	}

	PlanetModel(GLfloat positionFactor, GLfloat sizeFactor, glm::vec3 color, string shaderName)
	{
		this->shaderName = shaderName;

		basicTransforms = glm::mat4(1.0);
		glm::mat4 scaleM = glm::scale(glm::mat4(1.0), glm::vec3(scaleFactor * sizeFactor, scaleFactor * sizeFactor, scaleFactor * sizeFactor));
		glm::mat4 translateM = glm::translate(glm::mat4(1.0), glm::vec3(10 * positionFactor, 0.0, 0.0));
		basicTransforms = translateM * scaleM * basicTransforms;

		loadModel(color);
	}

	void setUpShader()
	{
		std::string strVert = "shaders/" + shaderName + ".vert";
		std::vector<char> writableVert(strVert.begin(), strVert.end());
		writableVert.push_back('\0');
		char const * vertPath = &writableVert[0];

		std::string strFrag = "shaders/" + shaderName + ".frag";
		std::vector<char> writableFrag(strFrag.begin(), strFrag.end());
		writableFrag.push_back('\0');
		char const * fragPath = &writableFrag[0];

		//char const * vertPath = "shaders/simple.vert";
		//char const * fragPath = "shaders/simple.frag";

		shaderProg = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);
	
		checkGLError("shader");
	}

	void setUpBuffers()
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		GLuint positionBuffer;
		GLuint colorBuffer;
		GLuint elementBuffer;

		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, getPositionBytes(), &getPosition()[0], GL_STATIC_DRAW);
		GLint positionSlot = glGetAttribLocation(shaderProg, "pos");
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Do the same thing for the color data
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, getColorBytes(), &getColor()[0], GL_STATIC_DRAW);
		GLint colorSlot = glGetAttribLocation(shaderProg, "colorIn");
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, getElementBytes(), &getElements()[0], GL_STATIC_DRAW);
		//leave the element buffer active

		glBindVertexArray(0);

		checkGLError("setup");
	}

	void draw(glm::mat4 P, glm::mat4 C, glm::mat4 currentModelTransform)
	{
		//use shader
		glUseProgram(shaderProg);
		glm::mat4 M = currentModelTransform * basicTransforms;
		glm::mat4 T = P*C*M;

		GLint timeSlot = glGetUniformLocation(shaderProg, "timeIn");
		GLint matSlot = glGetUniformLocation(shaderProg, "m");

		glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);

		//draw
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, getElements().size(), GL_UNSIGNED_INT, 0);
	}

	vector<GLfloat> const getPosition() const
	{
		return positions;
	}

	vector<GLfloat> const getColor() const
	{
		return colors;
	}

	vector<GLuint> const getElements() const
	{
		return elements;
	}

	size_t getVertexCount() const
	{
		return positions.size() / 3;
	}

	size_t getPositionBytes() const
	{
		return positions.size()*sizeof(GLfloat);
	}

	size_t getColorBytes() const
	{
		return colors.size()*sizeof(GLfloat);
	}

	size_t getElementBytes() const
	{
		return elements.size()*sizeof(GLuint);
	}

	glm::vec3 getMinBound()
	{
		return min;
	}

	glm::vec3 getMaxBound()
	{
		return max;
	}

	glm::vec3 getCentroid()
	{
		return center;
	}

	glm::vec3 getDimension()
	{
		return dim;
	}

private:
	void loadModel(glm::vec3 color)
	{
		objLoader loader;
		loader.load("/Users/misato/ROSE/CSSE351/1516a-csse351-groupG/resources/sphere.obj");
//		loader.load("/resources/sphere.obj");
	
		// push all vertecies into the positions vector
		for (size_t i = 0; i<loader.vertexCount; i++) {
			positions.push_back(loader.vertexList[i]->e[0]);
			positions.push_back(loader.vertexList[i]->e[1]);
			positions.push_back(loader.vertexList[i]->e[2]);
		}

		// push all faces to the elements vector
		for (size_t i = 0; i<loader.faceCount; i++) {
			if (loader.faceList[i]->vertex_count != 3) {
				fprintf(stderr, "Only triangle primitives are supported.\n");
				exit(1);
			}

			elements.push_back(loader.faceList[i]->vertex_index[0]);
			elements.push_back(loader.faceList[i]->vertex_index[1]);
			elements.push_back(loader.faceList[i]->vertex_index[2]);
		}

		// set color to red
		for (size_t i = 0; i < positions.size(); i++)
		{
			colors.push_back(color.r);
			colors.push_back(color.g);
			colors.push_back(color.b);
		}

		min = computeMinBound();
		max = computeMaxBound();
		center = computeCentroid();
		dim = computeDimension();
	}

	glm::vec3 computeMinBound()
	{
		glm::vec3 bound;

		for (int c = 0; c < 3; c++)
		{
			bound[c] = std::numeric_limits<float>::max();
		}
			
		for (int i = 0; i<positions.size(); i += 3)
		{
			for (int c = 0; c<3; c++)
			{
				if (positions[i + c] < bound[c])
				{
					
					bound[c] = positions[i + c];
				}
			}
		}

		return bound;
	}

	glm::vec3 computeMaxBound()
	{
		glm::vec3 bound;

		for (int c = 0; c<3; c++)
			bound[c] = -std::numeric_limits<float>::max();

		for (int i = 0; i<positions.size(); i += 3)
		{
			for (int c = 0; c<3; c++)
			{
				if (positions[i + c] > bound[c])
					bound[c] = positions[i + c];
			}
		}

		return bound;
	}

	glm::vec3 computeCentroid()
	{
		glm::vec3 center = glm::vec3(0);
		float positionCount = 1.0f / (positions.size() / 3.0f);

		for (int i = 0; i<positions.size(); i += 3)
		{
			center[0] += positions[i] * positionCount;
			center[1] += positions[i + 1] * positionCount;
			center[2] += positions[i + 2] * positionCount;
		}

		return center;
	}

	glm::vec3 computeDimension()
	{
		glm::vec3 max = getMaxBound();
		glm::vec3 min = getMinBound();
		glm::vec3 dim = max - min;
		return dim;
	}

	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<GLuint> elements;
	size_t objectCount;

	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 dim;
	glm::vec3 center;

	glm::mat4 basicTransforms;

	// for setup and draw
	GLuint vertexArray;
	GLuint shaderProg;

	std::string shaderName;
};

#endif