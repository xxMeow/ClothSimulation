#pragma once

#include <stdio.h>
#include <math.h>

#include <vector>
#include <algorithm>

#include "Spring.h"
#include "Node.h"

class mass_cloth
{
public:

	std::vector<Node *> nodes;
	std::vector<mass_spring *> spring;
	std::vector<Node*> faces;

	int			size_x, size_y, size_z;
	double		dx, dy, dz;
	double		structural_coef;
	double		shear_coef;
	double		bending_coef;
	int			iteration_n;
	int			drawMode;
	

	mass_cloth()
	{ 	 
	}
	~mass_cloth()
	{ 
		for (int i = 0; i < nodes.size(); i++){ delete nodes[i]; }
		for (int i = 0; i < spring.size(); i++){ delete spring[i]; }
		nodes.clear();
		spring.clear();
		faces.clear();
	}
	enum DrawModeEnum{
		DRAW_MASS_NODES,
		DRAW_SPRINGS,
		DRAW_FACES
	};
 
public:
	void init()
	{
		//Basic Implements 1. Init Nodes and Shear and Structural Springs
		//Additional Implements 1. Init Bending Spring
		/*
			Node *xp = new Node(vec3(x, y, z));
		
			mass_spring *sp = new mass_spring(p[Node_Index_A], p[Node_Index_B]);
 			sp->spring_coef = spring_Type_coef;
 			spring.push_back(sp);
		*/
		//Basic Implements 3-1. Generate Faces
		/*
			faces.push_back(p[Node_Index_A]);
			faces.push_back(p[Node_Index_C]);
			faces.push_back(p[Node_Index_B]);
		*/
		//Additional Implements 4-2. Initialize Texture Coordinates	
	}
	
	void computeNormal()
	{
		//Basic Implements 3-2. Compute Vertex Normal
		/*
			for(each face)
			{
				compute face normal
			}
			for(each node)
			{
				인접한 face의 평균 normal
			}
		*/
	}
	
	void add_force(vec3 additional_force)
	{		 
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->add_force(additional_force);
		}
	}

	void compute_force(double dt, vec3 gravity)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->add_force(gravity * nodes[i]->mass);
		}
		/* Compute Force for all springs */
		for (int i = 0; i < spring.size(); i++)
		{
			spring[i]->internal_force(dt);
		}
	}

	void integrate(double dt)
	{
		/* integrate Nodes*/
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->integrate(dt);
		}
	}
	
	void collision_response(vec3 ground)
	{
		//Basic Implements 4. Collision Check with ground
		//Additional Implements 2. Collision Check with Sphere
		//Additional Implements 3. Collision Check with Mesh Object
		/*
			if(Collision Detection)
			{
				Collision Response
			}
		*/

	}

	void draw();
};
