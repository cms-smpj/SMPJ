#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include "TTree.h"
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <regex>
#include <TLorentzVector.h>

using std::cout;
using std::endl;
using std::vector;
using std::set;
using std::map;

class MCJettify : public edm::EDAnalyzer
{
public:
  using LorentzVector = reco::Particle::LorentzVector;

  MCJettify(edm::ParameterSet const& cfg) :
    mGenJetsName (consumes<reco::GenJetCollection>(edm::InputTag("ak4GenJetsNoNu"))),
    mEventInfo(consumes<GenEventInfoProduct>(edm::InputTag("generator"))),
    mGenParts(consumes<reco::GenParticleCollection>(edm::InputTag("genParticles")))
  {
    mTree = fs->make<TTree>("GenJets","GenJets");
    mTree->Branch("weight", &mWeight, "weight/F");
    mTree->Branch("Pt", &mPt, "Pt/F");
    mTree->Branch("Eta", &mEta, "Eta/F");
    mTree->Branch("Phi", &mPhi, "Phi/F");
    mTree->Branch("E", &mE, "E/F");
    mTree->Branch("JIdx", &mJid, "JIdx/I");
    mTree->Branch("Status", &mStat, "Status/I");
    mTree->Branch("EFracs", mEFracs, "EFracs[19]/F");
  }
  virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
  virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
  void beginJob() {
    //mCases[0] = 0.;
    //mCases[1] = 0.;
    //mCases[2] = 0.;
    //mCases[3] = 0.;
    //mCases[4] = 0.;
  }
  void endJob() {
    //double totE = 0.;
    //for (auto &jaa : mPhot) totE += jaa.second;
    //cout << "Ene: " << totE << endl;
    //for (auto &jaa : mPhot) {
    //  const double pers = 100.*jaa.second/totE;
    //  if (pers > 0.01) cout << " > " << jaa.first << " " << pers << endl;
    //}
    //double tots = mCases[0] + mCases[1] + mCases[2] + mCases[3];
    //if (tots > 0.) cout << 100.*mCases[0]/tots << " " << 100.*mCases[1]/tots << " " << 100.*mCases[2]/tots << " " << 100.*mCases[3]/tots << endl;
    cout << "Happily finished processing! :)" << endl;
  }
  void beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& set) {}
  ~MCJettify() {}
