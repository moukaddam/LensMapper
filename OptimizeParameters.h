#ifndef _OptimizeParameters_CLASS
#define _OptimizeParameters_CLASS

#include "Defines.h"
#include "HistManager.h"
#include "ExperimentalPoint.h"

// This class is used to optimize the magnetization values and the offset of the probe 
// using a chi2 fit

#include <map>
#include <string>

// ---------------------- Helper Class
// ---------------------  simple class to concatenate all the experimental information
class Double3DVector
{

  public: 
    Double3DVector(TVector3 position, TVector3 field)
    {
      this->fPosition = position;
      this->fField = field;
    }
    
    TVector3 fPosition;
    TVector3 fField;
    
    void ClearDouble3DVector(void)
    {
      this->fPosition.SetXYZ(0,0,0);
      this->fField.SetXYZ(0,0,0);
    }

};

// ---------------------- Helper Class
// ---------------------  Key of pair values for the map holding the magnetization
class Key
{
  public: 
    Key(int magInner, int magOuter)
    {
      this->magInner = magInner;
      this->magOuter = magOuter;
    }
    
    int magInner;
    int magOuter;
    
    bool operator<(const Key& k) const
    {
      
      if(this->magInner - k.magInner== 0) // if inner mag are equal 
      {
        return this->magOuter < k.magOuter; // return the result of outer magnet
      }
      else 
      return this->magInner < k.magInner;  // else return the result of inner magnet
    }
};


// ---------------------- The main class 
class OptimizeParameters
{
 public:

//Champ statiques

TH3D *fHistBx ; 
TH3D *fHistBy ;  
TH3D *fHistBz ;
  
HistManager* fHistBxManager;
HistManager* fHistByManager; 
HistManager* fHistBzManager; 

TTree* fTree;
TFile* fRootFile;
TGraph2D * f2DGraph;

ExperimentalPoint* ExpPoint ;
vector < Double3DVector > fPositionAndField  ;


// best estimates
bool   fFirstValue ;
double fBestChi2X;
double fBestChi2Y;
double fBestChi2Z;
double fBestChi2XYZ;

int fBestMagInner[4]; // 0 1 2 3 --> according to Bx By Bz Bmag
int fBestMagOuter[4];
double fBestOffsetX[4];
double fBestOffsetY[4];
double fBestOffsetZ[4];


// variables stored in the tree
int fMagInner; 
int fMagOuter;

double fChi2X;
double fChi2Y;
double fChi2Z;
double fChi2XYZ;

double fOffsetX;
double fOffsetY;
double fOffsetZ;

double fBsimX;
double fBsimY;
double fBsimZ;

double fBexpX;
double fBexpY;
double fBexpZ;


std::map<Key,string> fFieldPathMap;

//Methodes
 OptimizeParameters(TString FileNameExpData, TString FileNameSimDataPath);
~OptimizeParameters(void);

void Init(void);
void Clear(void);

void LinkTree(void);

void ReadFieldTable(string);
void ReadExperimentalData(TString);

void Treat(void);
void TreatSensorFixed(float fixedXx, float fixedYx, float fixedZx, float fixedXy, float fixedYy, float fixedZy, float fixedXz, float fixedYz, float fixedZz);
void Minimize(void);

TVector3 GetSensorOffset(int index);
TVector3 GetSensorXOffset(void);
TVector3 GetSensorYOffset(void);
TVector3 GetSensorZOffset(void);

void ShowBestValues(void); 

void WriteTree(void);
void WriteGraph(void);
 };

#endif
