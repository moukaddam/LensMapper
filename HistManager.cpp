#include "HistManager.h"

HistManager::HistManager(TH3D* hist3d, double xerr, double yerr, double zerr){
	//cout << "Inside HistManager::HistManager()" << endl;
	fSim3dHistogram = hist3d;
	fXErr = xerr;
	fYErr = yerr;
	fZErr = zerr;
}

HistManager::~HistManager(void){
	//cout << "Inside HistManager::Destructor()" << endl;
}


void HistManager::SetOffsets(double X , double Y, double Z){
	fOffsetx = X ;
	fOffsety = Y ;
	fOffsetz = Z ;
}

void HistManager::Clear(void){
 fSim3dHistogram->Reset();
}


double HistManager::GetPoint(double X , double Y, double Z) {
	// interpolate
	double Xmin = fSim3dHistogram->GetXaxis()->GetXmin(); 
	double Ymin = fSim3dHistogram->GetYaxis()->GetXmin(); 
	double Zmin = fSim3dHistogram->GetZaxis()->GetXmin();
	double Xmax = fSim3dHistogram->GetXaxis()->GetXmax(); 
	double Ymax = fSim3dHistogram->GetYaxis()->GetXmax(); 
	double Zmax = fSim3dHistogram->GetZaxis()->GetXmax();
	
	if( X>Xmax || X<Xmin ) return 0 ; 
	if( Y>Ymax || Y<Ymin ) return 0 ; 
	if( Z>Zmax || Z<Zmin ) return 0 ; 
	
	double data = fSim3dHistogram->Interpolate(X,Y,Z);
	
	return data ;
}


double HistManager::GetPointError(double X, double Y, double Z){  // returns the maximum error found 

	//cout << "HistManager::GetPointError  ";
	//cout << "X Y Z : "<<X<<"  "<<Y<<"  "<<Z<<endl;
	//cout << "fXErr fYErr fZErr : "<<fXErr<<"  "<<fYErr<<"  "<<fZErr<<endl;

    double data = -1;
	double data_plus_x = -1, data_minus_x = -1, data_error_plus_x = -1, data_error_minus_x = -1;
	double data_plus_y = -1, data_minus_y = -1, data_error_plus_y = -1, data_error_minus_y = -1;
	double data_plus_z = -1, data_minus_z = -1, data_error_plus_z = -1, data_error_minus_z = -1;
	double error_total_plus = -1, error_total_minus = -1;	

	// interpolate
	 data = GetPoint(X,Y,Z)	;
	 //cout << " data "<<  data << endl ;
	 
	 // Interpolate with shifts and then calculate errors
	 data_plus_x = GetPoint( X+fXErr, Y , Z )	; 
	 data_minus_x = GetPoint( X-fXErr, Y , Z )	; 
	 data_error_plus_x = fabs(data_plus_x - data) ; 
	 data_error_minus_x = fabs(data_minus_x - data);

    // cout <<  data_error_plus_x << "  " << data_error_minus_x << endl ;

	 data_plus_y = GetPoint( X, Y+fYErr , Z )	; 
	 data_minus_y = GetPoint( X , Y-fYErr , Z )	; 		 
	 data_error_plus_y = fabs(data_plus_y - data) ; 
	 data_error_minus_y = fabs(data_minus_y - data);

    // cout <<  data_error_plus_y << "  " << data_error_minus_y << endl ;
     
	 data_plus_z = GetPoint( X, Y , Z+fZErr )	; 
	 data_minus_z = GetPoint( X , Y , Z-fZErr )	; 
	 data_error_plus_z = fabs(data_plus_z - data) ; 
	 data_error_minus_z = fabs(data_minus_z - data);

    // cout <<  data_error_plus_z << "  " << data_error_minus_z << endl ;
         	 
	 // Errors
	//--------
	// add up errors
	// error_total_plus = data_error_plus_x + data_error_plus_y + data_error_plus_z;
	// error_total_minus = data_error_minus_x + data_error_minus_y + data_error_minus_z;
	
	// Or get the maximum error   //NEW
	error_total_plus = max(max(data_error_plus_x , data_error_plus_y),data_error_plus_z);  
	error_total_minus = max(max(data_error_minus_x , data_error_minus_y),data_error_minus_z);
	
	//cout << error_total_plus << "  " << error_total_minus << " ";
	//cout << max(error_total_plus,error_total_minus) << endl ;
	
	return max(error_total_plus,error_total_minus);

}


