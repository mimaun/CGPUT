#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "WorldModel.h"
#include "TrackBall.h"

#define NUM_TRACKED_FRAMES 10

/*
--------------------------------------------------------------------------------------------------
World state represents the state of the whole world. As its main attribute it has the world model.
--------------------------------------------------------------------------------------------------
*/

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	bool running;
	WorldModel worldModel;
	TrackBall trackball;
	
	glm::mat4 translateToOrigin;
	glm::mat4 currentModelTransform;
	
	bool moveCameraForwardEnabled = false;
	bool moveCameraBackwardEnabled = false;
	bool moveCameraLeftEnabled = false;
	bool moveCameraRightEnabled = false;

	glm::vec3 e  = glm::vec3(0, 0, 3);
	glm::vec3 c = glm::vec3(0, 0, 0);
	glm::vec3 u = glm::vec3(0, 1, 0);
	glm::mat4 C = glm::lookAt(e, c, u);


public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		
		running = true;
		worldModel = WorldModel();
		
		//
		// TODO figure out how to get the cube to the origin
		//
		//translateToOrigin = glm::translate(glm::mat4(1.0f), -worldModel.getCentroid());
		//currentModelTransform = translateToOrigin;
	}
	
	void updateFrameTime(float timeAsSeconds)
	{
		for(size_t i=1; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = frameTimes[i-1];
		frameTimes[0] = timeAsSeconds;
	}
	
	void printFPS() const
	{
		float sum = 0.0f;
		float avg = 0.0f;
		float fps = 0.0f;
		
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			sum += frameTimes[i];
		
		avg = sum / NUM_TRACKED_FRAMES;
		fps = 1.0f / avg;
		printf("fps %f\n", fps);
	}
	
	WorldModel const & getWorldModel() const
	{ return worldModel; }
	
	void setRunning(bool r)
	{ running = r; }

	bool isRunning() const
	{ return running; }

	float getCurrentTime() const
	{ return this->currentTime; }

	void timeStep(float t)
	{
		float elapsed = t - this->currentTime;
		this->updateFrameTime(elapsed);
		
		if (moveCameraForwardEnabled) {
			moveCameraForward();
		}
		if (moveCameraBackwardEnabled) {
			moveCameraBackward();
		}
		if (moveCameraLeftEnabled) {
			moveCameraLeft();
		}
		if (moveCameraRightEnabled) {
			moveCameraRight();
		}
		this->currentTime = t;
	}
	
	void updateRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		float phi;
		glm::vec3 axis;
		#define ROT_SENSITIVITY 2.0f // might be helpful to scale rotation angle
		
		//computes the appropriate rotation data and stores in phi and axis
		trackball.getRotation(phi, axis, oldPos, newPos);
		
		// do rotation
		glm::mat4 newRotation = glm::rotate(glm::mat4(1.0f), phi * ROT_SENSITIVITY, axis);
		currentModelTransform = glm::inverse(C) * newRotation * C * currentModelTransform;
	}

	void updateRoll() {
		
	}
	

	void updateMoveCameraForward(bool enabled) {
		moveCameraForwardEnabled = enabled;
	}
	void moveCameraForward()
	{
		glm::mat4 newTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.04));
		currentModelTransform = newTranslation * currentModelTransform;
	}

	void updateMoveCameraBackward(bool enabled) {
		moveCameraBackwardEnabled = enabled;
	}
	void moveCameraBackward()
	{
		glm::mat4 newTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.04));
		currentModelTransform = newTranslation * currentModelTransform;
	}

	void updateMoveCameraLeft(bool enabled) {
		moveCameraLeftEnabled = enabled;
	}
	void moveCameraLeft() {
		glm::mat4 newTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0.04, 0, 0));
		currentModelTransform = newTranslation * currentModelTransform;
	}

	void updateMoveCameraRight(bool enabled) {
		moveCameraRightEnabled = enabled;
	}
	void moveCameraRight() {
		glm::mat4 newTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(-0.04, 0, 0));
		currentModelTransform = newTranslation * currentModelTransform;
	}
	
	void setSize(unsigned int x, unsigned int y)
	{
		trackball.setSize(x, y);
	}
	
	WorldModel & getWorldModel()
	{ return worldModel; }
	
	glm::mat4 getCurrentModelTransform() const
	{ return currentModelTransform; }
};

#endif
