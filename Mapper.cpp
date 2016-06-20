

#include "TFile.h"

#include "ExperimentalPoint.h"
#include "SimulationPoint.h"
#include "ExpManager.h"
#include "SimManager.h"

// functions 
double mapping(TString, TString, TString);


//main 
int main(int argc,char *argv[]) {

if (argc < 3 || argc > 4  ) {
	cout << " use only 3 arguments e.g.\n  ./bin/mapperTool    [experimental-file]  [comsol-file]   [ --optional-- <background-file> ] " << endl ; 
	exit(-1);
}

double chi2 = -1 ;
if (argc == 3) chi2 = mapping(TString(argv[1]),TString(argv[2]),"nobackground");
else if(argc == 4) chi2 = mapping(TString(argv[1]),TString(argv[2]),TString(argv[3]));

cout << " chi2 " << chi2 << endl; 

 return 0;
}



double mapping(TString SimData, TString ExpData, TString background = "nobackground"){

/////////////////////// OPEN INPUT AND OUTPUT FILES ///////////////////////

SimulationPoint* SimPoint = new SimulationPoint();

// create a TH3D
TH3D *f3DHistBx = new TH3D("f3DHistBX", "Bx"         , 111,-111, 111, 111,-111, 111, 106, -106, 106); 
TH3D *f3DHistBy = new TH3D("f3DHistBY", "By"         , 111,-111, 111, 111,-111, 111, 106, -106, 106);  
TH3D *f3DHistBz = new TH3D("f3DHistBZ", "Bz"         , 111,-111, 111, 111,-111, 111, 106, -106, 106);  
TH3D *f3DHistBmag = new TH3D("f3DHistBMAG", "Bmag"   , 111,-111, 111, 111,-111, 111, 106, -106, 106);     
TH3D *f3DHistBtan = new TH3D("f3DHistBTAN", "Btan"   , 111,-111, 111, 111,-111, 111, 106, -106, 106);  
TH3D *f3DHistBdiff = new TH3D("f3DHistBDIFF", "Bdiff", 111,-111, 111, 111,-111, 111, 106, -106, 106); 
    
    
///////////////////////////// OPEN INPUT AND OUTPUT FILES ///////////////////////
// open the data file
ifstream input_exp;
TString fullpathtofile = Form("./input/%s",ExpData.Data() ) ; cout<<"opening file : |"<<ExpData<< "| ... ";
input_exp.open( ExpData.Data() );
if (!input_exp) { cout<<"problem opening experimental data file " << fullpathtofile << endl; exit(-1);}
else cout<<"Experimental file is opened "<<endl;

//open the field files from comsol  
ifstream input_sim;
fullpathtofile = Form("./input/%s",SimData.Data() ) ; 
cout<<"opening file : |"<<SimData<< "| ... ";
input_sim.open(SimData.Data() ) ; // from local input if not go seek in data 1
if(!input_sim.is_open()) {
	cout<<"problem opening simulation data file |"<< fullpathtofile <<"|"<< endl;
	SimData.ReplaceAll("./input/","");
	fullpathtofile = Form("/data1/moukaddam/MagnetsMapping/%s",SimData.Data()) ;
	cout<<"trying main directory, opening file : |"<<fullpathtofile<< "| ... ";
}
if (!input_sim.is_open()) { cout<<"problem opening simulation data file |"<< fullpathtofile <<"|"<< endl; exit(-1);}
else cout<<" Simulation file " <<  fullpathtofile <<  " is opened " <<endl;

// create root output file 
ExpData.ReplaceAll("./input/",""); ExpData.ReplaceAll(".dat",""); ExpData.ReplaceAll(".txt","");
SimData.ReplaceAll("./input/",""); SimData.ReplaceAll(".dat",""); SimData.ReplaceAll(".txt","");
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
cout<<"                                    "<<" X "<<" Y "<<" Z "<<endl;
cout<<" Simulated Field Table dimensions : "<<dimx_2<<"  "<<dimy_2<<"  "<<dimz_2<<endl;
d_buffer=-1;
while (d_buffer != 0 && counter1< 15){ // find a solution for this with do while
	input_sim>>d_buffer>>s_buffer;
	counter2++;
	}
getline(input_sim,s_buffer);
cout<< " Number of skipped lines in comsol file : " <<counter2 << "  last line content : "<<s_buffer<<endl;


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
	   if (line%5000 == 0) {
		   printf("\r     @line : %d  ... Still reading ...",line);
		   //cout<< X<<"   "<<Y <<"   "<<Z <<"   "<<BX <<"   "<<BY<<"   "<<BZ <<"   "<< EX<<"   "<< EY<<"   "<< EZ<<"   "<< Perm<<endl ;
	   }
}  
cout<<" total number of lines read : "<<line<<endl;


