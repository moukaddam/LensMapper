#include "ExperimentalPoint.h"

ExperimentalPoint::ExperimentalPoint(void){
	LoadMap();
	fSensorOffsetX.SetXYZ(SENSORXOFFSETX,SENSORXOFFSETY,SENSORXOFFSETZ); 
    fSensorOffsetY.SetXYZ(SENSORYOFFSETX,SENSORYOFFSETY,SENSORYOFFSETZ); 
    fSensorOffsetZ.SetXYZ(SENSORZOFFSETX,SENSORZOFFSETY,SENSORZOFFSETZ); 
}

ExperimentalPoint::~ExperimentalPoint(void){}

void ExperimentalPoint::CalculateCentralPosition(void){ // taking into account the level and the quadrant 
 
	 //Calculate position with respect to the Mapper
	double x,y,z;
	TString label = fGrid+fGrid+Form("%d",fLocation); // AA1
	x = fmapPosition[label].X() ; 
	y = fmapPosition[label].Y() ;
	// 210.0mm is the height of the firsthole, 15mm difference between holes, 5mm is the half thickness of the plate, 11.5mm is the pedestal where the probe rests 
	// 104.8mm is the length of the probe, "L" in the catalogue however it's not ver accurate 
	// 193.5mm is the distance from the contact surface of the pedestal to the tip of the probe
	z = (210 + (fLevel-1)*15 + 5 ) - 193.5  ;    
	//cout << " x y z " << x << " " << y << " " << z << endl; 
	fPosition.SetXYZ(x,y,z); 
}

double ExperimentalPoint::CalculateRotationAngle(int MagnetQuadrant, TString Grid){ // returns  angfle in radian 

if (Grid=="B") {
	if (MagnetQuadrant==1)	return 0 ; 
	if (MagnetQuadrant==2)	return +90 ; 
	if (MagnetQuadrant==3)	return +180 ; 
	if (MagnetQuadrant==4)	return +270 ; 
	}
if (Grid=="A") {
	if (MagnetQuadrant==2)	return 0 ; 
	if (MagnetQuadrant==3)	return +90 ; 
	if (MagnetQuadrant==4)	return +180 ; 
	if (MagnetQuadrant==1)	return +270 ; 
	}
if (Grid=="C") {
	if (MagnetQuadrant==3)	return 0 ; 
	if (MagnetQuadrant==4)	return +90 ; 
	if (MagnetQuadrant==1)	return +180 ; 
	if (MagnetQuadrant==2)	return +270 ; 
	}
if (Grid=="D") {
	if (MagnetQuadrant==4)	return 0 ; 
	if (MagnetQuadrant==1)	return +90 ; 
	if (MagnetQuadrant==2)	return +180 ; 
	if (MagnetQuadrant==3)	return +270 ; 
	}

} 

//Read a line and fill parameter
void ExperimentalPoint::ReadLineAndTreat(int MagnetQuadrant, TString Grid, int Location, int Level, double Bx, double By, double Bz){
	fGrid = Grid ;
	fMagnetQuadrant = MagnetQuadrant ; 
	fLocation = Location ; 
	fLevel = Level; 
    
    CalculateCentralPosition() ; 
    fSensorPositionX = fPosition + fSensorOffsetX; 
    fSensorPositionY = fPosition + fSensorOffsetY; 
    fSensorPositionZ = fPosition + fSensorOffsetZ; 
    fBField.SetXYZ(Bx/10.,By/10.,Bz/10.); // turn to millitesla 
    CheckTag(); 
    ShowParameters();

    //correct for rotations 
    double angle = CalculateRotationAngle(MagnetQuadrant, Grid);
    fPosition.RotateZ(angle*TMath::DegToRad()); 
    fSensorPositionX.RotateZ(angle*TMath::DegToRad()); 
    fSensorPositionY.RotateZ(angle*TMath::DegToRad()); 
    fSensorPositionZ.RotateZ(angle*TMath::DegToRad()); 

	fBField.RotateZ(angle*TMath::DegToRad()); 
    CheckTag();
    ShowParameters(); 

	cin.get(); 
}


void ExperimentalPoint::ShowParameters(void){
	cout<<" /////////////////////// ShowParameters /////////////////////// "<<endl;
	cout<<endl<<"Grid (Pattern)              : "<<fGrid<<endl;
	cout<<endl<<"Magnet Quadrant             : "<<fMagnetQuadrant <<endl;
	cout<<endl<<"Probe angle (deg)           : "<<fProbeAngle <<endl;
	cout<<endl<<"Magnetic Field (mT)         : "<<fBField.X()<<" ; "<<fBField.Y()<<" ; "<<fBField.Z()<<endl;
	cout<<endl<<"Position Central (mm)       : "<<fPosition.X()<<" ; "<<fPosition.Y()<<" ; "<<fPosition.Z()<<endl;
	cout<<endl<<"Position Sensor (mm)  X     : "<<fSensorPositionX.X()<<" ; "<<fSensorPositionX.Y()<<" ; "<<fSensorPositionX.Z()<<endl;
	cout<<endl<<"Position Sensor (mm)  Y     : "<<fSensorPositionY.X()<<" ; "<<fSensorPositionY.Y()<<" ; "<<fSensorPositionY.Z()<<endl;
	cout<<endl<<"Position Sensor (mm)  Z     : "<<fSensorPositionZ.X()<<" ; "<<fSensorPositionZ.Y()<<" ; "<<fSensorPositionZ.Z()<<endl;
	cout<<endl<<"Sensor tag (X,Y,Z)==(0,1,2) : "<<fSensorTag<<endl;
	cout<<endl<<" //////////////////////////////////////////////////////////////// "<<endl;

}

void ExperimentalPoint::CheckTag(){
	if (fabs(fBField.X()) > 0 ) fSensorTag = 0; 
	if (fabs(fBField.Y()) > 0 ) fSensorTag = 1; 
    if (fabs(fBField.Z()) > 0 ) fSensorTag = 2; 
}

int ExperimentalPoint::GetTag(){
	return fSensorTag ; 
}

//Clear all parameters
void ExperimentalPoint::ClearParameters(){
	fGrid = "NULL" ;
	fMagnetQuadrant = -10000 ; 
	fProbeAngle = 0 ;
	fBField.SetXYZ(-10000,-10000,-10000); 
	fPosition.SetXYZ(-10000,-10000,-10000);
	fSensorPositionX.SetXYZ(-10000,-10000,-10000);
	fSensorPositionY.SetXYZ(-10000,-10000,-10000);
	fSensorPositionZ.SetXYZ(-10000,-10000,-10000);

}

void ExperimentalPoint::LoadMap(){
	TString pos; 
	double X, Y ;
	ifstream file;

	file.open("./input/PositionMap.txt");
	while (file>>pos>>X>>Y){
		//cout << " pos " << pos << "  " << X << " " << Y << endl ; 
		fmapPosition.insert ( std::pair<TString,TVector2>(pos,TVector2(X,Y)));
	}
	cout << "  finished reading the position map. " << endl ; 
}

