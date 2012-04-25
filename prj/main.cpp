//#define _CRT_SECURE_NO_DEPRECATE 1
//#define _CRT_NONSTDC_NO_DEPRECATE 1

#include <windows.h>														// Header File For Windows
#include <gl\gl.h>															// Header File For The OpenGL32 Library
#include <gl\glu.h>															// Header File For The GLu32 Library
#include "gl\glaux.h"	
#include "graphics.h"
#include "VectorMath.h"														// Header File For Physics2.h
#include "CPoint.h"
#include "CConnector.h"
#include "CNeuron.h"
#include "CElegans.h"
#include "fstream"
#include <sstream>
#define MaxPossible_dt 0.009f
//0.016000001
#pragma comment( lib, "opengl32.lib" )										// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )											// Search For GLu32.lib While Linking
#pragma comment( lib, "glaux.lib" )			


#ifndef CDS_FULLSCREEN														// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4													// Compilers. By Defining It This Way,
#endif

extern int window_height;
extern Vector3D ort1,ort2,ort3;

//extern TextureImage texture[1];									

//extern bool LoadTGA(TextureImage *texture, char *filename);   // Загрузка TGA файла в память

GLuint	myTexture[1];		// Место для одной текстуры

GL_Window*	g_window;
Keys*		g_keys;

///////////////////////////////////////

CElegans * worm = new CElegans(0.5, 26);

AUX_RGBImageRec *texture1;

#define GLUT_LEFT_BUTTON  1
#define GLUT_RIGHT_BUTTON 2
#define GLUT_DOWN 1
#define GLUT_UP   0

GLfloat  rx=0;		// Угол поворта сцены вокруг оси X
GLfloat  ry=0;		// Y
GLfloat  rz=0;		// Z

unsigned mouse_button = 0;
int mouse_state  = 0;
int mx,my;				// Координаты мыши
bool ldown=false;		// Нажата левая клавиша мыши?
bool rdown=false;		// Нажата правая клавиша мыши?
Vector3D vcenter(0,0,-8);
Vector3D pos_rc(0,0,0);



//float c=0.02f;
float scale = 1.00f;//0.25

int meet_obstacle = 0;
float direction = 1.0f;

bool mode = 1;
bool mode2 = 0;
bool mode3 = 0;
bool mode4 = 0;
bool f9 = false;
bool f1 = false;
bool f10 = false;
bool f11 = false;
bool f6 = true;
bool key_a = false;
bool key_a_prev = false;
bool key_r = false;
bool key_r_prev = false;

extern GLvoid BuildFont();	
extern GLvoid KillFont();

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("Data/sand.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &myTexture[0]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, myTexture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
}


BOOL Initialize (GL_Window* window, Keys* keys)								// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;

	// Build Our Font Display List

	glClearColor(0.3f, 0.3f, 0.5f, 0.0f);						// Black Background
	glClearDepth(1.0f);											// Depth Buffer Setup
	glShadeModel (GL_SMOOTH);	
	glDepthFunc(GL_LEQUAL);										// Type Of Depth Testing
	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	BuildFont();	

// Загрузка картинки
	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}
//////////////////////////////////////////
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);

	float ambient[4] = {1.0, 1.0, 1.0, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	worm->ClearSelection();

	worm->rotateWormAroundAnterPosterAxis(90.f);


	return TRUE;															// Return TRUE (Initialization Successful)
}

void Deinitialize (void)													// Any User DeInitialization Goes Here
{
	KillFont();
}

