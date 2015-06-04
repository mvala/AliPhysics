#ifndef AliAnalysisTaskEMCALTimeCalib_h
#define AliAnalysisTaskEMCALTimeCalib_h

//_________________________________________________________________________
/// \class AliAnalysisTaskEMCALTimeCalib
/// \brief Task to work on Time Calibration for EMCal/DCal.
///
/// Derived from "Exercice with a task to work on T0 from TOF or T0" by Hugues Delagrange (SUBATECH)"
/// History:
/// TimeTaskMB2_v2     wrt TimeTaskMB add histos time_vs_energy
/// TimeTaskMB2_v2b    (memory optimization.. t_vs_E splitted in hi and low res  part)
/// TimeTaskMB2_v2m    memory leak patch: sterilized ComputeT0TOF.. temporarly it is not called (to be understood and fixed
/// TimeTaskMB2_v3m   fix "dÃ¨calage" in the binning 
/// TimeTaskMB2_v4a   Implement single cell histos+ correction from average computed on run 128503 
/// TimeTaskMB2_v4b   Implement BC offsetCorrectio + histo to look at partial RCU
/// TimeTaskMB2_v4d   histoaming convention ""=raw  "Corr"= cell/cell correction "BCCorr"= cell/cell+BC 
/// TimeTaskMB2_v4e   check of BCnb vs time vs SM new histos
/// TimeTaskMB2_v4f   new histos for time for each BC 
/// TimeTaskMB2_v5   new histos for time for each BC calc average for each BC -> new ref  
///
/// Extended to DCal, added setters and getters
///
/// \author Hugues Delagrange+, SUBATECH
/// \author Marie Germain <marie.germain@subatech.in2p3.fr>, SUBATECH
/// \author Adam Matyja <adam.tomasz.matyja@ifj.edu.pl>, INP PAN Cracow
/// \date Jun 3, 2015
//_________________________________________________________________________

class TH1F;
class TH1D;
class TH2D;

