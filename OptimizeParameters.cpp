#include "OptimizeParameters.h"

OptimizeParameters::~OptimizeParameters(void)
{
cout << "Inside OptimizeParameters::Destructor()" << endl;
}

OptimizeParameters::OptimizeParameters(TString FileNameExpData, TString FileNameSimData)
{
cout << "Inside OptimizeParameters::OptimizeParameters()" << endl;

// Initiate some variables
Init();
LinkTree();

///////////////////////////// OPEN INPUT AND OUTPUT FILES ///////////////////////
   
TString pathTo="./input/";

//open the field files from comsol  
   ifstream input_2;
   input_2.open((pathTo+FileNameSimData).Data());
   cout<<"opening file : "<<FileNameSimData.Data()<<endl;

// check if files were opened
if (!input_2) { cout<<"problem opening the files! -->exit "<<endl; exit(-1);}
else cout<<"files are opened "<<endl;

// create root output file 
  TString  filename = "./output/Chi2_" + FileNameExpData + "_" + FileNameSimData + ".root";
  fRootFile = new TFile(filename,"RECREATE");
  
// read the file containing the path and the names of the magfield table and the respective magnetization
// take out comment 
string comment="";
getline(input_2,comment);
cout<< " first line : "<<comment<<endl;
while ( !input_2.eof() )
{
   //Clear parameters
   string path="";
   int magInner=0;
   int magOuter=0; 
   
   // read one line and do some changes 
   input_2>>path>>magInner>>magOuter;
   //cout<<path<<" "<< magInner << " "<< magOuter <<endl;
   
   if (path!="")
   {
   Key magkey(magInner, magOuter); 
   fFieldPathMap[magkey] = path;
   }
   
   getline(input_2,path); // here the string path is usd as a dummy variable
    
}

// read the input experimental file
// create a vector of double
ReadExperimentalData(FileNameExpData);

//treat the data and estimate the positions
//Treat();
TreatSensorFixed(2.413, 1.016, -0.127, 0.127, 3.048, -1.397, 0.889, 1.143, -0.508); // these values were fixed with the low energy lens

//Show the best values
ShowBestValues();

//write everything
WriteTree();
WriteGraph();

fRootFile->Close();

//cout << "Inside OptimizeParameters END " << endl;
}

