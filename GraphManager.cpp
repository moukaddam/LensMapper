#include "GraphManager.h"

GraphManager::GraphManager(void)
{
//cout << "Inside GraphManager::GraphManager()" << endl;
Clear();
}

GraphManager::~GraphManager(void)
{
//cout << "Inside GraphManager::Destructor()" << endl;
}

void  GraphManager::FillVectors(Int_t datatype, Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t bx, Double_t by, Double_t bz)
{
    if (datatype==0) {
    fExpX.push_back(x) ;
        fExpXErr.push_back(0.5) ; //mm
    fExpY.push_back(y) ;
        fExpYErr.push_back(0.5) ; //mm
    fExpZ.push_back(z) ;
        fExpZErr.push_back(0.5) ; //mm
    fExpR.push_back(r) ;
        fExpYErr.push_back(0.5) ; //mm  CHECK
    fExpTheta.push_back(theta) ;
        fExpThetaErr.push_back(0.5) ; //degree CHECK
        
    fExpBx.push_back(bx) ;
    fExpBy.push_back(by) ;
    fExpBz.push_back(bz) ;
    
    fExpBmag.push_back(sqrt(bx*bx + by*by + bz*bz)) ;
    }


}


void GraphManager::ShowVectorSizes(void)
{
cout<<endl<<"-----------EXP"<<endl;
cout<<"x : "<<fExpX.size();
cout<<endl<<"y : "<<fExpY.size();
cout<<endl<<"z : "<<fExpZ.size();
cout<<endl<<"Bx : "<<fExpBx.size();
cout<<endl<<"By : "<<fExpBy.size();
cout<<endl<<"Bz : "<<fExpBz.size();

cout<<endl<<"-----------SIM"<<endl;
cout<<endl<<"Bx : "<<fSimBx.size();
cout<<endl<<"By : "<<fSimBy.size();
cout<<endl<<"Bz : "<<fSimBz.size();
cout<<endl<<"------------------------"<<endl;
}


void GraphManager::ShowVectorsContent(void)
{

for(unsigned i=0 ; i< fExpX.size() ; i++)
cout<<" "<<fExpX.at(i)<<endl;
for(unsigned i=0 ; i< fExpY.size() ; i++)
cout<<" "<<fExpY.at(i)<<endl;
for(unsigned i=0 ; i< fExpZ.size() ; i++)
cout<<" "<<fExpZ.at(i)<<endl;
cout<<endl<<"------------------------"<<endl;
for(unsigned i=0 ; i< fExpBx.size() ; i++)
cout<<" "<<fExpBx.at(i)<<endl;
for(unsigned i=0 ; i< fExpBy.size() ; i++)
cout<<" "<<fExpBy.at(i)<<endl;
for(unsigned i=0 ; i< fExpBz.size() ; i++)
cout<<" "<<fExpBz.at(i);
cout<<endl<<"------------------------"<<endl;
for(unsigned i=0 ; i< fExpBxErr.size() ; i++)
cout<<" "<<fExpBxErr.at(i)<<endl;
for(unsigned i=0 ; i< fExpByErr.size() ; i++)
cout<<" "<<fExpByErr.at(i)<<endl;
for(unsigned i=0 ; i< fExpBzErr.size() ; i++)
cout<<" "<<fExpBzErr.at(i)<<endl;

cout<<endl<<"========================"<<endl;
}

