#ifndef _GRAPHMANAGER_CLASS
#define _GRAPHMANAGER_CLASS

#include "Defines.h"

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
vector<Double_t> fExpXErr; //New
vector<Double_t> fExpYErr; //New
vector<Double_t> fExpZErr; //New

vector<Double_t> fExpRErr;   //New
vector<Double_t> fExpThetaErr;  //New

//Experimental Field
vector<Double_t> fExpBx;
vector<Double_t> fExpBy;
vector<Double_t> fExpBz;
vector<Double_t> fExpBmag ;

vector<Double_t> fExpBxErr;    //New
vector<Double_t> fExpByErr;    //New
vector<Double_t> fExpBzErr;    //New
vector<Double_t> fExpBmagErr;  //New


//Simulated field
vector<Double_t> fSimBx;
vector<Double_t> fSimBy;
vector<Double_t> fSimBz;
vector<Double_t> fSimBmag ;


//Methodes
GraphManager(void );
~GraphManager(void);

void FillVectors(Int_t datatype, Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t bx, Double_t by, Double_t bz);
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