void OptimizeParameters::Treat()
{
cout << "Inside OptimizeParameters::Treat()" << endl;

// Loop on the map (magInner,magOuter)
    for (std::map<Key,string>::iterator it=fFieldPathMap.begin(); it!=fFieldPathMap.end(); ++it)
    {
    
    //inspection
    //std::cout << it->first.magInner << " " << it->first.magOuter << " => " << it->second << '\n';
    fMagInner = it->first.magInner ; 
    fMagOuter = it->first.magOuter ;
    
// we need a function that takes the name, read the file, and feed the TH3D Histograms (Bx By Bz)
// the histogram will be given to the corresponding (Bx By Bz) HistManager
    ReadFieldTable(it->second);
   
   //define step
   int step_number = 50 ; // always pair number
   int half_step_number = (int) step_number*0.5 ;
   double stepx = 6.35/step_number; // rectangular probe 6.35*6.35 mm2
     double stepy = 6.35/step_number; // rectangular probe 6.35*6.35 mm2
        double stepz = 6.35/step_number; // depth of 10 mm        
        
	// for offset in X
	for (int i = -half_step_number ; i<= half_step_number ; i++ ) 
	
		// for Offset in Y
		for (int j = -half_step_number ; j<= half_step_number ; j++ ) 
		
			// for Offset in Z
			for (int k = -half_step_number ; k<= half_step_number ; k++ ) // in our case all the offset > 0 are not accepted. 
						  {
			 //Here we need 3 variables to hold the chi2Bx = chi2By = chi2Bz = chi2Btotal = 0 
			    fChi2X =  0 ; 
			    fChi2Y =  0 ; 
			    fChi2Z =  0 ; 
			    fChi2XYZ = 0;
			    	    		
			    // for the element in the experimental vector
			    // calculate the Interpolated value for X Y Z i
			    fOffsetX = i*stepx ; 
			    fOffsetY = j*stepy ; 
			    fOffsetZ = k*stepz ; 
				   	   
			     for (unsigned q = 0 ; q<fPositionAndField.size(); q++ ) 
							{			  

							// calculate the differences in Bfields for the chi2Bx chi2By chi2Bz chi2Btotal
							// define the Weight
							double weightx = 1 ;
							double weighty = 1 ;
							double weightz = 1 ;
					
					
							// calculate the Interpolated value for X Y Z in BX BY BZ
							double x = fPositionAndField.at(q).fPosition.X() + fOffsetX ;
							double y = fPositionAndField.at(q).fPosition.Y() + fOffsetY ;
							double z = fPositionAndField.at(q).fPosition.Z() + fOffsetZ ;
					
						   /*
							fBsimX = fabs(fHistBxManager->GetInterpolationOnePoint(x,y,z )); 
			  			    fBsimY = fabs(fHistByManager->GetInterpolationOnePoint(x,y,z ));
			   			    fBsimZ = fabs(fHistBzManager->GetInterpolationOnePoint(x,y,z ));
			   			    */
			
							fBsimX = fHistBxManager->GetInterpolationOnePoint(x,y,z ); 
			  			    fBsimY = fHistByManager->GetInterpolationOnePoint(x,y,z );
			   			    fBsimZ = - fHistBzManager->GetInterpolationOnePoint(x,y,z ); // minus sign to compensate for the difference in z orientation
			   			     
							// calculate BtotalSimu and BtotalExperimental
					
							// add the new values to the previous valuse for all chi2
							/*double bx = fabs(fPositionAndField.at(q).fField.X()) ;
							double by = fabs(fPositionAndField.at(q).fField.Y()) ;
							double bz = fabs(fPositionAndField.at(q).fField.Z()) ;*/
						   
						   	double bx = fPositionAndField.at(q).fField.X() ;
							double by = fPositionAndField.at(q).fField.Y() ;
							double bz = fPositionAndField.at(q).fField.Z() ;	
							 	
							 	// Chi2		
								// [ (yexp - ysim)^2 ] / [ dyexp^2 + (ysimderivative dxexp)^2 ] for X and Y errors
								// [ (yexp - ysim)^2 ] / [ dyexp^2 + (ysimderivative dxexp)^2 ] for Y errors  
							
							//  0.02 mT as given by the sensors, however this value could be increased to compensate the errors on X
							// we can also take it as a percentage value 2%, 5% is clearly not the best estimate
							double percent = 0 ;
        					double dbx = (percent/100.)*bx + 1 ; //0.02
        					double dby = (percent/100.)*by + 1 ; // 0.02
        					double dbz = (percent/100.)*bz + 1 ; //0.02
        					
					       dbx = fabs( (bx - fBsimX) / dbx ) ;
					       dby = fabs( (by - fBsimY) / dby ) ;
					       dbz = fabs( (bz - fBsimZ) / dbz ) ;
        					       
							fChi2X = fChi2X +  dbx * weightx; 
							fChi2Y = fChi2Y +  dby * weighty; 
							fChi2Z = fChi2Z +  dbz * weightz; 
							
    					       
							if (0 /*fabs(fBsimX-bx)>10 || fBsimY*by<0 || fBsimZ*bz<0*/)
								{
								// inspection
								cout<< " # "<< q <<endl;
								cout<< " ORIG.POS "<< fPositionAndField.at(q).fPosition.X() << " " << fPositionAndField.at(q).fPosition.Y() << " " << fPositionAndField.at(q).fPosition.Z() <<endl;
								cout<< " dx dy dz "<< fOffsetX << " " << fOffsetY << " " << fOffsetZ <<endl;			     
								cout<< " POSITION "<< x << " " << y << " " << z <<endl;
								cout<< " VAL. SIM "<< fBsimX << " " << fBsimY << " " << fBsimZ <<endl; 
								cout<< " VAL. EXP "<< bx << " " << by << " " << bz <<endl;
								cout<< " ERR. EXP "<< dbx << " " << dby << " " << dbz <<endl;
								cout<< " chi2 "<< fChi2X  << " " << fChi2Y  << " " << fChi2Z <<endl;
								cin.get();
						   		}
					
						   } // loop on the experimental data 
			          
			// normalize chi2 
			 fChi2X = fChi2X/fPositionAndField.size();			      
			 fChi2Y = fChi2Y/fPositionAndField.size();
			 fChi2Z = fChi2Z/fPositionAndField.size();
			
			//cout<< " Chi2     "<< fChi2X << " " << fChi2Y << " " << fChi2Z <<endl;
				
			//if ( fOffsetX < -1) { cout<<fOffsetX<<endl; cin.get(); }    
			// store the chi2s X Y Z dx dy dz magInner magOuter in a Tree 
			fTree->Fill();

			// Elect the best (magInner magOuter offsets according to BX, By, Bz, Btot separatly)
            Minimize();

			 } // loop on  dz 
			    
// Clear the histograms and the Histmanagers 
    Clear();
    
	}
				
}



