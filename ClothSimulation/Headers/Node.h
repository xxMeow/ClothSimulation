#pragma once

#include <stdio.h>
#include <math.h>

#include <vector>
#include <algorithm>

#include "Vector.h"

class Node
{
public:
    double  mass;           // In this project it will always be 1
    bool    isFixed;        // Use to pin the cloth
    Vec2    texCoord;       // Texture coord
    Vec3    normal;         // TODO: For shading
	Vec3	position;
    Vec3    velocity;
    Vec3    force;
	Vec3	acceleration;

public:
    Node(void) {
        mass = 1.0;
        isFixed = false;
        velocity.setZeroVec();
        force.setZeroVec();
        acceleration.setZeroVec();
    }
	Node(Vec3 pos)
    {
        mass = 1.0;
        isFixed = false;
        position = pos;
        velocity.setZeroVec();
        force.setZeroVec();
        acceleration.setZeroVec();
    }
	~Node(void) {}

	void addForce(Vec3 f)
	{
        force += f;
	}

	void integrate(double timeStep) // Only non-fixed nodes take integration
	{
		if (!isFixed) // Verlet integration
		{
//            Vec3 tempPos = position;
//            position = position + (position-prevPos)*(1-airFriction) + acceleration*pow(timeStep, 2);
//            prevPos = tempPos;
//            acceleration = Vec3(0.0, 0.0, 0.0); // Reset acceleration
            acceleration = force/mass;
            velocity += acceleration*timeStep;
            position += velocity*timeStep;
        }
        force.setZeroVec();
	}

//	void draw();
};
