#include "ExpManager.h"

ExpManager::ExpManager(void)
{
Clear();
}

ExpManager::~ExpManager(void){}

void  ExpManager::FillValue(Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t b, Double_t bsim )
{
	fExpX.push_back(x) ;
	fExpY.push_back(y) ;
	fExpZ.push_back(z) ;
	fExpR.push_back(r) ;
	fExpTheta.push_back(theta) ;
	fExpB.push_back(b) ;
	fSimB.push_back(bsim) ;
}

void  ExpManager::FillValueError(Double_t x, Double_t y, Double_t z, Double_t r, Double_t theta, Double_t b)
{
	fExpXErr.push_back(x) ; //mm
	fExpYErr.push_back(y) ; //mm
	fExpZErr.push_back(z) ; //mm
	fExpRErr.push_back(r) ; //mm 
	fExpThetaErr.push_back(theta) ; //degree CHECK
	fExpBErr.push_back(b) ;
}



void ExpManager::ShowVectorSizes(void)
{
cout<<endl<<"-----------EXP"<<endl;
cout<<"x : "<<fExpX.size();
cout<<endl<<"y : "<<fExpY.size();
cout<<endl<<"z : "<<fExpZ.size();
cout<<endl<<"B : "<<fExpB.size();
cout<<endl<<"-----------SIM"<<endl;
cout<<endl<<"B : "<<fSimB.size();
cout<<endl<<"------------------------"<<endl;
}


void ExpManager::ShowVectorsContent(void)
{

for(unsigned i=0 ; i< fExpX.size() ; i++)
cout<<" "<<fExpX.at(i)<<endl;
for(unsigned i=0 ; i< fExpY.size() ; i++)
cout<<" "<<fExpY.at(i)<<endl;
for(unsigned i=0 ; i< fExpZ.size() ; i++)
cout<<" "<<fExpZ.at(i)<<endl;
cout<<endl<<"------------------------"<<endl;
for(unsigned i=0 ; i< fExpB.size() ; i++)
cout<<" "<<fExpB.at(i)<<endl;
cout<<endl<<"------------------------"<<endl;
for(unsigned i=0 ; i< fExpBErr.size() ; i++)
cout<<" "<<fExpBErr.at(i)<<endl;
cout<<endl<<"========================"<<endl;
}


void ExpManager::GetExp1DGraphPolar(TString NameTitle, double zmin, double zmax, double anglemin, double anglemax) {

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Experimental Data;Radius (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpR.size(); i++)   {
        if( (fExpTheta.at(i) >= anglemin && fExpTheta.at(i) <= anglemax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){ 
            fGraph->SetPoint(graph_counter,fExpR.at(i),fExpB.at(i));    // CHECK, add new the stuff for emag
            fGraph->SetPointError(graph_counter,fExpRErr.at(i),fExpBErr.at(i));   // CHECK, add new the stuff for emag
            graph_counter++;
        }
   }

    fGraph->SetTitle(NameTitle+Form("Experimental Data : %.2f < Depth < %.2f mm  __  %.2f#circ < Angle < %.2f#circ;Radius (mm);Magnetic Field (mT)",zmin,zmax,anglemin,anglemax));
    fGraph->SetName(NameTitle+Form("Exp_Depth_%.2f_%.2fmm_Angle_%.2f_%.2fdeg",zmin,zmax,anglemin,anglemax));
    fGraph->Write();

}



void ExpManager::GetExp1DGraphX(TString NameTitle, double zmin, double zmax, double ymin, double ymax){   // do the same thing as for the polar interpolation

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Experimental Data;X (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpY.at(i) >= ymin && fExpY.at(i) <= ymax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){
            cout << "  < ----- " ; 
            fGraph->SetPoint(graph_counter,fExpX.at(i),fExpB.at(i));    // CHECK, add new the stuff for emag
            fGraph->SetPointError(graph_counter,fExpXErr.at(i),fExpBErr.at(i));   // CHECK, add new the stuff for emag
            graph_counter++;
        }
        cout << endl ; 
   }

    fGraph->SetTitle(NameTitle+Form("Experimental Data : %.2f < Depth < %.2f mm  __  %.2f < Y < %.2f mm;X (mm);Magnetic Field (mT)",zmin,zmax,ymin,ymax));
    fGraph->SetName(NameTitle+Form("Exp_Depth_%.2f_%.2fmm_Y_%.2f_%.2fmm",zmin,zmax,ymin,ymax));
    fGraph->Write();

}