void OptimizeParameters::TreatSensorFixed(float fixedXx, float fixedYx, float fixedZx, float fixedXy, float fixedYy, float fixedZy, float fixedXz, float fixedYz, float fixedZz)
{
cout << "Inside OptimizeParameters::TreatSensorFixed(...)" << endl;

// Loop on the map (magInner,magOuter)
    for (std::map<Key,string>::iterator it=fFieldPathMap.begin(); it!=fFieldPathMap.end(); ++it)
    {
    
    //inspection
    //std::cout << it->first.magInner << " " << it->first.magOuter << " => " << it->second << '\n';
    fMagInner = it->first.magInner ; 
    fMagOuter = it->first.magOuter ;
    
// we need a function that takes the name, read the file, and feed the TH3D Histograms (Bx By Bz)
// the histogram will be given to the corresponding (Bx By Bz) HistManager
    ReadFieldTable(it->second);

			 //Here we need 3 variables to hold the chi2Bx = chi2By = chi2Bz = chi2Btotal = 0 
			    fChi2X =  0 ; 
			    fChi2Y =  0 ; 
			    fChi2Z =  0 ; 
			    fChi2XYZ = 0;
			    	    		
			    // for the element in the experimental vector
			    // calculate the Interpolated value for X Y Z i
				   	   
			     for (unsigned q = 0 ; q<fPositionAndField.size(); q++ ) 
							{			  

							// calculate the differences in Bfields for the chi2Bx chi2By chi2Bz chi2Btotal
							// define the Weight
							double weightx = 1 ;
							double weighty = 1 ;
							double weightz = 1 ;
										
										
							fOffsetX = fixedXx ; 
			    			fOffsetY = fixedYx ; 
			    			fOffsetZ = fixedZx ; 
							// calculate the Interpolated value for X Y Z in BX BY BZ
							double x = fPositionAndField.at(q).fPosition.X() + fOffsetX ;
							double y = fPositionAndField.at(q).fPosition.Y() + fOffsetY ;
							double z = fPositionAndField.at(q).fPosition.Z() + fOffsetZ ;
						    fBsimX = fHistBxManager->GetInterpolationOnePoint(x,y,z ); 	
						   /*
							fBsimX = fabs(fHistBxManager->GetInterpolationOnePoint(x,y,z )); 
			  			    fBsimY = fabs(fHistByManager->GetInterpolationOnePoint(x,y,z ));
			   			    fBsimZ = fabs(fHistBzManager->GetInterpolationOnePoint(x,y,z ));
			   			    */
							fOffsetX = fixedXy ; 
			    			fOffsetY = fixedYy ; 
			    			fOffsetZ = fixedZy ; 
							       x = fPositionAndField.at(q).fPosition.X() + fOffsetX ;
							       y = fPositionAndField.at(q).fPosition.Y() + fOffsetY ;
							       z = fPositionAndField.at(q).fPosition.Z() + fOffsetZ ;						
			  			    fBsimY = fHistByManager->GetInterpolationOnePoint(x,y,z );
		    
			  				fOffsetX = fixedXz ; 
			    			fOffsetY = fixedYz ; 
			    			fOffsetZ = fixedZz ; 	    
			  			    	   x = fPositionAndField.at(q).fPosition.X() + fOffsetX ;
							       y = fPositionAndField.at(q).fPosition.Y() + fOffsetY ;
							       z = fPositionAndField.at(q).fPosition.Z() + fOffsetZ ;
			   			    fBsimZ = - fHistBzManager->GetInterpolationOnePoint(x,y,z ); // minus sign to compensate for the difference in z orientation
			   			     
							// calculate BtotalSimu and BtotalExperimental
					
							// add the new values to the previous valuse for all chi2
							/*double bx = fabs(fPositionAndField.at(q).fField.X()) ;
							double by = fabs(fPositionAndField.at(q).fField.Y()) ;
							double bz = fabs(fPositionAndField.at(q).fField.Z()) ;*/
						   
						   	double bx = fPositionAndField.at(q).fField.X() ;
							double by = fPositionAndField.at(q).fField.Y() ;
							double bz = fPositionAndField.at(q).fField.Z() ;	
							 	
							 	// Chi2		
								// [ (yexp - ysim)^2 ] / [ dyexp^2 + (ysimderivative dxexp)^2 ] for X and Y errors
								// [ (yexp - ysim)^2 ] / [ dyexp^2 + (ysimderivative dxexp)^2 ] for Y errors  
							
							//  0.02 mT as given by the sensors, however this value could be increased to compensate the errors on X
							// we can also take it as a percentage value 2%, 5% is clearly not the best estimate
							double percent = 0 ;
        					double dbx = (percent/100.)*bx + 1 ; //0.02
        					double dby = (percent/100.)*by + 1 ; // 0.02
        					double dbz = (percent/100.)*bz + 1 ; //0.02
        					
					       dbx = fabs( (bx - fBsimX) / dbx ) ;
					       dby = fabs( (by - fBsimY) / dby ) ;
					       dbz = fabs( (bz - fBsimZ) / dbz ) ;
        					       
							fChi2X = fChi2X +  dbx * weightx; 
							fChi2Y = fChi2Y +  dby * weighty; 
							fChi2Z = fChi2Z +  dbz * weightz; 
							
    					       
							if (0 /*fabs(fBsimX-bx)>10 || fBsimY*by<0 || fBsimZ*bz<0*/)
								{
								// inspection
								cout<< " # "<< q <<endl;
								cout<< " ORIG.POS "<< fPositionAndField.at(q).fPosition.X() << " " << fPositionAndField.at(q).fPosition.Y() << " " << fPositionAndField.at(q).fPosition.Z() <<endl;
								cout<< " dx dy dz "<< fOffsetX << " " << fOffsetY << " " << fOffsetZ <<endl;			     
								cout<< " POSITION "<< x << " " << y << " " << z <<endl;
								cout<< " VAL. SIM "<< fBsimX << " " << fBsimY << " " << fBsimZ <<endl; 
								cout<< " VAL. EXP "<< bx << " " << by << " " << bz <<endl;
								cout<< " ERR. EXP "<< dbx << " " << dby << " " << dbz <<endl;
								cout<< " chi2 "<< fChi2X  << " " << fChi2Y  << " " << fChi2Z <<endl;
								cin.get();
						   		}
					
						   } // loop on the experimental data 
			          
			// normalize chi2 
			 fChi2X = fChi2X/fPositionAndField.size();			      
			 fChi2Y = fChi2Y/fPositionAndField.size();
			 fChi2Z = fChi2Z/fPositionAndField.size();
			
			//cout<< " Chi2     "<< fChi2X << " " << fChi2Y << " " << fChi2Z <<endl;
				
			//if ( fOffsetX < -1) { cout<<fOffsetX<<endl; cin.get(); }    
			// store the chi2s X Y Z dx dy dz magInner magOuter in a Tree 
			fTree->Fill();

			// Elect the best (magInner magOuter offsets according to BX, By, Bz, Btot separatly)
            Minimize();
			    
// Clear the histograms and the Histmanagers 
    Clear();
    
	}
				
}







