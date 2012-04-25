/**********************************************
*                                              *
*    Jeff Molofee's Revised OpenGL Basecode    *
*  Huge Thanks To Maxwell Sayles & Peter Puck  *
*            http://nehe.gamedev.net           *
*                     2001                     *
*                                              *
***********************************************/

//WM_MOUSEWHEEL is only defined in later versions of windows. 
//To have the identifier defined you'll need to put the line 
#define _WIN32_WINDOWS 0x501

#include <stdio.h>			// Header File For Standard Input/Output
#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library

#include "graphics.h"														// Header File For The NeHeGL Basecode
#include "VectorMath.h"	
#include "CElegans.h"	
#include"GL/glut.h"
#include "CNeuron.h"

/*
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4


float red=1.0, blue=1.0, green=1.0;
*/

extern CElegans * worm;

#define WM_TOGGLEFULLSCREEN (WM_USER+1)									// Application Define Message For Toggling

static BOOL g_isProgramLooping;											// Window Creation Loop, For FullScreen/Windowed Toggle																		// Between Fullscreen / Windowed Mode
static BOOL g_createFullScreen;											// If TRUE, Then Create Fullscreen

extern int mx,my;
extern bool ldown, rdown;
extern GLfloat  rx;			// ”гол поворта сцены вокруг оси X
extern GLfloat  ry;			// Y
extern GLfloat  rz;			// Z
//extern GLfloat  tx;			// —двиг по оси X
//extern GLfloat	ty;			// Y
//extern GLfloat	tz;			// Z
//extern GLint	tt;			// јктивна€ плоскось: 0 - XY, 1 - XZ
extern float scale;
int window_height = 0;
//int iter_counter = 5;// draw only one of 5 iterations in physical world to make everything faster
Vector3D vbeg(0,0,0);
Vector3D vend(0,0,0);
Vector3D ort1(1,0,0),ort2(0,1,0),ort3(0,0,1);
extern Vector3D vcenter;
extern GL_Window*	g_window;

GLuint	base;				// Base Display List For The Font Set
GLYPHMETRICSFLOAT gmf[256];	// Storage For Information About Our Outline Font Characters
HDC			hDC=NULL;		// Private GDI Device Context

/*
TextureImage texture[1];										// Storage For 10 Textures

bool LoadTGA(TextureImage *texture, char *filename)				// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if(	file==NULL ||											// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
			return FALSE;										// Return False
		else													// Otherwise
		{
			fclose(file);										// If Anything Failed, Close The File
			return FALSE;										// Return False
		}
	}

	texture->width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	texture->width	<=0	||									// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||									// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		return FALSE;											// Return False
	}

	texture->bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||								// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)							// Was Image Data Loaded
			free(texture->imageData);							// If So, Release The Image Data

		fclose(file);											// Close The File
		return FALSE;											// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	
	if (texture[0].bpp==24)										// Was The TGA 24 Bits
	{
		type=GL_RGB;											// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;												// Texture Building Went Ok, Return True
}
*/

/*
GLvoid BuildFont(GLvoid)										// Build Our Font Display List
{
	base=glGenLists(95);										// Creating 95 Display Lists
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Font Texture
	for (int loop=0; loop<95; loop++)							// Loop Through All 95 Lists
	{
		float cx=float(loop%16)/16.0f;							// X Position Of Current Character
		float cy=float(loop/16)/8.0f;							// Y Position Of Current Character

		glNewList(base+loop,GL_COMPILE);						// Start Building A List
			glBegin(GL_QUADS);									// Use A Quad For Each Character
				glTexCoord2f(cx,         1.0f-cy-0.120f); glVertex2i(0,0);	// Texture / Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f, 1.0f-cy-0.120f); glVertex2i(16,0);	// Texutre / Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f, 1.0f-cy);		  glVertex2i(16,16);// Texture / Vertex Coord (Top Right)
				glTexCoord2f(cx,         1.0f-cy);		  glVertex2i(0,16);	// Texture / Vertex Coord (Top Left)
			glEnd();											// Done Building Our Quad (Character)
			glTranslated(10,0,0);								// Move To The Right Of The Character
		glEndList();											// Done Building The Display List
	}															// Loop Until All 256 Are Built
}

GLvoid glPrint(GLint x, GLint y, const char *string, ...)		// Where The Printing Happens
{
	char		text[256];										// Holds Our String
	va_list		ap;												// Pointer To List Of Arguments

	if (string == NULL)											// If There's No Text
		return;													// Do Nothing

	va_start(ap, string);										// Parses The String For Variables
	    vsprintf(text, string, ap);								// And Converts Symbols To Actual Numbers
	va_end(ap);													// Results Are Stored In Text

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Select Our Font Texture
	glPushMatrix();												// Store The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	glTranslated(x,y,0);										// Position The Text (0,0 - Bottom Left)
	glListBase(base-32);										// Choose The Font Set
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);			// Draws The Display List Text
	glPopMatrix();												// Restore The Old Projection Matrix
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
//	glDeleteLists(base, 96);							// Delete All 96 Characters
}
*/



