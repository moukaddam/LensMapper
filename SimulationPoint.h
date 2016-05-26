#ifndef _SIMULATIONPOINT_CLASS
#define _SIMULATIONPOINT_CLASS

#include "Defines.h"

class SimulationPoint
{
 public:

//Members
TVector3 fPosition; // position 

TVector3 fBField; //magnetic field 
TVector3 fEField; //electric field
TVector3 fBFieldUnitTheta; //unit vector in theta

double fBFieldMag; //magnetic field mag
double fBFieldPerp; //magnetic field perpendicular
double fBFieldTan; //magnetic field tangential
double fBFieldDiff; //magnetic field difference between mag and tan
double fEFieldMag; //electric field mag

double fPositionRadius; //Radius in mm
double fPositionAngle; //in degree

double fPerm; 


//Methods
SimulationPoint(void);
~SimulationPoint(void);

void Show(void);

void Treat(void);
void ReadLineAndTreat(double  X, double  Y, double  Z , double Bx, double By, double Bz, double Ex, double Ey, double Ez, double Perm);
void ClearParameters(void) ;

 };

#endif