void OptimizeParameters::Clear()
{
cout << "Inside OptimizeParameters::Clear()" << endl;

  fHistBx->Reset(); 
  fHistBy->Reset();  
  fHistBz->Reset();

  fHistBxManager->Clear();
  fHistByManager->Clear(); 
  fHistBzManager->Clear(); 
  
fMagInner = 0; 
fMagOuter = 0;

fChi2X = -1;
fChi2Y = -1;
fChi2Z = -1;
fChi2XYZ = -1;

fOffsetX = -1;
fOffsetY = -1;
fOffsetZ = -1;

fBsimX= -1;
fBsimY= -1;
fBsimZ= -1;

fBexpX= -1;
fBexpY= -1;
fBexpZ= -1;

}


void OptimizeParameters::ReadFieldTable(string path)
{
cout << "Inside OptimizeParameters::ReadFieldTable()"<< path << endl;


/////////////////////////////  READ THE SIMULATION FILE ///////////////////////

//open the field files from comsol  
   ifstream input_2;
   input_2.open(path.c_str());
   //cout<<"opening file : "<<path<<endl;

// check if files were opened
if ( !input_2) { cout<<"problem opening this file! -->exit "<<endl; exit(-1);}
else cout<<"file"<< path << " is opened "<<endl;


// read the first lines
Int_t dimx_2, dimy_2, dimz_2;
input_2>>dimx_2>>dimy_2>>dimz_2;
cout<<"                                    "<<" X "<<"  "<<" Y "<<"  "<<" Z "<<endl;
cout<<" Simulated Field Table dimensions : "<<dimx_2<<"  "<<dimy_2<<"  "<<dimz_2<<endl;

//reset buffer
int d_buffer=-1;
int counter2=0;
string s_buffer;

while (d_buffer != 0 && counter2< 15) // find a solution for this with do while
{
input_2>>d_buffer>>s_buffer;
//cout<< d_buffer<< "  " << s_buffer<<endl;
counter2++;
}

//test for table 2
getline(input_2,s_buffer);
cout<<counter2<<s_buffer<<endl;

// read and fill 
Double_t X(0), Y(0), Z(0), BX(0), BY(0), BZ(0), EX(-100), EY(-100), EZ(-100), Perm(0);
Int_t line=0;

while (!input_2.eof())
	{
   
	   // Choose format
	  if(counter2<9)   input_2 >> X >> Y >> Z >> BX >> BY >> BZ >> Perm ;
	  else   	    input_2 >> X >> Y >> Z >> BX >> BY >> BZ >> EX >> EY >> EZ >> Perm ;
	   
	   // Read the line, apply offset and do some modifications
	   Z = Z + 4 ; //Simulation is offset by -4mm, this corrects for it
		 
	// fill in TH3D all the simulation data
	Int_t binNumber = fHistBx->FindBin(X,Y,Z);
	fHistBx->SetBinContent(binNumber,BX);		
	fHistBy->SetBinContent(binNumber,BY);
	fHistBz->SetBinContent(binNumber,BZ);
  	
	   //count the lines for inspection
	   line++;
	   //inspection
	   if (line%10000 == 0) 
		   {
		   cout<<"line : "<<std::setw(10)<<line << "  ... Still reading file "<< path <<"\r";
		   //inspection 
		   //cout<< X<<"   "<<Y <<"   "<<Z <<"   "<<BX <<"   "<<BY<<"   "<<BZ <<"   "<< EX<<"   "<< EY<<"   "<< EZ<<"   "<< Perm<<endl ;
		   }
		   
	}  // end of loop while

//count the lines 
cout<<endl<<endl<<" -------------------------------------- Total number of lines in file : "<< line <<endl;

fHistBxManager = new HistManager(fHistBx, 0/*mm*/, 0./*mm*/, 0 /*mm*/);
fHistByManager = new HistManager(fHistBy, 0/*mm*/, 0./*mm*/, 0 /*mm*/);
fHistBzManager = new HistManager(fHistBz, 0/*mm*/, 0./*mm*/, 0 /*mm*/);

//make sure offsets are set to zero
fHistBxManager->SetOffsets(0, 0, 0) ;
fHistByManager->SetOffsets(0, 0, 0) ;
fHistBzManager->SetOffsets(0, 0, 0) ;
}



