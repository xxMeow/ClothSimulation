#include "Simulator.h"

using namespace std;
Simulator::Simulator()
{
	
}
Simulator::~Simulator()
{

}


void Simulator::Initialize()
{
	timsStep = 0.01;
	ground =vec3(0.0,-15,0.0);
	cloth = new mass_cloth();

	cloth->dx = 1;
	cloth->dy = 1;
	cloth->dz = 1;
	cloth->size_x = 50;
	cloth->size_y = 50;
	cloth->size_z = 1;
	cloth->structural_coef = 1500;
	cloth->shear_coef = 50;
	cloth->bending_coef = 1000;
	cloth->iteration_n = 20; 
	cloth->drawMode = 0;
	
	cloth->init(); 
}

void Simulator::Update()
{
	vec3 gravity(0.0, -9.8 / cloth->iteration_n, 0.0);
		 
	for (int iter = 0; iter < cloth->iteration_n; iter++)
	{
		cloth->compute_force(timsStep, gravity);
		cloth->integrate(timsStep);
		cloth->collision_response(ground);
	}

	cloth->computeNormal();
}
 
 
void Simulator::Render()
{
	//Lighting();
  	DrawGround();
 	cloth->draw();
}

void Simulator::Lighting()
{
//	glShadeModel(GL_SMOOTH);
//
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_COLOR_MATERIAL);

	float light_pos[] = {150.0, 150.0, 0.0, 1.0 };
	float light_dir[] = { -1, -1, 0 , 0.0};
	float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float frontColor[] = { 0.8, 0.8, 0.8, 1 };

	float matShininess = 20;
	float noMat[] = { 0, 0, 0, 1 };

	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
//	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);					// Set ambient property frontColor.
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);					// Set diffuse property frontColor.
//	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
//
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF , 80.0f );                  // 80도 원뿔
//	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80.0f );                 // 초점 설정
//	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
void Simulator::DrawGround(void){
	
//	glBegin(GL_QUADS);
//	glColor3f(1.0, 1.0, 1.0);
	 
	for(int x = 0; x<128;x++){
		for(int y = 0; y<128;y++){
//			glNormal3f(0.0,1.0,0.0);
//			glVertex3f(- 250.0f+250.0f/64*x, ground.y, -250.0f+250.0f/64*y);
//			glVertex3f(- 250.0f+250.0f/64*(x+1), ground.y, -250.0f+250.0f/64*y);
//			glVertex3f(- 250.0f+250.0f/64*(x+1), ground.y, -250.0f+250.0f/64*(y+1));
//			glVertex3f(- 250.0f+250.0f/64*x, ground.y, -250.0f+250.0f/64*(y+1));
		}
	}
//	glEnd();
}