double HistManager::GetPointErrorX(double X, double Y, double Z){  // returns the maximum error found 

	//cout << "HistManager::GetPointError  ";
	//cout << "X Y Z : "<<X<<"  "<<Y<<"  "<<Z<<endl;
	//cout << "fXErr fYErr fZErr : "<<fXErr<<"  "<<fYErr<<"  "<<fZErr<<endl;

    double data = -1;
	double data_plus = -1, data_minus = -1, data_error_plus = -1, data_error_minus = -1;

	// interpolate
	 data = GetPoint(X,Y,Z)	;
	 
	 // Interpolate with shifts and then calculate errors
	 data_plus = GetPoint( X+fXErr, Y , Z); 
	 data_minus = GetPoint( X-fXErr, Y , Z); 
	 data_error_plus = fabs(data_plus - data) ; 
	 data_error_minus= fabs(data_minus - data);
	
	return max(data_error_plus,data_error_minus);
}

double HistManager::GetPointErrorY(double X, double Y, double Z){  // returns the maximum error found 

    double data = -1;
	double data_plus = -1, data_minus = -1, data_error_plus = -1, data_error_minus = -1;

	// interpolate
	 data = GetPoint(X,Y,Z)	;
 
	 // Interpolate with shifts and then calculate errors
	 data_plus = GetPoint( X, Y+fYErr , Z )	; 
	 data_minus = GetPoint( X, Y-fYErr , Z )	; 
	 data_error_plus = fabs(data_plus - data) ; 
	 data_error_minus= fabs(data_minus - data);
	
	return max(data_error_plus,data_error_minus);
}

double HistManager::GetPointErrorZ(double X, double Y, double Z){  // returns the maximum error found 

    double data = -1;
	double data_plus = -1, data_minus = -1, data_error_plus = -1, data_error_minus = -1;

	// interpolate
	 data = GetPoint(X,Y,Z)	;
	 //cout << " data "<<  data << endl ;
	 
	 // Interpolate with shifts and then calculate errors
	 data_plus = GetPoint( X, Y , Z+fZErr); 
	 data_minus = GetPoint( X, Y , Z-fZErr ); 
	 data_error_plus = fabs(data_plus - data) ; 
	 data_error_minus= fabs(data_minus - data);
	
	return max(data_error_plus,data_error_minus);
}