void OptimizeParameters::Init(void)
{
  // create a TH3D
  fHistBx = new TH3D("fHistBx", "Bx", 111,-111, 111, 111,-111, 111, 141, -141, 141); 
  fHistBy = new TH3D("fHistBy", "By", 111,-111, 111, 111,-111, 111, 141, -141, 141);  
  fHistBz = new TH3D("fHistBz", "Bz", 111,-111, 111, 111,-111, 111, 141, -141, 141); 
  
  //Initiate this variable for the minimizer
  fFirstValue = true ;
  
  fPositionAndField.clear();
  }

void OptimizeParameters::LinkTree(void)
{
cout << "Inside OptimizeParameters::LinkTree()" << endl;
//prepare the Tree ; 

  fTree = new TTree("Chi2Tree","Chi2Tree_");

  fTree->Branch("MagInner",&fMagInner);  
  fTree->Branch("MagOuter",&fMagOuter);
  
  fTree->Branch("Chi2X",&fChi2X);  
  fTree->Branch("Chi2Y",&fChi2Y);
  fTree->Branch("Chi2Z",&fChi2Z);  
  fTree->Branch("Chi2XYZ",&fChi2XYZ);
  
  fTree->Branch("OffsetX",&fOffsetX);  
  fTree->Branch("OffsetY",&fOffsetY);
  fTree->Branch("OffsetZ",&fOffsetZ);  

/*
 one chi2 corresponds to several of this values , so we need a class to write this guys.. 
  fTree->Branch("BsimX",&fBsimX);  
  fTree->Branch("BsimY",&fBsimY);
  fTree->Branch("BsimZ",&fBsimZ); 
  
  fTree->Branch("BexpX",&fBexpX);  
  fTree->Branch("BexpY",&fBexpY);
  fTree->Branch("BexpZ",&fBexpZ);
  */

}


