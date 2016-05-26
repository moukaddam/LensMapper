#ifndef _EXPERIMENTALPOINT_CLASS
#define _EXPERIMENTALPOINT_CLASS

#include "Defines.h"
#include "TVector2.h"


#define SENSORXOFFSETX  +2.1
#define SENSORXOFFSETY  +0.0
#define SENSORXOFFSETZ  -1.8
  
#define SENSORYOFFSETX  +0.0
#define SENSORYOFFSETY  +2.1
#define SENSORYOFFSETZ  -1.8
 
#define SENSORZOFFSETX  +0.0
#define SENSORZOFFSETY  +0.0
#define SENSORZOFFSETZ  -1.8


class ExperimentalPoint
{
private:
	std::map<TString,TVector2> fmapPosition;

 public:
//members
TString  fGrid ; // A, B, C, D (will decide if the reading is Cartezian or not)  (counter clock wise )
double fMagnetQuadrant ; // Q12 , Q23, Q34, Q41 (will be fixed by the flat tip) or 0, 1, 2, 3 (counter clock wise )
double fProbeAngle ; // 0, 90, 180, 260  (counter clock wise )
int fLocation ; // an integer corresponding to the location 
int fLevel ; // an integer corresponding to the level of the mapper  (1 to 8, 1 being the top)

// NEW this will be used to correct the sensor position
TVector3 fPosition; //central position
TVector3 fXSensorOffset ; 
TVector3 fXPosition; //position after conversion
TVector3 fYSensorOffset ; 
TVector3 fYPosition; //position after conversion
TVector3 fZSensorOffset ; 
TVector3 fZPosition; //position after conversion

TVector3 fBField; //magnetic field 

//Methodes
ExperimentalPoint(void);
~ExperimentalPoint(void);

void ClearParameters(void);
void ShowParameters(void);
void ReadLineAndTreat(int MagnetQuadrant, TString Grid, int Location, int Level , double Bx, double By, double Bz);

void CalculateCentralPosition();
double CalculateRotationAngle(int MagnetQuadrant, TString Grid); // returns  angfle in radian 

void LoadMap();
};

#endif

