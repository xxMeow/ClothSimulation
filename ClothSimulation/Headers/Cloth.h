#pragma once

#include <stdio.h>
#include <math.h>

#include <vector>
#include <algorithm>

#include "Node.h"
#include "Spring.h"

class Cloth
{
public:
    const int nodesDensity = 4; // Should be even number?
    
    int width, height;
    int nodesPerRow, nodesPerCol;
    
	std::vector<Node*> nodes;
	std::vector<Spring*> springs;
	std::vector<Node*> faces;

//    double structuralCoef;
//    double shearCoef;
//    double bendingCoef;
//
//	int		size_x, size_y, size_z;
//	double	dx, dy, dz;
//
//	int		iteration_n;
//	int		drawMode;

	Cloth(int w, int h)
	{
        width = w;
        height = h;
        init();
	}
	~Cloth()
	{ 
		for (int i = 0; i < nodes.size(); i++){ delete nodes[i]; }
		for (int i = 0; i < springs.size(); i++){ delete springs[i]; }
		nodes.clear();
		springs.clear();
		faces.clear();
	}
	enum DrawModeEnum{
		DRAW_MASS_NODES,
		DRAW_SPRINGS,
		DRAW_FACES
	};
 
public:
    Node* getNode(int x, int y) { return nodes[y*nodesPerRow+x]; }
    Vec3 getFaceNormal(Node* n1, Node* n2, Node* n3)
    {
        return Vec3::cross(n2->currPos - n1->currPos, n3->currPos - n1->currPos);
    }
    
	void init()
	{
        nodesPerRow = width * nodesDensity;
        nodesPerCol = height * nodesDensity;
        
        /** Add nodes **/
        printf("Init cloth with %d nodes\n", nodesPerRow*nodesPerCol);
        for (int i = 0; i < nodesPerRow; i ++) {
            for (int j = 0; j < nodesPerCol; j ++) {
                /** Create node by position **/
                Node* node = new Node(Vec3((double)j/nodesDensity, -((double)i/nodesDensity), 0));
                /** Set texture coordinates **/
                node->texCoord.x = (double)j/(nodesPerRow-1);
                node->texCoord.y = (double)i/(nodesPerCol-1);
                /** Add node to cloth **/
                nodes.push_back(node);
                
                printf("\t[%d, %d] (%f, %f, %f) - (%f, %f)\n", i, j, node->currPos.x, node->currPos.y, node->currPos.z, node->texCoord.x, node->texCoord.y);
            }
            std::cout << std::endl;
        }
        
        /** Add springs **/
        for (int i = 0; i < nodesPerRow; i ++) {
            for (int j = 0; j < nodesPerCol; j ++) {
                /** Structural **/
                if (i < nodesPerRow-1) springs.push_back(new Spring(getNode(i, j), getNode(i+1, j)));
                if (j < nodesPerCol-1) springs.push_back(new Spring(getNode(i, j), getNode(i, j+1)));
                /** Shear **/
                if (i < nodesPerRow-1 && j < nodesPerCol-1) {
                    springs.push_back(new Spring(getNode(i, j), getNode(i+1, j+1)));
                    springs.push_back(new Spring(getNode(i+1, j), getNode(i, j+1)));
                }
                /** Bending **/
                if (i < nodesPerRow-2) springs.push_back(new Spring(getNode(i, j), getNode(i+2, j)));
                if (j < nodesPerCol-2) springs.push_back(new Spring(getNode(i, j), getNode(i, j+2)));
                if (i < nodesPerRow-2 && j < nodesPerCol-2) {
                    springs.push_back(new Spring(getNode(i, j), getNode(i+2, j+2)));
                    springs.push_back(new Spring(getNode(i+2, j), getNode(i, j+2)));
                }
            }
        }
        
        /** Pin the cloth **/
        Vec3 pinOffset(1.0, 0.0, 0.0);
        getNode(0, 0)->currPos += pinOffset;
        getNode(0, 0)->isFixed = true;
        getNode(nodesPerRow-1, 0)->currPos -= pinOffset;
        getNode(nodesPerRow-1, 0)->isFixed = true;
        
		/** Triangle faces **/
        for (int i = 0; i < nodesPerRow-1; i ++) {
            for (int j = 0; j < nodesPerCol-1; j ++) {
                // Left upper triangle
                faces.push_back(getNode(i+1, j));
                faces.push_back(getNode(i, j));
                faces.push_back(getNode(i, j+1));
                // Right bottom triangle
                faces.push_back(getNode(i+1, j+1));
                faces.push_back(getNode(i+1, j));
                faces.push_back(getNode(i, j+1));
            }
        }
	}
	
	void computeNormal()
	{
        /** Reset nodes' normal **/
        Vec3 normal(0.0, 0.0, 0.0);
        for (int i = 0; i < nodes.size(); i ++) {
            nodes[i]->normal = normal;
        }
        /** Compute normal of each face **/
        for (int i = 0; i < faces.size()/3; i ++) { // 3 nodes in each face
            Node* n1 = faces[3*i+0];
            Node* n2 = faces[3*i+1];
            Node* n3 = faces[3*i+2];
            
            // Face normal
            normal = Vec3::cross(n2->currPos - n1->currPos, n3->currPos - n1->currPos);
            // Add all face normal
            n1->normal += normal;
            n2->normal += normal;
            n3->normal += normal;
        }
	}
	
	void addForce(Vec3 force)
	{		 
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->addForce(force);
		}
	}

//	void computeForce(double dt, Vec3 gravity)
//	{
//		for (int i = 0; i < nodes.size(); i++)
//		{
//			nodes[i]->addForce(gravity * nodes[i]->mass);
//		}
//		/* Compute Force for all springs */
//		for (int i = 0; i < springs.size(); i++)
//		{
//			springs[i]->applyInternalForce();
//		}
//	}

	void integrate(double airFriction, double timeStep)
	{
        /** Node **/
        for (int i = 0; i < nodes.size(); i++)
        {
            nodes[i]->integrate(airFriction, timeStep);
        }
        /** Spring **/
        for (int i = 0; i < 25; i ++) {
            for (int i = 0; i < springs.size(); i ++) {
                springs[i]->applyInternalForce();
            }
        }
	}
	
	void collision_response(Vec3 ground) // Assume ground is Vec3(7, -5, 0);
	{
		//Basic Implements 4. Collision Check with ground
        /** Collision check with ground **/
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->currPos.y <= ground.y) {
                nodes[i]->currPos.y = ground.y;
            }
        }
        
        //Additional Implements 2. Collision Check with Sphere
        //Additional Implements 3. Collision Check with Mesh Object
        /*
            if(Collision Detection)
            {
                Collision Response
            }
        */
	}

//	void draw();
};