void OptimizeParameters::Minimize(void)
{
//cout << "Inside OptimizeParameters::Minimize()" << endl;

// 0 1 2 3 --> according to Bx By Bz Bmag

	if (fFirstValue == true) 
		{
		fFirstValue = false ;
		fBestChi2X = fChi2X ;
		fBestChi2Y = fChi2Y ;
		fBestChi2Z = fChi2Z ;
		}

	if (fChi2X < fBestChi2X )
		{
		fBestChi2X = fChi2X;
		fBestMagInner[0] = fMagInner; 
		fBestMagOuter[0] = fMagOuter;
		fBestOffsetX[0] = fOffsetX;
		fBestOffsetY[0] = fOffsetY;
		fBestOffsetZ[0]  = fOffsetZ;

		}


	if (fChi2Y < fBestChi2Y )
		{
		fBestChi2Y = fChi2Y;
		fBestMagInner[1] = fMagInner; 
		fBestMagOuter[1] = fMagOuter;
		fBestOffsetX[1] = fOffsetX;
		fBestOffsetY[1] = fOffsetY;
		fBestOffsetZ[1]  = fOffsetZ;
/*
cout << " The best estimates according to According to By : " << endl;
cout << " Inner magnets manetization : " <<setw(7)<< fBestMagInner[1] << endl;
cout << " Outer magnets manetization : " <<setw(7)<< fBestMagOuter[1] << endl;
cout << " Offset in X                : " <<setw(7)<< fBestOffsetX[1]  << endl;
cout << " Offset in Y                : " <<setw(7)<< fBestOffsetY[1]  << endl;
cout << " Offset in Z                : " <<setw(7)<< fBestOffsetZ[1]  << endl;
*/
		}



//cout << " fBestChi2Z " <<setw(7)<< fBestChi2Z << " -- " << " fChi2Z " << fChi2Z << endl;
	if (fChi2Z < fBestChi2Z )
		{
//cout << " fBestChi2Z " <<setw(7)<< fBestChi2Z << " > " << " fChi2Z " << fChi2Z << endl;
//cin.get();
		fBestChi2Z = fChi2Z;
		fBestMagInner[2] = fMagInner; 
		fBestMagOuter[2] = fMagOuter;
		fBestOffsetX[2] = fOffsetX;
		fBestOffsetY[2] = fOffsetY;
		fBestOffsetZ[2]  = fOffsetZ;
		}

}


