
//Root
#include "Riostream.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h" 
#include "TH2D.h"
#include "TH3D.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TString.h"
#include "TTree.h"
#include "TVector3.h"
#include "TView.h"
#include "TPolyMarker3D.h"

#include "Math/InterpolationTypes.h"
#include "Math/Interpolator.h"
using namespace ROOT::Math;

//c++
#include "string"
#include "vector"
#include "iostream"
#include "fstream"
#include "sstream"
#include "math.h"
#include "map"
#include "utility"
#include "algorithm"
#include "stdexcept"
#include "exception"
#include "stdio.h"
#include "stdlib.h"
using namespace std;
       
       
             
////////////////////////////
// Other definitions
//////////////////////////////
//define
//#define ZOFFSET -124.20  // Zsim = - 124.2 + Zdata  Right value for now is - 124.2
                        // -124 value is just for testing
 
#define SENSORXOFFSETX  +2.1
#define SENSORXOFFSETY  +0.0
#define SENSORXOFFSETZ  -1.8
  
#define SENSORYOFFSETX  +0.0
#define SENSORYOFFSETY  +2.1
#define SENSORYOFFSETZ  -1.8
 
#define SENSORZOFFSETX  +0.0
#define SENSORZOFFSETY  +0.0
#define SENSORZOFFSETZ  -1.8
 

 
