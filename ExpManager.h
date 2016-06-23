/*
Graph Manager is a class that keeps a record of all the measured values and their correspondent simulated values



*/

#ifndef _ExpManager_CLASS
#define _ExpManager_CLASS

#include "TGraphErrors.h"
#include "TGraph2DErrors.h"
#include "TMultiGraph.h"

#include "stdio.h"
#include "iostream"
#include "vector"

using namespace std;


class ExpManager
{
 public:

TString fInfo; 

//Experimental Position
vector<TString> fGrid;
vector<Double_t> fExpX; 
vector<Double_t> fExpY; 
vector<Double_t> fExpZ; 
vector<Double_t> fExpR;  
vector<Double_t> fExpTheta;

// Now we consider the same position error on all the sensors 
vector<Double_t> fExpXErr; 
vector<Double_t> fExpYErr;
vector<Double_t> fExpZErr; 
vector<Double_t> fExpRErr;   
vector<Double_t> fExpThetaErr; 

//Experimental Field
vector<Double_t> fExpB ; 
vector<Double_t> fExpBErr;

//Simulated field 
vector<Double_t> fSimB ;


//Methodes
ExpManager(void );
~ExpManager(void);

void SetInfo(TString info){fInfo = info;};

void FillValue(Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t b, Double_t bsim,TString grid);
void FillValueError(Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t b); // no error on simulation 
void ShowVectorSizes(void) ;
void ShowVectorsContent(void) ;
void Clear(void) ;

double GetSumChi2() ; // not normalised 
int GetNpoints() { return fExpX.size(); } ; // all have the same size

// returns the offset of the sensor
double GetOffset(TString Grid, int Quadrant, TString Direction) ; 

// Returns the index of the first point validating specific position requirements 
int GetExpBFieldPointXYZ(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax  ) ;
int GetExpBFieldPoinRTZ(double rmin, double rmax, double tmin, double tmax, double zmin, double zmax  ) ;

//1D
void GetExp1DGraphPolar(TString NameTitle, double zmin, double zmax, double anglmin, double anglemax, TString grid) ; 
void GetSim1DGraphPolar(TString NameTitle, double zmin, double zmax, double anglemin, double anglemax, TString grid) ; 
void GetExp1DGraphX(TString NameTitle, double zmin, double zmax, double ymin, double ymax, TString grid);
void GetExp1DGraphY(TString NameTitle, double zmin, double zmax, double xmin, double xmax, TString grid);
void GetExp1DGraphZ(TString NameTitle, double xmin, double xmax, double ymin, double ymax, TString grid);
void GetSim1DGraphX(TString NameTitle, double zmin, double zmax, double ymin, double ymax, TString grid);
void GetSim1DGraphY(TString NameTitle, double zmin, double zmax, double xmin, double xmax, TString grid);
void GetSim1DGraphZ(TString NameTitle, double xmin, double xmax, double ymin, double ymax, TString grid);

void DrawGraphs(TString Grid, int Quad, double depth); // it Draws a specific set of the above graphs 

//2D 
void GetExp2DGraph(TString NameTitle, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) ;

void DrawMap(TString NameTitle, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) ;


 };

#endif