void OptimizeParameters::ShowBestValues(void)
{
cout << "Inside OptimizeParameters::ShowBestValues()" << endl;

// 0 1 2 3 --> according to Bx By Bz Bmag

cout << " **************************************************" << endl;
cout << " The best estimates according to Bx : " << endl;
cout << " Best Chi2                  : " <<setw(7)<< fBestChi2X << endl;
cout << " Inner magnets manetization : " <<setw(7)<< fBestMagInner[0] << endl;
cout << " Outer magnets manetization : " <<setw(7)<< fBestMagOuter[0] << endl;
cout << " Offset in X                : " <<setw(7)<< fBestOffsetX[0]<< "mm     " <<setw(7)<< fBestOffsetX[0]/6.35*100<< " % "<< endl;
cout << " Offset in Y                : " <<setw(7)<< fBestOffsetY[0]<< "mm     " <<setw(7)<< fBestOffsetY[0]/6.35*100<< " % "<<endl;
cout << " Offset in Z                : " <<setw(7)<< fBestOffsetZ[0]<< "mm     " <<setw(7)<< fBestOffsetZ[0]/6.35*100<< " % "<<endl;

if (fBestOffsetZ[0]>0) cout <<               "                                                         * Offset in Z is out of border ! " <<endl;
if ( fabs(fBestOffsetX[0])> 6.35/2) cout <<  "                                                         * Offset in X is out of border ! " <<endl;
if ( fabs(fBestOffsetY[0])> 6.35/2 ) cout << "                                                         * Offset in Y is out of border ! " <<endl;
cout <<  endl;

cout << " The best estimates according to By : " << endl;
cout << " Best Chi2                  : " <<setw(7)<< fBestChi2Y << endl;
cout << " Inner magnets manetization : " <<setw(7)<< fBestMagInner[1] << endl;
cout << " Outer magnets manetization : " <<setw(7)<< fBestMagOuter[1] << endl;
cout << " Offset in X                : " <<setw(7)<< fBestOffsetX[1]<< "mm     " <<setw(7)<< fBestOffsetX[1]/6.35*100<< " % "<< endl;
cout << " Offset in Y                : " <<setw(7)<< fBestOffsetY[1]<< "mm     " <<setw(7)<< fBestOffsetY[1]/6.35*100<< " % "<<endl;
cout << " Offset in Z                : " <<setw(7)<< fBestOffsetZ[1]<< "mm    " <<setw(7)<< fBestOffsetZ[1]/6.35*100<< " % "<<endl;

if (fBestOffsetZ[1]>0) cout <<               "                                                          * Offset in Z is out of border ! " <<endl;
if ( fabs(fBestOffsetX[1])> 6.35/2 ) cout << "                                                          * Offset in X is out of border ! " <<endl;
if ( fabs(fBestOffsetY[1])> 6.35/2 ) cout << "                                                          * Offset in Y is out of border ! " <<endl;
cout <<  endl;

cout << " The best estimates according to Bz : " << endl;
cout << " Best Chi2                  : " <<setw(7)<< fBestChi2Z << endl;
cout << " Inner magnets manetization : " <<setw(7)<< fBestMagInner[2] << endl;
cout << " Outer magnets manetization : " <<setw(7)<< fBestMagOuter[2] << endl;
cout << " Offset in X                : " <<setw(7)<< fBestOffsetX[2]<< "mm     " <<setw(7)<< fBestOffsetX[2]/6.35*100<< " % "<< endl;
cout << " Offset in Y                : " <<setw(7)<< fBestOffsetY[2]<< "mm     " <<setw(7)<< fBestOffsetY[2]/6.35*100<< " % "<<endl;
cout << " Offset in Z                : " <<setw(7)<< fBestOffsetZ[2]<< "mm     " <<setw(7)<< fBestOffsetZ[2]/6.35*100<< " % "<<endl;
if (fBestOffsetZ[2]>0) cout <<               "                                                           * Offset in Z is out of border ! " <<endl;
if ( fabs(fBestOffsetX[2])> 6.35/2 ) cout << "                                                           * Offset in X is out of border ! " <<endl;
if ( fabs(fBestOffsetY[2])> 6.35/2 ) cout << "                                                           * Offset in Y is out of border ! " <<endl;
cout << " **************************************************" << endl;

}


void OptimizeParameters::WriteTree(void)
{
cout << "Inside OptimizeParameters::WriteTree()" << endl;

fRootFile->cd();

fTree->Write();

}



