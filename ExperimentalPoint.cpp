#include "ExperimentalPoint.h"

ExperimentalPoint::ExperimentalPoint(TString background){
	
	LoadMap();
	if(background != "nobackground") {
		LoadBackGround(background); 
		fSetBackground=true; 
		}
	fSensorOffsetX.SetXYZ(SENSORXOFFSETX,SENSORXOFFSETY,SENSORXOFFSETZ); 
    fSensorOffsetY.SetXYZ(SENSORYOFFSETX,SENSORYOFFSETY,SENSORYOFFSETZ); 
    fSensorOffsetZ.SetXYZ(SENSORZOFFSETX,SENSORZOFFSETY,SENSORZOFFSETZ);
    fFieldOffset = 0 ;  
}

ExperimentalPoint::~ExperimentalPoint(void){}

void ExperimentalPoint::CalculateCentralPosition(void){ // taking into account the level and the quadrant 
	 //Calculate position with respect to the Mapper
	double x,y,z;
	TString label = fGrid+fGrid+Form("%d",fLocation); // AA1
	x = fmapPosition[label].X() ; 
	y = fmapPosition[label].Y() ;
	z = GetDepth(fLevel); 
	fPosition.SetXYZ(x,y,z); 
}

void ExperimentalPoint::SetFieldOffset(double x){
	fFieldOffset = x; 
}

double ExperimentalPoint::GetDepth(int level){ // taking into account the level and the quadrant 
 
    // Calculate the sensor position wrt to the base of the plate :
	// 200 is the height of the hole #8 (the hole closer to base plate bottom) with respect to the base of the pole : -200 (design)
	// 15 mm difference between holes : -15 for every level as we go up  (design)  =>  -((8-level)*15)
	// 5 mm is the half thickness of the plate : +5 mm (design)
	// 193mm is the distance from the contact surface of the pedestal to the tip of the probe : +193   (measured)
	// all the sensors have -1.8 mm offset inside the stem : -1.8 mm (specsheet)
	double z = -200 -((8-level)*15) -5 +193 -1.8 ;    
    //the base plate (zero of design) is at -1mm wrt to the target (represents the zero of comsol) 
    z = z - 1 ; 
    //add the offset due to the moving of the magnets.
    z = z - fFieldOffset ;
    // Additional info not used :
	// 11.5mm is the pedestal where the probe rests (design)
	// 104.8mm is the length of the probe, "L" in the catalogue however it's not very accurate 
	// Levels {1 to 8 }-->{ -119.8, -104.8, -89.8, -74.8, -59.8, -44.8, -29.8, -14.8}

	return z ; 
}


void ExperimentalPoint::SubtractBackground(TString Grid, int loc, double &Bx,double &By, double &Bz){ // taking into account the level and the quadrant 
 
 	TString label = Grid+Form("%d",loc); // A1
	Bx = Bx - fmapBackground[label].X() ; 
	By = By - fmapBackground[label].Y() ;
	Bz = Bz - fmapBackground[label].Z() ;
}


double ExperimentalPoint::CalculateRotationAngle(int MagnetQuadrant, TString Grid){ // returns  angle in radian 

if (Grid=="B") {
	if (MagnetQuadrant==3)	return 0 ; 
	if (MagnetQuadrant==4)	return +90 ; 
	if (MagnetQuadrant==1)	return +180 ; 
	if (MagnetQuadrant==2)	return +270 ; 
	}
if (Grid=="A") {
	if (MagnetQuadrant==2)	return 0 ; 
	if (MagnetQuadrant==3)	return +90 ; 
	if (MagnetQuadrant==4)	return +180 ; 
	if (MagnetQuadrant==1)	return +270 ; 
	}
if (Grid=="C") {
	if (MagnetQuadrant==1)	return 0 ; 
	if (MagnetQuadrant==2)	return +90 ; 
	if (MagnetQuadrant==3)	return +180 ; 
	if (MagnetQuadrant==4)	return +270 ; 
	}
if (Grid=="D") {
	if (MagnetQuadrant==4)	return 0 ; 
	if (MagnetQuadrant==1)	return +90 ; 
	if (MagnetQuadrant==2)	return +180 ; 
	if (MagnetQuadrant==3)	return +270 ; 
	}

return 0; 

} 