//extern int window_height;

/*
struct CVector3
{
  float x,y,z;
};*/

void calc_select_line(int mouse_x, int mouse_y, Vector3D& p1, Vector3D& p2)
{
	glLoadIdentity ();
  // mouse_x, mouse_y  - оконные координаты курсора мыши.
  // p1, p2            - возвращаемые параметры - концы селектирующего отрезка,
  //                     лежащие соответственно на ближней и дальней плоскост€х
  //                     отсечени€.
  GLint    viewport[4];    // параметры viewport-a.
  GLdouble projection[16]; // матрица проекции.
  GLdouble modelview[16];  // видова€ матрица.
  GLdouble vx,vy,vz;       // координаты курсора мыши в системе координат viewport-a.
  GLdouble wx,wy,wz;       // возвращаемые мировые координаты.

  //glScalef(scale,scale,scale);

  glGetIntegerv(GL_VIEWPORT,viewport);           // узнаЄм параметры viewport-a.
  glGetDoublev(GL_PROJECTION_MATRIX,projection); // узнаЄм матрицу проекции.
  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);   // узнаЄм видовую матрицу.
  // переводим оконные координаты курсора в систему координат viewport-a.
  vx = mouse_x;
  vy = window_height - mouse_y - 1; // где height - текуща€ высота окна.



  // вычисл€ем ближний конец селектирующего отрезка.
  vz = -1;
  gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
  p1 = Vector3D(wx,wy,wz);
  // вычисл€ем дальний конец селектирующего отрезка.
  vz = 1;
  gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
  p2 = Vector3D(wx,wy,wz);
}

GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-16,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);								// Address Of Buffer To Recieve Data							// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(GLint x, GLint y, const char *fmt, ...)					// Custom GL "Print" Routine
{
	/*
	float		length=0;								// Used To Find The Length Of The Text
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf_s(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	for (unsigned int loop=0;loop<(strlen(text));loop++)	// Loop To Find Text Length
	{
		length+=gmf[text[loop]].gmfCellIncX;			// Increase Length By Each Characters Width
	}

	glTranslatef(x,y,0.0f);					// Center Our Text On The Screen

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base);									// Sets The Base Character to 0
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();	
	*/
	// Pops The Display List Bits

	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	RECT window;												// Storage For Window Dimensions
	GetClientRect (g_window->hWnd,&window);						// Get Window Dimensions
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glDisable(GL_LIGHTING);
	glPushMatrix();												// Store The Projection Matrix
	glLoadIdentity();											// Reset The Projection Matrix
	glOrtho(0,window.right,0,window.bottom,0,1);				// Set Up An Ortho Screen
	//////////////////////////////////////////
	glTranslatef((GLfloat)x,(GLfloat)(window_height-y),-0.1f);	
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glPushMatrix();
	glRasterPos2f(0.1f,0.1f);
	//glTranslatef(x,y,-1.0f);	
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopMatrix();
	glPopAttrib();										// Pops The Display List Bits
	/////////////////////////////////////////
	glEnable(GL_LIGHTING);
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);
}


