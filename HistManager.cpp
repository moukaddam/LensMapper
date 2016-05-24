#include "HistManager.h"

HistManager::HistManager(TH3D* hist3d, double dx, double dy, double dz){
	//cout << "Inside HistManager::HistManager()" << endl;
	fSim3dHistogram = hist3d;
	fdx = dx;
	fdy = dy;
	fdz = dz;
	}

HistManager::~HistManager(void){
	//cout << "Inside HistManager::Destructor()" << endl;
	}

double HistManager::GetInterpolationOnePointError(double X, double Y, double Z){  // Get values at point X Y Z  // NEW

	cout << "HistManager::GetInterpolationOnePointError  ";
	cout << "X Y Z : "<<X<<"  "<<Y<<"  "<<Z<<endl;
	//cout << "fdx fdy fdz : "<<fdx<<"  "<<fdy<<"  "<<fdz<<endl;

    double data = -1;
	double data_plus_x = -1, data_minus_x = -1, data_error_plus_x = -1, data_error_minus_x = -1;
	double data_plus_y = -1, data_minus_y = -1, data_error_plus_y = -1, data_error_minus_y = -1;
	double data_plus_z = -1, data_minus_z = -1, data_error_plus_z = -1, data_error_minus_z = -1;
	double error_total_plus = -1, error_total_minus = -1;	

	// interpolate
	 data = GetInterpolationOnePoint(X,Y,Z)	;
	 //cout << " data "<<  data << endl ;
	 
	 // Interpolate with shifts and then calculate errors
	 data_plus_x = GetInterpolationOnePoint( X+fdx, Y , Z )	; 
	 data_minus_x = GetInterpolationOnePoint( X-fdx, Y , Z )	; 
	 data_error_plus_x = fabs(data_plus_x - data) ; 
	 data_error_minus_x = fabs(data_minus_x - data);

    // cout <<  data_error_plus_x << "  " << data_error_minus_x << endl ;

	 data_plus_y = GetInterpolationOnePoint( X, Y+fdy , Z )	; 
	 data_minus_y = GetInterpolationOnePoint( X , Y-fdy , Z )	; 		 
	 data_error_plus_y = fabs(data_plus_y - data) ; 
	 data_error_minus_y = fabs(data_minus_y - data);

    // cout <<  data_error_plus_y << "  " << data_error_minus_y << endl ;
     
	 data_plus_z = GetInterpolationOnePoint( X, Y , Z+fdz )	; 
	 data_minus_z = GetInterpolationOnePoint( X , Y , Z-fdz )	; 
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

void HistManager::GetPolarInterpolation(double theta , double z_level, int number_of_interpolation_points, TString Bdirection, int draw_errors){

    cout << " HistManager::GetPolarInterpolation " <<endl ; 
    
	double data = -1, data_plus_x = -1, data_minus_x = -1, data_error_plus_x = -1, data_error_minus_x = -1;
	double data_plus_y = -1, data_minus_y = -1, data_error_plus_y = -1, data_error_minus_y = -1;
	double data_plus_z = -1, data_minus_z = -1, data_error_plus_z = -1, data_error_minus_z = -1;
	double error_total_plus = -1, error_total_minus = -1;
	double r = 0;
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the diagonal of the comsol 
	double dr = rmax/number_of_interpolation_points; // 110 mm
	double theta_rad = theta * TMath::Pi() / 180 ;	
	//create histogram	
	TGraphAsymmErrors* graph1d = new TGraphAsymmErrors(number_of_interpolation_points);
		
	//interpolate 
	for (int i=0 ; i<number_of_interpolation_points; i++)	{
	
		// interpolate
		if (r*cos(theta_rad)>rmax || r*sin(theta_rad)>rmax) break; 
		 data = GetInterpolationOnePoint(r*cos(theta_rad),r*sin(theta_rad),z_level)	;
		 
		 // Interpolate with shifts and then calculate errors
		 data_plus_x = GetInterpolationOnePoint( r*cos(theta_rad)+fdx, r*sin(theta_rad) , z_level )	; 
		 data_minus_x = GetInterpolationOnePoint( r*cos(theta_rad)-fdx , r*sin(theta_rad) , z_level )	; 
		 data_error_plus_x = fabs(data_plus_x - data) ; 
		 data_error_minus_x = fabs(data_minus_x - data);

		 data_plus_y = GetInterpolationOnePoint( r*cos(theta_rad), r*sin(theta_rad)+fdy , z_level )	; 
		 data_minus_y = GetInterpolationOnePoint( r*cos(theta_rad) , r*sin(theta_rad)-fdy , z_level )	; 		 
		 data_error_plus_y = fabs(data_plus_y - data) ; 
		 data_error_minus_y = fabs(data_minus_y - data);

		 data_plus_z = GetInterpolationOnePoint( r*cos(theta_rad), r*sin(theta_rad) , z_level+fdz )	; 
		 data_minus_z = GetInterpolationOnePoint( r*cos(theta_rad) , r*sin(theta_rad) , z_level-fdz )	; 
		 data_error_plus_z = fabs(data_plus_z - data) ; 
		 data_error_minus_z = fabs(data_minus_z - data);
		 
		// Errors
		//--------
		// add up errors
		// error_total_plus = data_error_plus_x + data_error_plus_y + data_error_plus_z;
		// error_total_minus = data_error_minus_x + data_error_minus_y + data_error_minus_z;
		
		// Or get the maximum error   //NEW
		error_total_plus = max(max(data_error_plus_x , data_error_plus_y),data_error_plus_z);  
		error_total_minus = max(max(data_error_minus_x , data_error_minus_y),data_error_minus_z);
		 
		 graph1d->SetPoint(i, r , data);  // NEW 
		
		 if (draw_errors) {
			 graph1d->SetPointEYhigh(i, error_total_plus);
			 graph1d->SetPointEYlow(i, error_total_minus);	 
			 }

		 // increment step	
		 r = r + dr	;
		
	}
	
	graph1d->SetName(Bdirection+(Form("Angle_%.2fdeg_Depth_%.2fmm_Interpol_%d_points",theta, z_level, number_of_interpolation_points)));
	graph1d->SetTitle(Form("Total Magnetic Field as a Function of Radius at Angle %.2fdeg",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)");
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Magnetic Field Strength (mT)");
	graph1d->GetYaxis()->CenterTitle();
	graph1d->Write();
	 
}

TGraphAsymmErrors* HistManager::GetCartesianInterpolation(double y , double z_level, int number_of_interpolation_points, TString Bdirection) {

cout <<  "HistManager::GetCartesianInterpolation " << endl ; 
	double data = -1, data_plus_x = -1, data_minus_x = -1, data_error_plus_x = -1, data_error_minus_x = -1;
	double data_plus_y = -1, data_minus_y = -1, data_error_plus_y = -1, data_error_minus_y = -1;
	double data_plus_z = -1, data_minus_z = -1, data_error_plus_z = -1, data_error_minus_z = -1;
	double error_total_plus = -1, error_total_minus = -1;
	double x = 0.;
	double dx = 90.0/number_of_interpolation_points; // 90 mm
	
	//create histogram	
	TGraphAsymmErrors* graph1d = new TGraphAsymmErrors(number_of_interpolation_points);
	
	//interpolate 
	for (int i=0 ; i<number_of_interpolation_points; i++) 	{
	// code from Chris
	
		// interpolate
		 data = GetInterpolationOnePoint(x,y,z_level)	;
		 // Interpolate with shifts and then calculate errors
		 data_plus_x = GetInterpolationOnePoint( x+fdx, y , z_level )	; 
		 data_minus_x = GetInterpolationOnePoint( x-fdx , y , z_level )	; 
		 data_error_plus_x = fabs(data_plus_x - data) ; 
		 data_error_minus_x = fabs(data_minus_x - data);

		 data_plus_y = GetInterpolationOnePoint( x, y+fdy , z_level )	; 
		 data_minus_y = GetInterpolationOnePoint( x , y-fdy , z_level )	; 		 
		 data_error_plus_y = fabs(data_plus_y - data) ; 
		 data_error_minus_y = fabs(data_minus_y - data);

		 data_plus_z = GetInterpolationOnePoint( x, y , z_level+fdz )	; 
		 data_minus_z = GetInterpolationOnePoint( x , y , z_level-fdz )	; 
		 data_error_plus_z = fabs(data_plus_z - data) ; 
		 data_error_minus_z = fabs(data_minus_z - data);
		 
		 
		 // Errors 
		 //----------
		 // add up errors
		// error_total_plus = data_error_plus_x + data_error_plus_y + data_error_plus_z;
		// error_total_minus = data_error_minus_x + data_error_minus_y + data_error_minus_z;
		
		// Or get the maximum error   //NEW
		error_total_plus = max(max(data_error_plus_x , data_error_plus_y),data_error_plus_z);  
		error_total_minus = max(max(data_error_minus_x , data_error_minus_y),data_error_minus_z);
				
		 // fill the histogram
		 graph1d->SetPoint(i, x , data);
		 graph1d->SetPointEYhigh(i, error_total_plus);
		 graph1d->SetPointEYlow(i, error_total_minus);
		
		 // increment step	
		 x = x + dx	;
		
	}
	
	graph1d->SetName(Bdirection+(Form("YPosition_%.2fmm_Depth_%.2fmm_Interpol_%d_points",y, z_level, number_of_interpolation_points)));
	graph1d->SetTitle(Form("Total Magnetic Field as a Function of x position at %.2fmm",y));
	graph1d->GetXaxis()->SetTitle("x (mm)");
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Magnetic Field Strength (mT)");
	graph1d->GetYaxis()->CenterTitle();
	
	return graph1d ;
 
}

TGraph* HistManager::GetPolarInterpolationOffsetX(double theta, double z_level, int offset, int number_of_interpolation_points, TString NameTitle) {

	double data = -1;
	double r = 0;
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the diagonal of the comsol 
	double dr = rmax/number_of_interpolation_points; // 110 mm
	double theta_rad = theta * TMath::Pi() / 180 ;	
	
	//create histogram	
	TGraph* graph1d = new TGraph(number_of_interpolation_points);
	
	//interpolate 
	for (int i=0 ; i<number_of_interpolation_points; i++)	{
		 if (r*cos(theta_rad)>rmax || r*sin(theta_rad)>rmax) break; 
		 data = GetInterpolationOnePoint((r*cos(theta_rad))+offset,r*sin(theta_rad),z_level);
		 // fill the histogram	
		 graph1d->SetPoint(i, r , data);
		 // increment step	
		 r = r + dr;	
	}
	
	graph1d->SetName(NameTitle+(Form("Angle_%.2fdegrees_Depth_%.2fmm_Offset_%dmm",theta, z_level,offset)));
	graph1d->SetTitle(Form("X Magnetic Field as a Function of Radius at Angle %.2fdegrees",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)") ;
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("X Magnetic Field Strength (mT)") ;
	graph1d->GetYaxis()->CenterTitle();
	
	return graph1d ;
 
}

TGraph* HistManager::GetPolarInterpolationOffsetY(double theta, double z_level, int offset, int number_of_interpolation_points, TString NameTitle) {

	double data = -1;
	double r = 0;
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the diagonal of the comsol 
	double dr = rmax/number_of_interpolation_points; // 110 mm
	double theta_rad = theta * TMath::Pi() / 180 ;	
	
	//create histogram	
	TGraph* graph1d = new TGraph(number_of_interpolation_points);
	
	//interpolate 
	for (int i=0 ; i<number_of_interpolation_points; i++){
		if (r*cos(theta_rad)>rmax || r*sin(theta_rad)>rmax) break; 
		 data = GetInterpolationOnePoint(r*cos(theta_rad),(r*sin(theta_rad))+offset,z_level);
		 // fill the histogram	
		 graph1d->SetPoint(i, r , data);
		 // increment step	
		 r = r + dr;	
	}
	
	graph1d->SetName(NameTitle+(Form("Angle_%.2fdegrees_Depth_%.2fmm_Offset_%dmm",theta, z_level,offset)));
	graph1d->SetTitle(Form("Y Magnetic Field as a Function of Radius at Angle %.2fdegrees",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)") ;
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Y Magnetic Field Strength (mT)") ;
	graph1d->GetYaxis()->CenterTitle();
	
	return graph1d ;
 
}


TGraph* HistManager::GetPolarInterpolationOffsetZ(double theta, double z_level, double offset, int number_of_interpolation_points, TString Bdirection) {

	double data = -1;
	double r = 0; 
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the diagonal of the comsol 
	double dr = rmax/number_of_interpolation_points; // 110 mm
	double theta_rad = theta * TMath::Pi() / 180 ;	
	
	//create histogram	
	TGraph* graph1d = new TGraph(number_of_interpolation_points);
	
	//interpolate 
	for (int i=0 ; i<number_of_interpolation_points; i++)	{
		if (r*cos(theta_rad)>rmax || r*sin(theta_rad)>rmax) break; 
		 data = GetInterpolationOnePoint(r*cos(theta_rad),r*sin(theta_rad),z_level+offset);	
		 // fill the histogram	
		 graph1d->SetPoint(i, r , data);
		 // increment step	
		 r = r + dr;	
	}
	
	graph1d->SetName(Bdirection+(Form("Angle_%.2fdegrees_Depth_%.2fmm",theta,(z_level+offset))));
	graph1d->SetTitle(Form("Z Magnetic Field as a Function of Radius at Angle %.2fdegrees",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)") ;
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Z Magnetic Field Strength (mT)") ;
	graph1d->GetYaxis()->CenterTitle();
	
	return graph1d ;
 
}


void HistManager::GetPolarInterpolationOffsetXYZ(double theta, double z_level, int number_of_interpolation_points, TString Bdirection) { //NEW

	double data = -1;
	double r = 0; 
	double rmax = fSim3dHistogram->GetXaxis()->GetXmax(); // the maximum extent would be the diagonal of the comsol x and y have the same axis
	double dr = rmax/number_of_interpolation_points; // 110 mm
	double zmax = fSim3dHistogram->GetZaxis()->GetXmax(); 
	double theta_rad = theta * TMath::Pi() / 180 ;     
        
	//create histogram	
	TGraph* graph1d = new TGraph(number_of_interpolation_points);
	
	//interpolate
	for (int i=0 ; i<number_of_interpolation_points; i++){

         // calculate positions with offsets
		 double xx =  r*cos(theta_rad) + fOffsetx ;
		 double yy =  r*sin(theta_rad) + fOffsety ;
		 double zz =  z_level          + fOffsetz ;

		if (xx>rmax || yy>rmax || zz>zmax) break; 		 
		 // interpolate value
		 data = GetInterpolationOnePoint(xx,yy,zz);
		 // correction for offsets and direction
		 TVector3 dummy(xx,yy,zz);	 
         double rr = dummy.Perp() ;
         //fill the graph
		 graph1d->SetPoint(i, rr , data);
		 // increment step	
		 r = r + dr;
	}
	
	graph1d->SetName(Bdirection+(Form("Angle_%.2fdegrees_Depth_%.2fmm_Offsets_%.2f_%.2f_%.2f_mm",theta,(z_level),fOffsetx,fOffsety,fOffsetz)));
	graph1d->SetTitle(Form("Magnetic Field as a Function of Radius at Angle %.2fdegrees",theta));
	graph1d->GetXaxis()->SetTitle("Radius (mm)") ;
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle(Bdirection+" Magnetic Field Strength (mT)") ;
	graph1d->GetYaxis()->CenterTitle();
	
	graph1d->Write() ;
 
}


TGraph* HistManager::GetPolarInterpolationTan(double radius, double z_level, TString NameTitle){

	double data = -1 ;
	double theta = 0 ;	
	//create histogram	
	TGraph* graph1d = new TGraph(91);
	
	//interpolate 
	for (int i=0 ; i<91; i++){

		double theta_rad = theta * TMath::Pi() / 180 ;	
		// interpolate
		data = GetInterpolationOnePoint(radius*cos(theta_rad),radius*sin(theta_rad),z_level);
		// fill the histogram	
		graph1d->SetPoint(i, theta , data);
		// increment step	
		theta = theta + 1	;	
	}
	
	graph1d->SetName(NameTitle+(Form("Radius_%.2fmm_Depth_%.2fmm",radius, z_level)));
	graph1d->SetTitle(Form("Tangential Magnetic Field as a Function of Angle at Radius %.2fmm",radius));
	graph1d->GetXaxis()->SetTitle("Angle (degrees)") ;
	graph1d->GetXaxis()->CenterTitle();
	graph1d->GetYaxis()->SetTitle("Tangential Magnetic Field Strength (mT)") ;
	graph1d->GetYaxis()->CenterTitle();
	
	return graph1d ;
 
}


// This function is used only for Btan, Bmag or their difference, it returns a 2DGraph of the mentioned variables
TGraph2D* HistManager::GetPolarInterpolation2D(double z_level, int number_of_interpolation_points, TString Bdirection){

	double data = -1;
	double x = 0., y = 0.; 
	
	//create histogram	
	TGraph2D* graph2d = new TGraph2D(number_of_interpolation_points);
	
	//interpolate 
	for (int j=0 ; j<number_of_interpolation_points ; j++)	{	
		for (int i=0 ; i<number_of_interpolation_points; i++) {
			// interpolate
		 	data = GetInterpolationOnePoint(x,y,z_level);
		 	// fill the histogram	
		 	graph2d->SetPoint(i+(j*number_of_interpolation_points), x , y, data);
		 	// increment step	
		 	x = x+1	;
		}
		y = y+1 ;
		x = 0 ;
	}
	
	graph2d->SetName(Bdirection+(Form("Depth_%.2fmm_Interpol_%d_points",z_level, number_of_interpolation_points)));
	
	if( Bdirection == "Btan" )	{
		graph2d->SetTitle(Form("Tangential Magnetic Field at Depth %.2fmm;x (mm);y (mm);Tangential Magnetic Field Strength (mT)", z_level));
	}
	else if( Bdirection == "Bmag" )	{
		graph2d->SetTitle(Form("Total Magnetic Field at Depth %.2fmm;x (mm);y (mm);Total Magnetic Field Strength (mT)", z_level));
	}
	else 	{
		graph2d->SetTitle(Form("Difference between Total Magnetic Field and Tangential Magnetic Field at Depth %.2fmm;x (mm);y (mm);Total Magnetic Field Strength (mT)", z_level));
	}
	
	return graph2d ;
 
}


// writes a file on the disc that could be used for minimization, By comparing one value to all its corresponding in the simulation after applying an offset. 
int HistManager::GetMagnetisationOffset(double angle,double radius,double z_level,double magnetisation,double expbfield,TString Bdirection){

	double theta = angle * TMath::DegToRad();
	double initialx = radius * TMath::Cos(theta);
	double x = 0, y = 0, z = 0;
	double initialy = radius * TMath::Sin(theta);
	double data = -1;

	ofstream outputfile;
	if(Bdirection == "Bx")	{
		outputfile.open("magcomparisonx.txt", ios::out | ios::app);
		}
	else if(Bdirection == "By")	{
		outputfile.open("magcomparisony.txt", ios::out | ios::app);
			}
	else if(Bdirection == "Bz")	{
		outputfile.open("magcomparisonz.txt", ios::out | ios::app);
			}

	for (double j=-3 ; j<3.1 ; j=j+0.1)	{

		if(Bdirection == "Bx"){
			x = initialx+j ;
			y = initialy ;
			z = z_level ;
			data = expbfield-(GetInterpolationOnePoint(x,y,z)) ;
		}
		else if(Bdirection == "By"){
			x = initialx ;
			y = initialy+j ;
			z = z_level ;
			data = expbfield-(GetInterpolationOnePoint(x,y,z)) ;
		}
		else if(Bdirection == "Bz"){
			x = initialx ;
			y = initialy ;
			z = z_level+j ;
			data = expbfield-(GetInterpolationOnePoint(x,y,z)) ;
		}
		outputfile<<j<<"  "<<magnetisation<<"  "<<data<<"  "<<x<<"  "<<y<<"  "<<z<<endl;
	}
	
	outputfile.close();	

	return 0;
}



// The graph returned represent a plot of the magnetic field at a certain angle and depth after applying a fixed offset (defined by the user) to X and Y.
int HistManager::GetTotalOffsetGraph(double angle,double z_level,double x_offset,double y_offset,TString Bdirection){

	double theta = angle*TMath::DegToRad();
	double r = 0;
	double data = 0;
	TGraph* graph = new TGraph();

	for(int i=0;i<90;i++){
		data = GetInterpolationOnePoint( r*TMath::Cos(theta)+x_offset, r*TMath::Sin(theta)+y_offset, z_level);
		graph->SetPoint(i,r,data);
		r++;
	}

	graph->SetName(Bdirection+Form("stationarypointfinder_x_%.0f_y_%.0f_z_%.2f",x_offset,y_offset,z_level));
	graph->Write();

	return 0;
}



// The histogram returned represent a plot that could be used for minimization to search for the position of the sensor.
TH2D* HistManager::GetXYMagnetisationOffsetGraph(double angle,double radius,double z_level,double exp,TString Bdirection){

	double data = 0;
	double theta = angle*TMath::DegToRad();
	double x = 0, y = 0;
	TH2D* hist = new TH2D("title1","title2",60,-3.05,3.05,60,-3.05,3.05);

	for(double i=-3;i<4;i=i+0.1){
		for(double j=-3;j<4;j=j+0.1){
			x = (radius*TMath::Cos(theta))+i;
			y = (radius*TMath::Sin(theta))+j;
			data = TMath::Abs(exp)-TMath::Abs((GetInterpolationOnePoint(x,y,z_level)));
			Int_t binNumber = hist->FindBin(i,j);		
			hist->SetBinContent(binNumber,TMath::Abs(data));
		}
	}

	hist->SetOption("colz");
	hist->SetStats(0);
	hist->SetName(Bdirection+"xymagnetisationoffsetgraph");
	hist->SetTitle("Comparison of xy offset with magnetic field;x offset (mm);y offset (mm)");

	return hist;
}



TH2D* HistManager::GetMagnitisation2D(double z_level,double xlow,double xhigh,double ylow,double yhigh, int contour, TString Bdirection){

	double data = 0;
	double x = 0, y = 0;
	double step = 1 ; 
	int nbinx = (int) fabs(xhigh-xlow)/step;
	int nbiny = (int) fabs(yhigh-ylow)/step;
	TH2D* hist = new TH2D("title1","title2",nbinx,xlow,xhigh,nbiny,ylow,yhigh);

	for(double i=xlow ; i<=xhigh ; i=i+step){
		for(double j=ylow ; j<=yhigh ; j=j+step){
			data = GetInterpolationOnePoint(i,j,z_level);
			Int_t binNumber = hist->FindBin(i,j);		
			hist->SetBinContent(binNumber,data);
		}
	}

//double* levels = new double[contour];
//for (int i = 0 ; i< contour ; i++)
//levels[i] = 0;
double levels[49] = { 0.0005, 0.001, 0.01, 0.1, //4
                 1 , 2,  3,  4,  5,  6,  7,  8,  9,        //9
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19, //10
            20,30,40,50,60,70,80,90,        //8
            100,120,140,160,180,               //5
            200, 250,                          
            300, 350,
            400, 450,
            500, 550,
            600, 650,                         //10
            700,
            800,
            900} ;                            //3    sum = 49   
	hist->SetContour(49,levels);
	hist->SetContour(contour,levels);
	hist->SetContour(255);
	hist->SetOption("CONT1 COLZ LIST");
	hist->SetStats(0);
	hist->SetName(Bdirection+"MagnetisationContour2DHist");
	hist->SetTitle("Magnitisation 2D ;x (mm);y (mm)");
	hist->GetZaxis()->SetRangeUser(0.0001,700);
    hist->Write() ;
//	return hist;
}



void HistManager::Clear(void){//NEW
 fSim3dHistogram->Reset();
}


double HistManager::GetInterpolationOnePoint(double X , double Y, double Z) {// NEW 	

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


void HistManager::SetOffsets(double X , double Y, double Z){

 fOffsetx = X ;
 fOffsety = Y ;
 fOffsetz = Z ;

}