void GraphManager::GetExp1dGraphPolar(TString NameTitle, double angle) // NEW
{

// get size
    Int_t np   = fExpX.size();

    Double_t* x_array = GetArrayFromCVector(fExpX) ;
    Double_t* y_array = GetArrayFromCVector(fExpY) ;
    Double_t* z_array = GetArrayFromCVector(fExpZ) ;    
    Double_t* r_array = GetArrayFromCVector(fExpR) ;
    Double_t* theta_array = GetArrayFromCVector(fExpTheta) ;
    
    Double_t* ex_array = GetArrayFromCVector(fExpXErr) ;
    Double_t* ey_array = GetArrayFromCVector(fExpYErr) ;
    Double_t* ez_array = GetArrayFromCVector(fExpZErr) ;
    Double_t* er_array = GetArrayFromCVector(fExpZErr) ;  
    
    Double_t* bx_array = GetArrayFromCVector(fExpBx) ;
    Double_t* by_array = GetArrayFromCVector(fExpBy) ;
    Double_t* bz_array = GetArrayFromCVector(fExpBz) ;
    Double_t* bmag_array = GetArrayFromCVector(fExpBmag) ;
    
    Double_t* ebx_array = GetArrayFromCVector(fExpBxErr) ;
    Double_t* eby_array = GetArrayFromCVector(fExpByErr) ;
    Double_t* ebz_array = GetArrayFromCVector(fExpBzErr) ;
    
    
    //Errors
    //create
    Double_t *ebmag=0;
    Double_t *etheta=0;
    // allocate 
    etheta = new Double_t[np];
    ebmag = new Double_t[np]; // for mag   (must propagate the errors)
    
    
    //fill
    for (Int_t N=0; N<np;N++)
    {
    etheta[N] = 1.0;   
    ebmag[N] = 0.02; // CHECK (must propagate errors) these errors are really small and  negligible
    }

//inspection

//cout<<"GraphManager::GetExp1dGraphPolar  :          "<<" X  "<<" Y "<<" Z "<<endl;
//for (Int_t i=0;i<np ; i++)
//{
//cout<<"GraphManager::GetExp1dGraphPolar "<<i<<" : "<<x_array[i]<<" "<<y_array[i]<<" "<<z_array[i]<< " : ";
//cout<<bx_array[i]<<" "<<by_array[i]<<" "<<bz_array[i]<<endl;
//cout<<"GraphManager::GetExp1dGraphPolar "<<i<<" : "<<ex[i]<<" "<<ey[i]<<" "<<ez[i]<<endl;
//cout<<"GraphManager::GetExp1dGraphPolar "<<i<<" : "<<ebx_array[i]<<" "<<eby_array[i]<<" "<<ebz_array[i]<<endl;
//}



    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Experimental Data;Radius (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
   

int graph_counter = 0 ; 
   for (Int_t N=0; N<np; N=N+1)
   {

		if(theta_array[N] == angle )
			{
			  
			if(NameTitle=="Bmag")	{
			   fGraph->SetPoint(graph_counter,r_array[N],bmag_array[N]);    // CHECK, add new the stuff for emag
			   fGraph->SetPointError(graph_counter,er_array[N],ebmag[N]);   // CHECK, add new the stuff for emag
						}
						
			else if(NameTitle=="Bx"){
			   fGraph->SetPoint(graph_counter,r_array[N],bx_array[N]);
			   fGraph->SetPointError(graph_counter,er_array[N],ebx_array[N]);
			
						}
						
			else if(NameTitle=="By"){
			   fGraph->SetPoint(graph_counter,r_array[N],by_array[N]);
			   fGraph->SetPointError(graph_counter,er_array[N],eby_array[N]);
						}
						
			else if(NameTitle=="Bz"){
			   fGraph->SetPoint(graph_counter,r_array[N],bz_array[N]);
			   fGraph->SetPointError(graph_counter,er_array[N],ebz_array[N]);
						}
						
		   graph_counter++;
		   
		}
   }
   
   
      
   	fGraph->SetTitle(NameTitle+Form(" Experimental Data / Depth %.2f mm / Angle %.2f degrees;Radius (mm);Magnetic Field (mT)",fExpZ.at(0),angle));
	fGraph->SetName(NameTitle+Form("Exp_Depth_%.2fmm_Angle_%.2fdegrees",fExpZ.at(0),angle));
	fGraph->Write();
	
}




TGraphErrors* GraphManager::GetExp1dGraphCartesian(double y){   // do the same thing as for the polar interpolation

// get size
    Int_t np   = fExpX.size();

    Double_t* x_array = GetArrayFromCVector(fExpX) ;
    Double_t* y_array = GetArrayFromCVector(fExpY) ;
    Double_t* z_array = GetArrayFromCVector(fExpZ) ;
    Double_t* bx_array = GetArrayFromCVector(fExpBx) ;
    Double_t* by_array = GetArrayFromCVector(fExpBy) ;
    Double_t* bz_array = GetArrayFromCVector(fExpBz) ;
    Double_t* bmag_array = GetArrayFromCVector(fExpBmag) ;
    
    //Errors
    Double_t *ebx=0, *eby=0, *ebz=0, *ebmag=0;
    Double_t *ex=0, *ey=0, *ez=0;
    // allocate 
    ex = new Double_t[np];
    ey = new Double_t[np];
    ez = new Double_t[np];
    ebx = new Double_t[np];
    eby = new Double_t[np];
    ebz = new Double_t[np];
    ebmag = new Double_t[np]; // for mag   (must propagate the errors)

    for (Int_t N=0; N<np;N++)  {
	    ex[N] = 1.0; // error 1mm
	    ey[N] = 1.0;
	    ez[N] = 1.0;
	    ebx[N] = 0.02; // error 0.2 mT
	    eby[N] = 0.02;
	    ebz[N] = 0.02;
	    ebmag[N] = 0.02; // CHECK (must propagate errors)
    }

//inspection 
/*
for (Int_t i=0;i<np ; i++)
{
cout<<i<<":"<<x_array[i]<<" "<<y_array[i]<<" "<<bz_array[i]<<endl;
cout<<i<<":"<<ex[i]<<" "<<ey[i]<<" "<<ez[i]<<endl;
}
*/

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Experimental Data;x (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(0.8);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kRed);
    fGraph->SetLineColor(kBlue-3);
    fGraph->SetLineWidth(2);
   
	double yvalue = y;
	int graph_counter = 0 ; 
	for (Int_t N=0; N<np; N=N+1) {
		if( fabs(y_array[N]-yvalue)<0.01 )	{
			fGraph->SetPoint(graph_counter,x_array[N],bmag_array[N]);
			fGraph->SetPointError(graph_counter,ex[N],ebmag[N]);
			graph_counter++;
		}
	}
	
   return fGraph ;
}




void GraphManager::GetSim1dGraphPolar(TString NameTitle, double angle) // NEW
{

// get size
    Int_t np   = fExpX.size();

    Double_t* x_array = GetArrayFromCVector(fExpX) ;
    Double_t* y_array = GetArrayFromCVector(fExpY) ;
    Double_t* z_array = GetArrayFromCVector(fExpZ) ;
    Double_t* r_array = GetArrayFromCVector(fExpR) ;
    Double_t* theta_array = GetArrayFromCVector(fExpTheta) ;
    
    Double_t* ex_array = GetArrayFromCVector(fExpXErr) ;
    Double_t* ey_array = GetArrayFromCVector(fExpYErr) ;
    Double_t* ez_array = GetArrayFromCVector(fExpZErr) ;
    Double_t* er_array = GetArrayFromCVector(fExpZErr) ;  
    
        
    Double_t* bx_array = GetArrayFromCVector(fSimBx) ;
    Double_t* by_array = GetArrayFromCVector(fSimBy) ;
    Double_t* bz_array = GetArrayFromCVector(fSimBz) ;
    Double_t* bmag_array = GetArrayFromCVector(fSimBmag) ;
    
   
    //Errors
    Double_t *ebmag=0;
    Double_t *etheta=0;
    etheta = new Double_t[np];
    ebmag = new Double_t[np]; // for mag   (must propagate the errors)
    for (Int_t N=0; N<np;N++)    {
	    etheta[N] = 1.0; // CHECK (must propagate errors)    
	    ebmag[N] = 0.02; // CHECK (must propagate errors) these errors are really small and  negligible
    }


    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Simulated Data;Radius (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kRed);
    fGraph->SetLineColor(kRed);
    fGraph->SetLineWidth(2);   
   

int graph_counter = 0 ; 
   for (Int_t N=0; N<np; N=N+1)
   {
		if(fabs(theta_array[N]-angle)<0.01) 
			{
			  
			if(NameTitle=="Bmag")	{
			   fGraph->SetPoint(graph_counter,r_array[N],bmag_array[N]);    
			   fGraph->SetPointError(graph_counter,er_array[N],0); // simulated values => no errors
						}
						
			else if(NameTitle=="Bx"){
			   fGraph->SetPoint(graph_counter,r_array[N],bx_array[N]);
			   fGraph->SetPointError(graph_counter,er_array[N],0);
						}
						
			else if(NameTitle=="By"){
			   fGraph->SetPoint(graph_counter,r_array[N],by_array[N]);
			   fGraph->SetPointError(graph_counter,er_array[N],0);
						}
						
			else if(NameTitle=="Bz"){
			   fGraph->SetPoint(graph_counter,r_array[N],bz_array[N]);
			   fGraph->SetPointError(graph_counter,er_array[N],0);
						}
						
		   graph_counter++;
		   
			}
   }
  
     
    fGraph->SetTitle(NameTitle+Form(" Simulated Data / Depth %.2f mm / Angle %.2f degrees;Radius (mm);Magnetic Field (mT)",fExpZ.at(0),angle));
	fGraph->SetName(NameTitle+Form("Sim_Depth_%.2fmm_Angle_%.2fdegrees",fExpZ.at(0),angle));
	fGraph->Write();
	
}








TGraph2DErrors* GraphManager::GetExp2dGraph(void /*ADD an option to select which physical quantity ? */)
{

// get size
    Int_t np   = fExpX.size();

    Double_t* x_array = GetArrayFromCVector(fExpX) ;
    Double_t* y_array = GetArrayFromCVector(fExpY) ;
    Double_t* z_array = GetArrayFromCVector(fExpZ) ;
    Double_t* bx_array = GetArrayFromCVector(fExpBx) ;
    Double_t* by_array = GetArrayFromCVector(fExpBy) ;
    Double_t* bz_array = GetArrayFromCVector(fExpBz) ;
    Double_t* bmag_array = GetArrayFromCVector(fExpBmag) ;
    
    
    //Errors
    //create
    Double_t *ebx=0, *eby=0, *ebz=0, *ebmag=0;
    Double_t *ex=0, *ey=0, *ez=0;
    // allocate 
    ex = new Double_t[np];
    ey = new Double_t[np];
    ez = new Double_t[np];
    ebx = new Double_t[np];
    eby = new Double_t[np];
    ebz = new Double_t[np];
    ebmag = new Double_t[np]; // for mag   (must propagate the errors)
    //fill
    for (Int_t N=0; N<np;N++)
    {
    ex[N] = 1.0; // error 1mm
    ey[N] = 1.0;
    ez[N] = 1.0;
    ebx[N] = 0.02; // error 0.2 mT
    eby[N] = 0.02;
    ebz[N] = 0.02;
    ebmag[N] = 0.02; // CHECK (must propagate errors)
    }

//inspection 
/*
for (Int_t i=0;i<np ; i++)
{
cout<<i<<":"<<x_array[i]<<" "<<y_array[i]<<" "<<bz_array[i]<<endl;
cout<<i<<":"<<ex[i]<<" "<<ey[i]<<" "<<ez[i]<<endl;
}
*/


    TGraph2DErrors *f2dGraph = new TGraph2DErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    f2dGraph->SetTitle("TGraph2D example Exp");
    f2dGraph->SetMarkerSize(0.8);
    f2dGraph->SetMarkerStyle(20);
    f2dGraph->SetMarkerColor(kRed);
    f2dGraph->SetLineColor(kBlue-3);
    f2dGraph->SetLineWidth(2);
   
   for (Int_t N=0; N<np; N++) {
      f2dGraph->SetPoint(N,x_array[N],y_array[N],bmag_array[N]);
      f2dGraph->SetPointError(N,ex[N],ey[N],ebmag[N]);
        }


   return f2dGraph ;
}



Double_t* GraphManager::GetArrayFromCVector(vector<Double_t> vec)
{  
unsigned size = vec.size();
Double_t *array = new Double_t[size];

for (unsigned i = 0 ; i<size ; i++)
array[i] = vec.at(i) ;

return array ;

};

double GraphManager::GetExpBField(double angle, double radius,TString NameTitle)
{

	double output = 0;	

	// get size
   	Int_t np   = fExpX.size();

	Double_t* r_array = GetArrayFromCVector(fExpR) ;
	Double_t* theta_array = GetArrayFromCVector(fExpTheta) ;
	Double_t* bx_array = GetArrayFromCVector(fExpBx) ;
	Double_t* by_array = GetArrayFromCVector(fExpBy) ;
	Double_t* bz_array = GetArrayFromCVector(fExpBz) ;
	Double_t* bmag_array = GetArrayFromCVector(fExpBmag);


	int graph_counter = 0 ; 
   	for (Int_t N=0; N<np; N++) 
	{
		if( fabs(theta_array[N]-angle)<0.01 && fabs(r_array[N]-radius)<0.01 )
		{
			if(NameTitle == "Bx")
				output = bx_array[N];
			else if(NameTitle == "By")
				output = by_array[N];
			else if(NameTitle == "Bz")
				output = bz_array[N];
			else if(NameTitle = "Bmag")
				output = bmag_array[N];
		}
	}

	return output;
}
			




void GraphManager::Clear(void) //NEW
{ 
//Experimental 
 fExpX.clear();
 fExpY.clear();
 fExpZ.clear();

 fExpXErr.clear(); //New
 fExpYErr.clear(); //New
 fExpZErr.clear(); //New

 fExpR.clear();  
 fExpTheta.clear();

 fExpRErr.clear();   //New
 fExpThetaErr.clear();  //New

 fExpBx.clear();
 fExpBy.clear();
 fExpBz.clear();
 fExpBmag.clear();

 fExpBxErr.clear();    //New
 fExpByErr.clear();    //New
 fExpBzErr.clear();    //New
 fExpBmagErr.clear();  //New

//Simulation

 fSimBx.clear();
 fSimBy.clear();
 fSimBz.clear();
 fSimBmag.clear();


}


/* //////////////////////////////////////////////////

DUMPSTER, snippet could be used later 


    else {
    fSimX.push_back(x) ;
    fSimY.push_back(y) ;
    fSimZ.push_back(z) ;
    fSimR.push_back(sqrt(x*x + y*y)) ;
    fSimTheta.push_back(atan(y/x)  * 180/TMath::Pi() ) ;
    fSimBx.push_back(bx) ;
    fSimBy.push_back(by) ;
    fSimBz.push_back(bz) ;
    fSimBmag.push_back(sqrt(bx*bx + by*by + bz*bz)) ;
    }



TGraph2DErrors* GraphManager::GetSim2dGraph(Int_t u, Int_t v)
{

// get size
    Int_t np   = fSimX.size();

    Double_t* x_array = GetArrayFromCVector(fSimX) ;
    Double_t* y_array = GetArrayFromCVector(fSimY) ;
    Double_t* z_array = GetArrayFromCVector(fSimZ) ;
    Double_t* bx_array = GetArrayFromCVector(fSimBx) ;
    Double_t* by_array = GetArrayFromCVector(fSimBy) ;
    Double_t* bz_array = GetArrayFromCVector(fSimBz) ;
    Double_t* bmag_array = GetArrayFromCVector(fSimBmag) ;
    
    
    // Errors
    // create 
    Double_t *ebx=0, *eby=0, *ebz=0, *ebmag=0;
    Double_t *ex=0, *ey=0, *ez=0;
    // allocate 
    ex = new Double_t[np];
    ey = new Double_t[np];
    ez = new Double_t[np];
    ebx = new Double_t[np];
    eby = new Double_t[np];
    ebz = new Double_t[np];
    ebmag = new Double_t[np]; // for mag CHECK
    //fill
    for (Int_t N=0; N<np;N++)
    {
    ex[N] = 1.0; // error 1mm
    ey[N] = 1.0;
    ez[N] = 1.0;
    ebx[N] = 0.02; // error 0.02 mT
    eby[N] = 0.02;
    ebz[N] = 0.02;
    ebmag[N] = 0.2; // CHECK (must propagate errors)
    }

    TGraph2DErrors *f2dGraph = new TGraph2DErrors();//(np, x_array, y_array, bz_array, ex, ey, ez);

    f2dGraph->SetTitle("TGraph2D example Sim");
    f2dGraph->SetMarkerSize(0.8);
    f2dGraph->SetMarkerStyle(20);
    f2dGraph->SetMarkerColor(kRed);
    f2dGraph->SetLineColor(kBlue-3);
    f2dGraph->SetLineWidth(2);
  
  Int_t count = 0 ;
     for (Int_t N=0; N<np; N++) { // loop on all points in plane Z
     
            Int_t x = ((Int_t)x_array[N]);
            Int_t y = ((Int_t)y_array[N]);
            Int_t z = ((Int_t)z_array[N]);


            //Divider => less points.. less time, better performance, replace condition with (x%8==u*4 && y%8==v*4 ) for less points
            if  ( (x%4==u*2 && y%4==v*2) )  // comment this line for full spectrum
                {
            f2dGraph->SetPoint(count,x_array[N],y_array[N],bz_array[N]);
            f2dGraph->SetPointError(count,ex[N],ey[N],ebz[N]);
            count++;
                }
    
        }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ INTERPOLATION CODE UP IN HERE @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//cout<<"Interpolating Data:"<<endl;
	double r = 0.;
	double theta = 22.5 * TMath::Pi()/180;
	double data = 0.;

//Initialise data file here

    TGraphErrors *fgraph = new TGraphErrors();
    fgraph->SetMarkerStyle(20);
    fgraph->SetMarkerColor(kBlue);
    fgraph->SetLineColor(kBlue);
    fgraph->SetLineWidth(2);

  ofstream myfile;
  myfile.open ("interpolated_67_5_bmag.dat");

	for (Int_t i=0; i<120; i++) {
		data = f2dGraph->Interpolate(r*cos(theta),r*sin(theta));
		//cout<<r<<"  "<<r*cos(theta)<<"  "<<r*sin(theta)<<"  "<<data<<endl;
		myfile<<r<<"  "<<data<<endl;
		fgraph->SetPoint(i,r,data);
		r = r+1.;
		} 

		r = 28.5714285714;
		for (Int_t i=0; i<4; i++) {
		data = f2dGraph->Interpolate(r*cos(theta),r*sin(theta));
		//cout<<r<<"  "<<r*cos(theta)<<"  "<<r*sin(theta)<<"  "<<data<<endl;
		myfile<<r<<"  "<<data<<endl;
		r = r+20.;
		}

		myfile.close();
		fgraph->Draw("same l");

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ END OF INTERPOLATION CODE @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                 
    return f2dGraph ;
}






TGraphErrors* GraphManager::GetSim1dGraph(TString NameTitle, double angle )
{


// get size
    Int_t np   = fExpX.size();

    Double_t* x_array = GetArrayFromCVector(fExpX) ;
    Double_t* y_array = GetArrayFromCVector(fExpY) ;
    Double_t* z_array = GetArrayFromCVector(fExpZ) ;
    Double_t* r_array = GetArrayFromCVector(fSimR) ;
    Double_t* theta_array = GetArrayFromCVector(fSimTheta) ;
    Double_t* bx_array = GetArrayFromCVector(fSimBx) ;
    Double_t* by_array = GetArrayFromCVector(fSimBy) ;
    Double_t* bz_array = GetArrayFromCVector(fSimBz) ;
    Double_t* bmag_array = GetArrayFromCVector(fSimBmag) ;
    
    
    //Errors
    //create
    Double_t *ebx=0, *eby=0, *ebz=0, *ebmag=0;
    Double_t *ex=0, *ey=0, *ez=0, *er=0, *etheta=0;
    // allocate 
    ex = new Double_t[np];
    ey = new Double_t[np];
    ez = new Double_t[np];
    er = new Double_t[np];
    etheta = new Double_t[np];

    ebx = new Double_t[np];
    eby = new Double_t[np];
    ebz = new Double_t[np];
    ebmag = new Double_t[np]; // for mag   (must propagate the errors)
    //fill
    for (Int_t N=0; N<np;N++)
    {
    ex[N] = 0; // error 1mm
    ey[N] = 0;
    ez[N] = 0;
    er[N] = 0;
    etheta[N] = 0;
    ebx[N] = 0.02; // error 0.2 mT
    eby[N] = 0.02;
    ebz[N] = 0.02;
    ebmag[N] = 0.02; // CHECK (must propagate errors)
    }

//inspection 

for (Int_t i=0;i<np ; i++)
{
cout<<i<<":"<<x_array[i]<<" "<<y_array[i]<<" "<<bz_array[i]<<endl;
cout<<i<<":"<<ex[i]<<" "<<ey[i]<<" "<<ez[i]<<endl;
}



    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("TGraph1 example Sim");
    fGraph->SetMarkerSize(0.8);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kRed);
    fGraph->SetLineColor(kBlue-3);
    fGraph->SetLineWidth(2);

 
   return fGraph ;
}

*/


