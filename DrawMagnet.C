void DrawMagnet(TString type,double offset=0, double height=50) {

if (type=="M") Cluster(offset, height, 55); 
else 
if (type=="L") Cluster(offset,height, 30);
else
if(type=="P") PShield(offset,height); 
else
if(type=="FM") Facade(type,55,3.4);
else 
if (type=="FL") Facade(type,30,5);

}

void Cluster(double offset, double height, double dim /*length of secondary magnetes*/) {

double scale = height/50.;
    //Draw a cross
    TGraph *lens = new TGraph(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    lens->SetPoint(0,17,0*scale+offset);
    lens->SetPoint(1,17,20*scale+offset);
    lens->SetPoint(2,17+17.32,50*scale+offset);
    lens->SetPoint(3,17+75,50*scale+offset); 
    lens->SetPoint(4,17+75,0*scale+offset);
    lens->SetPoint(5,17+75-dim,0*scale+offset); 
    lens->SetPoint(6,17+75-dim,50*scale+offset); 
    lens->SetPoint(7,17+75-dim,0*scale+offset); 
    lens->SetPoint(8,17,0*scale+offset); 
    lens->SetLineColor(kRed); 
    lens->SetLineStyle(kDashed);

    lens->SetDrawOption("l");
    lens->Draw("same");
    //lens->Draw("");
}

void PShield(double offset, double height) {

offset = offset - 4.2 ; // the photon shield is shifted with respect to the lens
double scale = height/50.;
    //Draw a cross
    TGraph *ps = new TGraph(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
    ps->SetPoint(0,5,0*scale+offset);
    ps->SetPoint(1,5,30*scale+offset);
    ps->SetPoint(2,5+23.5/2,30*scale+offset);
    ps->SetPoint(3,26,0*scale+offset); 
    ps->SetPoint(4,5,0*scale+offset);
    ps->SetLineColor(kRed); 
    ps->SetLineStyle(kDashed);

    ps->SetDrawOption("l");
    ps->Draw("same");
    //ps->Draw("");
}


void Facade(TString type, double len, double thic ) {

   TVector3 f[12];
   f[0].SetXYZ(17,-3./2,0);
   f[1].SetXYZ(17+75-len,-3./2,0);
   f[2].SetXYZ(17+75-len,-3./2-thic,0);
   f[3].SetXYZ(17+75,-3./2-thic,0);
   f[4].SetXYZ(17+75,+3./2+thic,0);
   f[5].SetXYZ(17+75-len,+3./2+thic,0);
   f[6].SetXYZ(17+75-len,+3./2,0);
   f[7].SetXYZ(17,+3./2,0);
   f[8].SetXYZ(17,-3./2,0);
   f[9].SetXYZ(17+75,-3./2,0);
   f[10].SetXYZ(17+75,+3./2,0);
   f[11].SetXYZ(17,+3./2,0);


   TGraph *facade1 = new TGraph(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
   for (int i = 0 ; i < 12 ; i++)
   facade1->SetPoint(i,f[i].X(),f[i].Y());
   facade1->SetDrawOption("l");
   facade1->Draw("same");

   for (int i = 0 ; i < 12 ; i++)    f[i].RotateZ(TMath::Pi()/2);

   TGraph *facade2 = new TGraph(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
   for (int i = 0 ; i < 12 ; i++)
   facade2->SetPoint(i,f[i].X(),f[i].Y());
   facade2->SetDrawOption("l");
   facade2->Draw("same");

   for (int i = 0 ; i < 12 ; i++)    f[i].RotateZ(TMath::Pi()/2);

   TGraph *facade3 = new TGraph(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
   for (int i = 0 ; i < 12 ; i++)
   facade3->SetPoint(i,f[i].X(),f[i].Y());
   facade3->SetDrawOption("l");
   facade3->Draw("same");

   for (int i = 0 ; i < 12 ; i++)    f[i].RotateZ(TMath::Pi()/2);

   TGraph *facade4 = new TGraph(); //= new TGraph2DErrors(np, x_array, y_array, bz_array, ex, ey, ez);
   for (int i = 0 ; i < 12 ; i++)
   facade4->SetPoint(i,f[i].X(),f[i].Y());
   facade4->SetDrawOption("l");
   facade4->Draw("same");

}