GLvoid glPrint2(GLdouble x, GLdouble y, GLdouble z, GLubyte r, GLubyte g, GLubyte b, const char *string, ...)		// Where The Printing Happens
{

//////////////////////////////////
  GLint    viewport[4];    // параметры viewport-a.
  GLdouble projection[16]; // матрица проекции.
  GLdouble modelview[16];  // видова€ матрица.
  //GLdouble vx,vy,vz;       // мировые координаты.
  GLdouble wx,wy,wz;       // возвращаемые координаты в системе координат viewport-a.

  //glScalef(scale,scale,scale);

  glGetIntegerv(GL_VIEWPORT,viewport);           // узнаЄм параметры viewport-a.
  glGetDoublev(GL_PROJECTION_MATRIX,projection); // узнаЄм матрицу проекции.
  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);   // узнаЄм видовую матрицу.
  // переводим оконные координаты курсора в систему координат viewport-a.
//  vx = mouse_x;
//  vy = window_height - mouse_y - 1; // где height - текуща€ высота окна.
  // вычисл€ем ближний конец селектирующего отрезка.
//  vz = -1;
  gluProject(x, y, z, modelview, projection, viewport, &wx, &wy, &wz);

  glColor3ub(0, 0, 0);
  glPrint((int)wx+1,window_height-(int)wy  ,string);
  glPrint((int)wx-1,window_height-(int)wy  ,string);
  glPrint((int)wx  ,window_height-(int)wy+1,string);
  glPrint((int)wx  ,window_height-(int)wy-1,string);

  glColor3ub(r, g, b);
  glPrint((int)wx,window_height-(int)wy,string);

///////////////////////////////////
/*	char		text[256];										// Holds Our String
	va_list		ap;												// Pointer To List Of Arguments

	if (string == NULL)											// If There's No Text
		return;													// Do Nothing

	va_start(ap, string);										// Parses The String For Variables
	    vsprintf(text, string, ap);								// And Converts Symbols To Actual Numbers
	va_end(ap);													// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);	// Position The Text (0,0 - Bottom Left)
	glPushMatrix();												// Store The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	//glTranslated(x,y,z);
	glTranslatef(wx,window_height-wy,-1.f);
	glListBase(base-32);
	glDisable(GL_LIGHTING);
	glRasterPos2f(0.03*scale,0.03*scale);// Choose The Font Set
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);			// Draws The Display List Text
	glEnable(GL_LIGHTING);
	glPopMatrix();	
	glPopAttrib();// Restore The Old Projection Matrix
*/
}

void TerminateApplication (GL_Window* window)							// Terminate The Application
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	g_isProgramLooping = FALSE;											// Stop Looping Of The Program
}

void ToggleFullscreen (GL_Window* window)								// Toggle Fullscreen/Windowed
{
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// Send A WM_TOGGLEFULLSCREEN Message
}

void ReshapeGL (int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	window_height = height;
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
					0.1f, 1000.0f);		
	//glOrtho(-10, 10, -10, 10, -10, 100);

	//glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix

	///////////////
	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);

	float ambient[4] = {0.7, 0.7, 0.7, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	*/
	//////////////////
}

BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// Display Change Failed, Return False
	}
	return TRUE;														// Display Change Was Successful, Return True
}

int winModeChanged = 0;

