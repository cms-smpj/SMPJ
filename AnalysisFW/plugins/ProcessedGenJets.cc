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
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "PhysicsTools/PatUtils/interface/bJetSelector.h"
#include "PhysicsTools/PatExamples/interface/BTagPerformance.h"
#include "PhysicsTools/PatExamples/interface/PatBTagCommonHistos.h"

#include "CLHEP/Random/RandomEngine.h"

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
#include <cassert>
#include <regex>
#include <TLorentzVector.h>

using std::cout;
using std::endl;
using std::vector;

class GenJettify : public edm::EDAnalyzer
{
public:
  using LorentzVector = reco::Particle::LorentzVector;

  GenJettify(edm::ParameterSet const& cfg) :
    mGenJetsName (consumes<reco::GenJetCollection>(edm::InputTag("slimmedGenJets"))),
    mEventInfo(consumes<GenEventInfoProduct>(edm::InputTag("generator")))
  {
    mTree = fs->make<TTree>("GenJets","GenJets");
    mTree->Branch("weight", &mWeight, "weight/F");
    mTree->Branch("Pt", &mPt, "Pt/F");
    mTree->Branch("Eta", &mEta, "Eta/F");
    mTree->Branch("Phi", &mPhi, "Phi/F");
    mTree->Branch("E", &mE, "E/F");
    mTree->Branch("JIdx", &mJid, "JIdx/I");
    mTree->Branch("EFracs", mEFracs, "EFracs[16]/F");
  }
  virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
  virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
  void beginJob() {}
  void endJob() { cout << "Happily finished processing! :)" << endl; }
  void beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& set) {}
  ~GenJettify() {}
private:
  // Configurable parameters

  // GEN //
  edm::EDGetTokenT<reco::GenJetCollection> mGenJetsName;
  edm::EDGetTokenT<GenEventInfoProduct>    mEventInfo;

  edm::Service<TFileService>               fs;
  TTree                                   *mTree;
    
  float mWeight;
  float mPt;
  float mEta;
  float mPhi;
  float mE;
  float mEfEr;
  float mEfE;
  float mEfHr;
  float mEfH;
  float mEFracs[16];
  int mJid;
  double mFracs[16];
};
//////////////////////////////////////////////////////////////////////////////////////////
void GenJettify::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup) {}
//////////////////////////////////////////////////////////////////////////////////////////
void GenJettify::analyze(edm::Event const& event, edm::EventSetup const& iSetup) {
  //---------------- GenJets ------------------------------------------
  // Setting up handy handles
  edm::Handle<reco::GenJetCollection> genJets;
  event.getByToken(mGenJetsName,genJets);
  // Genjet loop
  edm::Handle<GenEventInfoProduct> hEventInfo;
  event.getByToken(mEventInfo, hEventInfo);
  
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
    for (size_t i = 0; i < 16; ++i) mFracs[i] = 0.;

    for (unsigned idx = 0; idx < igen->numberOfDaughters(); ++idx) {
      auto *dgt = dynamic_cast<const pat::PackedGenParticle*>(igen->daughter(idx));
      int pid = dgt->pdgId();
      double ene = dgt->energy();
      if (pid == 211) { // pi+
        mFracs[0] += ene; 
      } else if (pid == -211) { // pi-
        mFracs[1] += ene; 
      } else if (pid == 22) { // gamma
        auto moth = dgt->motherRef();
        if (moth.isNonnull() && moth.isAvailable() && abs(moth->pdgId()) == 111) {
          mFracs[2] += ene; 
        } else {
          mFracs[11] += ene;
        }
      } else if (pid == 321) { // K+
        mFracs[3] += ene; 
      } else if (pid == -321) { // K-
        mFracs[4] += ene; 
      } else if (abs(pid) == 310) { // KS
        mFracs[5] += ene; 
      } else if (abs(pid) == 130) { // KL
        mFracs[6] += ene; 
      } else if (pid == 2212) { // p+
        mFracs[7] += ene; 
      } else if (pid == -2212) { // p-
        mFracs[8] += ene; 
      } else if (pid == 2112) { // n
        mFracs[9] += ene; 
      } else if (pid == -2112) { // nbar
        mFracs[10] += ene; 
      } else if (abs(pid) == 3122 || abs(pid) == 3322) { // Lambda0/Xi0
        mFracs[12] += ene; 
      } else if (abs(pid) == 3222 || abs(pid) == 3112) { // Sigma+-/Xi-/Omega-
        mFracs[13] += ene; 
      } else if (abs(pid) == 3312 || abs(pid) == 3334) { // e/mu
        mFracs[14] += ene; 
      } else if (abs(pid) == 11 || abs(pid) == 13) {
        mFracs[15] += ene; 
      } else {
        cout << pid << " " << dgt->energy() << endl;
      }
    }
    for (size_t i = 0; i < 16; ++i) mEFracs[i] = mFracs[i];

    mTree->Fill();
    if (++mJid > 10) break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(GenJettify);
