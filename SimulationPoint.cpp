#include "SimulationPoint.h"

SimulationPoint::~SimulationPoint(void)
{
//cout << "Inside SimulationPoint::Destructor()" << endl;

}

SimulationPoint::SimulationPoint(void)
{
//cout << "Inside SimulationPoint::SimulationPoint()" << endl;

}


void SimulationPoint::Show(void)
{

cout << "fBFieldTan   "<<fBFieldTan<< endl;
cout << "fBFieldMag   "<<fBFieldMag<< endl;
cout<<  "Angle       "<<fBFieldUnitTheta.Phi()*TMath::RadToDeg()<<endl;
fBField.Dump();
fPosition.Dump();

}


void SimulationPoint::Treat()// this could be automatic
{
// x and y -> R theta
fPositionAngle = fPosition.Phi() * TMath::RadToDeg(); // in degrees (Phi is the azimuthal angle)
fPositionRadius = fPosition.Perp(); // in mm 

//Btan
fBFieldUnitTheta.SetXYZ((-TMath::Sin(fPosition.Phi())),(TMath::Cos(fPosition.Phi())),0.);
fBFieldTan = fabs( fBFieldUnitTheta.Dot(fBField) );

// absolute magnitudes
fBFieldMag = fBField.Mag();
fBFieldPerp = fBField.Perp();
fEFieldMag = fEField.Mag();

//difference

fBFieldDiff = fBFieldMag-fBFieldTan ;
}


void SimulationPoint::ReadLineAndTreat(Double_t  X, Double_t  Y, Double_t  Z , Double_t Bx, Double_t By, Double_t Bz, Double_t Ex, Double_t Ey, Double_t Ez, Double_t Perm)
{
fBField.SetXYZ(Bx,By,Bz); //Simulation is offset by -4mm, this corrects for it
fEField.SetXYZ(Ex,Ey,Ez);
fPosition.SetXYZ(X,Y,Z);
fPerm = Perm ;


Treat();
}

//Reset parameters
void SimulationPoint::ClearParameters() 
{
fBField.SetXYZ(-100,-100,-100); 
fPosition.SetXYZ(-100,-100,-100);
fEField.SetXYZ(-100,-100,-100);
fBFieldMag=-100; //magnetic field mag
fEFieldMag=-100; //electric field mag

fPositionRadius=-100; //Radius in mm
fPositionAngle=-100; //in degree
fPerm =-100;
}