void ExpManager::GetExp1DGraphY(TString NameTitle, double zmin, double zmax, double xmin, double xmax){   // do the same thing as for the polar interpolation

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Experimental Data;Y (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpX.at(i) >= xmin && fExpX.at(i) <= xmax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){
            fGraph->SetPoint(graph_counter,fExpY.at(i),fExpB.at(i));    // CHECK, add new the stuff for emag
            fGraph->SetPointError(graph_counter,fExpYErr.at(i),fExpBErr.at(i));   // CHECK, add new the stuff for emag
            graph_counter++;
        }
        cout << endl ; 
   }

    fGraph->SetTitle(NameTitle+Form("Experimental Data : %.2f < Depth < %.2f mm  __  %.2f < X < %.2f mm;Y (mm);Magnetic Field (mT)",zmin,zmax,xmin,xmax));
    fGraph->SetName(NameTitle+Form("Exp_Depth_%.2f_%.2fmm_X_%.2f_%.2fmm",zmin,zmax,xmin,xmax));
    fGraph->Write();

}


void ExpManager::GetExp1DGraphZ(TString NameTitle, double xmin, double xmax, double ymin, double ymax ){   // do the same thing as for the polar interpolation

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Experimental Data;Z (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpX.at(i) >= xmin && fExpX.at(i) <= xmax) && (fExpY.at(i) >= ymin && fExpY.at(i) <= ymax) ){
            fGraph->SetPoint(graph_counter,fExpZ.at(i),fExpB.at(i));    // CHECK, add new the stuff for emag
            fGraph->SetPointError(graph_counter,fExpZErr.at(i),fExpBErr.at(i));   // CHECK, add new the stuff for emag
            graph_counter++;
        }
        //cout << endl ; 
   }

    fGraph->SetTitle(NameTitle+Form("Experimental Data : %.2f < X < %.2f mm  __  %.2f < Y < %.2f mm;Z (mm);Magnetic Field (mT)",xmin,xmax,ymin,ymax));
    fGraph->SetName(NameTitle+Form("Exp_X_%.2f_%.2fmm_Y_%.2f_%.2fmm",xmin,xmax,ymin,ymax));
    fGraph->Write();

}

void ExpManager::GetSim1DGraphPolar(TString NameTitle, double zmin, double zmax, double anglemin, double anglemax) {

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Simulated Data;Radius (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kRed);
    fGraph->SetLineColor(kRed);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpR.size(); i++) {
        if( (fExpTheta.at(i) >= anglemin && fExpTheta.at(i) <= anglemax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){
            fGraph->SetPoint(graph_counter,fExpR.at(i),fSimB.at(i));    
            fGraph->SetPointError(graph_counter,fExpRErr.at(i),0);  
            graph_counter++;
        }
   }

    fGraph->SetTitle(NameTitle+Form("Simulated Data : %.2f < Depth < %.2f mm  __  %.2f#circ < Angle < %.2f#circ;Radius (mm);Magnetic Field (mT)",zmin,zmax,anglemin,anglemax));
    fGraph->SetName(NameTitle+Form("Sim_Depth_%.2f_%.2fmm_Angle_%.2f_%.2fdeg",zmin,zmax,anglemin,anglemax));
    fGraph->Write();

}


void ExpManager::GetSim1DGraphX(TString NameTitle, double zmin, double zmax, double ymin, double ymax){   // do the same thing as for the polar interpolation

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Simulated Data;X (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpY.at(i) >= ymin && fExpY.at(i) <= ymax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){ 
            fGraph->SetPoint(graph_counter,fExpX.at(i),fSimB.at(i)); 
            fGraph->SetPointError(graph_counter,fExpXErr.at(i),0);  
            graph_counter++;
        }
   }

    fGraph->SetTitle(NameTitle+Form("Simulated Data : %.2f < Depth < %.2f mm  __  %.2f < Y < %.2f mm;X (mm);Magnetic Field (mT)",zmin,zmax,ymin,ymax));
    fGraph->SetName(NameTitle+Form("Sim_Depth_%.2f_%.2fmm_Y_%.2f_%.2fmm",zmin,zmax,ymin,ymax));
    fGraph->Write();

}

void ExpManager::GetSim1DGraphY(TString NameTitle, double zmin, double zmax, double xmin, double xmax){   // do the same thing as for the polar interpolation

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Simulated Data;Y (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpX.at(i) >= xmin && fExpX.at(i) <= xmax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){
            fGraph->SetPoint(graph_counter,fExpY.at(i),fSimB.at(i));    // CHECK, add new the stuff for emag
            fGraph->SetPointError(graph_counter,fExpYErr.at(i),0);   // CHECK, add new the stuff for emag
            graph_counter++;
        } 
   }

    fGraph->SetTitle(NameTitle+Form("Simulated Data : %.2f < Depth < %.2f mm  __  %.2f < X < %.2f mm;Y (mm);Magnetic Field (mT)",zmin,zmax,xmin,xmax));
    fGraph->SetName(NameTitle+Form("Sim_Depth_%.2f_%.2fmm_X_%.2f_%.2fmm",zmin,zmax,xmin,xmax));
    fGraph->Write();

}