void Update (DWORD milliseconds)											// Perform Motion Updates Here
{

		// Keys Are Used To Move The Rope
		
	if (g_keys->keyDown [VK_ESCAPE] == TRUE)								// Is ESC Being Pressed?
		TerminateApplication (g_window);									// Terminate The Program

	if (g_keys->keyDown [VK_F2] == TRUE)									// Is F1 Being Pressed?
		ToggleFullscreen (g_window);										// Toggle Fullscreen Mode

	if (g_keys->keyDown [VK_F1] == TRUE)
	{
		g_keys->keyDown [VK_F1] = false;
		f1 = !f1;
	}

	if (g_keys->keyDown [VK_F5]){									
		g_keys->keyDown [VK_F5] = false;
		mode = !mode;
	}

	if (g_keys->keyDown [VK_F6]){
		g_keys->keyDown [VK_F6] = false;
		mode2 = !mode2;
		f6 = false;
	}

	if (g_keys->keyDown [VK_F7]){
		g_keys->keyDown [VK_F7] = false;
		mode3 = !mode3;
		mode4 = false;
		f6 = false;
	}

	if (g_keys->keyDown [VK_F8]){
		g_keys->keyDown [VK_F8] = false;
		mode4 = !mode4;
		mode3 = false;
		f6 = false;
	}

	if (g_keys->keyDown [VK_F2]){
		g_keys->keyDown [VK_F2] = false;
		f9 = !f9;
	}

	if (g_keys->keyDown [VK_F3]){
		g_keys->keyDown [VK_F3] = false;
		f10 = !f10;
	}

	if (g_keys->keyDown [VK_F4]){
		g_keys->keyDown [VK_F4] = false;
		f11 = !f11;
	}

	/////// KEY A //////////////
	if (g_keys->keyDown [65])
	{
		if(key_a_prev) 
			key_a = true;

		key_a_prev = true;
	}
	else
	{
		if(!key_a_prev) 
			key_a = false;

		key_a_prev = false;
	}

	/////// KEY R //////////////
	if (g_keys->keyDown [82])
	{
		if(key_r_prev) 
			key_r = true;

		key_r_prev = true;
	}
	else
	{
		if(!key_r_prev) 
			key_r = false;

		key_r_prev = false;
	}

		if(key_r)
		{
			worm->rotateWormAroundAnterPosterAxis(1.f);

			f6 = false;
		}

	//Управление позициями нейронов	
	
	//X
	if(f6)
	{
		/*
		for(int ind = 0; ind<100;ind++)
		{
			if (g_keys->keyDown [ind])
			{
				break;
			}
		}
		*/



	

		if (g_keys->keyDown [100]){
			//g_keys->keyDown [100] = false;
			worm->UpdateSelection(1);
		}
		if (g_keys->keyDown [102]){
			//g_keys->keyDown [102] = false;
			worm->UpdateSelection(2);
		}
		//
		//Z
		if (g_keys->keyDown [104]){
			//g_keys->keyDown [104] = false;
			worm->UpdateSelection(3);
		}
		if (g_keys->keyDown [98]){
			//g_keys->keyDown [98] = false;
			worm->UpdateSelection(4);
		}
		//
		//Y
		if (g_keys->keyDown [97]){
			//g_keys->keyDown [103] = false;
			worm->UpdateSelection(5);
		}
		if (g_keys->keyDown [105]){
			//g_keys->keyDown [105] = false;
			worm->UpdateSelection(6);
		}
		if (g_keys->keyDown [83]){
			g_keys->keyDown [83] = false;
			worm->savePosition();
		}
	}
	//
	//

	float dt = milliseconds / 1000.0f;										// Let's Convert Milliseconds To Seconds
  	int numOfIterations = (int)(dt / MaxPossible_dt) + 1;

	if (numOfIterations != 0)												// Avoid Division By Zero
	{
		dt = dt / numOfIterations;
	}

	for(int i = 0; i < numOfIterations; ++i)
	{
		worm->iteration(dt);
	}

}

extern GLvoid glPrint(GLint x, GLint y, const char *string, ...);		// Where The Printing Happens
extern Vector3D vbeg,vend;

void CElegans::ClearSelection()
{
	for(unsigned int i=0; i< neuron.size(); ++i)
		neuron[i] ->unselect();
}

