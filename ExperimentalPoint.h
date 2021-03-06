#ifndef _EXPERIMENTALPOINT_CLASS
#define _EXPERIMENTALPOINT_CLASS


#include "TVector2.h"
#include "TVector3.h"
#include "TMath.h"

#include "map"
#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "fstream"
using namespace std;

#define SENSORXOFFSETX  -2.1
#define SENSORXOFFSETY  +0.0
#define SENSORXOFFSETZ  +0.0 //all the sensors have the same offset in z = -1.8 mm, this will be taken into account directly in the ExperimentalPoint class
  
#define SENSORYOFFSETX  +0.0
#define SENSORYOFFSETY  +2.1
#define SENSORYOFFSETZ  +0.0 //idem
 
#define SENSORZOFFSETX  +0.0
#define SENSORZOFFSETY  +0.0
#define SENSORZOFFSETZ  +0.0 //idem

class ExperimentalPoint
{
private:
	std::map<TString,TVector2> fmapPosition;
	std::map<TString,TVector3> fmapBackground;

public:
	std::map<TString,double*> fmapInspect; // a list strings e.g. 4B1 (=quadrant grid level) that makes it easy to choose right graphs to inspect the mapping
	vector<TString> flistGrid; // a list strings e.g. 4B1 (=quadrant grid level) that makes it easy to choose right graphs to inspect the mapping
	vector<int> flistQuad; 
	vector<double> flistDepth;

 public:
//members
bool  fSetBackground ; 
TString  fGrid ; // A, B, C, D (will decide if the reading is Cartezian or not)  (counter clock wise )
int fMagnetQuadrant ; // Q12 , Q23, Q34, Q41 (will be fixed by the flat tip) or 0, 1, 2, 3 (counter clock wise )
int fLocation ; // an integer corresponding to the location 
int fLevel ; // an integer corresponding to the level of the mapper  (1 to 8, 1 being the top)
double fFieldOffset ; // if the magnets in the comsol simulation are shifted towards the target by x, fFieldOffset should be set to +x 

// NEW this will be used to correct the sensor position
TVector3 fPosition; //central position
TVector3 fSensorOffsetX ; // offset of sensor from the central position 
TVector3 fSensorPositionX; //position of sensor X
TVector3 fSensorOffsetY ; // offset of sensor from the central position 
TVector3 fSensorPositionY; //position of sensor Y
TVector3 fSensorOffsetZ ; // offset of sensor from the central position 
TVector3 fSensorPositionZ; //position of sensor Z
TVector3 fBField; //magnetic field, it will be filled with one value, but keeping it as a vector will make it easy to handle rotations

//Methodes
ExperimentalPoint(TString background="nobackground");
~ExperimentalPoint(void);

void ClearParameters(void);
void ShowParameters(void);
void SetFieldOffset(double x); 

void CheckList(TString Grid, int MagnetQuadrant, int Level);
void CalculateCentralPosition();
void ReadLineAndTreat(int MagnetQuadrant, TString Grid, int Location, int Level , double Bx, double By, double Bz);
void SubtractBackground(TString Grid, int Location, double &Bx, double &By, double &Bz);
double CalculateRotationAngle(int MagnetQuadrant, TString Grid); // returns  angfle in radian 
double GetDepth(int level); 
TVector3 GetOffsetDirection(TString Grid, int MagnetQuadrant, TString Direction);

void LoadMap(); // load the map for positions in the mapper plate, Shaun Georges's drawings are used (can be found on the vault)
void LoadBackGround(TString background);
};

#endif

