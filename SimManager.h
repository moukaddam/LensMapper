#ifndef _SimManager_CLASS
#define _SimManager_CLASS

#include "TH2D.h"
#include "TH3D.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphAsymmErrors.h"
#include "TMath.h"
#include "TVector3.h"
#include "TCanvas.h"

#include "stdio.h"
#include "iostream"
#include "fstream"

using namespace std;


class SimManager
{
 public:

//3D histogram
TH3D *fSim3dHistogram ;

double fXErr;
double fYErr;
double fZErr;  

double fOffsetx;
double fOffsety;
double fOffsetz;  

//Methodes
SimManager(TH3D* hist3d, double dx, double dy, double dz);
~SimManager(void);

void SetOffsets(double offsetx, double offsety, double offsetz) ;
void Clear(void) ;

double GetPoint(double X, double Y, double Z) ; // no offset considered 
double GetPointError(double X, double Y, double Z) ; // no offset considered 
double GetPointErrorX(double X, double Y, double Z) ; // no offset considered 
double GetPointErrorY(double X, double Y, double Z) ; // no offset considered 
double GetPointErrorZ(double X, double Y, double Z) ; // no offset considered

void DrawPolar       (TString NameTitle, int samples, bool DrawError, double theta , double z_level) ;
void DrawPolarOffset(TString NameTitle, int samples, bool DrawError, TVector3 offset, double theta, double z_level); // adding the offset on the X-axis
void DrawCartesianFixedY  (TString NameTitle, int samples, bool DrawError, double y , double z_level) ; // no offset needed in this case
void DrawCartesianFixedX  (TString NameTitle, int samples, bool DrawError, double x , double z_level) ; // no offset needed in this case
void Draw2DGraph     (TString NameTitle, int points, double xlow, double xhigh, double ylow, double yhigh, double z) ; // write a TGraph2D, 
void Draw2DHist      (TString NameTitle, int points, double xlow, double xhigh, double ylow, double yhigh, double z, int contour=50); // returns a 2D histogram, good for contour plots

 };

#endif



