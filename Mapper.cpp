
//#include "OptimizeParameters.h"
#include "ExperimentalPoint.h"
#include "SimulationPoint.h"
#include "GraphManager.h"
#include "HistManager.h"


// functions 
int optimizing(void) ;
int mapping(void);

int main() {

// optimize the values of offset 
//optimizing();

//map the intersting values 
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

ExperimentalPoint* ExpPointMag = new ExperimentalPoint();
ExperimentalPoint* ExpPointX = new ExperimentalPoint();
ExperimentalPoint* ExpPointY = new ExperimentalPoint();
ExperimentalPoint* ExpPointZ = new ExperimentalPoint();

// Set Offsets of Sensor
//ExpPointX->SetSensorOffset();
//ExpPointY->SetSensorOffset();
//ExpPointZ->SetSensorOffset();
//ExpPointMag->SetSensorOffset();  // the barycentre

SimulationPoint* SimPoint = new SimulationPoint();

       
  //create the map holding all the slices in z 
  std::map<Double_t,GraphManager> mapExpFieldMag;
  std::map<Double_t,GraphManager> mapExpFieldX;
  std::map<Double_t,GraphManager> mapExpFieldY;
  std::map<Double_t,GraphManager> mapExpFieldZ;   
  std::map<Double_t,GraphManager>::iterator it;

  
  // create a TH3D
  TH3D *f3dHistBx = new TH3D("f3dHistBX", "Bx"         , 111,-111, 111, 111,-111, 111, 151, -151, 151); 
  TH3D *f3dHistBy = new TH3D("f3dHistBY", "By"         , 111,-111, 111, 111,-111, 111, 151, -151, 151);  
  TH3D *f3dHistBz = new TH3D("f3dHistBZ", "Bz"         , 111,-111, 111, 111,-111, 111, 151, -151, 151);  
  TH3D *f3dHistBmag = new TH3D("f3dHistBMAG", "Bmag"   , 111,-111, 111, 111,-111, 111, 151, -151, 151);     
  TH3D *f3dHistBtan = new TH3D("f3dHistBTAN", "Btan"   , 111,-111, 111, 111,-111, 111, 151, -151, 151);  
  TH3D *f3dHistBdiff = new TH3D("f3dHistBDIFF", "Bdiff", 111,-111, 111, 111,-111, 111, 151, -151, 151); 
    
  //TH3D *f3dHistBmag_nim = new TH3D("f3dHistBMAG", "Bmag", 160 , -400, 400, 160, -400, 400, 30, -100, 50);       // for the nim paper 
    
    
///////////////////////////// OPEN INPUT AND OUTPUT FILES ///////////////////////

// name of files
TString ExpData ="tableExp.dat";
TString SimData ="30degcut.txt";

// open the data file
   ifstream input_exp;
   input_exp.open(Form("./input/%s",ExpData.Data() ) );
   cout<<"opening file : "<<ExpData.Data()<<endl;
	if (!input_exp) { cout<<"problem opening experimental data file! -->exit "<<endl; exit(-1);}
	else cout<<"Experimental file is opened "<<endl;

	//open the field files from comsol  
   ifstream input_sim;
   input_sim.open(Form("./input/%s",SimData.Data() ) ); // from local input if not go seek in data 1
   if(!input_sim) input_sim.open(Form("/data1/moukaddam/MagnetsMapping/%s",SimData.Data() ) );
   cout<<"opening file : "<<SimData.Data()<<endl;
	if (!input_sim) { cout<<"problem opening simulation data file! -->exit "<<endl; exit(-1);}
	else cout<<" Simulation file is opened "<<endl;

// create root output file 
	TString  filename = "./output/Compare_" + ExpData + "_" + SimData + ".root";
	TFile outputFile(filename,"RECREATE");
  
    
 ///////////////////////////// 
 /////////////////////////////  READ THE SIMULATION FILE ///////////////////////
///////////////////////////// 

// dump the first lines from the top
string s_buffer="buffer";
Int_t d_buffer=-1;
Int_t counter1=0; // counter on the first lines
Int_t counter2=0; // counter on the first lines

//FORMAT : A 90 12 -1 -1 -140  -0.000397326 0.000397326 -0.000341054
TString  Grid="buffer";
Double_t Quadrant(0);
Double_t XR(0), YT(0), Z(0) ;
Double_t Level(0) ;
Int_t Position; 
Double_t BX(0), BY(0), BZ(0);

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
// BX BY BZ Z are previously declared 
Double_t X(0), Y(0), EX(-100), EY(-100), EZ(-100), Perm(0);
Int_t line=0;

while ( !input_sim.eof() )
	{
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
		Int_t binNumber = f3dHistBx->FindBin(X,Y,Z);
		f3dHistBmag->SetBinContent(binNumber,SimPoint->fBFieldMag);
		f3dHistBtan->SetBinContent(binNumber,SimPoint->fBFieldTan);
		f3dHistBdiff->SetBinContent(binNumber,SimPoint->fBFieldDiff); //fBFieldMag-fBFieldTan 
		f3dHistBx->SetBinContent(binNumber,BX);
		f3dHistBy->SetBinContent(binNumber,BY);
		f3dHistBz->SetBinContent(binNumber,BZ);			
		
		//count the lines for inspection
		line++;
		//inspection
	   if (line%100000 == 0) 
		   {
		   cout<<"line : "<<line << "  ... Still reading ..."<<endl;
		   //inspection 
		   //cout<< X<<"   "<<Y <<"   "<<Z <<"   "<<BX <<"   "<<BY<<"   "<<BZ <<"   "<< EX<<"   "<< EY<<"   "<< EZ<<"   "<< Perm<<endl ;
		   }
		   
	}  // end of loop while

//count the lines 
cout<<" "<<line << " total number of lines "<<endl;


///////////////////////////// 
/////////////////////////////  READ THE DATA FILE ///////////////////////
///////////////////////////// 

// dump the first lines from the top
s_buffer="buffer";
d_buffer=-1;
counter1=0; // counter on the first lines

for (int i = 0 ; i < 1 ; i++){ // number of lines to neglect
	getline(input_exp,s_buffer);
	counter1++;
}

while (input_exp >> Quadrant){
	//if (input_exp.eof()) break; 

   //Clear parameters
	ExpPointMag->ClearParameters();
	ExpPointX->ClearParameters();
	ExpPointY->ClearParameters();
	ExpPointZ->ClearParameters();
	
   // read one line and do some changes 
    input_exp >> Grid >> Position >> Level >> BX >> BY >> BZ;
    cout <<  Quadrant << " " << Grid << " " << Position << " " << Level << " " << BX << " " << BY << " " << BZ << endl ; 
     
	//ExpPointMag->ReadLineAndTreat( Quadrant, Grid, Position, Level, BX , BY , BZ); 
	ExpPointX->ReadLineAndTreat( Quadrant, Grid, Position, Level , BX , BY , BZ); 
	ExpPointY->ReadLineAndTreat( Quadrant, Grid, Position, Level , BX , BY , BZ); 
	ExpPointZ->ReadLineAndTreat( Quadrant, Grid, Position, Level , BX , BY , BZ); 
        
	//ExpPointMag->ShowParameters();
	//ExpPointX->ShowParameters();
	//ExpPointY->ShowParameters();
	//ExpPointZ->ShowParameters(); 
               
   /*
   // create and/or fill the map at member Z
   mapExpFieldMag[Z].FillVectors(0 ,// 0 for exp, else  simu
				ExpPointMag->fPosition.X(), 
				ExpPointMag->fPosition.Y(),
				ExpPointMag->fPosition.Z(), 
				ExpPointMag->fPositionRadius,
				ExpPointMag->fPositionAngle,
				BX , BY , BZ);
   mapExpFieldX[Z].FillVectors(0 ,// 0 for exp, else  simu
				ExpPointX->fPosition.X(), 
				ExpPointX->fPosition.Y(),
				ExpPointX->fPosition.Z(), 
				ExpPointX->fPositionRadius,
				ExpPointX->fPositionAngle,
				BX , 0 , 0);				
   mapExpFieldY[Z].FillVectors(0 ,// 0 for exp, else  simu
				ExpPointY->fPosition.X(), 
				ExpPointY->fPosition.Y(),
				ExpPointY->fPosition.Z(), 
				ExpPointY->fPositionRadius,
				ExpPointY->fPositionAngle,
				0 , BY , 0);
   mapExpFieldZ[Z].FillVectors(0 ,// 0 for exp, else  simu
				ExpPointZ->fPosition.X(), 
				ExpPointZ->fPosition.Y(),
				ExpPointZ->fPosition.Z(), 
				ExpPointZ->fPositionRadius,
				ExpPointZ->fPositionAngle,
				0 , 0 , BZ);	
				*/	
  }





///////////////////////////// 
///////////////////////////// Create histograms with Histmanager ////////////////////////
///////////////////////////// 
 
//Bx
HistManager *SimBxManager = new HistManager(f3dHistBx, 1/*mm*/, 1/*mm*/, 1 /*mm*/);
SimBxManager->SetOffsets(SENSORXOFFSETX, SENSORXOFFSETY, SENSORXOFFSETZ) ;

SimBxManager->GetMagnitisation2D(-30,-100,+100,-100,+100, 50, "bx");
SimBxManager->GetPolarInterpolation(22.5 , -30 , 100 , "Bx", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
SimBxManager->GetPolarInterpolation(45 , -30 , 100 , "Bx", 0 /*1 for error bars*/);// angle, z level, offset(mm), # of points
SimBxManager->GetPolarInterpolation(67.5 , -30 , 100 , "Bx", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points

//SimBxManager->GetPolarInterpolation(22.5 , -61.7 , 100 , "Bx", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimBxManager->GetPolarInterpolation(45 , -61.7 , 100 , "Bx", 0 /*1 for error bars*/);// angle, z level, offset(mm), # of points
//SimBxManager->GetPolarInterpolation(67.5 , -61.7 , 100 , "Bx", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points

//SimBxManager->GetPolarInterpolation(22.5 , -81.7 , 100 , "Bx", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimBxManager->GetPolarInterpolation(45 , -81.7 , 100 , "Bx", 0 /*1 for error bars*/);// angle, z level, offset(mm), # of points
//SimBxManager->GetPolarInterpolation(67.5 , -81.7 , 100 , "Bx", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points


//By 
HistManager *SimByManager = new HistManager(f3dHistBy, 1/*mm*/, 1/*mm*/, 1/*mm*/);
SimByManager->SetOffsets( SENSORYOFFSETX, SENSORYOFFSETY, SENSORYOFFSETZ) ;

SimByManager->GetPolarInterpolation(22.5 , -30 , 100 , "By", 0 /*1 for error bars*/);// angle, z level, offset(mm), # of points
SimByManager->GetPolarInterpolation(45 , -30 , 100 , "By", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
SimByManager->GetPolarInterpolation(67.5 , -30 , 100 , "By", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points

//SimByManager->GetPolarInterpolation(22.5 , -61.7 , 100 , "By", 0 /*1 for error bars*/);// angle, z level, offset(mm), # of points
//SimByManager->GetPolarInterpolation(45 , -61.7 , 100 , "By", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimByManager->GetPolarInterpolation(67.5 , -61.7 , 100 , "By", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points

//SimByManager->GetPolarInterpolation(22.5 , -81.7 , 100 , "By", 0 /*1 for error bars*/);// angle, z level, offset(mm), # of points
//SimByManager->GetPolarInterpolation(45 , -81.7 , 100 , "By", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimByManager->GetPolarInterpolation(67.5 , -81.7 , 100 , "By", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points



//Bz
HistManager *SimBzManager = new HistManager(f3dHistBz, 1/*mm*/, 1/*mm*/, 1/*mm*/);
SimBzManager->SetOffsets( SENSORZOFFSETX, SENSORZOFFSETY, SENSORZOFFSETZ) ;

SimBzManager->GetPolarInterpolation(22.5 , -30 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
SimBzManager->GetPolarInterpolation(45 , -30 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
SimBzManager->GetPolarInterpolation(67.5 , -30 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points

//SimBzManager->GetPolarInterpolation(22.5 , -61.7 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimBzManager->GetPolarInterpolation(45 , -61.7 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimBzManager->GetPolarInterpolation(67.5 , -61.7 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points

//SimBzManager->GetPolarInterpolation(22.5 , -81.7 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimBzManager->GetPolarInterpolation(45 , -81.7 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points
//SimBzManager->GetPolarInterpolation(67.5 , -81.7 , 100 , "Bz", 0 /*1 for error bars*/); // angle, z level, offset(mm), # of points



//Mag
HistManager *SimBmagManager = new HistManager(f3dHistBmag, 1/*mm*/, 1/*mm*/, 1/*mm*/);
SimBmagManager->GetMagnitisation2D(-30,-106,+106,-106,+106, 50, "bmag");

//tan
HistManager *SimBtanManager = new HistManager(f3dHistBtan, 1/*mm*/, 1/*mm*/, 1/*mm*/);
SimBtanManager->GetMagnitisation2D(-30,-106,+106,-106,+106, 50, "btan");

//diff
HistManager *SimBdifManager = new HistManager(f3dHistBdiff, 1/*mm*/, 1/*mm*/, 1/*mm*/);
SimBdifManager->GetMagnitisation2D(-30,-106,+106,-106,+106, 50, "bdif");




///////////////////////////// Determine the error from the simulation of all experimental points ////////////////////////
//Loop on the map 
for(it = mapExpFieldMag.begin(); it != mapExpFieldMag.end(); ++it){

	for (unsigned i = 0 ; i<(*it).second.fExpX.size() ;  i++ ){
		// store inside the error containers eBxyz from HistBxyz
		// these values are already corrected for offset
		double X = (*it).second.fExpX.at(i) ;
		double Y = (*it).second.fExpY.at(i) ;
		double Z = (*it).second.fExpZ.at(i) ;
		//cout << "X Y Z " << X <<"  "<< Y <<"  "<< Z <<endl;
		(*it).second.fExpBxErr.push_back(SimBxManager->GetInterpolationOnePointError(X,Y,Z));
		(*it).second.fExpByErr.push_back(SimByManager->GetInterpolationOnePointError(X,Y,Z));
		(*it).second.fExpBzErr.push_back(SimBzManager->GetInterpolationOnePointError(X,Y,Z)); // CHECK make the same for mag and tan..

		(*it).second.fSimBx.push_back(SimBxManager->GetInterpolationOnePoint(X,Y,Z));
		(*it).second.fSimBy.push_back(SimByManager->GetInterpolationOnePoint(X,Y,Z));
		(*it).second.fSimBz.push_back(SimBzManager->GetInterpolationOnePoint(X,Y,Z)); // CHECK make the same for mag and tan..
		}
}


//Loop on the map 
for(it = mapExpFieldX.begin(); it != mapExpFieldX.end(); ++it){

	for (unsigned i = 0 ; i<(*it).second.fExpX.size() ;  i++ ) {
		// these values are already corrected for offset
		double X = (*it).second.fExpX.at(i) ; 
		double Y = (*it).second.fExpY.at(i) ;
		double Z = (*it).second.fExpZ.at(i) ;
		//cout << "X Y Z " << X <<"  "<< Y <<"  "<< Z <<endl;
		(*it).second.fExpBxErr.push_back(SimBxManager->GetInterpolationOnePointError(X,Y,Z));// CHECK make the same for mag and tan..
		(*it).second.fSimBx.push_back(SimBxManager->GetInterpolationOnePoint(X,Y,Z));
		//SimBxManager->GetInterpolationOnePointNoOffset(X,Y,Z);
		}
}


//Loop on the map 
for(it = mapExpFieldY.begin(); it != mapExpFieldY.end(); ++it){

	for (unsigned i = 0 ; i<(*it).second.fExpY.size() ;  i++ ){
		// store inside the error containers eBxyz from HistBxyz
		double X = (*it).second.fExpX.at(i) ;
		double Y = (*it).second.fExpY.at(i) ;
		double Z = (*it).second.fExpZ.at(i) ;
		
		(*it).second.fExpByErr.push_back(SimByManager->GetInterpolationOnePointError(X,Y,Z));// CHECK make the same for mag and tan..
		(*it).second.fSimBy.push_back(SimByManager->GetInterpolationOnePoint(X,Y,Z));
	
		}
}



//Loop on the map 
for(it = mapExpFieldZ.begin(); it != mapExpFieldZ.end(); ++it) {
	//cout << "Key: " << (*it).first <<endl;
	//Loop on the points stored (XYZ)
	//cout << "x: " << (*it).second.fExpX.size() <<endl;

	for (unsigned i = 0 ; i<(*it).second.fExpZ.size() ;  i++ )
		{
		// store inside the error containers eBxyz from HistBxyz
		double X = (*it).second.fExpX.at(i) ;
		double Y = (*it).second.fExpY.at(i) ;
		double Z = (*it).second.fExpZ.at(i) ;
		
		(*it).second.fExpBzErr.push_back(SimBzManager->GetInterpolationOnePointError(X,Y,Z)); // CHECK make the same for mag and tan..
		(*it).second.fSimBz.push_back(SimBzManager->GetInterpolationOnePoint(X,Y,Z));

		}
}



///////////////////////////// Create histograms with Graphmanager ////////////////////////

//mapExpFieldMag.at(-41.7).GetExp1dGraphPolar("Bmag",22.5);
//mapExpFieldX.at(-41.7).GetExp1dGraphPolar("Bx",22.5);
//mapExpFieldY.at(-41.7).GetExp1dGraphPolar("By",22.5);
//mapExpFieldZ.at(-41.7).GetExp1dGraphPolar("Bz",22.5);

//mapExpFieldMag.at(-41.7).GetExp1dGraphPolar("Bmag",45);
//mapExpFieldX.at(-41.7).GetExp1dGraphPolar("Bx",45);
//mapExpFieldY.at(-41.7).GetExp1dGraphPolar("By",45);
//mapExpFieldZ.at(-41.7).GetExp1dGraphPolar("Bz",45);

//mapExpFieldMag.at(-41.7).GetExp1dGraphPolar("Bmag",67.5);
//mapExpFieldX.at(-41.7).GetExp1dGraphPolar("Bx",67.5);
//mapExpFieldY.at(-41.7).GetExp1dGraphPolar("By",67.5);
//mapExpFieldZ.at(-41.7).GetExp1dGraphPolar("Bz",67.5);


//mapExpFieldMag.at(-41.7).GetSim1dGraphPolar("Bmag",22.5);
//mapExpFieldX.at(-41.7).GetSim1dGraphPolar("Bx",22.5);
//mapExpFieldY.at(-41.7).GetSim1dGraphPolar("By",22.5);
//mapExpFieldZ.at(-41.7).GetSim1dGraphPolar("Bz",22.5);

//mapExpFieldMag.at(-41.7).GetSim1dGraphPolar("Bmag",45);
//mapExpFieldX.at(-41.7).GetSim1dGraphPolar("Bx",45);
//mapExpFieldY.at(-41.7).GetSim1dGraphPolar("By",45);
//mapExpFieldZ.at(-41.7).GetSim1dGraphPolar("Bz",45);

//mapExpFieldMag.at(-41.7).GetSim1dGraphPolar("Bmag",67.5);
//mapExpFieldX.at(-41.7).GetSim1dGraphPolar("Bx",67.5);
//mapExpFieldY.at(-41.7).GetSim1dGraphPolar("By",67.5);
//mapExpFieldZ.at(-41.7).GetSim1dGraphPolar("Bz",67.5);




//mapExpFieldMag.at(-61.7).GetExp1dGraphPolar("Bmag",22.5);
//mapExpFieldX.at(-61.7).GetExp1dGraphPolar("Bx",22.5);
//mapExpFieldY.at(-61.7).GetExp1dGraphPolar("By",22.5);
//mapExpFieldZ.at(-61.7).GetExp1dGraphPolar("Bz",22.5);

//mapExpFieldMag.at(-61.7).GetExp1dGraphPolar("Bmag",45);
//mapExpFieldX.at(-61.7).GetExp1dGraphPolar("Bx",45);
//mapExpFieldY.at(-61.7).GetExp1dGraphPolar("By",45);
//mapExpFieldZ.at(-61.7).GetExp1dGraphPolar("Bz",45);

//mapExpFieldMag.at(-61.7).GetExp1dGraphPolar("Bmag",67.5);
//mapExpFieldX.at(-61.7).GetExp1dGraphPolar("Bx",67.5);
//mapExpFieldY.at(-61.7).GetExp1dGraphPolar("By",67.5);
//mapExpFieldZ.at(-61.7).GetExp1dGraphPolar("Bz",67.5);


//mapExpFieldMag.at(-61.7).GetSim1dGraphPolar("Bmag",22.5);
//mapExpFieldX.at(-61.7).GetSim1dGraphPolar("Bx",22.5);
//mapExpFieldY.at(-61.7).GetSim1dGraphPolar("By",22.5);
//mapExpFieldZ.at(-61.7).GetSim1dGraphPolar("Bz",22.5);

//mapExpFieldMag.at(-61.7).GetSim1dGraphPolar("Bmag",45);
//mapExpFieldX.at(-61.7).GetSim1dGraphPolar("Bx",45);
//mapExpFieldY.at(-61.7).GetSim1dGraphPolar("By",45);
//mapExpFieldZ.at(-61.7).GetSim1dGraphPolar("Bz",45);

//mapExpFieldMag.at(-61.7).GetSim1dGraphPolar("Bmag",67.5);
//mapExpFieldX.at(-61.7).GetSim1dGraphPolar("Bx",67.5);
//mapExpFieldY.at(-61.7).GetSim1dGraphPolar("By",67.5);
//mapExpFieldZ.at(-61.7).GetSim1dGraphPolar("Bz",67.5);



//mapExpFieldMag.at(-81.7).GetExp1dGraphPolar("Bmag",22.5);
//mapExpFieldX.at(-81.7).GetExp1dGraphPolar("Bx",22.5);
//mapExpFieldY.at(-81.7).GetExp1dGraphPolar("By",22.5);
//mapExpFieldZ.at(-81.7).GetExp1dGraphPolar("Bz",22.5);

//mapExpFieldMag.at(-81.7).GetExp1dGraphPolar("Bmag",45);
//mapExpFieldX.at(-81.7).GetExp1dGraphPolar("Bx",45);
//mapExpFieldY.at(-81.7).GetExp1dGraphPolar("By",45);
//mapExpFieldZ.at(-81.7).GetExp1dGraphPolar("Bz",45);

//mapExpFieldMag.at(-81.7).GetExp1dGraphPolar("Bmag",67.5);
//mapExpFieldX.at(-81.7).GetExp1dGraphPolar("Bx",67.5);
//mapExpFieldY.at(-81.7).GetExp1dGraphPolar("By",67.5);
//mapExpFieldZ.at(-81.7).GetExp1dGraphPolar("Bz",67.5);


//mapExpFieldMag.at(-81.7).GetSim1dGraphPolar("Bmag",22.5);
//mapExpFieldX.at(-81.7).GetSim1dGraphPolar("Bx",22.5);
//mapExpFieldY.at(-81.7).GetSim1dGraphPolar("By",22.5);
//mapExpFieldZ.at(-81.7).GetSim1dGraphPolar("Bz",22.5);

//mapExpFieldMag.at(-81.7).GetSim1dGraphPolar("Bmag",45);
//mapExpFieldX.at(-81.7).GetSim1dGraphPolar("Bx",45);
//mapExpFieldY.at(-81.7).GetSim1dGraphPolar("By",45);
//mapExpFieldZ.at(-81.7).GetSim1dGraphPolar("Bz",45);

//mapExpFieldMag.at(-81.7).GetSim1dGraphPolar("Bmag",67.5);
//mapExpFieldX.at(-81.7).GetSim1dGraphPolar("Bx",67.5);
//mapExpFieldY.at(-81.7).GetSim1dGraphPolar("By",67.5);
//mapExpFieldZ.at(-81.7).GetSim1dGraphPolar("Bz",67.5);

//Close the file on disk
	outputFile.Close();
	cout << " T.H.E  E.N.D " << endl;

 return 0;
}