void OptimizeParameters::ReadExperimentalData(TString FileNameExpData)
{
cout << "OptimizeParameters::ReadExperimentalData" << endl;

ExpPoint = new ExperimentalPoint(); // here the offset is zero

/////////////////////////////  READ THE DATA FILE ///////////////////////

TString pathTo="./input/";

// open the data file
   ifstream input_1;
   input_1.open((pathTo+FileNameExpData).Data());
   cout<<"opening file : "<<FileNameExpData.Data()<<endl;
   
// dump the first lines from the top
string s_buffer="buffer";
Int_t d_buffer=-1;
Int_t counter1=0; // counter on the first lines

while ( d_buffer != 0 && counter1< 2 /* number of lines to neglect*/) 
	{
	getline(input_1,s_buffer);
	counter1++;
	}

//FORMAT : A 90 12 -1 -1 -140  -0.000397326 0.000397326 -0.000341054
TString  e_Label="buffer";
Double_t e_Quadrant(0);
Double_t e_ProbeAngle(0), e_XR(0), e_YT(0), e_Z(0) ;
Double_t e_BX(0), e_BY(0), e_BZ(0);
bool check_label = true ;
  
input_1 >> e_Label ;
	do
	{
	   //Clear parameters
	   ExpPoint->ClearParameters();
	   // read one line and do some changes 
	   input_1 >> e_Quadrant >> e_ProbeAngle >> e_XR >> e_YT >> e_Z >> e_BX >> e_BY >> e_BZ;
	   ExpPoint->ReadLineAndTreat(e_Label, e_Quadrant, e_ProbeAngle , e_XR,  e_YT, e_Z , e_BX , e_BY , e_BZ);     
	  
	   // fill for values for a certain radius 
	   if ( e_Label=="A" && e_ProbeAngle == 0 /*&& ExpPoint->fPositionRadius < 50 */) 
	   {
	    Double3DVector bufferValue(ExpPoint->fPosition,ExpPoint->fBField);
	    fPositionAndField.push_back(bufferValue);
	    //inspection 
	     cout << " " << e_Label << " " << e_Quadrant << " " << e_ProbeAngle << " " << 
	     ExpPoint->fPositionRadius << " " << ExpPoint->fPositionAngle << " " << ExpPoint->fPosition.Z() << " " << e_BX << " " << e_BY << " " << e_BZ << endl;
	    }
	    	   
       input_1 >> e_Label;
	   //work on booleans to make sure of the reading a meaningful line
	   if (e_Label=="A" || e_Label=="B" || e_Label=="B'" || e_Label=="C" || e_Label=="D") 
	   check_label = true ;
	   else check_label = false ;
	  
	   
	}while ( check_label && !input_1.eof() );
	
	cout << " INFO : " << fPositionAndField.size() << " adopted points. " <<endl;
	
}


void OptimizeParameters::WriteGraph(void)
{
//cout << "OptimizeParameters::WriteGraph" << endl;

   gStyle->SetOptStat(0);

	fRootFile->cd();	
	
	TCanvas *c1 = new TCanvas("Probe","Sensor Offsets",700,800) ; 
	
   TH3F *frame3d = new TH3F("probe","Sensor positions;X offset (mm);Y offset (mm);Z offset (mm)",50,-3.2,3.2,50,-3.2,3.2,50,-3.2,0.2);
   frame3d->Draw();

   TPolyMarker3D *pm3d1 = new TPolyMarker3D(3);
   pm3d1->SetPoint(0,fBestOffsetX[0],fBestOffsetY[0],fBestOffsetZ[0]);
   pm3d1->SetPoint(1,fBestOffsetX[1],fBestOffsetY[1],fBestOffsetZ[1]);
   pm3d1->SetPoint(2,fBestOffsetX[2],fBestOffsetY[2],fBestOffsetZ[2]);

   pm3d1->SetMarkerSize(3);
   pm3d1->SetMarkerColor(kBlue);
   pm3d1->SetMarkerStyle(21);

   pm3d1->Draw();
   
   // the projection on the bottom
   TPolyMarker3D *pm3dz = new TPolyMarker3D(3);
   pm3dz->SetPoint(0,fBestOffsetX[0],fBestOffsetY[0],-3.2);
   pm3dz->SetPoint(1,fBestOffsetX[1],fBestOffsetY[1],-3.2);
   pm3dz->SetPoint(2,fBestOffsetX[2],fBestOffsetY[2],-3.2);

   pm3dz->SetMarkerSize(1);
   pm3dz->SetMarkerColor(kRed);
   pm3dz->SetMarkerStyle(7);
   
   pm3dz->Draw();


	c1->Write();
	
}



TVector3  OptimizeParameters::GetSensorXOffset(void)
{
return GetSensorOffset(0);
}

TVector3  OptimizeParameters::GetSensorYOffset(void)
{
return GetSensorOffset(1);
}

TVector3  OptimizeParameters::GetSensorZOffset(void)
{
return GetSensorOffset(2);
}

TVector3  OptimizeParameters::GetSensorOffset(int index)
{
return TVector3(fBestOffsetX[index],fBestOffsetY[index],fBestOffsetZ[index]);
}
 



