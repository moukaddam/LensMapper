#include "ExperimentalPoint.h"

ExperimentalPoint::ExperimentalPoint(void)
{
//cout << "Inside ExperimentalPoint::ExperimentalPoint()" << endl;
fSensorOffset.SetXYZ(0,0,0);
}

ExperimentalPoint::ExperimentalPoint(double offset_x, double offset_y, double offset_z)
{
//cout << "Inside ExperimentalPoint::ExperimentalPoint()" << endl;
fSensorOffset.SetXYZ(offset_x, offset_y, offset_z);
}

ExperimentalPoint::~ExperimentalPoint(void)
{
//cout << "Inside ExperimentalPoint::Destructor()" << endl;

}


//Read a line and fill parameter
void ExperimentalPoint::ReadLineAndTreat(TString PlateLabel, Int_t MagnetQuadrant, Int_t ProbeAngle, Double_t  XR, Double_t  YT, Double_t  Z , Double_t Bx, Double_t By, Double_t Bz)
{
fPlateLabel = PlateLabel ;
fMagnetQuadrant = MagnetQuadrant ; 
fProbeAngle = ProbeAngle ;

fPrePosition.SetXYZ(XR,YT,Z ); 

	if(fProbeAngle == 0){
		// No operation on sensor positions
		fBField.SetXYZ(Bx,By,Bz);
    			      }
	else if(fProbeAngle == 90){
		fBField.SetXYZ(-By,Bx,Bz);
		fSensorOffset.SetXYZ(-fSensorOffset.Y(),fSensorOffset.X(),fSensorOffset.Z()) ;
				  }
	else if(fProbeAngle == 180){
		fBField.SetXYZ(-Bx,-By,Bz);
		fSensorOffset.SetXYZ(-fSensorOffset.X(),-fSensorOffset.Y(),fSensorOffset.Z()) ;
	    			  }
	else if(fProbeAngle == 270){
		fBField.SetXYZ(By,-Bx,Bz);
		fSensorOffset.SetXYZ( fSensorOffset.Y(),-fSensorOffset.X(),fSensorOffset.Z()) ;
				  }
				  
	
Treat();


}


void ExperimentalPoint::SetSensorOffset( double x, double y, double z )
{
fSensorOffset.SetXYZ(x,y,z);
}

void ExperimentalPoint::ShowParameters(void)
{
cout<<" /////////////////////// ShowParameters /////////////////////// "<<endl;
cout<<endl<<"Plate label : "<<fPlateLabel<<endl;
cout<<endl<<"Magnet Quadrant : "<<fMagnetQuadrant <<endl;
cout<<endl<<"Probe angle : "<<fProbeAngle <<endl;
cout<<endl<<"Magnetic Field : "<<fBField.X()<<" ; "<<fBField.Y()<<" ; "<<fBField.Z()<<endl;
cout<<endl<<"Pre-treated Position : "<<fPrePosition.X()<<" ; "<<fPrePosition.Y()<<" ; "<<fPrePosition.Z()<<endl;
cout<<endl<<"Position : "<<fPosition.X()<<" ; "<<fPosition.Y()<<" ; "<<fPosition.Z()<<endl;
cout<<" //////////////////////////////////////////////////////////////// "<<endl;
}


//Clear all parameters
void ExperimentalPoint::ClearParameters()
{
fPlateLabel = "NULL" ;
fMagnetQuadrant = -100 ; 
fProbeAngle = -100 ;
fBField.SetXYZ(-100,-100,-100); 
fBFieldMag=-100;
fPrePosition.SetXYZ(-100,-100,-100);
fPosition.SetXYZ(-100,-100,-100);
}

//convert  to absolute positions taking into account fPlateLabel, fMagnetQuadrant, fProbe
    void ExperimentalPoint::Treat()// this could be automatic
{

    if (fPlateLabel=="A" || fPlateLabel=="B" || fPlateLabel=="B'") {
        
        Double_t r=0;
        //Double_t r0=0;
        Double_t theta=0;

           // if (fPrePosition.Y() == 45) r0 = 20; // 2 cm
           // else r0 = 20.*10./7. ; //cf the mapper by Robert Henderson
           
        r =  fPrePosition.X() ; //cf the mapper by Robert Henderson
        theta = fPrePosition.Y()*TMath::Pi()/180.0; // turn angle in rad
        
        //offset correction
        fPosition.SetX( r*TMath::Cos(theta) + fSensorOffset.X());
        fPosition.SetY( r*TMath::Sin(theta) + fSensorOffset.Y());
        fPosition.SetZ( fPrePosition.Z()    + fSensorOffset.Z());
        
               
        // x and y -> R theta
        fPositionAngle = theta*TMath::RadToDeg(); // in degrees   fPosition.Phi()*TMath::RadToDeg();
        fPositionRadius = fPosition.Perp(); // in mm 

       //Inspection
       //fPrePosition.Dump();
       //fPosition.Dump();   

    }
    else { // fPlateLabel=="C" or "D"
    
        fPosition.SetX( fPrePosition.X() + fSensorOffset.X());
        fPosition.SetY( fPrePosition.Y() + fSensorOffset.Y());
        fPosition.SetZ( fPrePosition.Z() + fSensorOffset.Z());

        // x and y -> R theta
        fPositionAngle = fPosition.Phi() * 180/TMath::Pi(); // in degrees
        fPositionRadius = fPosition.Perp(); // in mm 
    }
    


// absolute magnitudes
fBFieldMag=fBField.Mag(); 
}




