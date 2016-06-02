

#include "TFile.h"

//#include "OptimizeParameters.h"
#include "ExperimentalPoint.h"
#include "SimulationPoint.h"
#include "GraphManager.h"
#include "HistManager.h"


#define SENSORXOFFSETX  +2.1
#define SENSORXOFFSETY  +0.0
#define SENSORXOFFSETZ  -1.8
  
#define SENSORYOFFSETX  +0.0
#define SENSORYOFFSETY  +2.1
#define SENSORYOFFSETZ  -1.8
 
#define SENSORZOFFSETX  +0.0
#define SENSORZOFFSETY  +0.0
#define SENSORZOFFSETZ  -1.8



// functions 
int optimizing(void) ;
int mapping(void);


int main() {

// optimize the values of offset 
//optimizing();

//map the measured values 
mapping();

 return 1;
}




int optimizing() {
///////////////////////////// OPTIMIZE paramters ///////////////////////////
TString file1,file2;
file1="MEL_MagFieldExp.dat";
file2="MEL_MagFieldTablePaths.dat";  
//OptimizeParameters* optimizer = new OptimizeParameters(file1,file2) ;
//delete optimizer ;

return 1 ; 
}


int mapping(){

/////////////////////// OPEN INPUT AND OUTPUT FILES ///////////////////////

SimulationPoint* SimPoint = new SimulationPoint();

// create a TH3D
TH3D *f3DHistBx = new TH3D("f3DHistBX", "Bx"         , 111,-111, 111, 111,-111, 111, 106, -106, 106); 
TH3D *f3DHistBy = new TH3D("f3DHistBY", "By"         , 111,-111, 111, 111,-111, 111, 106, -106, 106);  
TH3D *f3DHistBz = new TH3D("f3DHistBZ", "Bz"         , 111,-111, 111, 111,-111, 111, 106, -106, 106);  
TH3D *f3DHistBmag = new TH3D("f3DHistBMAG", "Bmag"   , 111,-111, 111, 111,-111, 111, 106, -106, 106);     
TH3D *f3DHistBtan = new TH3D("f3DHistBTAN", "Btan"   , 111,-111, 111, 111,-111, 111, 106, -106, 106);  
TH3D *f3DHistBdiff = new TH3D("f3DHistBDIFF", "Bdiff", 111,-111, 111, 111,-111, 111, 106, -106, 106); 
//TH3D *f3DHistBmag_nim = new TH3D("f3DHistBMAG", "Bmag", 160 , -400, 400, 160, -400, 400, 30, -100, 50);       // for the nim paper 
    
    
///////////////////////////// OPEN INPUT AND OUTPUT FILES ///////////////////////
TString fullpathtofile;  
// name of files
TString ExpData ="tableExpGridB.dat";
//TString SimData ="SPICEField3D.v51.3.21.TABLE";
TString SimData ="SPICEField3Dv5220.dat";

// open the data file
ifstream input_exp;
fullpathtofile = Form("./input/%s",ExpData.Data() ) ; 
cout<<"opening file : |"<<fullpathtofile<< "| ... ";
input_exp.open( fullpathtofile.Data() );
if (!input_exp) { cout<<"problem opening experimental data file " << fullpathtofile << endl; exit(-1);}
else cout<<"Experimental file is opened "<<endl;

//open the field files from comsol  
ifstream input_sim;
fullpathtofile = Form("./input/%s",SimData.Data() ) ; 
cout<<"opening file : |"<<fullpathtofile<< "| ... ";
input_sim.open(fullpathtofile.Data() ) ; // from local input if not go seek in data 1
if(!input_sim.is_open()) {
	cout<<"problem opening simulation data file |"<< fullpathtofile <<"|"<< endl;
	fullpathtofile = Form("/data1/moukaddam/MagnetsMapping/%s",SimData.Data()) ;
	cout<<"trying main directory, opening file : |"<<fullpathtofile<< "| ... ";
}
if (!input_sim.is_open()) { cout<<"problem opening simulation data file |"<< fullpathtofile <<"|"<< endl; exit(-1);}
else cout<<" Simulation file " <<  fullpathtofile <<  " is opened " <<endl;

// create root output file 
TString  filename = "./output/compare_" + ExpData + "_" + SimData + ".root";
TFile outputFile(filename,"RECREATE");

///////////////////////////// 
/////////////////////////////  READ THE SIMULATION FILE ///////////////////////
///////////////////////////// 

// dump the first lines from the top
string s_buffer="buffer";
Int_t d_buffer=-1;
Int_t counter1=0; // counter on the first lines
Int_t counter2=0; // counter on the first lines

// read the first lines
Int_t dimx_2, dimy_2, dimz_2;
input_sim>>dimx_2>>dimy_2>>dimz_2;
cout<<"                                    "<<" X "<<"  "<<" Y "<<"  "<<" Z "<<endl;
cout<<" Simulated Field Table dimensions : "<<dimx_2<<"  "<<dimy_2<<"  "<<dimz_2<<endl;

//reset buffer
d_buffer=-1;
while (d_buffer != 0 && counter1< 15){ // find a solution for this with do while
	input_sim>>d_buffer>>s_buffer;
	counter2++;
	}

//test for table 2
cout<<"READING FIELD TABLE"<<endl;
getline(input_sim,s_buffer);
cout<<counter2<<s_buffer<<endl;

// read and fill
Double_t X(0), Y(0), Z(0), EX(-100), EY(-100), EZ(-100), Perm(0);
Double_t BX(0), BY(0), BZ(0);
Int_t line=0;

while ( !input_sim.eof() ) {
	   //Clear parameters
	   SimPoint->ClearParameters();
	   
	   // Choose format
	   if(counter2<9)   input_sim >> X >> Y >> Z >> BX >> BY >> BZ >> Perm ;
	   else   			input_sim >> X >> Y >> Z >> BX >> BY >> BZ >> EX >> EY >> EZ >> Perm ;
	   
		// Read the line, apply offset and do some modifications
		//Z = Z + 4 ; //Simulation is offset by -4mm, this corrects for it
		SimPoint->ReadLineAndTreat(X,Y,Z,BX,BY,BZ,EX,EY,EZ,Perm );
		//SimPoint->Show();

		// fill in TH3D all the simulation data
		Int_t binNumber = f3DHistBx->FindBin(X,Y,Z);
		f3DHistBx->SetBinContent(binNumber,BX);
		f3DHistBy->SetBinContent(binNumber,BY);
		f3DHistBz->SetBinContent(binNumber,BZ);	
		f3DHistBmag->SetBinContent(binNumber,SimPoint->fBFieldMag);
		f3DHistBtan->SetBinContent(binNumber,SimPoint->fBFieldTan);
		f3DHistBdiff->SetBinContent(binNumber,SimPoint->fBFieldDiff); //fBFieldMag-fBFieldTan 		
		//count the lines for inspection
		line++;
	   if (line%100000 == 0) {
		   cout<<"line : "<<line << "  ... Still reading ..."<<endl;
		   //cout<< X<<"   "<<Y <<"   "<<Z <<"   "<<BX <<"   "<<BY<<"   "<<BZ <<"   "<< EX<<"   "<< EY<<"   "<< EZ<<"   "<< Perm<<endl ;
	   }
}  // end of loop while
//count the lines 
cout<<" "<<line << " total number of lines "<<endl;

///////////////////////////// Create histograms with Histmanager //////////////////////// 
//Bx
HistManager *SimBxManager = new HistManager(f3DHistBx, 0.4/*mm*/, 0.4/*mm*/, 0.4/*mm*/);
SimBxManager->Draw2DHist ("bx",200,-100,+100,-100,+100,-30) ;  
SimBxManager->Draw2DGraph("bx",200,-100,+100,-100,+100,-30) ;  
SimBxManager->DrawPolarOffsetX("Bx", 100, true, 2.1, 22.5, -30); 
SimBxManager->DrawPolarOffsetX("Bx (no offset)", 100, false, 0.0, 22.5, -30);
SimBxManager->DrawPolarOffsetX("Bx", 100, false, 2.1, 45.0, -30);
SimBxManager->DrawPolarOffsetX("Bx", 100, true, 2.1, 67.5, -30);
SimBxManager->DrawCartesianFixedY("Bx cartesian ", 100, false, 20,-30) ; 
SimBxManager->DrawCartesianFixedX("Bx cartesian ", 100, false, 20,-30) ;

//By 
HistManager *SimByManager = new HistManager(f3DHistBy, 0.4/*mm*/, 0.4/*mm*/, 0.4/*mm*/);
SimByManager->DrawPolarOffsetX("By", 100, true, 2.1, 22.5, -30);
SimByManager->DrawPolarOffsetX("By", 100, true, 2.1, 45.0, -30);
SimByManager->DrawPolarOffsetX("By", 100, false, 2.1, 67.5, -30); 

//Bz
HistManager *SimBzManager = new HistManager(f3DHistBz, 0.4/*mm*/, 0.4/*mm*/, 0.4/*mm*/);
SimBzManager->DrawPolarOffsetY("Bz", 100, false, 2.1, 22.5, -30);
SimBzManager->DrawPolarOffsetY("Bz", 100, true, 2.1, 45.0, -30); 
SimBzManager->DrawPolarOffsetY("Bz", 100, true, 2.1, 67.5, -30); 

//Mag
HistManager *SimBmagManager = new HistManager(f3DHistBmag, 0.4/*mm*/, 0.4/*mm*/, 0.4/*mm*/);
SimBmagManager->Draw2DHist("bmag",200,-100,+100,-100,+100,-30);
SimBmagManager->Draw2DHist("bmag",300,-100,+100,-100,+100,-30);
SimBmagManager->Draw2DHist("bmag",500,-100,+100,-100,+100,-30);

//tan
HistManager *SimBtanManager = new HistManager(f3DHistBtan, 0.4/*mm*/, 0.4/*mm*/, 0.4/*mm*/);
SimBtanManager->Draw2DHist("btan",100,-100,+100,-100,+100,-30);
SimBtanManager->Draw2DHist("btan",200,-100,+100,-100,+100,-30);
SimBtanManager->Draw2DHist("btan",300,-100,+100,-100,+100,-30);

//diff
HistManager *SimBdifManager = new HistManager(f3DHistBdiff, 0.4/*mm*/, 0.4/*mm*/, 0.4/*mm*/);
SimBdifManager->Draw2DHist("bdif",200,-100,+100,-100,+100,-30);


///////////////////////////// 
/////////////////////////////  READ THE DATA FILE ///////////////////////
///////////////////////////// 

//create the map holding all the slices in z
std::map<Double_t,GraphManager> mapExpField; 
std::map<Double_t,GraphManager> mapExpFieldMag;
std::map<Double_t,GraphManager> mapExpFieldX;
std::map<Double_t,GraphManager> mapExpFieldY;
std::map<Double_t,GraphManager> mapExpFieldZ;   
std::map<Double_t,GraphManager>::iterator it;

//ExperimentalPoint* ExpPointMag = new ExperimentalPoint();
ExperimentalPoint* ExpPoint[3]; 
ExpPoint[0] = new ExperimentalPoint();  // Attention, Sensor X,Y or Z measures in all directions (not necessarly what the 
										// name suggest). This is due to the fact that lens is rotated with respect to mapper plate in some cases

//FORMAT : 1	A	1	3	-904.5	568.7	232.7
TString  Grid=" ";
Double_t Quadrant(0);
Double_t Level(0);
Int_t Position ; 
s_buffer=" ";
d_buffer=-1;
counter1=0; // counter on the first lines

for (int i = 0 ; i < 1 ; i++){ // number of lines to neglect
	getline(input_exp,s_buffer);
	counter1++;
}

double R,Theta,Bexp,Bsim,Berr;
while (input_exp >> Quadrant){
   // read one line and do some changes 
    input_exp >> Grid >> Position >> Level >> BX >> BY >> BZ;
    //cout <<  Quadrant << " " << Grid << " " << Position << " " << Level << " " << BX << " " << BY << " " << BZ << endl ; 
	ExpPoint[0]->ClearParameters();	
	ExpPoint[0]->ReadLineAndTreat( Quadrant, Grid, Position, Level , BX , BY , BZ); 
    
	X = ExpPoint[0]->fSensorPositionX.X() ; 
    Y = ExpPoint[0]->fSensorPositionX.Y() ; 
	Z = ExpPoint[0]->fSensorPositionX.Z() ; 
	R = ExpPoint[0]->fSensorPositionX.Perp() ; 
	Theta = ExpPoint[0]->fSensorPositionX.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint[0]->fBField.X();
	Bsim = SimBxManager->GetPoint(X,Y,Z);
	Berr = SimBxManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[0].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[0].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint[0]->fSensorPositionY.X() ; 
    Y = ExpPoint[0]->fSensorPositionY.Y() ; 
	Z = ExpPoint[0]->fSensorPositionY.Z() ; 
	R = ExpPoint[0]->fSensorPositionY.Perp() ; 
	Theta = ExpPoint[0]->fSensorPositionY.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint[0]->fBField.Y();
	Bsim = SimByManager->GetPoint(X,Y,Z);
	Berr = SimByManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[1].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[1].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint[0]->fSensorPositionZ.X() ; 
    Y = ExpPoint[0]->fSensorPositionZ.Y() ; 
	Z = ExpPoint[0]->fSensorPositionZ.Z() ; 
	R = ExpPoint[0]->fSensorPositionZ.Perp() ; 
	Theta = ExpPoint[0]->fSensorPositionZ.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint[0]->fBField.Z();
	Bsim = SimBzManager->GetPoint(X,Y,Z);
	Berr = SimBzManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[2].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[2].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint[0]->fPosition.X() ; 
    Y = ExpPoint[0]->fPosition.Y() ; 
	Z = ExpPoint[0]->fPosition.Z() ; 
	R = ExpPoint[0]->fPosition.Perp() ; 
	Theta = ExpPoint[0]->fPosition.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint[0]->fBField.Mag();
	Bsim = SimBmagManager->GetPoint(X,Y,Z);
	Berr = SimBmagManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[3].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[3].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint[0]->fPosition.X() ; 
    Y = ExpPoint[0]->fPosition.Y() ; 
	Z = ExpPoint[0]->fPosition.Z() ; 
	R = ExpPoint[0]->fPosition.Perp() ; 
	Theta = ExpPoint[0]->fPosition.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint[0]->fBField.Perp();
	Bsim = SimBtanManager->GetPoint(X,Y,Z);
	Berr = SimBtanManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[4].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[4].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

}


///////////////////////////// Create histograms with Graphmanager ////////////////////////
mapExpField.at(0).GetExp1DGraphPolar("Bx",49,50,-69,-67);
mapExpField.at(0).GetExp1DGraphPolar("Bx",49,50,-48,-45);
mapExpField.at(0).GetExp1DGraphPolar("Bx",49,50,-26,-23);

mapExpField.at(0).GetSim1DGraphPolar("Bx",49,50,-69,-67);
mapExpField.at(0).GetSim1DGraphPolar("Bx",49,50,-48,-45);
mapExpField.at(0).GetSim1DGraphPolar("Bx",49,50,-26,-23); 

mapExpField.at(3).GetExp2DGraph("Bmag",-100,+100,-100,+100,-100,+100) ;


//Close the file on disk
	outputFile.Close();

 return 0;
}
