#pragma once

#include <iostream>

#include "Simulator.h"

using namespace std;
class Viewer
{
public:
	Simulator	    S_Simulator;

public:
	float			m_Zoom;				// view zoom
	float			m_Rotate[2];		// x,y
	float			m_Translate[2];		// x,y
	int				m_Mouse_Coord[2];	// previous mouse coordinates
	unsigned char	m_Mouse_Event[3];	// mouse event handler
	bool			m_start;
	int				renderMode;
		
public:
	Viewer(void);
	~Viewer(void);

public:
	void			Initialize(void);
	void			Render(void);
	void	    	Reshape(int w, int h);
	void		    Mouse(int mouse_event, int state, int x, int y);
	void			Motion(int x, int y);
	void			Keyboard(unsigned char key, int x, int y);
	void			Update();
};

