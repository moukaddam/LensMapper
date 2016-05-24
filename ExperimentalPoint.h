#ifndef _EXPERIMENTALPOINT_CLASS
#define _EXPERIMENTALPOINT_CLASS

#include "Defines.h"

class ExperimentalPoint
{
 public:
//members
TString  fPlateLabel ; // A, B, C, D (will decide if the reading is Cartezian or not)  (counter clock wise )
Double_t fMagnetQuadrant ; // Q12 , Q23, Q34, Q41 (will be fixed by the flat tip) or 0, 1, 2, 3 (counter clock wise )
Double_t fProbeAngle ; // 0, 90, 180, 260  (counter clock wise )
TVector3 fBField; //magnetic field 
Double_t fBFieldMag; //magnetic field 

// NEW this will be used to correct the sensor position
TVector3 fSensorOffset ; 

TVector3 fPrePosition; //position before conversion
TVector3 fPosition; //position after conversion
Double_t fPositionRadius; //Radius in mm
Double_t fPositionAngle; //in degree


//Methodes
ExperimentalPoint(void);
ExperimentalPoint(double offset_x, double offset_y, double offset_z);
~ExperimentalPoint(void);

void ClearParameters(void);
void ShowParameters(void);
void Treat(void);// this could be automatic
void ReadLineAndTreat(TString PlateLabel, Int_t MagnetQuadrant, Int_t ProbeAngle, Double_t  XR, Double_t  YT, Double_t  Z , Double_t Bx, Double_t By, Double_t Bz);

void SetSensorOffset(double offset_x, double offset_y, double offset_z);
	
};

#endif