BOOL CreateWindowGL (GL_Window* window)									// This Code Creates Our OpenGL Window
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support float Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		window->init.bitsPerPixel,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// Define Our Window Coordinates

	GLuint PixelFormat;													// Will Hold The Selected Pixel Format

	if (window->init.isFullScreen == TRUE)								// Fullscreen Requested, Try Changing Video Modes
	{
		windowRect.right = window->init.width = 1280;
		windowRect.bottom = window->init.height = 1024;

		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			// Fullscreen Mode Failed.  Run In Windowed Mode Instead
			MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// Set isFullscreen To False (Windowed Mode)
		}
		else															// Otherwise (If Fullscreen Mode Was Successful)
		{
			//ShowCursor (FALSE);											// Turn Off The Cursor
			windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
			windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
		}																// (Top Window Covering Everything Else)
	}
	else																// If Fullscreen Was Not Selected
	{
		windowRect.right = window->init.width = 800;
		windowRect.bottom = window->init.height = 600;

		// Adjust Window, Account For Window Borders
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	


	// Create The OpenGL Window
	window->hWnd = CreateWindowEx (windowExtendedStyle,					// Extended Style
								   window->init.application->className,	// Class Name
								   window->init.title,					// Window Title
								   windowStyle,							// Window Style
								   0, 0,								// Window X,Y Position
								   windowRect.right - windowRect.left,	// Window Width
								   windowRect.bottom - windowRect.top,	// Window Height
								   HWND_DESKTOP,						// Desktop Is Window's Parent
								   0,									// No Menu
								   window->init.application->hInstance, // Pass The Window Instance
								   window);

	if (window->hWnd == 0)												// Was Window Creation A Success?
	{
		return FALSE;													// If Not Return False
	}

	window->hDC = GetDC (window->hWnd);									// Grab A Device Context For This Window
	hDC = window->hDC;
	if (window->hDC == 0)												// Did We Get A Device Context?
	{
		// Failed
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// Find A Compatible Pixel Format
	if (PixelFormat == 0)												// Did We Find A Compatible Format?
	{
		// Failed
		ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		// Failed
		ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	window->hRC = wglCreateContext (window->hDC);						// Try To Get A Rendering Context
	if (window->hRC == 0)												// Did We Get A Rendering Context?
	{
		// Failed
		ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		// Failed
		wglDeleteContext (window->hRC);									// Delete The Rendering Context
		window->hRC = 0;												// Zero The Rendering Context
		ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	if(winModeChanged==0)
	{
		double 	alpha = 180+40;
		ort1 = Vector3D::RotateVector1AroundVector2(ort1,Vector3D(0,0,1),alpha);
		ort2 = Vector3D::RotateVector1AroundVector2(ort2,Vector3D(0,0,1),alpha);
		ort3 = Vector3D::RotateVector1AroundVector2(ort3,Vector3D(0,0,1),alpha);
		
		alpha = -70;
		ort1 = Vector3D::RotateVector1AroundVector2(ort1,Vector3D(1,0,0),alpha);
		ort2 = Vector3D::RotateVector1AroundVector2(ort2,Vector3D(1,0,0),alpha);
		ort3 = Vector3D::RotateVector1AroundVector2(ort3,Vector3D(1,0,0),alpha);
	}
	
	winModeChanged++;


	ShowWindow (window->hWnd, SW_NORMAL);								// Make The Window Visible
	window->isVisible = TRUE;											// Set isVisible To True

	ReshapeGL (window->init.width, window->init.height);				// Reshape Our GL Window

	ZeroMemory (window->keys, sizeof (Keys));							// Clear All Keys

	window->lastTickCount = GetTickCount ();							// Get Tick Count

	return TRUE;														// Window Creating Was A Success
																		// Initialization Will Be Done In WM_CREATE
}
BOOL DestroyWindowGL (GL_Window* window)								// Destroy The OpenGL Window & Release Resources
{
	if (window->hWnd != 0)												// Does The Window Have A Handle?
	{	
		if (window->hDC != 0)											// Does The Window Have A Device Context?
		{
			wglMakeCurrent (window->hDC, 0);							// Set The Current Active Rendering Context To Zero
			if (window->hRC != 0)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext (window->hRC);							// Release The Rendering Context
				window->hRC = 0;										// Zero The Rendering Context

			}
			ReleaseDC (window->hWnd, window->hDC);						// Release The Device Context
			window->hDC = 0;											// Zero The Device Context
		}
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
	}

	if (window->init.isFullScreen)										// Is Window In Fullscreen Mode
	{
		ChangeDisplaySettings (NULL,0);									// Switch Back To Desktop Resolution
		ShowCursor (TRUE);												// Show The Cursor
	}	
	return TRUE;														// Return True
}


// Process Window Message Callbacks
// Process Window Message Callbacks
#define ID_POPUP_EXIT 40001
void createMenus(LPPOINT p,HWND hWnd,CNeuron * cn) {

	HMENU hPopupMenu = CreatePopupMenu();
	InsertMenuA(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_POPUP_EXIT, (LPCSTR)cn->getName().c_str());
	SetForegroundWindow(hWnd);
	TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, p->x, p->y, 0, hWnd, NULL);
}
void createSearchMenu(LPPOINT p,HWND hWnd){
	HMENU hPopupMenu = CreatePopupMenu();
	//int textbox_id = 156;
	HWND hWnd2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT","Modify me",WS_VISIBLE | WS_CHILD,p->x, p->y,100,20,hWnd,(HMENU)hPopupMenu/*textbox_id*/,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	SetForegroundWindow(hWnd);
	TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, p->x, p->y, 0, hWnd, NULL);
	//CreateWindowEx(0, "BUTTON", "&Push Me", WS_CHILD, p->x, p->y,80, 25, hWnd, 0, NULL, 0);
	//ShowWindow(hWnd2, SW_SHOW);
	//UpdateWindow(hWnd2);

}
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Get The Window Context
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	switch (uMsg)														// Evaluate Window Message
	{
		case WM_SYSCOMMAND:												// Intercept System Commands
		{
			switch (wParam)												// Check System Calls
			{
				case SC_SCREENSAVE:										// Screensaver Trying To Start?
				case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
				return 0;												// Prevent From Happening
			}
			break;														// Exit
		}
		return 0;														// Return

		case WM_CREATE:													// Window Creation
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Store Window Structure Pointer
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														// Return

		case WM_CLOSE:													// Closing The Window
			TerminateApplication(window);								// Terminate The Application
		return 0;														// Return

		case WM_SIZE:													// Size Action Has Taken Place
			switch (wParam)												// Evaluate Size Action
			{
				case SIZE_MINIMIZED:									// Was Window Minimized?
					window->isVisible = FALSE;							// Set isVisible To False
				return 0;												// Return

				case SIZE_MAXIMIZED:									// Was Window Maximized?
					window->isVisible = TRUE;							// Set isVisible To True
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// Reshape Window - LoWord=Width, HiWord=Height
				return 0;												// Return

				case SIZE_RESTORED:										// Was Window Restored?
					window->isVisible = TRUE;							// Set isVisible To True
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// Reshape Window - LoWord=Width, HiWord=Height
				return 0;												// Return
			}
		break;															// Break

		case WM_KEYDOWN:												// Update Keyboard Buffers For Keys Pressed
			if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
			{
				//if(wParam != 70){
					window->keys->keyDown [wParam] = TRUE;					// Set The Selected Key (wParam) To True
					return 0;												// Return
				//}
				/*else{
					LPPOINT lpPoint = new POINT;
					GetCursorPos(lpPoint); 
					createSearchMenu(lpPoint,hWnd);
				}*/

			}
		break;															// Break

		case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
			if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
			{
				window->keys->keyDown [wParam] = FALSE;					// Set The Selected Key (wParam) To False
				return 0;												// Return
			}
		break;															// Break

		case WM_MOUSEMOVE:
			{
				int x = (lParam << 16) >> 16;
				int y = lParam >> 16;

				if (ldown)		// Ћева€ кнопка
				{
					
					
					ry = (GLfloat)(y - my);	//»зменение угола поворота
					rx = (GLfloat)(x - mx);
					mx=x;
					my=y;

					//rx/= 10;
					//ry/= 10;
					//ry = 0;

					//ry = 0;
					//ry = 1;

					if(rx)
					{
						ort1 = Vector3D::RotateVector1AroundVector2(ort1,Vector3D(0,1,0),rx);
						ort2 = Vector3D::RotateVector1AroundVector2(ort2,Vector3D(0,1,0),rx);
						ort3 = Vector3D::RotateVector1AroundVector2(ort3,Vector3D(0,1,0),rx);
					}
					
					if(ry)
					{
						ort1 = Vector3D::RotateVector1AroundVector2(ort1,Vector3D(1,0,0),ry);
						ort2 = Vector3D::RotateVector1AroundVector2(ort2,Vector3D(1,0,0),ry);
						ort3 = Vector3D::RotateVector1AroundVector2(ort3,Vector3D(1,0,0),ry);
					}

					//if((rx==0)&&(ry==0))
					//	worm->UpdateSelection();

					//glutPostRedisplay();	//ѕерерисовать экран
				}

				if (rdown)	//ѕрава€
				{
					/*tx+=0.05f*(x-mx);	//ѕеремещение вдоль активной плоскости
					if (tt)
					tz+=0.05f*(y-my);
					else
					ty+=0.05f*(my-y);
					mx=x;
					my=y;*/

					

					ry = (GLfloat)(y - my);	
					rx = (GLfloat)(x - mx);
					mx=x;
					my=y;

					vcenter += Vector3D(rx,-ry,0)*scale/50;
					/*
					if(rx)
					{
						ort1 = Vector3D::RotateVector1AroundVector2(ort1,Vector3D(0,1,0),rx);
						ort2 = Vector3D::RotateVector1AroundVector2(ort2,Vector3D(0,1,0),rx);
						ort3 = Vector3D::RotateVector1AroundVector2(ort3,Vector3D(0,1,0),rx);
						//vbeg = Vector3D::RotateVector1AroundVector2(vbeg,Vector3D(0,1,0),rx);
						//vend = Vector3D::RotateVector1AroundVector2(vend,Vector3D(0,1,0),rx);
					}
					
					if(ry)
					{
						ort1 = Vector3D::RotateVector1AroundVector2(ort1,Vector3D(1,0,0),ry);
						ort2 = Vector3D::RotateVector1AroundVector2(ort2,Vector3D(1,0,0),ry);
						ort3 = Vector3D::RotateVector1AroundVector2(ort3,Vector3D(1,0,0),ry);
						//vbeg = Vector3D::RotateVector1AroundVector2(vbeg,Vector3D(1,0,0),ry);
						//vend = Vector3D::RotateVector1AroundVector2(vend,Vector3D(1,0,0),ry);
					}*/
					//glutPostRedisplay();
				}
			}
		break;

		case WM_LBUTTONUP:
			{
				ldown = false;
				/*mx = (lParam << 16) >> 16;
				my = lParam >> 16;*/
			}
		break;

		case WM_LBUTTONDOWN:
			{
				ldown = true;	//установить флаг

				mx = (lParam << 16) >> 16;
				my = lParam >> 16;
				//CheckSelection();
				calc_select_line(mx,my,vbeg,vend);
				CNeuron * cn = worm->UpdateSelection();
				//Block with contextMenu
				/*if(cn != NULL)
				{
					LPPOINT lpPoint = new POINT;
					GetCursorPos(lpPoint); 
					createMenus(lpPoint,hWnd,cn);
					ldown = false;
				}
				else
				{	
					
				}*/
				
			}
		break;

		case WM_RBUTTONUP:
			{
				rdown = false;
				/*mx = (lParam << 16) >> 16;
				my = lParam >> 16;*/
			}
		break;

		case WM_RBUTTONDOWN:
			{
				rdown = true;	//установить флаг
				mx = (lParam << 16) >> 16;
				my = lParam >> 16;
			}
		break;

		case WM_MOUSEWHEEL:
			{
				if((short)HIWORD(wParam)>0) scale*=1.05f;//"down"
				else
					scale/=1.05f;//"up"
				/*
				if(wParam == -7864320) scale*=1.1f;//"down"
				if(wParam ==  7864320) scale/=1.1f;//"up"*/
			}
		break;

		case WM_TOGGLEFULLSCREEN:										// Toggle FullScreen Mode On/Off
			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			PostMessage (hWnd, WM_QUIT, 0, 0);
		break;															// Break
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}