TVector3 ExperimentalPoint::GetOffsetDirection(TString Grid, int quad, TString Direction){

TVector3 dir(0,0,0);
if (Direction=="X") dir = fSensorOffsetX;
else if (Direction=="Y") dir = fSensorOffsetY;
	else dir = fSensorOffsetZ;

double rotangle = CalculateRotationAngle(quad, Grid);
//cout << " rotangle " << rotangle*TMath::DegToRad() << endl ; 
//cout << " dir before " <<dir.X() << " " <<  dir.Y()<< " " << dir.Z()<< endl ; 
dir.RotateZ(rotangle*TMath::DegToRad()); 
//cout << " dir after " <<dir.X() << " " <<  dir.Y()<< " " << dir.Z()<< endl ; 

return dir ; 
}

void ExperimentalPoint::CheckList(TString grid, int quad, int level){
  
  //Check if it's already in the list
  for (unsigned i = 0 ; i < flistGrid.size() ; i++)
	if(grid == flistGrid.at(i) && quad == flistQuad.at(i) && GetDepth(level) == flistDepth.at(i) ) return ;

//otherwise 
	flistGrid.push_back(grid) ;
	flistQuad.push_back(quad) ;
	flistDepth.push_back(GetDepth(level)); 

return; 
}


//Read a line and fill parameter
void ExperimentalPoint::ReadLineAndTreat(int MagnetQuadrant, TString Grid, int Location, int Level, double Bx, double By, double Bz){

	fMagnetQuadrant = MagnetQuadrant ; 
	fLocation = Location ; 
	fLevel = Level; 
  	fGrid = Grid ;
    TString key = Grid + Form("%d%d",MagnetQuadrant,Level);
    CheckList(Grid, MagnetQuadrant, Level); 
	if(fSetBackground) SubtractBackground(Grid, Location, Bx,By,Bz);
    CalculateCentralPosition() ; 

//Correct for probe rotation
    if (Grid=="B" && Location>=4 && Location<=7) {
	    // when the probe is tilted in the B grid, the sensorX receives most of the magnetic field in contrast to sensorY
	    // to keep things in order, the sensorX value could be split into (x-component an y-component) 
	    fBField.SetXYZ(Bx/10.,0,-Bz/10.); //ignore By, it will be filled by rotation
	    fBField.RotateZ(-45*TMath::DegToRad());
	    TVector3 SensorOffsetX = fSensorOffsetX; // copy the offset and rotate 45 degrees
	    SensorOffsetX.RotateZ(-45*TMath::DegToRad());
	    fSensorPositionX = fPosition + SensorOffsetX; 
	    fSensorPositionY = fPosition + SensorOffsetX; // yes it is X!
	    fSensorPositionZ = fPosition + fSensorOffsetZ;
    }
    else if (Grid=="D" && Location>=5 && Location<=7) {
			// when the probe is tilted in the D grid, the sensorY receives most of the magnetic field in contrast to sensorX
			// again, the sensorY value could be split into (x-component an y-component) 
			fBField.SetXYZ(0.,-By/10.,-Bz/10.); //ignore Bx, it will be filled by rotation
			fBField.RotateZ(-45*TMath::DegToRad());
			TVector3 SensorOffsetY = fSensorOffsetY; // copy the offset and rotate 45 degrees
			SensorOffsetY.RotateZ(-45*TMath::DegToRad());
			fSensorPositionX = fPosition + SensorOffsetY; 
			fSensorPositionY = fPosition + SensorOffsetY; // yes it is Y!
			fSensorPositionZ = fPosition + fSensorOffsetZ;
		}
		else {
			fBField.SetXYZ(Bx/10.,-By/10.,-Bz/10.); // turn to millitesla, By need to be multiplied by (-) to account for the axis change between simulation/experiment and the way the mapper is set
			fSensorPositionX = fPosition + fSensorOffsetX; 
			fSensorPositionY = fPosition + fSensorOffsetY; 
			fSensorPositionZ = fPosition + fSensorOffsetZ; 
		}

    //correct for Lens rotations 
    double angle = CalculateRotationAngle(MagnetQuadrant, Grid);
    fPosition.RotateZ(angle*TMath::DegToRad()); 
    fSensorPositionX.RotateZ(angle*TMath::DegToRad()); 
    fSensorPositionY.RotateZ(angle*TMath::DegToRad()); 
    fSensorPositionZ.RotateZ(angle*TMath::DegToRad()); 
	fBField.RotateZ(angle*TMath::DegToRad()); 
    //ShowParameters();
	//cin.get(); 
}