void CElegans::savePosition()
{
	//    S A V E    N E U R O N    P O S I T I O N S
	std::fstream f_pout;
	f_pout.open("celegans302positions.new.txt",std::ios_base::out|std::ios_base::trunc);
	std::string name;
	std::string name2;
	std::string ctype;
	if(f_pout)
	{
		for(int i = 0; i < (int)neuron.size(); i++)
		{
			std::string str;
			double x_ = (-1.5 - (neuron[i]->getPos() - vshift).x/dx) / size;
			double z_ = 25 * (((neuron[i]->getPos() - vshift).z + dl/2)*0.095 / length - 0.045);
			double y_ = (neuron[i]->getPos() - vshift).y / (length * 0.3);
			//vshift+Vector3D(dx * ( -1.5 - x*size) , y, -dl/2+(0.045 + z/25) / 0.095 * length
			str = neuron[i]->getName();
			if(i != neuron.size() - 1)
				f_pout << str << "\t" << x_ << "\t" << y_ << "\t" << z_ << "\t" << neuron[i]->getColorIndex() << "\n";
			else
				f_pout << str << "\t" << x_ << "\t" << y_ << "\t" << z_ << "\t" << neuron[i]->getColorIndex();
		}
		f_pout.close();
	}
}
void CElegans::UpdateSelection(int direction)
{
	for(int i = 0; i<(int)neuron.size(); i++)
	{
		std::basic_string <char>::size_type contain;
		contain = neuron[i]->getName().find("Pse_");
		if(contain != std::string::npos)
		{
			if(neuron[i]->isSelected())
			{
				if(direction == 1)
					neuron[i]->setPosX(0.001);
				if(direction == 2)
					neuron[i]->setPosX(-0.001);
				if(direction == 3)
					neuron[i]->setPosZ(0.001);
				if(direction == 4)
					neuron[i]->setPosZ(-0.001);
				if(direction == 5)
					neuron[i]->setPosY(0.001);
				if(direction == 6)
					neuron[i]->setPosY(-0.001);
				return;
			}
		}
	}
}


CNeuron * CElegans::UpdateSelection()
{
	unsigned int i;
	Vector3D u = (vbeg - vend).unit();
	Vector3D w,n;//,uw;
	double d,z;
	int j=-1;

	for(i=0; i< neuron.size(); ++i)
	{
		n = Vector3D(neuron[i]->getX(),neuron[i]->getY(),neuron[i]->getZ());
		w = (ort1*(neuron[i]->getX()-pos_rc.x) + ort2*(neuron[i]->getY()-pos_rc.y) + ort3*(neuron[i]->getZ()-pos_rc.z))*scale + vcenter - vend;
		n = u*(u*w) + vend;
		w = w + vend;
		
		d = (n - w).length();
		
		if(d<=neuron_radius*1*scale)
		{
			if((j==-1)||((j>=0)&&(w.z>z)))
			{
				z = w.z;
				j = i; 
			}
		}
	}

	if(j>=0)
	{
		if(neuron[j] ->isSelected() == false)
		{
			std::basic_string <char>::size_type contain;
			contain = neuron[j]->getName().find("Pse_");
			if(contain != std::string::npos)
			{
				for(int k = 0; k < (int)neuron.size(); k++)
				{
					std::basic_string <char>::size_type contain_temp;
					contain_temp = neuron[k]->getName().find("Pse_");
					if(contain_temp != std::string::npos)
					{
						if(neuron[k]->isSelected())
						{
							neuron[k]->unselect();
							/*sel = true;
							break;*/
						}
						//neuron[j] ->select(); 
					}
				}
				neuron[j] ->select(); 
			}
			else
			{
				neuron[j] ->select(); 
			}
			return neuron[j];
		}
		else
			neuron[j] ->unselect(); 

		vcenter += (ort1*(neuron[j]->getX()-pos_rc.x) + ort2*(neuron[j]->getY()-pos_rc.y) + ort3*(neuron[j]->getZ()-pos_rc.z))*scale;

		pos_rc = neuron[j]->getPos();
		return NULL;
	}
	return NULL;
}

void my_glPrint(GLint x, GLint y, const char *string,int r,int g, int b)
{
	glColor3ub(0, 0, 100);	
	glPrint(x-1,y,string);
	glPrint(x,y-1,string);
	glPrint(x+1,y,string);
	glPrint(x,y+1,string);

	glColor3ub(r, g, b);
	glPrint(x,y,string);
}

