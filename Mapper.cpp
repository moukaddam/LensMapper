

#include "TFile.h"
#include "TDirectory.h"

#include "ExperimentalPoint.h"
#include "SimulationPoint.h"
#include "ExpManager.h"
#include "SimManager.h"

//global variables
int gVerbose = 0 ; 
// functions 
double mapping(TString, TString, TString);


//main 
int main(int argc,char *argv[]) {

	if (argc < 3 || argc > 4  ) {
		cout << " use 3 arguments e.g.\n  ./bin/mapperTool    [experimental-file]  [comsol-file]   [ --optional-- <background-file> ] " << endl ; 
		exit(-1);
	}

	double chi2 = -1 ;
	if (argc == 3) chi2 = mapping(TString(argv[1]),TString(argv[2]),"nobackground");
	else if(argc == 4) chi2 = mapping(TString(argv[1]),TString(argv[2]),TString(argv[3]));

	cout <<" file : "<< argv[1] <<"      chi2 : " << chi2 << endl; 

	return 0;
}



double mapping(TString SimData, TString ExpData, TString background = "nobackground"){

/////////////////////// OPEN INPUT AND OUTPUT FILES ///////////////////////
// open the data file
ifstream input_exp;
TString fullpathtofile = Form("%s",ExpData.Data() ) ; 
if(gVerbose>0) cout<<"opening file : |"<<ExpData<< "| ... ";
input_exp.open( ExpData.Data() );
if (!input_exp) { cout<<"problem opening experimental data file " << fullpathtofile << endl; exit(-1);}
else if(gVerbose>0) cout<<"Experimental file is opened "<<endl;

//open the field files from comsol  
ifstream input_sim;
fullpathtofile = Form("%s",SimData.Data() ) ; 
if(gVerbose>0) cout<<"opening file : |"<<SimData<< "| ... ";
input_sim.open(SimData.Data() ) ; 
if(!input_sim.is_open()) {
	if(gVerbose>0) cout<<"problem opening simulation data file |"<< fullpathtofile <<"|"<< endl;
	SimData.ReplaceAll("./input/","");
	fullpathtofile = Form("/data1/moukaddam/MagnetSimulation/%s",SimData.Data()) ;
	if(gVerbose>0) cout<<"trying main directory, opening file : |"<<fullpathtofile<< "| ... ";
	input_sim.open(fullpathtofile.Data() ) ; // from local input if not go seek in data 1
	if (!input_sim.is_open()) { cout<<"problem opening simulation data file |"<< fullpathtofile <<"|"<< endl<<endl; exit(-1);}
}
else if(gVerbose>0) cout<<" Simulation file " <<  fullpathtofile <<  " is opened " <<endl;

// create root output file 
ExpData.ReplaceAll("./input/",""); ExpData.ReplaceAll(".dat",""); ExpData.ReplaceAll(".txt","");
SimData.ReplaceAll("./input/",""); SimData.ReplaceAll(".dat",""); SimData.ReplaceAll(".txt","");
TString  filename = "./output/compare_" + ExpData + "_" + SimData + ".root";
TFile outputFile(filename,"RECREATE");
TDirectory *dirquad[4] ; 
dirquad[0] = outputFile.mkdir("Quad_1");
dirquad[1] = outputFile.mkdir("Quad_2");
dirquad[2] = outputFile.mkdir("Quad_3");
dirquad[3] = outputFile.mkdir("Quad_4");

//Read the simulation file from comsol 
// dump the first lines from the top
string s_buffer="buffer";
Int_t d_buffer=-1;
Int_t counter1=0; // counter on the first lines
Int_t counter2=0; // counter on the first lines
// read the first lines
Int_t dimX, dimY, dimZ;
input_sim>>dimX>>dimY>>dimZ;
if(gVerbose>0) cout<<"\nSimulated Field Table dimensions              X : "<<dimX<<"     Y : "<<dimY<<"     Z : "<<dimZ<<endl;
dimX=dimX+10;
dimY=dimY+10;
dimZ=dimZ+10;
TH3D *f3DHistBx = new TH3D("f3DHistBX", "Bx"         , dimX,-dimX, dimX, dimY,-dimY, dimY, dimZ, -dimZ, dimZ); 
TH3D *f3DHistBy = new TH3D("f3DHistBY", "By"         , dimX,-dimX, dimX, dimY,-dimY, dimY, dimZ, -dimZ, dimZ);  
TH3D *f3DHistBz = new TH3D("f3DHistBZ", "Bz"         , dimX,-dimX, dimX, dimY,-dimY, dimY, dimZ, -dimZ, dimZ);  
TH3D *f3DHistBmag = new TH3D("f3DHistBMAG", "Bmag"   , dimX,-dimX, dimX, dimY,-dimY, dimY, dimZ, -dimZ, dimZ);     
TH3D *f3DHistBtan = new TH3D("f3DHistBTAN", "Btan"   , dimX,-dimX, dimX, dimY,-dimY, dimY, dimZ, -dimZ, dimZ);  
TH3D *f3DHistBdiff = new TH3D("f3DHistBDIFF", "Bdiff", dimX,-dimX, dimX, dimY,-dimY, dimY, dimZ, -dimZ, dimZ); 

d_buffer=-1;
while (d_buffer != 0 && counter1< 15){ // find a solution for this with do while
	input_sim>>d_buffer>>s_buffer;
	counter2++;
	}
getline(input_sim,s_buffer);
if(gVerbose>0) cout<< " Number of skipped lines in comsol file : " <<counter2 << "  last line content : "<<s_buffer<<endl;

// read and fill
SimulationPoint* SimPoint = new SimulationPoint();
Double_t X(0), Y(0), Z(0), EX(-100), EY(-100), EZ(-100), Perm(0);
Double_t BX(0), BY(0), BZ(0);
Int_t line=0;

while ( !input_sim.eof() ) {
	   //Clear parameters
	   SimPoint->ClearParameters();
	   // Choose format
	   if(counter2<9)   input_sim >> X >> Y >> Z >> BX >> BY >> BZ >> Perm ;
	   else   			input_sim >> X >> Y >> Z >> BX >> BY >> BZ >> EX >> EY >> EZ >> Perm ;
	    
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
		   if(gVerbose>0) printf("\r     @line : %d  ... Still reading ...",line);
		   if(gVerbose>1) cout<< X<<"   "<<Y <<"   "<<Z <<"   "<<BX <<"   "<<BY<<"   "<<BZ <<"   "<< EX<<"   "<< EY<<"   "<< EZ<<"   "<< Perm<<endl ;
	   }
}  
if(gVerbose>0) cout<<" total number of lines read : "<<line<<endl;


