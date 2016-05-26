#include "ExperimentalPoint.h"

ExperimentalPoint::ExperimentalPoint(void){
//cout << "Inside ExperimentalPoint::ExperimentalPoint()" << endl;
LoadMap();
fXSensorOffset.SetXYZ(SENSORXOFFSETX,SENSORXOFFSETY,SENSORXOFFSETZ);
fYSensorOffset.SetXYZ(SENSORYOFFSETX,SENSORYOFFSETY,SENSORYOFFSETZ);
fZSensorOffset.SetXYZ(SENSORZOFFSETX,SENSORZOFFSETY,SENSORZOFFSETZ);
}

ExperimentalPoint::~ExperimentalPoint(void){
//cout << "Inside ExperimentalPoint::Destructor()" << endl;
}

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
    fXPosition = fPosition + fXSensorOffset; 
    fYPosition = fPosition + fYSensorOffset; 
    fZPosition = fPosition + fZSensorOffset; 
    fBField.SetXYZ(Bx/10.,By/10.,Bz/10.); // turn to millitesla 
   
    //correct for rotations 
    double angle = CalculateRotationAngle(MagnetQuadrant, Grid); 
    //cout << MagnetQuadrant << " " << Grid <<  " rotation angle "  << angle << endl ;
    fXPosition.RotateZ(angle*TMath::DegToRad()); 
    fYPosition.RotateZ(angle*TMath::DegToRad()); 
    fZPosition.RotateZ(angle*TMath::DegToRad()); 
	fBField.RotateZ(angle*TMath::DegToRad()); 

	cin.get(); 
}


void ExperimentalPoint::ShowParameters(void){
	cout<<" /////////////////////// ShowParameters /////////////////////// "<<endl;
	cout<<endl<<"Plate label : "<<fGrid<<endl;
	cout<<endl<<"Magnet Quadrant : "<<fMagnetQuadrant <<endl;
	cout<<endl<<"Probe angle : "<<fProbeAngle <<endl;
	cout<<endl<<"Magnetic Field : "<<fBField.X()<<" ; "<<fBField.Y()<<" ; "<<fBField.Z()<<endl;
	cout<<endl<<"Position Central  : "<<fPosition.X()<<" ; "<<fPosition.Y()<<" ; "<<fPosition.Z()<<endl;
	cout<<endl<<"Position Sensor X : "<<fXPosition.X()<<" ; "<<fXPosition.Y()<<" ; "<<fXPosition.Z()<<endl;
	cout<<endl<<"Position Sensor Y : "<<fYPosition.X()<<" ; "<<fYPosition.Y()<<" ; "<<fYPosition.Z()<<endl;
	cout<<endl<<"Position Sensor Z : "<<fZPosition.X()<<" ; "<<fZPosition.Y()<<" ; "<<fZPosition.Z()<<endl;
	cout<<" //////////////////////////////////////////////////////////////// "<<endl;

}


//Clear all parameters
void ExperimentalPoint::ClearParameters(){
	fGrid = "NULL" ;
	fMagnetQuadrant = -100 ; 
	fProbeAngle = -100 ;
	fBField.SetXYZ(-100,-100,-100); 
	fPosition.SetXYZ(-100,-100,-100);
	fXPosition.SetXYZ(-100,-100,-100);
	fYPosition.SetXYZ(-100,-100,-100);
	fZPosition.SetXYZ(-100,-100,-100);
}

void ExperimentalPoint::LoadMap(){

	TString pos; 
	double X, Y ;

	ifstream file;
	file.open("./input/PositionMap.txt");

	while (file>>pos>>X>>Y){
		cout << " pos " << pos << "  " << X << " " << Y << endl ; 
		fmapPosition.insert ( std::pair<TString,TVector2>(pos,TVector2(X,Y)));
	}

	cout << "  finished reading the position map. " << endl ; 
}