void ExperimentalPoint::ShowParameters(void){
	cout<<" /////////////////////// ShowParameters /////////////////////// "<<endl;
	printf("Magnet Quadrant            : %d\n",fMagnetQuadrant); 
	printf("Grid-Location              : %s%d \n",fGrid.Data(),fLocation); 
	printf("Magnetic Field    (mT)     : %2.2f\t%2.2f\t%2.2f\t   Mag %2.2f\tphi %2.2f\ttheta %2.2f\n",fBField.X(),fBField.Y(),fBField.Z(),fBField.Mag(),fBField.Phi()*TMath::RadToDeg(),fBField.Theta()*TMath::RadToDeg()); 
	printf("Position central  (mm)     : %2.2f\t%2.2f\t%2.2f\tradius %2.2f\tphi %2.2f\n",fPosition.X(),fPosition.Y(),fPosition.Z(),fPosition.Perp(),fPosition.Phi()*TMath::RadToDeg()); 
	printf("Position Sensor X (mm)     : %2.2f\t%2.2f\t%2.2f\tradius %2.2f\tphi %2.2f\n",fSensorPositionX.X(),fSensorPositionX.Y(),fSensorPositionX.Z(),fSensorPositionX.Perp(),fSensorPositionX.Phi()*TMath::RadToDeg()); 
	printf("Position Sensor Y (mm)     : %2.2f\t%2.2f\t%2.2f\tradius %2.2f\tphi %2.2f\n",fSensorPositionY.X(),fSensorPositionY.Y(),fSensorPositionY.Z(),fSensorPositionY.Perp(),fSensorPositionY.Phi()*TMath::RadToDeg()); 
	printf("Position Sensor Z (mm)     : %2.2f\t%2.2f\t%2.2f\tradius %2.2f\tphi %2.2f\n",fSensorPositionZ.X(),fSensorPositionZ.Y(),fSensorPositionZ.Z(),fSensorPositionZ.Perp(),fSensorPositionZ.Phi()*TMath::RadToDeg()); 
	printf("Field offset               : %2.2f\n",fFieldOffset); 

}

//Clear all parameters
void ExperimentalPoint::ClearParameters(){
	fGrid = "NULL" ;
	fMagnetQuadrant = -10000 ; 
	fLocation = -10000 ;
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

	file.open("./input/PositionMap.dat");
	while (file>>pos>>X>>Y){
		//cout << " pos " << pos << "  " << X << " " << Y << endl ; 
		fmapPosition.insert ( std::pair<TString,TVector2>(pos,TVector2(X,Y)));
	}
	//cout << "  finished reading the position map. " << endl ; 
}


void ExperimentalPoint::LoadBackGround(TString background){ // Earth Background
	string throwline ;
	TString grid; 
	int loc;  // level not used
	double Bx, By, Bz;
	ifstream file;
 
	file.open(background.Data());
	getline(file,throwline);
	while (file>>grid>>loc>>Bx>>By>>Bz){
		grid = grid + Form("%d",loc);
		//cout << " grid " << grid << "  " << Bx << " " << By << " " << Bz << endl ; 
		fmapBackground.insert ( std::pair<TString,TVector3>(grid,TVector3(Bx/10.,-By/10.,-Bz/10.)));
	}
	//cout << "  finished reading the background. " << endl ; 
}
