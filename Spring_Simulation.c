/*----------------------------------------------------------------------------*/
/* Include files                                                              */
/*----------------------------------------------------------------------------*/
#include <ansi_c.h>
#include <advanlys.h>
#include <cvirte.h>
#include <userint.h>
#include "Spring_Simulation.h"
/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#define dt 0.000001
#define g 9.81
#define timer_tick 0.02
/*----------------------------------------------------------------------------*/
/* Function prototypes                          		                      */
/*----------------------------------------------------------------------------*/
void update();
void draw();
void Fourier();
void terminate();
void Initialize();

/*----------------------------------------------------------------------------*/
/* Module-globals                                                             */
/*----------------------------------------------------------------------------*/
static int panelHandle;

typedef struct
{
	int top, left, hight , width
} Canvas;
typedef struct
{
	int top, left, hight , width, bottom, bitmap;
	double k, y, Vy, Ay
} Spring;
typedef struct
{
	int top, left, hight , width, bitmap;
	double mass
} Wheight;

Canvas canvas;
Spring s, s2;
Wheight m, m2;

// Fourier
double ft_real[50000]= {0}, ft_imagin[50000]= {0.0}, ft_X[50000], ft_Y[50000];
//Graphs
double stripch[2];
//
int counter=0;

/*----------------------------------------------------------------------------*/
/* 						MAIN and UIR										  */
/*----------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Spring_Simulation.uir", PANEL)) < 0)
		return -1;
	Initialize();
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	terminate();
	return 0;
}
/*----------------------------------------------------------------------------*/
/* 						Functions                                             */
/*----------------------------------------------------------------------------*/
void Initialize()
{
	GetCtrlBoundingRect (panelHandle, PANEL_CANVAS, &canvas.top, &canvas.left, &canvas.hight, &canvas.width);

	/* spring image size:	hight 240		width 80		*/
	s.top=0;
	s.left=200;
	s.hight=240;
	s.width=80;
	s.y=0.0;
	GetBitmapFromFile("Images\\spring.png", &s.bitmap);

	s2.top=240;
	s2.left=s.left;
	s2.hight=240;
	s2.width=80;
	s2.y=0.0;
	GetBitmapFromFile("Images\\spring.png", &s2.bitmap);

	/* wheight image size:	hight 200		width 160		*/
	m.top=0;
	m.left=0;
	m.hight=200;
	m.width=160;
	GetBitmapFromFile("Images\\weight.png", &m.bitmap);

	GetCtrlVal (panelHandle, PANEL_OFFSET, &s.y);
	GetCtrlVal (panelHandle, PANEL_OFFSET, &s2.y);

	draw();
}

void update()
{
	double ay1, ay2;

	for (int i=0; i<20000; i++)
	{
		ay1=g-(s.y*s.k/m.mass);
		s.y=s.y + s.Vy*dt;
		s.Vy=s.Vy+ ay1*dt;

		ay2=g-(s2.y*s2.k/m.mass);
		s2.y=s2.y+s2.Vy*dt;
		s2.Vy=s2.Vy + ay2*dt;
	}

	stripch[0]= s.y;
	stripch[1]= s2.y;
	ft_real[counter]=s.y+s2.y;
	counter++;
}

void draw()
{
	CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);

	CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);

	//CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect(0, 0, (double)canvas.hight, (double)canvas.width), VAL_DRAW_FRAME);  // just draw the canvas outline

	//spring 1
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, s.bitmap, VAL_ENTIRE_OBJECT,
					  MakeRect(s.top,
							   s.left,
							   s.hight+s.y,
							   s.width));
	//spring 2
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, s2.bitmap, VAL_ENTIRE_OBJECT,
					  MakeRect(s.hight+s.y,
							   s2.left,
							   s2.hight+s2.y,
							   s2.width));
	//mass at end
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, m.bitmap, VAL_ENTIRE_OBJECT,
					  MakeRect(s.hight+s2.top+s.y+s2.y-5,
							   s2.left+(s2.width-m.width)*0.5,
							   m.hight,
							   m.width));
	

	//CanvasDrawBitmap (panelHandle, PANEL_CANVAS, m.bitmap, VAL_ENTIRE_OBJECT,MakeRect(s.hight+s.y-10, (canvas.width/2)-80, m.hight, m.width));
	CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
	PlotStripChart (panelHandle, PANEL_STRIPCHART, stripch, 2, 0, 0, VAL_DOUBLE);
}

void Fourier()
{
	DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
	ReFFT(ft_real, ft_imagin, counter);
	for (int i=0; i<=counter; i++)
	{
		ft_Y[i]=sqrt((ft_imagin[i]*ft_imagin[i])+(ft_real[i]*ft_real[i]));
		ft_X[i]=i/(counter*timer_tick);
	}
	PlotXY (panelHandle, PANEL_GRAPH, ft_X, ft_Y, counter, VAL_DOUBLE, VAL_DOUBLE, VAL_FAT_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_CYAN);
}

int CVICALLBACK TimerFunc (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			update();
			draw();
			break;
	}
	return 0;
}

int CVICALLBACK offsetFunc (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			GetCtrlVal (panelHandle, PANEL_OFFSET, &s.y);
			GetCtrlVal (panelHandle, PANEL_OFFSET_2, &s2.y);
			draw();
			break;
	}
	return 0;
}

int CVICALLBACK startFunc (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_MASS, &m.mass);

			GetCtrlVal (panelHandle, PANEL_K, &s.k);
			GetCtrlVal (panelHandle, PANEL_K_2, &s2.k);

			SetCtrlAttribute (panelHandle, PANEL_START_BUTTON, ATTR_VISIBLE, 0);
			SetCtrlAttribute (panelHandle, PANEL_PAUSE_BUTTON, ATTR_VISIBLE, 1);
			SetCtrlAttribute (panelHandle, PANEL_RESET_BUTTON, ATTR_VISIBLE, 1);
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
			DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			SetCtrlAttribute (panelHandle, PANEL_TEXTMSG_3, ATTR_VISIBLE, 1);

			break;
	}
	return 0;
}

int CVICALLBACK pauseFunc (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			SetCtrlAttribute (panelHandle, PANEL_PAUSE_BUTTON, ATTR_VISIBLE, 0);
			SetCtrlAttribute (panelHandle, PANEL_START_BUTTON, ATTR_VISIBLE, 1);
			SetCtrlAttribute (panelHandle, PANEL_TEXTMSG_3, ATTR_VISIBLE, 0);
			Fourier();
			break;
	}
	return 0;
}

int CVICALLBACK resetFunc (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			SetCtrlAttribute (panelHandle, PANEL_PAUSE_BUTTON, ATTR_VISIBLE, 0);
			SetCtrlAttribute (panelHandle, PANEL_RESET_BUTTON, ATTR_VISIBLE, 0);
			SetCtrlAttribute (panelHandle, PANEL_START_BUTTON, ATTR_VISIBLE, 1);

			for (int i=0; i<=counter; i++)
				ft_real[i]=ft_imagin[i]=ft_X[i]=ft_Y[i]=0.0;

			s.y=s2.y=counter=0;
			s.Vy=s2.Vy=0.0;
			draw();
			ClearStripChart (panelHandle, PANEL_STRIPCHART);
			DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			break;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
/* Shut down.						  										  */
/*----------------------------------------------------------------------------*/
int CVICALLBACK exitFunc (int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

void terminate()
{
	DiscardBitmap(s.bitmap);
	DiscardBitmap(s2.bitmap);
	DiscardBitmap(m.bitmap);
}

