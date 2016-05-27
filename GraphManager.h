/*
Graph Manager is a class that keeps a record of all the measured values and their correspondent simulated values



*/

#ifndef _GRAPHMANAGER_CLASS
#define _GRAPHMANAGER_CLASS

#include "TGraphErrors.h"
#include "TGraph2DErrors.h"

#include "stdio.h"
#include "iostream"
#include "vector"

using namespace std;


class GraphManager
{
 public:

//Experimental Position
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
GraphManager(void );
~GraphManager(void);

void FillValue(Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t b, Double_t bsim);
void FillValueError(Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t b); // no error on simulation 
void ShowVectorSizes(void) ;
void ShowVectorsContent(void) ;
void Clear(void) ;

//1D
void GetExp1dGraphPolar(TString NameTitle, double angle) ; // NEW
void GetSim1dGraphPolar(TString NameTitle, double angle ); // NEW

TGraphErrors* GetExp1dGraphCartesian(double y) ;  // CHECK 

//2D 
TGraph2DErrors* GetExp2dGraph(void /*ADD an option to select which physical quantity ? */);

// this function gets a specific experimental value at a specific polar position
double GetExpBField(double angle, double radius, TString NameTitle); 

//Helper Methods
Double_t* GetArrayFromCVector(vector<Double_t> vec) ;


 };

#endif