void Draw (void)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity ();	// Reset The Modelview Matrix
	char title[]  = "Cyber Elegans 2.087 beta+, build 10.04.2012 (c) Palyanov Andrey, Khayrulin Sergey, Alexander Dibert 2007-2012";
	char title2[] = "A.P. Ershov Institute of Informatics Systems SB RAS, Novosibirsk. e-mail: palyanov@iis.nsk.su";


	GLUquadricObj *quadObj; 
	quadObj = gluNewQuadric(); 

	glPushMatrix();

	//Чтобы освещение работало нормально, желательно включить нормализацию: 
	glColor3ub(255, 255, 0);
	glLineWidth(2);
	double sc = 0.7*scale;
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3d(vcenter.x,vcenter.y,vcenter.z);
	glVertex3d(vcenter.x+ort1.x*sc,vcenter.y+ort1.y*sc,vcenter.z+ort1.z*sc);

	glColor3ub(0,255, 0);
	glVertex3d(vcenter.x,vcenter.y,vcenter.z);
	glVertex3d(vcenter.x+ort2.x*sc,vcenter.y+ort2.y*sc,vcenter.z+ort2.z*sc);


	glColor3ub(0, 0, 255);
	glVertex3d(vcenter.x,vcenter.y,vcenter.z);
	glVertex3d(vcenter.x+ort3.x*sc,vcenter.y+ort3.y*sc,vcenter.z+ort3.z*sc);


	glEnd();

	Vector3D rp1 = (ort1*(0-pos_rc.x) + ort2*(0-pos_rc.y) + ort3*(-0.2-pos_rc.z))*scale + vcenter;
	Vector3D rp2 = (ort1*(0-pos_rc.x) + ort2*(0-pos_rc.y) + ort3*(1-pos_rc.z))*scale + vcenter;
	float pi = 3.14159f;

	Vector3D p = rp2 - rp1;
	double h = p.length() / 2;

	double bkg_w = 10;

	

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, myTexture[0]);
	/*Vector3D 
	vtmp = ( ort1*( bkg_w-pos_rc.x) + ort2*( bkg_w-pos_rc.y) + ort3*(-0.1-pos_rc.z) )*scale + vcenter;
	GLdouble p1[] = {vtmp.x,vtmp.y,vtmp.z};

	vtmp = ( ort1*(-bkg_w-pos_rc.x) + ort2*( bkg_w-pos_rc.y) + ort3*(-0.1-pos_rc.z) )*scale + vcenter;
	GLdouble p2[] = {vtmp.x,vtmp.y,vtmp.z};

	vtmp = ( ort1*(-bkg_w-pos_rc.x) + ort2*(-bkg_w-pos_rc.y) + ort3*(-0.1-pos_rc.z) )*scale + vcenter;
	GLdouble p3[] = {vtmp.x,vtmp.y,vtmp.z};

	vtmp = ( ort1*( bkg_w-pos_rc.x) + ort2*(-bkg_w-pos_rc.y) + ort3*(-0.1-pos_rc.z) )*scale + vcenter;
	GLdouble p4[] = {vtmp.x,vtmp.y,vtmp.z};*/
	glColor3ub(255, 255, 255);	
	//////////////////////////////////////////////////////////

	/**/
	glBegin(GL_TRIANGLE_FAN);

	Vector3D center = (ort1*(0-pos_rc.x) + ort2*(0-pos_rc.y) + ort3*( -0.1-pos_rc.z))*scale + vcenter;//(ort1*( -pos_rc.x) + ort2*( -pos_rc.y) + ort3*(-pos_rc.z) )*scale + vcenter;
	//center.z = 0.0;
	//
	bkg_w = 10.4;
	//glTranslated(rp1.x, rp1.y, rp1.z);
	
	for(double phi=0; phi<=PI*2; phi+=PI/8)
	{
		
		//Vector3D ro = ( ort1*((bkg_w-center.x)*sin(phi)) + ort2*(( bkg_w-center.y)*cos(phi)) + ort3*(-0.1-center.z) )*scale + vcenter;
		//Vector3D vv1 = (ort1*(bkg_w-pos_rc.x)*sin(phi) + ort2*(0-pos_rc.y) + ort3*( -0.1-pos_rc.z))*scale;
		//Vector3D vv2 = (ort1*(0-pos_rc.x) + ort2*(bkg_w-pos_rc.y)*cos(phi) + ort3*( -0.1-pos_rc.z))*scale;

		Vector3D ro = rp1+(ort1*bkg_w*sin(phi)+ort2*bkg_w*cos(phi))*scale;
		GLdouble point[] = {(GLfloat)ro.x,(GLfloat)ro.y,(GLfloat)ro.z};
		glTexCoord2f((GLfloat)(bkg_w*sin(phi)),(GLfloat)(bkg_w*cos(phi)));
		glVertex3dv(point);
	}
	
	bkg_w = 10;
	glEnd();
	glDisable(GL_TEXTURE_2D);
	


	////////////////////////////////////////////////////////////
	glColor3ub(100,150,200);
	/*
	//////////////////////////////////////////////////////////
	glBegin(GL_QUADS);
	
	//

	glTexCoord2f(0.0f, 0.0f); glVertex3dv(p1);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(p2);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(p3);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(p4);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	*/
	//glTranslated(rp1.x, rp1.y, rp1.z);
	//gluSphere(quadObj,bkg_w*scale,10,10);
	//glPopMatrix();
	/*
	Vector3D rp1 = (ort1*(p1->getX()-pos_rc.x) + ort2*(p1->getY()-pos_rc.y) + ort3*(p1->getZ()-pos_rc.z))*scale + vcenter;
	Vector3D rp2 = (ort1*(p2->getX()-pos_rc.x) + ort2*(p2->getY()-pos_rc.y) + ort3*(p2->getZ()-pos_rc.z))*scale + vcenter;
	*/


	/**/
	glPushMatrix();
	
	glTranslated(rp1.x, rp1.y, rp1.z);
	glRotated(180.0f*(double)atan2(p.x,p.z)/pi,0,1,0); 
	glRotated(-180.0f*(double)atan2(p.y,sqrt(p.x*p.x+p.z*p.z))/pi,1,0,0); 
	
	gluCylinder(quadObj, bkg_w*scale, bkg_w*scale, h, 32, 1);
	gluCylinder(quadObj, bkg_w*scale*1.1, bkg_w*scale, h, 32, 1);

	glPopMatrix();
	/**/
	/**/
	////////////////////////////////////////////////////////////


	worm->draw();

	my_glPrint(15,20,title,0,255,255);
	my_glPrint(15,40,title2,0,255,255);
	my_glPrint(15,60,"F1 = help show / hide",0,255,175);

	if(f1) 
	{
		my_glPrint(15, 80,"F2 = toggle fullscreen",0,255,75);
		my_glPrint(15,100,"F5 = toggle full view / neuromuscular system only",0,255,75);
		my_glPrint(15,120,"F6 = physics on/off",0,255,75);
		my_glPrint(15,140,"F7 = send sinusoidal pattern directly to muscles",0,255,75);
		my_glPrint(15,160,"F8 = send sinusoidal pattern to ventral cord motorneurons",0,255,75);
		my_glPrint(15,180,"mouse scroll - change scale",175,255,0);
		my_glPrint(15,200,"mouse right button - rotate scene",175,255,0);
		my_glPrint(15,220,"mouse right button - move scene",175,255,0);
		my_glPrint(15,240,"click at neuron with mouse right button - select / deselect neuron",175,255,0);
		my_glPrint(15,260,"key 'a' - activate selected neuron(s)",255,255,0);
		my_glPrint(15,280,"key 'r' - rotate nematode worm around longitudinal axis",255,255,0);
		my_glPrint(15,300,"NumPad's '4' = x--, '6' = x++, '2' = y--, '8' = y++, '1' = z--, '9' = z++",255,125,0);
		my_glPrint(15,320,"for moving small points defining connection paths between neurons",255,125,0);
		my_glPrint(15,340,"key 's' to save its positions changes [only in editor mode, when physics is still off and no worm rotation was performed]",255,125,0);
	}

	glFlush ();	

	glPopMatrix();
 
	gluDeleteQuadric(quadObj);

	return;
														// Flush The GL Rendering Pipeline
}

