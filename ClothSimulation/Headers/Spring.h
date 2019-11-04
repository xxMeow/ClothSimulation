#pragma once
 
#include <iostream>

#include "Node.h"

using namespace std;

class Spring
{
public:
    Node *node1;
    Node *node2;
	double initLength;
public:
	Spring(Node *n1, Node *n2)
	{
        node1 = n1;
        node2 = n2;
		
        Vec3 sLength = node2->currPos - node1->currPos;
        initLength = sLength.length();
	}

	void applyInternalForce() // Compute spring internal force
	{
        Vec3 diffVec = node2->currPos - node1->currPos;
        double currDist = diffVec.length();
        Vec3 posOffset = diffVec * (1 - initLength/currDist) / 2;
        
        if (!node1->isFixed && !node2->isFixed) {
            node1->currPos += posOffset;
            node2->currPos -= posOffset;
        } else if (node1->isFixed && !node2->isFixed) {
            node2->currPos -= posOffset*2;
        } else if (!node1->isFixed && node2->isFixed) {
            node1->currPos += posOffset*2;
        }
        
	}
};
