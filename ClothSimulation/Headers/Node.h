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
	Vec3	currPos;
	Vec3	prevPos;
    Vec3    normal;         // TODO: For shading
	Vec3	acceleration;

public:
    Node(void) {}
	Node(Vec3 pos)
    {
        mass = 1.0;
        isFixed = false;
        currPos = pos;
    }
	~Node(void) {}

	void addForce(Vec3 force)
	{
        acceleration += force/mass;
	}

	void integrate(double airFriction, double timeStep) // Only non-fixed nodes take integration
	{
		if (!isFixed) // Verlet integration
		{
            Vec3 tempPos = currPos;
            currPos = currPos + (currPos-prevPos)*(1-airFriction) + acceleration*pow(timeStep, 2);
            prevPos = tempPos;
            acceleration = Vec3(0.0, 0.0, 0.0); // Reset acceleration
        }
	}

//	void draw();
};