///////////////////////////// Create histograms with SimManager //////////////////////// 
SimManager *SimBxManager = new SimManager(f3DHistBx,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);// 0.38 radius of sensor, 0.25 on sensor position(treated elsewhere)
SimManager *SimByManager = new SimManager(f3DHistBy,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBzManager = new SimManager(f3DHistBz,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBmagManager = new SimManager(f3DHistBmag,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBtanManager = new SimManager(f3DHistBtan,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);
SimManager *SimBdifManager = new SimManager(f3DHistBdiff,0.38/*mm*/,0.38/*mm*/,0.38/*mm*/);


// read the data file 
//create the map holding all the slices in z
map<Double_t,ExpManager> mapExpField;
mapExpField[0].SetInfo("Bx"); 
mapExpField[1].SetInfo("By");
mapExpField[2].SetInfo("Bz");
mapExpField[3].SetInfo("Bmag");      
mapExpField[4].SetInfo("Btan"); // tangential field   

ExperimentalPoint* ExpPoint = new ExperimentalPoint(background);  
ExpPoint->SetFieldOffset(0); // if magnets are moved 1mm towards the target (positive z direction), the value is +1

// Attention, Sensor X,Y or Z measures in all directions (not necessarly what the 
// name suggest). This is due to the fact that lens is rotated with respect to mapper plate in some cases
// The ExperimentalPoint Class will take care fo all these rotations once the location of the measurment is provided 

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
	if(gVerbose>1) cout << "Quadrant : " << Quadrant <<endl;
	if (Quadrant == 0) { getline(input_exp,s_buffer); continue ;} 	//skip lines starting by zero
	//FORMAT : 1	A	1	3	-904.5	568.7	232.7
	input_exp >> Grid >> Position >> Level >> BX >> BY >> BZ;
	if(gVerbose>1) cout << "     Grid "<< Grid << "  " << Position << "    level " << Level << "     Bx:By:Bz " << BX << " " << BY << " " << BZ <<endl;

	ExpPoint->ClearParameters();	
	ExpPoint->ReadLineAndTreat( Quadrant, Grid, Position, Level , BX , BY , BZ); 
	//ExpPoint->ShowParameters(); 

	X = ExpPoint->fSensorPositionX.X() ; 
    Y = ExpPoint->fSensorPositionX.Y() ; 
	Z = ExpPoint->fSensorPositionX.Z() ; 
	R = ExpPoint->fSensorPositionX.Perp() ; 
	Theta = ExpPoint->fSensorPositionX.Phi()*TMath::RadToDeg() ; 
	Bexp = ExpPoint->fBField.X();
	Bsim = SimBxManager->GetPoint(X,Y,Z);
	Berr = SimBxManager->GetPointError(X,Y,Z); // used as an error on the experimental values 
	//
	if(gVerbose>2) cout << "     XYZ "<< X << "  " << Y << "  " << Z << "  R " << R << "  theta " << Theta << "   Bexp " << Bexp <<endl;
	mapExpField[0].FillValue(X,Y,Z,R,Theta,Bexp,Bsim,Grid);
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
	if(gVerbose>2) cout << "     XYZ "<< X << "  " << Y << "  " << Z << "  R " << R << "  theta " << Theta << "   Bexp " << Bexp <<endl;
	mapExpField[1].FillValue(X,Y,Z,R,Theta,Bexp,Bsim,Grid);
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
	if(gVerbose>2) cout << "     XYZ "<< X << "  " << Y << "  " << Z << "  R " << R << "  theta " << Theta << "   Bexp " << Bexp <<endl;
	mapExpField[2].FillValue(X,Y,Z,R,Theta,Bexp,Bsim,Grid);
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
	if(gVerbose>2) cout << "     XYZ "<< X << "  " << Y << "  " << Z << "  R " << R << "  theta " << Theta << "   Bexp " << Bexp <<endl;
	mapExpField[3].FillValue(X,Y,Z,R,Theta,Bexp,Bsim,Grid);
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
	if(gVerbose>2) cout << "     XYZ "<< X << "  " << Y << "  " << Z << "  R " << R << "  theta " << Theta << "   Bexp " << Bexp <<endl;
	mapExpField[4].FillValue(X,Y,Z,R,Theta,Bexp,Bsim,Grid);
	mapExpField[4].FillValueError(0.5,0.5,0.5,0.7,0.5,Berr);
	
}

///////////////////////////// Create histograms with ExpManager ////////////////////////
for (unsigned i = 0 ; i <ExpPoint->flistGrid.size() ; i++) {
	
    TString grid = ExpPoint->flistGrid.at(i);     
    int quad = ExpPoint->flistQuad.at(i);    
    double depth = ExpPoint->flistDepth.at(i);  
    //cout << " Drawing graphs for      Grid :" << grid << "     Quad : " << quad << "     depth : " << depth << endl ; 
    
	outputFile.cd(); 
    dirquad[quad-1]->cd();
    TDirectory *dir = dirquad[quad-1]->GetDirectory(Form("Depth_%.2f_Grid_",depth)+grid);
	if (!dir) // if not found create it
	    dir = dirquad[quad-1]->mkdir(Form("Depth_%.2f_Grid_",depth)+grid); 
	dir->cd(); 

    mapExpField.at(0).DrawGraphs(grid, quad, depth);
    mapExpField.at(1).DrawGraphs(grid, quad, depth);
    mapExpField.at(2).DrawGraphs(grid, quad, depth);

    SimBxManager->DrawGraphs(ExpPoint->GetOffsetDirection(grid, quad, "X"), grid, quad, depth);
    SimByManager->DrawGraphs(ExpPoint->GetOffsetDirection(grid, quad, "Y"), grid, quad, depth);
    SimBzManager->DrawGraphs(ExpPoint->GetOffsetDirection(grid, quad, "Z"), grid, quad, depth);
}

//2D
outputFile.cd(); 
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
