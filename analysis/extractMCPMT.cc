// #include "/hep-sw/jade/include/Event.h"


#include "RAT/DSReader.hh"
#include "RAT/DS/RunStore.hh"
#include "RAT/DS/MC.hh"
#include "RAT/DS/Root.hh"
#include "RAT/DS/PMT.hh"
#include "RAT/DS/Run.hh"
#include "RAT/DS/EV.hh"

#include <iostream>
#include <fstream>
#include <limits>




using namespace std;

int usage(const std::string& pName)
{
  std::cout << pName << " -  extracts MC PMT information from the JSNS2-RAT-PAC root file\n";
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
  fout << "Run,Event,nPE";
  for ( int i = 0, n = pmtInfo->GetPMTCount(); i < n; ++i ) {
    fout << ",Charge" << (i+1);
  }
  for ( int i = 0, n = pmtInfo->GetPMTCount(); i < n; ++i ) {
    fout << ",Time" << (i+1);
  }
  fout << endl;

  // Load the dataset
  auto ds = std::unique_ptr<RAT::DSReader>(new RAT::DSReader(finName.c_str()));
  const int nEvents = ds->GetTotal();
  cout << "Number of Events=" << nEvents << endl;
  

  std::map<int, float> pmtId2recoCharge;
  for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) {
    cout << "Processing event " << iEvent << "/" << nEvents << "\r";
    const auto event = ds->NextEvent();
      
//     for ( int i = 0, n = event->GetNumPMTs(); i < n;++i) {
// 	  const float pmtCharge = event->GetPMTCharge(i);
// 	  const int pmtID = event->GetPMTID(i);
// 	  pmtId2recoCharge[pmtID] = pmtCharge;
    
//     }
    
//     for ( int i = 0, n = event->GetNumPMTs(); i < n; ++i ) {
//       auto itr = pmtId2recoCharge.find(i);
//       const float pmtCharge = (itr == pmtId2recoCharge.end() ? 0 : itr->second);
//       fout << "," << pmtCharge;
//     }
		    

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
    fout << run->GetID() << "," << mc->GetID();
    fout << "," << mc->GetNumPE();

    // Get MCPMT information
    std::map<int, float> pmtId2Charge;
    std::map<int, float> pmtId2Time;
    for ( int i = 0, n = mc->GetMCPMTCount(); i < n; ++i ) {
      const auto pmt = mc->GetMCPMT(i);
      const int pmtId = pmt->GetID();
      const float pmtCharge = pmt->GetCharge();
      // Calculate average time of the PMT, weighted by the charge (nPE)
      double sumW = 0, sumWT = 0;
      for ( int j = 0, m = pmt->GetMCPhotonCount(); j < m; ++j ) {
        const auto photon = pmt->GetMCPhoton(j);
        const double w = photon->GetCharge();
        sumWT += w*photon->GetHitTime();
        sumW += w;
      }
      const double pmtTime = (sumW == 0 ? 0 : sumWT/sumW);
      
      pmtId2Charge[pmtId] = pmtCharge;
      pmtId2Time[pmtId] = static_cast<float>(pmtTime);
    }
    for ( int i = 0, n = pmtInfo->GetPMTCount(); i < n; ++i ) {
      auto itr = pmtId2Charge.find(i);
      const float pmtCharge = (itr == pmtId2Charge.end() ? 0 : itr->second);
      fout << "," << pmtCharge;
    }
    for ( int i = 0, n = pmtInfo->GetPMTCount(); i < n; ++i ) {
      auto itr = pmtId2Time.find(i);
      const float pmtTime = (itr == pmtId2Time.end() ? 0 : itr->second);
      fout << "," << pmtTime;
    }

    // Finish the line
    fout << endl;
  }
  cout << endl;

  return 0;
}

