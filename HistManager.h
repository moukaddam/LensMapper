#ifndef _HISTMANAGER_CLASS
#define _HISTMANAGER_CLASS


#include "TH2D.h"
#include "TH3D.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphAsymmErrors.h"
#include "TMath.h"
#include "TVector3.h"

#include "stdio.h"
#include "iostream"
#include "fstream"

using namespace std;


class HistManager
{
 public:

//3D histogram
TH3D *fSim3dHistogram ;
double fdx;
double fdy;
double fdz;  

double fOffsetx;
double fOffsety;
double fOffsetz;  

//Methodes
HistManager(TH3D* hist3d, double dx, double dy, double dz);
~HistManager(void);

void SetOffsets(double offsetx, double offsety, double offsetz) ;

TGraphAsymmErrors* GetCartesianInterpolation(double angle /*degree*/, double level_z_diretion, int number_of_interpolation_points, TString NameTitle) ;

void    GetPolarInterpolation(double angle /*degree*/, double level_z_diretion, int number_of_interpolation_points, TString NameTitle, int draw_error) ;
TGraph* GetPolarInterpolationOffsetX(double theta, double z_level, int offset, int number_of_interpolation_points, TString NameTitle);
TGraph* GetPolarInterpolationOffsetY(double theta, double z_level, int offset, int number_of_interpolation_points, TString NameTitle);
TGraph* GetPolarInterpolationOffsetZ(double theta, double z_level, double offset, int number_of_interpolation_points, TString NameTitle);
void    GetPolarInterpolationOffsetXYZ(double theta, double z_level, int number_of_interpolation_points, TString NameTitle); //NEW
TGraph* GetPolarInterpolationTan(double radius, double level_z_direction, TString NameTitle) ;
TGraph2D* GetPolarInterpolation2D(double level_z_direction, int number_of_interpolation_points, TString NameTitle) ;

int GetTotalOffsetGraph(double angle,double z_level,double x_offset,double y_offset,TString NameTitle);
TH2D* GetXYMagnetisationOffsetGraph(double angle,double radius,double z_level,double exp,TString NameTitle);
int GetMagnetisationOffset(double angle,double radius,double z_level,double magnetisation,double expbfield,TString NameTitle);

// good for contour plots
TH2D* GetMagnitisation2D(double z_level,double xlow,double xhigh,double ylow,double yhigh, int contour, TString Bdirection);


double GetInterpolationOnePoint(double X, double Y, double Z) ; // Get values at point X Y Z  //NEW
double GetInterpolationOnePointError(double X, double Y, double Z) ; // Get values at point X Y Z  // NEW

void Clear(void) ;

 };

#endif