private:
  // Configurable parameters

  // GEN //
  edm::EDGetTokenT<reco::GenJetCollection>     mGenJetsName;
  edm::EDGetTokenT<GenEventInfoProduct>        mEventInfo;
  edm::EDGetTokenT<reco::GenParticleCollection>mGenParts;

  edm::Service<TFileService>               fs;
  TTree                                   *mTree;
  //map<int,double> mPhot;
    
  float mWeight;
  float mPt;
  float mEta;
  float mPhi;
  float mE;
  float mEfEr;
  float mEfE;
  float mEfHr;
  float mEfH;
  float mEFracs[19];
  int mJid;
  int mStat;
  double mFracs[19];
  //double mCases[5];
};
//////////////////////////////////////////////////////////////////////////////////////////
void MCJettify::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup) {}
//////////////////////////////////////////////////////////////////////////////////////////
void MCJettify::analyze(edm::Event const& event, edm::EventSetup const& iSetup) {
  //---------------- GenJets ------------------------------------------
  auto genJets = event.getHandle(mGenJetsName);
  auto hEventInfo = event.getHandle(mEventInfo);
  auto hParts = event.getHandle(mGenParts);
  int pid1 = -10, pid2 = -10;
  int oid1 = -10, oid2 = -10;
  for (auto iprt = hParts->begin(); iprt!=hParts->end(); ++iprt) {
    int pid = iprt->pdgId();
    if (pid == 2212) continue;
    int stat = iprt->status(); 
    // Only works with pythia8!
    if (stat == 21) {
      if (pid1 < -9) pid1 = pid;
      else if (pid2 < -9) pid2 = pid; 
    } else if (stat < 30) {
      if (stat == 23) {
        if (oid1 < -9) oid1 = pid;
        else if (oid2 < -9) oid2 = pid; 
      }
    } else if (pid1 > -10 && pid2 > -10) {
      break;
    }
  }
  if (pid1 < 0) {
    int tmp = pid1;
    pid1 = pid2;
    pid2 = tmp;
  }
  if (oid1 < 0) {
    int tmp = oid1;
    oid1 = oid2;
    oid2 = tmp;
  }
  if (pid1 == 21) {
    pid1 = pid2;
    pid2 = 21;
  }
  if (oid1 == 21) {
    oid1 = oid2;
    oid2 = 21;
  }
  if (pid1 == 21) {
    if (oid1 != 21 || oid2 != 21) {
      mStat = 10 + abs(oid1) + 5 * (abs(oid2) - 1);
      //mCases[1] += 1.;
    } else {
      mStat = 0;
      //mCases[0] += 1.;
    }
  } else if (pid2 == 21) {
    if (pid1 > 0) {
      mStat = pid1;
    } else {
      mStat = 5 + abs(pid1);
    }
    //mCases[2] += 1.;
  } else {
    if (pid1 == -pid2 && (pid1 != oid1 || pid2 != oid2)) {
      mStat = 60 + abs(pid1) + 5 * (abs(pid2) - 1);
      //mCases[4] += 1.;
    } else {
      mStat = 35 + abs(pid1) + 5 * (abs(pid2) - 1);
      //mCases[3] += 1.;
    }
  }
  
  mWeight = hEventInfo->weight();

  mJid = 0;
  for (auto igen = genJets->begin(); igen!=genJets->end(); ++igen) {
    double cEta = igen->eta();
    if (fabs(cEta) > 5.2) continue;
    const double cPt = igen->pt();
    if (cPt < 15.) continue;

    mPt  = cPt;
    mEta = cEta;
    mPhi = igen->phi();
    mE   = igen->energy();
    for (size_t i = 0; i < 19; ++i) mFracs[i] = 0.;

    for (unsigned idx = 0; idx < igen->numberOfDaughters(); ++idx) {
      auto *dgt = dynamic_cast<const reco::GenParticle*>(igen->daughter(idx));
      int pid = dgt->pdgId();
      double ene = dgt->energy();
      if (pid == 211) { // pi+
        mFracs[0] += ene; 
      } else if (pid == -211) { // pi-
        mFracs[1] += ene; 
      } else if (pid == 22) { // gamma
        auto moth = dgt->motherRef();
        if (moth.isNonnull() && moth.isAvailable()) {
          int mid = moth->pdgId();
          while (mid == 22) {
            moth = moth->motherRef();
            mid = moth->pdgId();
          }
          int amid = abs(mid);
          //const bool prot = mid == 2212 && moth->energy() > 6499.;
          //const bool noid = mid == 21 || amid < 10 || prot;
          //if (cPt > 2000.) {
          ////if (cPt < 2000. && cPt > 1200.) {
          ////if (cPt < 1200. && cPt > 600.) {
          ////if (cPt < 600. && cPt > 200.) {
          ////if (cPt < 200. && cPt > 50.) {
          ////if (cPt < 50. && cPt > 30.) {
          ////if (cPt < 30.) {
          //  int recog = prot ? 0 : (moth->charge() == 0 ? (amid < 10 ? 2 : amid) : 1);
          //  if (mPhot.find(recog) == mPhot.end()) {
          //    mPhot[recog] = ene;
          //  } else {
          //    mPhot[recog] += ene;
          //  }
          //}
          if (amid == 111) { // pi0
            mFracs[2] += ene;
          } else if (amid == 221) { // eta
            mFracs[3] += ene;
          } else if (amid == 223) { // omega
            mFracs[4] += ene;
          } else {
            mFracs[13] += ene;
          }
        } else {
          cout << "Not found!" << endl;
          mFracs[13] += ene;
        }
      } else if (pid == 321) { // K+
        mFracs[5] += ene; 
      } else if (pid == -321) { // K-
        mFracs[6] += ene; 
      } else if (abs(pid) == 310) { // KS
        mFracs[7] += ene; 
      } else if (abs(pid) == 130) { // KL
        mFracs[8] += ene; 
      } else if (pid == 2212) { // p+
        mFracs[9] += ene; 
      } else if (pid == -2212) { // p-
        mFracs[10] += ene; 
      } else if (pid == 2112) { // n
        mFracs[11] += ene; 
      } else if (pid == -2112) { // nbar
        mFracs[12] += ene; 
      } else if (abs(pid) == 3122 || abs(pid) == 3322) { // Lambda0/Xi0
        mFracs[14] += ene; 
      } else if (abs(pid) == 3222 || abs(pid) == 3112) { // Sigma+-
        mFracs[15] += ene; 
      } else if (abs(pid) == 3312 || abs(pid) == 3334) { // Xi-/Omega-
        mFracs[16] += ene; 
      } else if (abs(pid) == 11 || abs(pid) == 13) { // e/mu
        mFracs[17] += ene; 
      } else { // Others: should be empty, but check
        mFracs[18] += ene; 
        cout << pid << " " << dgt->energy() << endl;
      }
    }
    for (size_t i = 0; i < 19; ++i) mEFracs[i] = mFracs[i];

    mTree->Fill();
    if (++mJid > 10) break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(MCJettify);