class AliESDEvent;
class AliESDCaloCluster;
class AliTOFT0maker;

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskEMCALTimeCalib : public AliAnalysisTaskSE 
{
 public:

  enum { kNSM = 20, kNBCmask = 4 };

   AliAnalysisTaskEMCALTimeCalib() : AliAnalysisTaskSE(),
    fESD(0),
    fRunNumber(-1),
    fTOFmaker(0),
    fOutputList(0),
    fgeom(0),
    fMinClusterEnergy(0),
    fMaxClusterEnergy(0),
    fMinNcells(0),
    fMaxNcells(0),
    fMinLambda0(0),
    fMaxLambda0(0),
    fMaxRtrack(0),
    fMinCellEnergy(0),
    fReferenceFileName(),
    fhcalcEvtTime(0),
    fhEventType(0),
    fhTOFT0vsEventNumber(0),
    fhTcellvsTOFT0(0),
    fhTcellvsTOFT0HD(0),
    fhTcellvsSM(0),
    fhEneVsAbsIdHG(0),
    fhEneVsAbsIdLG(0),
    fhTimeVsBC(0),
    fhTimeSumSq(),
    fhTimeEnt(),
    fhTimeSum(),
    fhAllAverageBC(),
    fhTimeDsup(),
    fhTimeDsupBC(),
    fhRawTimeVsIdBC(),
    fhRawTimeSumBC(),
    fhRawTimeEntriesBC(),
    fhRawTimeSumSqBC(),
    fhRawTimeVsIdLGBC(),
    fhRawTimeSumLGBC(),
    fhRawTimeEntriesLGBC(),
    fhRawTimeSumSqLGBC()
    { ; }
  
  AliAnalysisTaskEMCALTimeCalib(const char *name);
  virtual ~AliAnalysisTaskEMCALTimeCalib() { ; }
  
  virtual void   LocalInit();
  virtual Bool_t Notify();
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
  // Getters and setters
  Double_t GetMinClusterEnergy()  { return fMinClusterEnergy  ; }
  Double_t GetMaxClusterEnergy()  { return fMaxClusterEnergy  ; }
  Int_t    GetMinNcells()         { return fMinNcells         ; }
  Int_t    GetMaxNcells()         { return fMaxNcells         ; }
  Double_t GetMinLambda0()        { return fMinLambda0        ; }
  Double_t GetMaxLambda0()        { return fMaxLambda0        ; }
  Double_t GetMaxRtrack()         { return fMaxRtrack         ; }
  Double_t GetMinCellEnergy()     { return fMinCellEnergy     ; }
  TString  GetReferenceFileName() { return fReferenceFileName ; }

  void SetMinClusterEnergy (Double_t v) { fMinClusterEnergy = v ; }
  void SetMaxClusterEnergy (Double_t v) { fMaxClusterEnergy = v ; }
  void SetMinNcells        (Int_t    v) { fMinNcells        = v ; }  
  void SetMaxNcells        (Int_t    v) { fMaxNcells        = v ; }   
  void SetMinLambda0       (Double_t v) { fMinLambda0       = v ; }	   
  void SetMaxLambda0       (Double_t v) { fMaxLambda0       = v ; }	   
  void SetMaxRtrack        (Double_t v) { fMaxRtrack        = v ; }	   
  void SetMinCellEnergy    (Double_t v) { fMinCellEnergy    = v ; }	   
  void SetReferenceFileName(TString  v) { fReferenceFileName= v ; }

  void SetDefaultCuts();

 private:
  
  virtual void PrepareTOFT0maker();
  
  Bool_t SetEMCalGeometry();
  
  Bool_t AcceptCluster(AliESDCaloCluster* clus);
  
  Bool_t CheckCellRCU(Int_t nSupMod,Int_t icol,Int_t irow);
  
  void   ProduceCalibConsts(TString inputFile="time186319testWOL0.root",TString outputFile="Reference.root");

  // data members

  /// pointer to ESD object
  AliESDEvent   *fESD ;       //->
 
  Int_t          fRunNumber ; //!<! run number
  
  // Use the RemakePID method in the task::UserExec                  //
  // Double_t* calcolot0;                                            //
  // calcolot0=fTOFmaker->RemakePID(fESD);                           //
  // calcolot0[0] = calculated event time                            //

  /// pointer to get T0 from TOF
  AliTOFT0maker *fTOFmaker;   //->
  
  /// pointer to output list
  TList         *fOutputList; //->
  
  /// pointer to EMCal geometry
  AliEMCALGeometry *fgeom;    //->
   
  // setable variables for cuts
  
  Double_t       fMinClusterEnergy ;    ///< minimum cluster energy
  Double_t       fMaxClusterEnergy ;    ///< maximum cluster energy
  
  Int_t          fMinNcells ;           ///< minimum number of cells in cluster
  Int_t          fMaxNcells ;           ///< maximum number of cells in cluster
  
  Double_t       fMinLambda0 ;          ///< minimum cluster lambda0
  Double_t       fMaxLambda0 ;          ///< maximum cluster lambda0
  
  Double_t       fMaxRtrack ;           ///< maximum cluster track distance
  
  Double_t       fMinCellEnergy;        ///< minimum cell energy

  TString        fReferenceFileName ;   //!<! name of reference file

  // histograms
  TH1F          *fhcalcEvtTime;         //!<! spectrum calcolot0[0]
  
  TH1F          *fhEventType;           //!<! spectrum calcolot0[0]
  
  TH1F          *fhTOFT0vsEventNumber;  //!<! TOF T0 evolution as a function of time 
  
  TH2F          *fhTcellvsTOFT0;        //!<! time of cell vs TOF T0 time
  
  TH2F          *fhTcellvsTOFT0HD;      //!<! time of cell vs TOF T0 time for higher energy threshold
  
  TH2F          *fhTcellvsSM;           //!<! cell time vs SM
  
//  TH1F          *fhTmp;                 //!<! id of cells with strange time
//  TH1F          *fhTmp2;                //!<! id of cells with strange time
//  TH2F          *fhEtavsPhi;            //!<! eta vs phi of cells with strange time
  
  TH2F          *fhEneVsAbsIdHG;        //!<! energy of each cell for high gain cells with strange time
  
  TH2F          *fhEneVsAbsIdLG;        //!<! energy of each cell for low gain cells with strange time
  
//  TH2F          *fhEneVsAbsIdNormHG;    //!<! energy of each cell for high gain cells with normal time
  
//  TH2F          *fhEneVsAbsIdNormLG;    //!<! energy of each cell for low gain cells with normal time
  
  TH2F          *fhTimeVsBC;            //!<!cell time vs BC

  // histos for storing the time values per cell for further averaging;
  TH1F		*fhTimeSumSq[kNBCmask]; //!<!  4
  TH1F		*fhTimeEnt  [kNBCmask]; //!<!  4
  TH1F		*fhTimeSum  [kNBCmask]; //!<!  4

  // control histos
  TH1D		*fhAllAverageBC     [kNBCmask]; //!<! 4 BCmask
  TH2F		*fhTimeDsup  [kNSM];            //!<! 20 SM
  TH2F		*fhTimeDsupBC[kNSM][kNBCmask];  //!<! 20 x 4

  //main histos
  TH2F          *fhRawTimeVsIdBC     [kNBCmask]; //!<! 4 BCmask HG
  TH1F          *fhRawTimeSumBC      [kNBCmask]; //!<! 4 BCmask HG
  TH1F          *fhRawTimeEntriesBC  [kNBCmask]; //!<! 4 BCmask HG
  TH1F          *fhRawTimeSumSqBC    [kNBCmask]; //!<! 4 BCmask HG
  TH2F          *fhRawTimeVsIdLGBC   [kNBCmask]; //!<! 4 BCmask LG
  TH1F          *fhRawTimeSumLGBC    [kNBCmask]; //!<! 4 BCmask LG
  TH1F          *fhRawTimeEntriesLGBC[kNBCmask]; //!<! 4 BCmask LG
  TH1F          *fhRawTimeSumSqLGBC  [kNBCmask]; //!<! 4 BCmask LG

  /// Copy constructor not implemented.
  AliAnalysisTaskEMCALTimeCalib(           const AliAnalysisTaskEMCALTimeCalib&);
  
  /// Assignment operator not implemented.
  AliAnalysisTaskEMCALTimeCalib& operator=(const AliAnalysisTaskEMCALTimeCalib&); 
  
/// \cond CLASSIMP
  ClassDef(AliAnalysisTaskEMCALTimeCalib, 1) ;
/// \endcond
};

#endif