void HistManager::DrawPolar(TString title, int samples, bool DrawError, double theta , double z){

    //cout << " HistManager::DrawPolarInterpolation " <<endl ; 
	double data = -1, data_plus_x = -1, data_minus_x = -1, data_error_plus_x = -1, data_error_minus_x = -1;
	double data_plus_y = -1, data_minus_y = -1, data_error_plus_y = -1, data_error_minus_y = -1;
	double data_plus_z = -1, data_minus_z = -1, data_error_plus_z = -1, data_error_minus_z = -1;
	double error_total_plus = -1, error_total_minus = -1;
	double r = 0;
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the size of the comsol simulation
	double dr = rmax/samples; //  this is the step
	double theta_rad = theta * TMath::Pi() / 180 ;	
	TGraphAsymmErrors* graph1d = new TGraphAsymmErrors(samples);
		
	//interpolate 
	for (int i=0 ; i<samples; i++)	{
	
		// interpolate
		if (r*cos(theta_rad)>rmax || r*sin(theta_rad)>rmax) break; 
		 	data = GetPoint(r*cos(theta_rad),r*sin(theta_rad),z)	;
		 graph1d->SetPoint(i, r , data);  
		 if (DrawError) {
				// Interpolate with shifts and then calculate errors
				data_plus_x = GetPoint( r*cos(theta_rad)+fXErr, r*sin(theta_rad) , z )	; 
				data_minus_x = GetPoint( r*cos(theta_rad)-fXErr , r*sin(theta_rad) , z )	; 
				data_error_plus_x = fabs(data_plus_x - data) ; 
				data_error_minus_x = fabs(data_minus_x - data);

				data_plus_y = GetPoint( r*cos(theta_rad), r*sin(theta_rad)+fYErr , z )	; 
				data_minus_y = GetPoint( r*cos(theta_rad) , r*sin(theta_rad)-fYErr , z )	; 		 
				data_error_plus_y = fabs(data_plus_y - data) ; 
				data_error_minus_y = fabs(data_minus_y - data);

				data_plus_z = GetPoint( r*cos(theta_rad), r*sin(theta_rad) , z+fZErr )	; 
				data_minus_z = GetPoint( r*cos(theta_rad) , r*sin(theta_rad) , z-fZErr )	; 
				data_error_plus_z = fabs(data_plus_z - data) ; 
				data_error_minus_z = fabs(data_minus_z - data);

				// Or get the maximum error   //NEW
				error_total_plus = max(max(data_error_plus_x , data_error_plus_y),data_error_plus_z);  
				error_total_minus = max(max(data_error_minus_x , data_error_minus_y),data_error_minus_z);

				graph1d->SetPointEYhigh(i, error_total_plus);
				graph1d->SetPointEYlow(i, error_total_minus);	 
		 }
		 // increment step	
		 r = r + dr	;
	}
	
	graph1d->SetName(title+(Form("Angle_%.2fdeg_Depth_%.2fmm_Interpol_%d_points",theta, z, samples)));
	graph1d->SetTitle(Form("Total Magnetic Field as a Function of Radius at Angle %.2fdeg",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)");
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Magnetic Field Strength (mT)");
	graph1d->GetYaxis()->CenterTitle();
	graph1d->Write();
	 
}

void HistManager::DrawCartesianFixedX(TString title, int samples,bool DrawError, double x , double z) {

	double y = 0.;
	double ymax = fSim3dHistogram->GetYaxis()->GetXmax(); // the maximum extent would be the size of the comsol simulation
	double dy = ymax/samples; 
	TGraphAsymmErrors* graph1d = new TGraphAsymmErrors(samples);
	
	//interpolate 
	for (int i=0 ; i<samples; i++) 	{
		 double data = GetPoint(x,y,z)	;
		 graph1d->SetPoint(i, y , data);
		 if(DrawError){
			double data_plus_y = GetPoint( x, y+fYErr , z )	; 
			double data_minus_y = GetPoint( x , y-fYErr , z )	; 		 
			double data_error_plus_y = fabs(data_plus_y - data) ; 
			double data_error_minus_y = fabs(data_minus_y - data);
		 	graph1d->SetPointEYhigh(i, data_error_plus_y);
		 	graph1d->SetPointEYlow(i, data_error_minus_y);
		}
		 // increment step	
		 y = y + dy	;
	}
	
	graph1d->SetName(title+(Form("X(fixed)_%.2fmm_Z_%.2fmm_%d_points",x, z, samples)));
	graph1d->SetTitle(Form("Total Magnetic Field as a Function of y position at fixed x = %.2fmm",x));
	graph1d->GetXaxis()->SetTitle("y (mm)");	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Magnetic Field Strength (mT)");	graph1d->GetYaxis()->CenterTitle();
	graph1d->Write(); 
}

void HistManager::DrawCartesianFixedY(TString title, int samples,bool DrawError, double y , double z) {

	double x = 0.;
	double xmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the size of the comsol simulation
	double dx = xmax/samples;
	TGraphAsymmErrors* graph1d = new TGraphAsymmErrors(samples);
	
	//interpolate 
	for (int i=0 ; i<samples; i++) 	{
		 double data = GetPoint(x,y,z);
		 graph1d->SetPoint(i, x , data);
		if(DrawError) {
			double data_plus_x = GetPoint(x+fXErr, y , z); 
			double data_minus_x = GetPoint(x-fXErr , y , z); 
			double data_error_plus_x = fabs(data_plus_x - data); 
			double data_error_minus_x = fabs(data_minus_x - data);
			graph1d->SetPointEYhigh(i, data_error_plus_x);
			graph1d->SetPointEYlow(i, data_error_minus_x);
		}
		 // increment step	
		 x = x + dx	;
	}
	
	graph1d->SetName(title+(Form("Y(Fixed)_%.2fmm_Z_%.2fmm_%d_points",y, z, samples)));
	graph1d->SetTitle(Form("Total Magnetic Field as a Function of x position at fixed y = %.2fmm",y));
	graph1d->GetXaxis()->SetTitle("x (mm)");	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Magnetic Field Strength (mT)");	graph1d->GetYaxis()->CenterTitle();
	graph1d->Write(); 
}



void HistManager::DrawPolarOffsetX(TString NameTitle, int samples, bool DrawError, double offset, double theta, double z) {

	double r = 0;
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the diagonal of the comsol 
	double dr = rmax/samples; // 110 mm
	double theta_rad = theta * TMath::Pi() / 180 ;	
	TGraphAsymmErrors*  graph1d = new TGraphAsymmErrors(samples);
	
	//interpolate 
	for (int i=0 ; i<samples; i++)	{
		 if (r*cos(theta_rad)>rmax || r*sin(theta_rad)>rmax) break; 
		 double x (r*cos(theta_rad)) ; double y = (r*sin(theta_rad)) ; 
		 double data = GetPoint(x+offset,y,z);

		 // fill the histogram	
		 graph1d->SetPoint(i, r , data);
		if(DrawError) {
			double data_plus_y = GetPoint( x, y+fYErr , z )	; 
			double data_minus_y = GetPoint( x , y-fYErr , z )	; 		 
			double data_error_plus_y = fabs(data_plus_y - data) ; 
			double data_error_minus_y = fabs(data_minus_y - data);
			graph1d->SetPointEYhigh(i, data_error_plus_y);
			graph1d->SetPointEYlow(i, data_error_minus_y);
		}
		 // increment step	
		 r = r + dr;	
	}
	
	graph1d->SetName(NameTitle+(Form("Angle_%.2f#circ _Z_%.2fmm_OffsetX_%.2fmm",theta, z,offset)));
	graph1d->SetTitle(NameTitle+Form(" Magnetic Field  (X offset) as a function of Radius at Angle=%.2f#circ",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)") ; 	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("X Magnetic Field Strength (mT)") ; 	graph1d->GetYaxis()->CenterTitle();
	graph1d->Write();
 
}

void HistManager::DrawPolarOffsetY(TString NameTitle, int samples, bool DrawError, double offset, double theta, double z) {

	double r = 0;
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the diagonal of the comsol 
	double dr = rmax/samples; // 110 mm
	double theta_rad = theta * TMath::Pi() / 180 ;	
	TGraphAsymmErrors* graph1d = new TGraphAsymmErrors(samples);
	
	//interpolate 
	for (int i=0 ; i<samples; i++){

		if (r*cos(theta_rad)>rmax || r*sin(theta_rad)>rmax) break; 
		 double x (r*cos(theta_rad)) ; double y = (r*sin(theta_rad)) ; 
		 double data = GetPoint(x,y+offset,z);
		 // fill the histogram
		 data = GetPoint(r*cos(theta_rad),(r*sin(theta_rad))+offset,z);
		 graph1d->SetPoint(i, r , data);

		if(DrawError) {
			double data_plus_x = GetPoint( x+fXErr, y , z )	; 
			double data_minus_x = GetPoint( x+fXErr , y , z )	; 		 
			double data_error_plus_x = fabs(data_plus_x - data) ; 
			double data_error_minus_x = fabs(data_minus_x - data);
			graph1d->SetPointEYhigh(i, data_error_plus_x);
			graph1d->SetPointEYlow(i, data_error_minus_x);
        }
		 // increment step	
		 r = r + dr;	
	}
	
	graph1d->SetName(NameTitle+(Form("Angle_%.2f#circ _Z_%.2fmm_OffsetY_%.2fmm",theta, z,offset)));
	graph1d->SetTitle(NameTitle+Form(" Magnetic Field (Y offset) as a function of Radius at Angle=%.2f#circ",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)") ; 	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Y Magnetic Field Strength (mT)") ; 	graph1d->GetYaxis()->CenterTitle();
	graph1d->Write();
	
}

void HistManager::Draw2DGraph(TString NameTitle, int points , double xlow,double xhigh,double ylow,double yhigh, double z){
	
	TGraph2D* graph2d = new TGraph2D();
	int graph_counter=0; 
	double stepx = fabs(xhigh-xlow)/points;
	double stepy = fabs(yhigh-ylow)/points;

	for(double x=xlow ; x<=xhigh ; x=x+stepx){
		for(double y=ylow ; y<=yhigh ; y=y+stepy){
			double data = GetPoint(x,y,z);
			graph2d->SetPoint(graph_counter, x , y, data);
            graph_counter++;
		}
	}

	graph2d->SetName(NameTitle+(Form(" z_%.2fmm_Interpolpoints_%d_xstepof_%.2fmm_ystepof_%.2fmm",z, graph_counter,stepx,stepy)));
	graph2d->SetTitle(NameTitle+Form(" z=%.2fmm  %.2f<x<%.2f  %.2f<y<%.2f ;x (mm);y (mm); Magnetic Field Strength (mT)", z, xlow, xhigh, ylow, yhigh) );
	graph2d->SetDrawOption("TRI1");
	graph2d->Write();
}

void HistManager::Draw2DHist(TString NameTitle, int points, double xlow,double xhigh,double ylow,double yhigh, double z, int contour){

	double stepx = fabs(xhigh-xlow)/points;
	double stepy = fabs(yhigh-ylow)/points;
	TH2D* hist = new TH2D(NameTitle,NameTitle,points,xlow,xhigh,points,ylow,yhigh);

	for(double x=xlow ; x<=xhigh ; x=x+stepx){
		for(double y=ylow ; y<=yhigh ; y=y+stepy){
			double data = GetPoint(x,y,z);
			int binNumber = hist->FindBin(x,y);	
			//cout <<  " " << x << " " << y << " " << z << " " << data << " " << binNumber <<  endl ; cin.get(); 
			hist->SetBinContent(binNumber,data);
		}
	}

//double* levels = new double[contour];
//for (int i = 0 ; i< contour ; i++)
//levels[i] = 0;
double levels[49] ={0.0005, 0.001, 0.01, 0.1,     //4
                	1 , 2,  3,  4,  5,  6,  7,  8,  9,      //9
            		10, 11, 12, 13, 14, 15, 16, 17, 18, 19, //10
            		20,30,40,50,60,70,80,90,        //8
            		100,120,140,160,180,            //5
            		200, 250,                          
            		300, 350,
            		400, 450,
            		500, 550,
            		600, 650,                       //10
            		700,
            		800,
            		900} ;                          //3    sum = 49   
	hist->SetContour(49,levels);
	hist->SetContour(contour,levels);
	hist->SetContour(255);
	hist->SetOption("CONT1 COLZ LIST");
	hist->SetStats(0);
	hist->SetName(NameTitle+(Form(" Z_%.2fmm_%d_points",z,points)));
	hist->SetTitle(NameTitle+Form(" z=%.2fmm  %.2f<x<%.2f  %.2f<y<%.2f ;x (mm);y (mm); Magnetic Field Strength (mT)", z, xlow, xhigh, ylow, yhigh) );
	hist->GetZaxis()->SetRangeUser(-700,700);
    hist->Write() ;
}