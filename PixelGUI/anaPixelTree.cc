/*  anaPixelTree.cc
 *  	Author: Caleb Fangmeier
 * 		contact: cfangmeier@hotmail.com
 * 		date: Mon May 23 14:32:31 CEST 2011
 * 
 * 		anaPixelTree was originally created by from a pixelTree n-tuple
 * 		via TTree::MakeClass(). It was later modifed to integrate its
 * 		functionality into PixelGUI.
		AUTOGENERATED BY: ./configure
 */

#ifndef anaPixelTree_cxx
#define anaPixelTree_cxx
#include "anaPixelTree.h"




anaPixelTree::anaPixelTree(){}

anaPixelTree::anaPixelTree(TChain *pixelTree) {
	TChain *chain = new TChain("pixelTree","");
	chain->Add(pixelTree);

	Init(chain);
}

anaPixelTree::~anaPixelTree()
{
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

Int_t anaPixelTree::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

Long64_t anaPixelTree::LoadTree(Long64_t entry)
{
	// Set the environment to read one entry
	if (!fChain) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;
	if (!fChain->InheritsFrom(TChain::Class()))  return centry;
	TChain *chain = (TChain*)fChain;
	if (chain->GetTreeNumber() != fCurrent) {
		fCurrent = chain->GetTreeNumber();
		Notify();
	}
	return centry;
}

void anaPixelTree::Init(TTree *tree)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).
	
	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);
	   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumiblock", &lumiblock, &b_lumiblock);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("bx", &bx, &b_bx);
   fChain->SetBranchAddress("orbit", &orbit, &b_orbit);
   fChain->SetBranchAddress("bz", &bz, &b_bz);
   fChain->SetBranchAddress("tlo", &tlo, &b_tlo);
   fChain->SetBranchAddress("thi", &thi, &b_thi);
   fChain->SetBranchAddress("fed1", &fed1, &b_fed1);
   fChain->SetBranchAddress("fed2", &fed2, &b_fed2);
   fChain->SetBranchAddress("l1t", &l1t, &b_l1t);
   fChain->SetBranchAddress("l1ta", l1ta, &b_l1ta);
   fChain->SetBranchAddress("l1tt", l1tt, &b_l1tt);
   fChain->SetBranchAddress("hlta", hlta, &b_hlta);
   fChain->SetBranchAddress("ttA", ttA, &b_ttA);
   fChain->SetBranchAddress("l1A", l1A, &b_l1A);
   fChain->SetBranchAddress("hlA", hlA, &b_hlA);
   fChain->SetBranchAddress("hlt", &hlt, &b_hlt);
   fChain->SetBranchAddress("PvN", &PvN, &b_PvN);
   fChain->SetBranchAddress("PvX", PvX, &b_PvX);
   fChain->SetBranchAddress("PvY", PvY, &b_PvY);
   fChain->SetBranchAddress("PvZ", PvZ, &b_PvZ);
   fChain->SetBranchAddress("PvXe", PvXe, &b_PvXe);
   fChain->SetBranchAddress("PvYe", PvYe, &b_PvYe);
   fChain->SetBranchAddress("PvZe", PvZe, &b_PvZe);
   fChain->SetBranchAddress("PvChi2", PvChi2, &b_PvChi2);
   fChain->SetBranchAddress("PvNdof", PvNdof, &b_PvNdof);
   fChain->SetBranchAddress("PvIsFake", PvIsFake, &b_PvIsFake);
   fChain->SetBranchAddress("MuN", &MuN, &b_MuN);
   fChain->SetBranchAddress("MuType", &MuType, &b_MuType);
   fChain->SetBranchAddress("MuTkI", &MuTkI, &b_MuTkI);
   fChain->SetBranchAddress("MuPt", &MuPt, &b_MuPt);
   fChain->SetBranchAddress("MuTheta", &MuTheta, &b_MuTheta);
   fChain->SetBranchAddress("MuPhi", &MuPhi, &b_MuPhi);
   fChain->SetBranchAddress("MuT", &MuT, &b_MuT);
   fChain->SetBranchAddress("MuTcorr", &MuTcorr, &b_MuTcorr);
   fChain->SetBranchAddress("MuTerr", &MuTerr, &b_MuTerr);
   fChain->SetBranchAddress("MuTmean", &MuTmean, &b_MuTmean);
   fChain->SetBranchAddress("HfEplus", &HfEplus, &b_HfEplus);
   fChain->SetBranchAddress("HfEminus", &HfEminus, &b_HfEminus);
   fChain->SetBranchAddress("HfTplus", &HfTplus, &b_HfTplus);
   fChain->SetBranchAddress("HfTminus", &HfTminus, &b_HfTminus);
   fChain->SetBranchAddress("TkN", &TkN, &b_TkN);
   fChain->SetBranchAddress("TkQuality", TkQuality, &b_TkQuality);
   fChain->SetBranchAddress("TkCharge", TkCharge, &b_TkCharge);
   fChain->SetBranchAddress("TkChi2", TkChi2, &b_TkChi2);
   fChain->SetBranchAddress("TkNdof", TkNdof, &b_TkNdof);
   fChain->SetBranchAddress("TkPt", TkPt, &b_TkPt);
   fChain->SetBranchAddress("TkTheta", TkTheta, &b_TkTheta);
   fChain->SetBranchAddress("TkEta", TkEta, &b_TkEta);
   fChain->SetBranchAddress("TkPhi", TkPhi, &b_TkPhi);
   fChain->SetBranchAddress("TkD0", TkD0, &b_TkD0);
   fChain->SetBranchAddress("TkDz", TkDz, &b_TkDz);
   fChain->SetBranchAddress("TkVx", TkVx, &b_TkVx);
   fChain->SetBranchAddress("TkVy", TkVy, &b_TkVy);
   fChain->SetBranchAddress("TkVz", TkVz, &b_TkVz);
   fChain->SetBranchAddress("TkAlpha", TkAlpha, &b_TkAlpha);
   fChain->SetBranchAddress("TkBeta", TkBeta, &b_TkBeta);
   fChain->SetBranchAddress("TkResX", TkResX, &b_TkResX);
   fChain->SetBranchAddress("TkResY", TkResY, &b_TkResY);
   fChain->SetBranchAddress("TkResXe", TkResXe, &b_TkResXe);
   fChain->SetBranchAddress("TkResYe", TkResYe, &b_TkResYe);
   fChain->SetBranchAddress("TkRes2X", TkRes2X, &b_TkRes2X);
   fChain->SetBranchAddress("TkRes2Xe", TkRes2Xe, &b_TkRes2Xe);
   fChain->SetBranchAddress("TkClN", TkClN, &b_TkClN);
   fChain->SetBranchAddress("TkClI", TkClI, &b_TkClI);
   fChain->SetBranchAddress("TkType", TkType, &b_TkType);
   fChain->SetBranchAddress("TkMuI", TkMuI, &b_TkMuI);
   fChain->SetBranchAddress("ClN", &ClN, &b_ClN);
   fChain->SetBranchAddress("ClRow", ClRow, &b_ClRow);
   fChain->SetBranchAddress("ClCol", ClCol, &b_ClCol);
   fChain->SetBranchAddress("ClLx", ClLx, &b_ClLx);
   fChain->SetBranchAddress("ClLxE", ClLxE, &b_ClLxE);
   fChain->SetBranchAddress("ClLy", ClLy, &b_ClLy);
   fChain->SetBranchAddress("ClLyE", ClLyE, &b_ClLyE);
   fChain->SetBranchAddress("ClGx", ClGx, &b_ClGx);
   fChain->SetBranchAddress("ClGy", ClGy, &b_ClGy);
   fChain->SetBranchAddress("ClGz", ClGz, &b_ClGz);
   fChain->SetBranchAddress("ClSize", ClSize, &b_ClSize);
   fChain->SetBranchAddress("ClSizeX", ClSizeX, &b_ClSizeX);
   fChain->SetBranchAddress("ClSizeY", ClSizeY, &b_ClSizeY);
   fChain->SetBranchAddress("ClFlipped", ClFlipped, &b_ClFlipped);
   fChain->SetBranchAddress("ClLayer", ClLayer, &b_ClLayer);
   fChain->SetBranchAddress("ClLadder", ClLadder, &b_ClLadder);
   fChain->SetBranchAddress("ClModule", ClModule, &b_ClModule);
   fChain->SetBranchAddress("ClDisk", ClDisk, &b_ClDisk);
   fChain->SetBranchAddress("ClBlade", ClBlade, &b_ClBlade);
   fChain->SetBranchAddress("ClPanel", ClPanel, &b_ClPanel);
   fChain->SetBranchAddress("ClPlaquette", ClPlaquette, &b_ClPlaquette);
   fChain->SetBranchAddress("ClDetId", ClDetId, &b_ClDetId);
   fChain->SetBranchAddress("ClCharge", ClCharge, &b_ClCharge);
   fChain->SetBranchAddress("ClChargeCorr", ClChargeCorr, &b_ClChargeCorr);
   fChain->SetBranchAddress("ClType", ClType, &b_ClType);
   fChain->SetBranchAddress("ClRhLx", ClRhLx, &b_ClRhLx);
   fChain->SetBranchAddress("ClRhLxE", ClRhLxE, &b_ClRhLxE);
   fChain->SetBranchAddress("ClRhLy", ClRhLy, &b_ClRhLy);
   fChain->SetBranchAddress("ClRhLyE", ClRhLyE, &b_ClRhLyE);
   fChain->SetBranchAddress("ClRhGx", ClRhGx, &b_ClRhGx);
   fChain->SetBranchAddress("ClRhGy", ClRhGy, &b_ClRhGy);
   fChain->SetBranchAddress("ClRhGz", ClRhGz, &b_ClRhGz);
   fChain->SetBranchAddress("ClRhProb", ClRhProb, &b_ClRhProb);
   fChain->SetBranchAddress("ClRhProbX", ClRhProbX, &b_ClRhProbX);
   fChain->SetBranchAddress("ClRhProbY", ClRhProbY, &b_ClRhProbY);
   fChain->SetBranchAddress("ClRhQualWord", ClRhQualWord, &b_ClRhQualWord);
   fChain->SetBranchAddress("ClRhqBin", ClRhqBin, &b_ClRhqBin);
   fChain->SetBranchAddress("ClRhSpansTwoROCs", ClRhSpansTwoROCs, &b_ClRhSpansTwoROCs);
   fChain->SetBranchAddress("ClRhIsOnEdge", ClRhIsOnEdge, &b_ClRhIsOnEdge);
   fChain->SetBranchAddress("ClRhHasBadPixels", ClRhHasBadPixels, &b_ClRhHasBadPixels);
   fChain->SetBranchAddress("ClTkN", ClTkN, &b_ClTkN);
   fChain->SetBranchAddress("ClTkI", ClTkI, &b_ClTkI);
   fChain->SetBranchAddress("ClDgN", ClDgN, &b_ClDgN);
   fChain->SetBranchAddress("ClDgI", ClDgI, &b_ClDgI);
   fChain->SetBranchAddress("DgN", &DgN, &b_DgN);
   fChain->SetBranchAddress("DgRow", DgRow, &b_DgRow);
   fChain->SetBranchAddress("DgCol", DgCol, &b_DgCol);
   fChain->SetBranchAddress("DgDetid", DgDetid, &b_DgDetid);
   fChain->SetBranchAddress("DgRoc", DgRoc, &b_DgRoc);
   fChain->SetBranchAddress("DgRocR", DgRocR, &b_DgRocR);
   fChain->SetBranchAddress("DgRocC", DgRocC, &b_DgRocC);
   fChain->SetBranchAddress("DgLx", DgLx, &b_DgLx);
   fChain->SetBranchAddress("DgLy", DgLy, &b_DgLy);
   fChain->SetBranchAddress("DgGx", DgGx, &b_DgGx);
   fChain->SetBranchAddress("DgGy", DgGy, &b_DgGy);
   fChain->SetBranchAddress("DgGz", DgGz, &b_DgGz);
   fChain->SetBranchAddress("DgAdc", DgAdc, &b_DgAdc);
   fChain->SetBranchAddress("DgCharge", DgCharge, &b_DgCharge);
   fChain->SetBranchAddress("DgClI", DgClI, &b_DgClI);
	Notify();
}

Bool_t anaPixelTree::Notify()
{
	// The Notify() function is called when a new file is opened. This
	// can be either for a new TTree in a TChain or when when a new TTree
	// is started when using PROOF. It is normally not necessary to make changes
	// to the generated code, but the routine can be extended by the
	// user if needed. The return value is currently not used.
	
	return kTRUE;
}

void anaPixelTree::Show(Long64_t entry)
{
	// Print contents of entry.
	// If entry is not specified, print current entry
	if (!fChain) return;
	fChain->Show(entry);
}

Int_t anaPixelTree::Cut(Long64_t entry)
{
	// This function may be called from Loop.
	// returns  1 if entry is accepted.
	// returns -1 otherwise.
	return 1;
}
#endif