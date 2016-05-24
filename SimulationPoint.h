#ifndef _SIMULATIONPOINT_CLASS
#define _SIMULATIONPOINT_CLASS

#include "Defines.h"

class SimulationPoint
{
 public:

//Members
TVector3 fPosition; //position
TVector3 fBField; //magnetic field 
TVector3 fEField; //electric field
TVector3 fBFieldUnitTheta; //unit vector in theta

Double_t fBFieldMag; //magnetic field mag
Double_t fBFieldPerp; //magnetic field perpendicular
Double_t fBFieldTan; //magnetic field tangential
Double_t fBFieldDiff; //magnetic field difference between mag and tan
Double_t fEFieldMag; //electric field mag

Double_t fPositionRadius; //Radius in mm
Double_t fPositionAngle; //in degree

Double_t fPerm; 


//Methods
SimulationPoint(void);
~SimulationPoint(void);

void Show(void);

void Treat(void);
void ReadLineAndTreat(Double_t  X, Double_t  Y, Double_t  Z , Double_t Bx, Double_t By, Double_t Bz, Double_t Ex, Double_t Ey, Double_t Ez, Double_t Perm);
void ClearParameters(void) ;

 };

#endif

