#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TCanvas.h"

// This example is a variant of hsimple.C but using a TTree instead
// of a TNtuple. It shows :
//   -how to fill a Tree with a few simple variables.
//   -how to read this Tree
//   -how to browse and analyze the Tree via the TBrowser and TTreeViewer
// This example can be run in many different ways:
//  way1:  .x tree1.C    using the CINT interpreter
//  way2:  .x tree1.C++  using the automatic compiler interface
//  way3:  .L tree1.C  or .L tree1.C++
//          tree1()
// One can also run the write and read parts in two separate sessions.
// For example following one of the sessions above, one can start the session:
//   .L tree1.C
//   tree1r();
//
//  Author: Rene Brun

void tree1r()
{
   //read the Tree generated by tree1w and fill two histograms

   //note that we use "new" to create the TFile and TTree objects !
   //because we want to keep these objects alive when we leave this function.
   TFile *f = new TFile("HMS_optics_fit_output.root");
   TTree *tout = (TTree*)f->Get("tout");
   Double_t ytar, ytartrue, yptar, yptartrue, xptar, xptartrue, zfoil;
   Int_t ifoil;
   tout->SetBranchAddress("ytar",&ytar);
   tout->SetBranchAddress("yptar",&yptar);
   tout->SetBranchAddress("xptar",&xptar);
   tout->SetBranchAddress("ytartrue",&ytartrue);
   tout->SetBranchAddress("yptartrue",&yptartrue);
   tout->SetBranchAddress("xptartrue",&xptartrue);
   tout->SetBranchAddress("zfoil",&zfoil);
   tout->SetBranchAddress("ifoil",&ifoil);

   //create 2D histograms
   TH2D *xpdiffxp = new TH2D("xpdiffxp","xpdiff vs xp",1000,-80,80,1000,-10,10);
   TH2D *ypdiffyp = new TH2D("ypdiffyp","ypdiff vs yp",1000,-40,40,1000,-4,4);
   TH2D *ydiffy = new TH2D("ydiffy","ydiff vs y",1000,-40,40,1000,-5,5);

   //read all entries and fill the histograms
   Long64_t nentries = tout->GetEntries();
   for (Long64_t i=0;i<nentries;i++) {
     tout->GetEntry(i);
     if (fabs(zfoil+6.55)<=1.0){
     	xpdiffxp->Fill(xptar*1000,(xptar-xptartrue)*1000);
     	ypdiffyp->Fill(yptar*1000,(yptar-yptartrue)*1000);
     }
     ydiffy->Fill(ytar*10,(ytar-ytartrue)*10);
  }

  TCanvas *c1 = new TCanvas("c1","c1",100,100,700,700);
  c1->Update();
  c1->Divide(1,3);
  c1->cd(1);
  xpdiffxp->Draw();
  c1->cd(2);
  ypdiffyp->Draw();
  c1->cd(3);
  ydiffy->Draw();
  c1->Modified();
  c1->Update();

  //we do not close the file. We want to keep the generated histograms
  //we open a browser and the TreeViewer
  if (gROOT->IsBatch()) return;
  //new TBrowser();
  //tout->StartViewer();
  // in the browser, click on "ROOT Files", then on "tree1.root".
  //     you can click on the histogram icons in the right panel to draw them.
  // in the TreeViewer, follow the instructions in the Help button.
}

void tree1() {
   tree1r();
}