void ExpManager::GetSim1DGraphZ(TString NameTitle, double xmin, double xmax, double ymin, double ymax ){   // do the same thing as for the polar interpolation

    TGraphErrors *fGraph = new TGraphErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Simulated Data;Z (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpX.at(i) >= xmin && fExpX.at(i) <= xmax) && (fExpY.at(i) >= ymin && fExpY.at(i) <= ymax) ){
            fGraph->SetPoint(graph_counter,fExpZ.at(i),fSimB.at(i));    
            fGraph->SetPointError(graph_counter,fExpZErr.at(i),0);   
            graph_counter++;
        } 
   }

    fGraph->SetTitle(NameTitle+Form("Simulated Data : %.2f < X < %.2f mm  __  %.2f < Y < %.2f mm;Z (mm);Magnetic Field (mT)",xmin,xmax,ymin,ymax));
    fGraph->SetName(NameTitle+Form("Sim_X_%.2f_%.2fmm_Y_%.2f_%.2fmm",xmin,xmax,ymin,ymax));
    fGraph->Write();

}

void ExpManager::GetExp2DGraph(TString NameTitle, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {

    TGraph2DErrors *fGraph = new TGraph2DErrors(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    fGraph->SetTitle("Exp Data;Y (mm);X (mm);Magnetic Field (mT)");
    fGraph->SetMarkerSize(1.2);
    fGraph->SetMarkerStyle(20);
    fGraph->SetMarkerColor(kBlue);
    fGraph->SetLineColor(kBlue);
    fGraph->SetLineWidth(2);
       
    int graph_counter = 0 ; 
   for (unsigned i=0; i< fExpY.size(); i++)   {
        if( (fExpX.at(i) >= xmin && fExpX.at(i) <= xmax) && (fExpY.at(i) >= ymin && fExpY.at(i) <= ymax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ) {
            fGraph->SetPoint(graph_counter,fExpX.at(i),fExpY.at(i),fExpB.at(i));    
            fGraph->SetPointError(graph_counter,fExpXErr.at(i),fExpYErr.at(i),fExpBErr.at(i)); 
            graph_counter++;
        } 
   }

    fGraph->SetTitle(NameTitle+Form("Experimental Data : %.2f < X < %.2f mm  __  %.2f < Y < %.2f mm;Y (mm);X (mm);Magnetic Field (mT)",xmin,xmax,ymin,ymax));
    fGraph->SetName(NameTitle+Form("Sim_X_%.2f_%.2fmm_Y_%.2f_%.2fmm",xmin,xmax,ymin,ymax));
    fGraph->Write();

}


int ExpManager::GetExpBFieldPointXYZ(TString NameTitle, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax  ) {
   
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpX.at(i) >= xmin && fExpX.at(i) <= xmax) && (fExpY.at(i) >= ymin && fExpY.at(i) <= ymax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){
            return (int) i ; 
        } 
   }
   return -1 ; 
}

int ExpManager::GetExpBFieldPoinRTZ(TString NameTitle, double rmin, double rmax, double tmin, double tmax, double zmin, double zmax  ) {
   
   for (unsigned i=0; i< fExpY.size(); i++)   {
    //cout <<  " X "  << fExpX.at(i) ;  
        if( (fExpR.at(i) >= rmin && fExpR.at(i) <= rmax) && (fExpTheta.at(i) >= tmin && fExpTheta.at(i) <= tmax) && (fExpZ.at(i) >= zmin && fExpZ.at(i) <= zmax) ){
            return (int) i ; 
        } 
   }
   return -1 ; 
}

			

void ExpManager::Clear(void) //NEW
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

 fExpB.clear();
 fExpBErr.clear();  //New

//Simulation
 fSimB.clear();

}


double ExpManager::GetSumChi2() {

    double sumchi2 = 0 ; 
    double chi2 = 0 ; 

    for (unsigned i=0 ; i < fSimB.size() ; i++)  {
        chi2 = (fSimB.at(i) - fExpB.at(i))/fExpBErr.at(i); 
        chi2 = chi2*chi2 ;
        sumchi2 = sumchi2 + chi2;
    }
    
    return sumchi2 ;  
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



TGraph2DErrors* ExpManager::GetSim2dGraph(Int_t u, Int_t v)
{

// get size
    Int_t np   = fSimX.size();

    Double_t* x_array = GetArrayFromCVector(fSimX) ;
    Double_t* y_array = GetArrayFromCVector(fSimY) ;
    Double_t* z_array = GetArrayFromCVector(fSimZ) ;
    Double_t* bx_array = GetArrayFromCVector(fSimBx) ;
    Double_t* by_array = GetArrayFromCVector(fSimBy) ;
    Double_t* bz_array = GetArrayFromCVector(fSimBz) ;
    Double_t* b_array = GetArrayFromCVector(fSimBmag) ;
    
    
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






TGraphErrors* ExpManager::GetSim1dGraph(TString NameTitle, double angle )
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
    Double_t* b_array = GetArrayFromCVector(fSimBmag) ;
    
    
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