///////////////////////////// Create histograms with SimManager //////////////////////// 
SimManager *SimBxManager = new SimManager(f3DHistBx,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);// 0.38 radius of sensor, 0.25 on sensor position(treated elsewhere)
SimManager *SimByManager = new SimManager(f3DHistBy,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBzManager = new SimManager(f3DHistBz,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBmagManager = new SimManager(f3DHistBmag,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBtanManager = new SimManager(f3DHistBtan,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBdifManager = new SimManager(f3DHistBdiff,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);


///////////////////////////// 
/////////////////////////////  READ THE DATA FILE ///////////////////////
///////////////////////////// 

//create the map holding all the slices in z
std::map<Double_t,ExpManager> mapExpField;   

ExperimentalPoint* ExpPoint = new ExperimentalPoint(background);  
// Attention, Sensor X,Y or Z measures in all directions (not necessarly what the 
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
	input_exp >> Grid >> Position >> Level >> BX >> BY >> BZ;
	//skip lines starting by zero
    if (Quadrant == 0) continue ; 

	ExpPoint->ClearParameters();	
	ExpPoint->ReadLineAndTreat( Quadrant, Grid, Position, Level , BX , BY , BZ); 
    
	X = ExpPoint->fSensorPositionX.X() ; 
    Y = ExpPoint->fSensorPositionX.Y() ; 
	Z = ExpPoint->fSensorPositionX.Z() ; 
	R = ExpPoint->fSensorPositionX.Perp() ; 
	Theta = ExpPoint->fSensorPositionX.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint->fBField.X();
	Bsim = SimBxManager->GetPoint(X,Y,Z);
	Berr = SimBxManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[0].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[0].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint->fSensorPositionY.X() ; 
    Y = ExpPoint->fSensorPositionY.Y() ; 
	Z = ExpPoint->fSensorPositionY.Z() ; 
	R = ExpPoint->fSensorPositionY.Perp() ; 
	Theta = ExpPoint->fSensorPositionY.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint->fBField.Y();
	Bsim = SimByManager->GetPoint(X,Y,Z);
	Berr = SimByManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[1].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[1].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint->fSensorPositionZ.X() ; 
    Y = ExpPoint->fSensorPositionZ.Y() ; 
	Z = ExpPoint->fSensorPositionZ.Z() ; 
	R = ExpPoint->fSensorPositionZ.Perp() ; 
	Theta = ExpPoint->fSensorPositionZ.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint->fBField.Z();
	Bsim = SimBzManager->GetPoint(X,Y,Z);
	Berr = SimBzManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[2].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[2].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint->fPosition.X() ; 
    Y = ExpPoint->fPosition.Y() ; 
	Z = ExpPoint->fPosition.Z() ; 
	R = ExpPoint->fPosition.Perp() ; 
	Theta = ExpPoint->fPosition.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint->fBField.Mag();
	Bsim = SimBmagManager->GetPoint(X,Y,Z);
	Berr = SimBmagManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[3].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[3].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

	X = ExpPoint->fPosition.X() ; 
    Y = ExpPoint->fPosition.Y() ; 
	Z = ExpPoint->fPosition.Z() ; 
	R = ExpPoint->fPosition.Perp() ; 
	Theta = ExpPoint->fPosition.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint->fBField.Perp();
	Bsim = SimBtanManager->GetPoint(X,Y,Z);
	Berr = SimBtanManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	mapExpField[4].FillValue(X,Y,Z,R,Theta,Bexp,Bsim);
	mapExpField[4].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);

}

///////////////////////////// Create histograms with ExpManager ////////////////////////
for (unsigned i = 0 ; i <ExpPoint->flistGrid.size() ; i++) {
	cout << " Drawing graphs for " << ExpPoint->flistGrid.at(i) << " " <<  ExpPoint->flistQuad.at(i) << " " << ExpPoint->flistDepth.at(i) << endl ; 
    mapExpField.at(0).DrawGraphs(ExpPoint->flistGrid.at(i) , ExpPoint->flistQuad.at(i), ExpPoint->flistDepth.at(i));
}



mapExpField.at(0).DrawMap("Bx",-100,+100,-100,+100,-100,+100) ;  
mapExpField.at(0).GetExp1DGraphPolar("Bx",-44.9,-44.7,22.5-5,22.5+5);
mapExpField.at(0).GetExp1DGraphPolar("Bx",-44.9,-44.7,45.0-5,45.0+5);
mapExpField.at(0).GetExp1DGraphPolar("Bx",-44.9,-44.7,67.5-5,67.5+5);
mapExpField.at(0).GetSim1DGraphPolar("Bx",-44.9,-44.7,22.5-5,22.5+5); 
mapExpField.at(0).GetSim1DGraphPolar("Bx",-44.9,-44.7,45.0-5,45.0+5);
mapExpField.at(0).GetSim1DGraphPolar("Bx",-44.9,-44.7,67.5-5,67.5+5);

mapExpField.at(0).GetExp1DGraphPolar("Bx",-44.9,-44.7,90+22.5-5,90+22.5+5);
mapExpField.at(0).GetExp1DGraphPolar("Bx",-44.9,-44.7,90+45.0-5,90+45.0+5);
mapExpField.at(0).GetExp1DGraphPolar("Bx",-44.9,-44.7,90+67.5-5,90+67.5+5);
mapExpField.at(0).GetSim1DGraphPolar("Bx",-44.9,-44.7,90+22.5-5,90+22.5+5);
mapExpField.at(0).GetSim1DGraphPolar("Bx",-44.9,-44.7,90+45.0-5,90+45.0+5);
mapExpField.at(0).GetSim1DGraphPolar("Bx",-44.9,-44.7,90+67.5-5,90+67.5+5);

mapExpField.at(1).GetExp1DGraphPolar("By",-44.9,-44.7,22.5-5,22.5+5);
mapExpField.at(1).GetExp1DGraphPolar("By",-44.9,-44.7,45.0-5,45.0+5);
mapExpField.at(1).GetExp1DGraphPolar("By",-44.9,-44.7,67.5-5,67.5+5);
mapExpField.at(1).GetSim1DGraphPolar("By",-44.9,-44.7,22.5-5,22.5+5); 
mapExpField.at(1).GetSim1DGraphPolar("By",-44.9,-44.7,45.0-5,45.0+5);
mapExpField.at(1).GetSim1DGraphPolar("By",-44.9,-44.7,67.5-5,67.5+5);

mapExpField.at(1).GetExp1DGraphPolar("By",-44.9,-44.7,90+22.5-5,90+22.5+5);
mapExpField.at(1).GetExp1DGraphPolar("By",-44.9,-44.7,90+45.0-5,90+45.0+5);
mapExpField.at(1).GetExp1DGraphPolar("By",-44.9,-44.7,90+67.5-5,90+67.5+5);
mapExpField.at(1).GetSim1DGraphPolar("By",-44.9,-44.7,90+22.5-5,90+22.5+5); 
mapExpField.at(1).GetSim1DGraphPolar("By",-44.9,-44.7,90+45.0-5,90+45.0+5);
mapExpField.at(1).GetSim1DGraphPolar("By",-44.9,-44.7,90+67.5-5,90+67.5+5);

mapExpField.at(2).GetExp1DGraphPolar("Bz",-44.9,-44.7,22.5-5,22.5+5);
mapExpField.at(2).GetExp1DGraphPolar("Bz",-44.9,-44.7,45.0-5,45.0+5);
mapExpField.at(2).GetExp1DGraphPolar("Bz",-44.9,-44.7,67.5-5,67.5+5);
mapExpField.at(2).GetSim1DGraphPolar("Bz",-44.9,-44.7,22.5-5,22.5+5); 
mapExpField.at(2).GetSim1DGraphPolar("Bz",-44.9,-44.7,45.0-5,45.0+5);
mapExpField.at(2).GetSim1DGraphPolar("Bz",-44.9,-44.7,67.5-5,67.5+5);

mapExpField.at(2).GetExp1DGraphPolar("Bz",-44.9,-44.7,90+22.5-5,90+22.5+5);
mapExpField.at(2).GetExp1DGraphPolar("Bz",-44.9,-44.7,90+45.0-5,90+45.0+5);
mapExpField.at(2).GetExp1DGraphPolar("Bz",-44.9,-44.7,90+67.5-5,90+67.5+5);
mapExpField.at(2).GetSim1DGraphPolar("Bz",-44.9,-44.7,90+22.5-5,90+22.5+5); 
mapExpField.at(2).GetSim1DGraphPolar("Bz",-44.9,-44.7,90+45.0-5,90+45.0+5);
mapExpField.at(2).GetSim1DGraphPolar("Bz",-44.9,-44.7,90+67.5-5,90+67.5+5);


//mapExpField.at(0).GetExp2DGraph("Bx",-100,+100,-100,+100,-100,+100) ;
//mapExpField.at(1).GetExp2DGraph("By",-100,+100,-100,+100,-100,+100) ;
//mapExpField.at(3).GetExp2DGraph("Bmag",-100,+100,-100,+100,-100,+100) ;

///////////////////////////// Get the Simulation curves 
SimBxManager->DrawPolarOffset("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 1, "X"), 22.5, -44.8);
SimBxManager->DrawPolarOffset("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 1, "X"), 45.0, -44.8);
SimBxManager->DrawPolarOffset("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 1, "X"), 67.5, -44.8);
SimBxManager->DrawPolarOffset("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), 90+22.5, -44.8);
SimBxManager->DrawPolarOffset("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), 90+45.0, -44.8);
SimBxManager->DrawPolarOffset("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), 90+67.5, -44.8);
SimBxManager->DrawCartesianFixedY("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), 20,-44.8) ; 
SimBxManager->DrawCartesianFixedX("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), -20,-44.8) ;
SimBxManager->DrawCartesianFixedX("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), -30,-44.8) ;
SimBxManager->DrawCartesianFixedX("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), -40,-44.8) ;
SimBxManager->DrawCartesianFixedX("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), -50,-44.8) ;
SimBxManager->DrawCartesianFixedX("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), -60,-44.8) ;
SimBxManager->DrawCartesianFixedX("Bx", 100, true, ExpPoint->GetOffsetDirection("B", 2, "X"), -60,-44.8) ;

//By 
SimByManager->DrawPolarOffset("By", 100, true, ExpPoint->GetOffsetDirection("B", 1, "Y"), 22.5, -44.8);
SimByManager->DrawPolarOffset("By", 100, true, ExpPoint->GetOffsetDirection("B", 1, "Y"), 45.0, -44.8);
SimByManager->DrawPolarOffset("By", 100, true, ExpPoint->GetOffsetDirection("B", 1, "Y"), 67.5, -44.8);
SimByManager->DrawPolarOffset("By", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Y"), 90+22.5, -44.8);
SimByManager->DrawPolarOffset("By", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Y"), 90+45.0, -44.8);
SimByManager->DrawPolarOffset("By", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Y"), 90+67.5, -44.8);
SimByManager->DrawCartesianFixedY("By", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Y"), 20,-44.8) ; 
SimByManager->DrawCartesianFixedX("By", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Y"), 20,-44.8) ;

//Bz
SimBzManager->DrawPolarOffset("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 1, "Z"), 22.5, -44.8);
SimBzManager->DrawPolarOffset("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 1, "Z"), 45.0, -44.8);
SimBzManager->DrawPolarOffset("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 1, "Z"), 67.5, -44.8);
SimBzManager->DrawPolarOffset("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Z"), 90+22.5, -44.8);
SimBzManager->DrawPolarOffset("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Z"), 90+45.0, -44.8);
SimBzManager->DrawPolarOffset("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Z"), 90+67.5, -44.8);
SimBzManager->DrawCartesianFixedY("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Z"), 20,-44.8) ; 
SimBzManager->DrawCartesianFixedX("Bz", 100, true, ExpPoint->GetOffsetDirection("B", 2, "Z"), 20,-44.8) ;

//2D
//Bx
SimBxManager->Draw2DHist ("Bx",200,-100,+100,-100,+100,-44.8) ;  
//By
SimByManager->Draw2DHist ("By",200,-100,+100,-100,+100,-44.8) ;  
//Bz
SimBzManager->Draw2DHist ("Bz",200,-100,+100,-100,+100,-44.8) ;  
//Bmag Btan Bdiff
SimBmagManager->Draw2DHist("Bmag",500,-100,+100,-100,+100,-44.8);
SimBtanManager->Draw2DHist("Btan",500,-100,+100,-100,+100,-44.8);
SimBdifManager->Draw2DHist("Bdif",500,-100,+100,-100,+100,-44.8);

//Close the file on disk
outputFile.Close();

///////////////////////////// Get the chi2 from the measurments (Bx,By,Bz) ///////////////////////
double sumchi2x = mapExpField.at(0).GetSumChi2(); 
double sumchi2y = mapExpField.at(1).GetSumChi2(); 
double sumchi2z = mapExpField.at(2).GetSumChi2(); 

int ndfx = mapExpField.at(0).GetNpoints(); 
int ndfy = mapExpField.at(1).GetNpoints(); 
int ndfz = mapExpField.at(2).GetNpoints(); 

double Allchi2 = sumchi2x + sumchi2y + sumchi2z ; 
int Allndf = ndfx + ndfy + ndfz ; 


 return Allchi2/Allndf;
}
