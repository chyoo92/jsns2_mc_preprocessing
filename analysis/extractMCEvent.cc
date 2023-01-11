#include "RAT/DSReader.hh"
#include "RAT/DS/RunStore.hh"
//#include "RAT/DS/MC.hh"
//#include "RAT/DS/Root.hh"
//#include "RAT/DS/PMT.hh"
//#include "RAT/DS/Run.hh"
//#include "RAT/DS/EV.hh"

#include "TChain.h"

#include <iostream>
#include <fstream>

using namespace std;

int usage(const std::string& pName)
{
  std::cout << pName << " -  extracts run,event information from the JSNS2-RAT-PAC root file\n";
  std::cout << "\tUsage: " << pName << " <INPUT_RAT_FILE.root>\n";
  return 1;
}

int main(int argc, char* argv[])
{
  if ( argc != 3 ) return usage(argv[0]);
  for ( int i = 1; i < argc; ++i ) {
    if ( std::strcmp(argv[i], "-h") == 0 or std::strcmp(argv[i], "--help") == 0 ) {
      return usage(argv[0]);
    }
  }
  const std::string finName = argv[1];
  const std::string foutName = argv[2];

  // Load Run-level tree
  TChain runTree("runT");
  runTree.Add(finName.c_str());
  RAT::DS::RunStore::SetReadTree(&runTree);

  cout << "Loading Run..." << endl;
  auto run = RAT::DS::RunStore::Get()->GetRun(1);
  if ( run == nullptr ) {
    cout << "Cound not find run. Exiting..." << endl;
    return 2;
  }
  cout << "...OK" << endl;

  // Load the PMT information
  auto pmtInfo = run->GetPMTInfo();
  cout << "Number of PMTs=" << pmtInfo->GetPMTCount() << endl;

  // Prepare output file
  std::ofstream fout(foutName);
  fout << "Run,Event";
  fout << ",EdepTotal,EdepQuench,EdepTarget,EdepAcryl,EdepCatcher,EdepSep,EdepVeto";
  fout << ",VertexX,VertexY,VertexZ,VertexT";
  /*for ( int i = 0, n = pmtInfo->GetPMTCount(); i < n; ++i ) {
    fout << ",PMTQ" << i;
  }*/
  fout << endl;

  // Load the dataset
  auto ds = std::unique_ptr<RAT::DSReader>(new RAT::DSReader(finName.c_str()));
  const int nEvents = ds->GetTotal();
  cout << "Number of Events=" << nEvents << endl;

  for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) {
    cout << "Processing event " << iEvent << "/" << nEvents << "\r";
    const auto event = ds->NextEvent();
    const auto mc = event->GetMC();
    if ( mc == nullptr ) {
      cout << "Null pointer to get MC information. Stop processing..." << endl;
      break;
    }

    if ( mc->GetMCParticleCount() == 0 ) {
      cout << "No MC particle found in this event. Skipping..." << endl;
      continue;
    }

    // Write out event level information
    fout << run->GetID() << "," << mc->GetID()
         << "," << mc->GetMCSummary()->GetTotalScintEdep()
         << "," << mc->GetMCSummary()->GetTotalScintEdepQuenched()
         << "," << mc->GetMCSummary()->GetEnergyLossByVolume("TargetVolume")
         << "," << mc->GetMCSummary()->GetEnergyLossByVolume("AcrylicVessel")
         << "," << mc->GetMCSummary()->GetEnergyLossByVolume("Buffer")
         << "," << mc->GetMCSummary()->GetEnergyLossByVolume("BlackBoard")
         << "," << mc->GetMCSummary()->GetEnergyLossByVolume("GCVeto");

    // Vertex position of the primary particle in the MC
    const auto vertexPos = mc->GetMCParticle(0)->GetPosition();
    fout << "," << vertexPos.X() << "," << vertexPos.Y() << "," << vertexPos.Z();
    fout << "," << mc->GetMCParticle(0)->GetTime();
    fout << endl;
  }
  cout << endl;

  return 0;
}