BOOL RegisterWindowClass (Application* application)						// Register A Window Class For This Application.
{																		// TRUE If Successful
	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WindowProc Handles Messages
	windowClass.hInstance		= application->hInstance;				// Set The Instance
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// Class Background Brush Color
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName	= application->className;				// Sets The Applications Classname
	if (RegisterClassEx (&windowClass) == 0)							// Did Registering The Class Fail?
	{
		// NOTE: Failure, Should Never Happen
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// Return False (Failure)
	}
	return TRUE;														// Return True (Success)
}
// Program Entry (WinMain)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application			application;									// Application Structure
	GL_Window			window;											// Window Structure
	Keys				keys;											// Key Structure
	BOOL				isMessagePumpActive;							// Message Pump Active?
	MSG					msg;											// Window Message Structure
	DWORD				tickCount;										// Used For The Tick Counter

	// Fill Out Application Data
	application.className = "OpenGL";									// Application Class Name
	application.hInstance = hInstance;									// Application Instance

	// Fill Out Window
	ZeroMemory (&window, sizeof (GL_Window));							// Make Sure Memory Is Zeroed
	window.keys					= &keys;								// Window Key Structure
	window.init.application		= &application;							// Window Application
	window.init.title			= "Physics Simulation Engine 1.0 beta, Palyanov Andrey 2007-2012";	// Window Title
	window.init.width			= 800;									// Window Width
	window.init.height			= 600;									// Window Height
	window.init.bitsPerPixel	= 16;									// Bits Per Pixel
	window.init.isFullScreen	= TRUE;									// Fullscreen? (Set To TRUE)

	ZeroMemory (&keys, sizeof (Keys));									// Zero keys Structure

	// Ask The User If They Want To Start In FullScreen Mode?
	//if (MessageBox (HWND_DESKTOP, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		window.init.isFullScreen = FALSE;								// If Not, Run In Windowed Mode
	}

	// Register A Class For Our Window To Use
	if (RegisterWindowClass (&application) == FALSE)					// Did Registering A Class Fail?
	{
		// Failure
		MessageBox (HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// Terminate Application
	}
	//int textbox_id = 156;
	

	g_isProgramLooping = TRUE;											// Program Looping Is Set To TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen Is Set To User Default
	while (g_isProgramLooping)											// Loop Until WM_QUIT Is Received
	{
		// Create A Window
		window.init.isFullScreen = g_createFullScreen;					// Set Init Param Of Window Creation To Fullscreen?
		if (CreateWindowGL (&window) == TRUE)							// Was Window Creation Successful?
		{
			// At This Point We Should Have A Window That Is Setup To Render OpenGL
			if (Initialize (&window, &keys) == FALSE)					// Call User Intialization
			{
				// Failure
				TerminateApplication (&window);							// Close Window, This Will Handle The Shutdown
			}
			else														// Otherwise (Start The Message Pump)
			{	// Initialize was a success
				isMessagePumpActive = TRUE;								// Set isMessagePumpActive To TRUE
				
				while (isMessagePumpActive == TRUE)						// While The Message Pump Is Active
				{
					
					// Success Creating Window.  Check For Window Messages
					if (PeekMessage (&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// Check For WM_QUIT Message
						if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
						{
							DispatchMessage (&msg);						// If Not, Dispatch The Message
						}
						else											// Otherwise (If Message Is WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// Terminate The Message Pump
						}
					}
					else												// If There Are No Messages
					{
						if (window.isVisible == FALSE)					// If Window Is Not Visible
						{
							WaitMessage ();								// Application Is Minimized Wait For A Message
						}
						else											// If Window Is Visible
						{
							// Process Application Loop
							
							tickCount = GetTickCount ();	// Get The Tick Count
							
							Update (tickCount - window.lastTickCount);	// Update The Counter
							window.lastTickCount = tickCount;			// Set Last Count To Current Count
							//iter_counter++;
							//if(iter_counter>=5)
							{
								//iter_counter = 0;
								
								Draw ();									// Draw Our Scene
							}
							
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							SwapBuffers (window.hDC);					// Swap Buffers (float Buffering)
						}
					}
				}														// Loop While isMessagePumpActive == TRUE
			}															// If (Initialize (...

			// Application Is Finished
			Deinitialize ();											// User Defined DeInitialization

			DestroyWindowGL (&window);									// Destroy The Active Window
		}
		else															// If Window Creation Failed
		{
			// Error Creating Window
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// Terminate The Loop
		}
	}																	// While (isProgramLooping)

	UnregisterClass (application.className, application.hInstance);		// UnRegister Window Class
	return 0;
}																		// End Of WinMain()
