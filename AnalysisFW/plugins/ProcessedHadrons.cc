#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
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
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

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

class ProcessedHadrons : public edm::EDAnalyzer
{
public:
  using LorentzVector = reco::Particle::LorentzVector;

  ProcessedHadrons(edm::ParameterSet const& cfg) :
    mIsMCarlo(                                                   cfg.getUntrackedParameter<bool>("isMCarlo",false)),
    mGenParticles(consumes<reco::GenParticleCollection>(cfg.getUntrackedParameter<edm::InputTag>("packedGenParticles",edm::InputTag("")))),
    mCands(mayConsume<pat::PackedCandidateCollection>(                             edm::InputTag("packedPFCandidates"))),
    // Trigger
    mFilterNames(                                    cfg.getParameter<std::vector<std::string> >("filterName")),
    mFilterBitsRECO(                               mayConsume<edm::TriggerResults>(edm::InputTag("TriggerResults","","RECO"))),
    mFilterBitsPAT(                                mayConsume<edm::TriggerResults>(edm::InputTag("TriggerResults","","PAT"))),
    mTriggerNames(                                   cfg.getParameter<std::vector<std::string> >("triggerName")),
    mTriggerBits(                                  mayConsume<edm::TriggerResults>(edm::InputTag("TriggerResults","","HLT")))
  {
    mTree = fs->make<TTree>("ProcessedTree","ProcessedTree");
    mTree->Branch("prescale", &mPrescale, "prescale/F");
    mTree->Branch("PtTrk", &mPtTrk, "PtTrk/F");
    mTree->Branch("Pt", &mPt, "Pt/F");
    mTree->Branch("Eta", &mEta, "Eta/F");
    mTree->Branch("Phi", &mPhi, "Phi/F");
    mTree->Branch("E", &mE, "E/F");
    mTree->Branch("Ef_ECALRaw", &mEfEr, "Ef_ECALRaw/F");
    mTree->Branch("Ef_ECAL", &mEfE, "Ef_ECAL/F");
    mTree->Branch("Ef_HCALRaw", &mEfHr, "Ef_HCALRaw/F");
    mTree->Branch("Ef_HCAL", &mEfH, "Ef_HCAL/F");
  }
  virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
  virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
  void beginJob() { // Generic boolean indicators
    mSatisfactory = true;
    mNewTrigs = false;
  }
  void endJob() { cout << "Happily finished processing! :)" << endl; }
  void beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& set) {}
  ~ProcessedHadrons() {}
private:
  // Configurable parameters
  bool            mSatisfactory;

  // GEN //
  bool                                             mIsMCarlo;
  bool                                             mNewTrigs;
  edm::EDGetTokenT<reco::GenParticleCollection>    mGenParticles;
  edm::EDGetTokenT<pat::PackedCandidateCollection> mCands;

  // TRIGGER & FILTER // 
  bool                                             mTrigObjs;
  bool                                             mFilterPAT;
  vector<int>                                      mFilterMissing;
  const vector<string>                             mFilterNames;
  vector<int>                                      mFilterIndex;
  edm::EDGetTokenT<edm::TriggerResults>            mFilterBitsRECO;
  edm::EDGetTokenT<edm::TriggerResults>            mFilterBitsPAT;
  const vector<string>                             mTriggerNames;
  edm::EDGetTokenT<edm::TriggerResults>            mTriggerBits;
  map<string,vector<string>>                       mTriggerNamesMap;
  map<string,int>                                  mTriggerNamesIndexMap;
  vector<int>                                      mTriggerIndex;
  map<string,vector<int>>                          mTriggerIndexMap;
  // MISC //
  HLTConfigProvider                                mHLTConfig;
  //HLTPrescaleProvider                              mHLTPrescale;

  edm::Service<TFileService>                       fs;
  TTree                                           *mTree;
    
  float mPrescale;
  float mPtTrk;
  float mPt;
  float mEta;
  float mPhi;
  float mE;
  float mEfEr;
  float mEfE;
  float mEfHr;
  float mEfH;

  // Private functions for various purposes
  bool trigUpdate(const vector<string> &tNames, vector<int> &tIndex);

  bool is_bhadr(int pdgid) {
    int aid = abs(pdgid);
    if (aid/100 == 5 or aid/1000==5) {
      return true;
    }
    return false;
  }
};
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedHadrons::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup) {
  bool changed = true;
  if (!mHLTConfig.init(iRun,iSetup,"HLT",changed)) {
    cout << "ProcessedHadrons::analyze: config extraction failure with process name HLT!" << endl;
    return;
  }
  if (!changed) return;
  cout << "Running on global tag " << mHLTConfig.globalTag() << "!!!" << endl;

  // Additional steps to be performed first time in the analysis loop!
  mNewTrigs = true;

  cout << "New trigger menu found!!!" << endl;

  // The triggers that are actively and passively monitored will be set up according to the wishes of the user.
  // We check mHLTConfig, which triggers of these are actually present.
  mSatisfactory = mSatisfactory and trigUpdate(mTriggerNames,mTriggerIndex); 
}
//////////////////////////////////////////////////////////////////////////////////////////
bool ProcessedHadrons::trigUpdate(const vector<string> &tNames, vector<int> &tIndex) {
  tIndex.clear();
  const auto &hltNames = mHLTConfig.triggerNames();
  const unsigned int nHLTs = mHLTConfig.size();
  assert(hltNames.size()==nHLTs);

  mTriggerNamesIndexMap.clear();
  mTriggerNamesMap.clear();
  mTriggerIndexMap.clear();
  for (const auto &tName : tNames) {
    // Create a temporary string and strip the zero from the end, in order to use this with regex.
    string tmpName = tName;
    tmpName.pop_back();
    regex trgversions(Form("%s([0-9]*)",tmpName.c_str()));
    if (tName.back()!='0' and tmpName.back()!='v') {
      cout << "In the simpleTrigs mode the trigger names should end with 'v0'! Please correct!" << endl;
      return false;
    }
    // Loop through all the hlt names with any version number matching the generic trigger type.
    int counter = 0;
    for (const auto &hName : hltNames) {
      if (regex_match(hName,trgversions)) {
        ++counter;
        if (mTriggerNamesMap.count(tName)==0) {
          mTriggerNamesMap[tName] = vector<string>();
          mTriggerIndexMap[tName] = vector<int>();
        }

        mTriggerNamesIndexMap[hName] = tIndex.size();
        mTriggerNamesMap[tName].emplace_back(hName); 
        mTriggerIndexMap[tName].emplace_back(mHLTConfig.triggerIndex(hName)); 
        cout << "Monitored trigger " << hName << " " << mTriggerIndexMap[tName].back() << " exists" << endl;
      }
    }
    tIndex.push_back(counter==0 ? -1 : counter);
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedHadrons::analyze(edm::Event const& event, edm::EventSetup const& iSetup) {
  // If the trigger information is not satisfactory, we should not enter the event analysis.
  if (!mSatisfactory) return;


  //-------------- Filter Info ---------------------------------------- 

  // Update the filter positions only when the trigger menu has changed.
  // These steps cannot be taken within "beginRun", as event.triggerNames() is not available there. 
  if (mNewTrigs) {
    mNewTrigs = false; // In data, this is set to false later.

    cout << "Found MET filters:" << endl;
    mFilterPAT = false;
    for (unsigned ifloc = 0; ifloc<2; ++ifloc) {
      edm::Handle<edm::TriggerResults> fBits;
      if (ifloc==0) {
        event.getByToken(mFilterBitsRECO,fBits); 
      } else { 
        event.getByToken(mFilterBitsPAT,fBits);
        mFilterPAT = true;
      }

      const edm::TriggerNames &fNames = event.triggerNames(*fBits);
      mFilterMissing.clear();
      mFilterIndex.clear();
      for (unsigned fltpos = 0; fltpos < mFilterNames.size(); ++fltpos) { // Loop through the requested filter names
        auto &flt = mFilterNames[fltpos];
        int fltIdx = -1;
        for (unsigned int iflt=0; iflt<fBits->size(); ++iflt) { // Try to find this
          string filterName = fNames.triggerName(iflt);
          if (filterName==flt) {
            fltIdx = iflt;
            break;
          }
        }
        mFilterIndex.push_back(fltIdx);
        if (fltIdx==-1) mFilterMissing.emplace_back(fltpos);
        else cout << " " << flt << " :)" << endl;
      }
      if (mFilterMissing.size() < mFilterNames.size()) break;
    }
    cout << "Using MET filters from " << (mFilterPAT ? "PAT" : "RECO") << endl;
    if (mFilterMissing.size()>0) {
      for (auto &fltpos : mFilterMissing) {
        auto &flt = mFilterNames[fltpos];
        cout << "MET filter " << flt << " missing :(" << endl;
      }
    }
  }

  // Fetching data using tokens.
  edm::Handle<edm::TriggerResults> filterBits;
  event.getByToken(mFilterPAT ? mFilterBitsPAT : mFilterBitsRECO,filterBits); 
  const edm::TriggerNames &filterNames = event.triggerNames(*filterBits);

  // Go through the filters and check that all of them are good
  assert(mFilterIndex.size()==mFilterNames.size());
  for (auto iflt = 0u; iflt<mFilterIndex.size(); ++iflt) {
    int fltIdx = mFilterIndex[iflt];
    string flt2;
    bool reject = false;
    if (fltIdx<0) {
      continue; // If a filter was not found, we skip it
    } else {
      reject = !filterBits->accept(fltIdx);
      flt2 = filterNames.triggerName(fltIdx);
    }
    
    string flt = mFilterNames[iflt];
    if (flt!=flt2) {
      cout << "Mismatch in filter names: " << flt << " " << flt2 << endl;
      mSatisfactory = false;
      return;
    }
    if (reject) return;
  }

  //-------------- Trigger Info (exlusively Data) --------------------- 
  edm::Handle<edm::TriggerResults> triggerBits;
  event.getByToken(mTriggerBits,triggerBits);
  // Trigger counts
  unsigned fire = 0;
  // Primary triggers
  for (auto itrig = 0u; itrig<mTriggerNames.size(); ++itrig) {
    int tIdx = mTriggerIndex[itrig]; 
    if (tIdx<0) continue; // We skip the triggers not present (perfectly normal!)
    const string &tName = mTriggerNames[itrig];

    const auto &tIndex2 = mTriggerIndexMap[tName];
    for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
      const auto &tIdx2 = tIndex2[itrig2];
      if (triggerBits->accept(tIdx2)) ++fire;
    } 
  }
  // It seems that the ZB trigger is active only 50% of the time, so we suppress trigger features for now
  if (!mIsMCarlo and fire==0) return;
  
  // PF Candidates (for beta calculus)
  edm::Handle<pat::PackedCandidateCollection> cands;
  event.getByToken(mCands, cands);

  // Rng
  //cout << "Setting up the rng!" << endl;
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine& engine = rng->getEngine(event.streamID());
  //cout << "Rng setup successful!" << endl;
  //cout << "A random number: " << engine.flat() << endl;

  // Pick the PF candidates removed by CHS (fromPV==0)
  for (auto cidx = 0u; cidx<cands->size(); ++cidx) {
    const auto &c = cands->at(cidx);
    // Prescale system for particles between 0.5 and 8 GeV 
    mPrescale = 1.;
    if (c.pt()<8) {
      double rand = engine.flat();
      if (c.pt()<6) {
        if (c.pt()<5) {
          if (c.pt()<4) {
            if (c.pt()<3.5) {
              if (c.pt()<3) {
                if (c.pt()<2) {
                  if (c.pt()<1) {
                    if (c.pt()<0.5) continue;
                    else mPrescale = 0.0001;
                  } else mPrescale = 0.001;
                } else mPrescale = 0.01;
              } else mPrescale = 0.03125;
            } else mPrescale = 0.0625;
          } else mPrescale = 0.125;
        } else mPrescale = 0.25;
      } else mPrescale = 0.5;
      if (rand>mPrescale) continue;
    }
    if (c.isPhoton() or c.isElectron() or c.isMuon()) continue;
    //if (fabs(c.pdgId())!=211) continue;
    if (!c.isIsolatedChargedHadron()) continue;
    double calob = c.rawCaloFraction();
    double caloa = c.caloFraction();
    double hb = c.rawHcalFraction()*calob;
    double ha = c.hcalFraction()*caloa;
    double eb = calob-hb;
    double ea = caloa-ha;
    //hb *= c.energy();
    //ha *= c.energy();
    //ea *= c.energy();
    //eb *= c.energy();
    //cout << eb << "/" << ea << " " << hb << "/" << ha << endl;
    //cout << "Good: " << c.pdgId() << " " << c.pt() << " " << c.ptTrk() << " " << c.ptTrk()/c.pt() << endl;
    mPtTrk = c.ptTrk();
    mPt  = c.pt();
    mEta = c.eta();
    mPhi = c.phi();
    mE   = c.energy();
    mEfEr = eb;
    mEfE  = ea;
    mEfHr = hb;
    mEfH  = ha;

    mTree->Fill();
  }
  //edm::Handle<reco::GenParticleCollection> gcands;
  //event.getByToken(mGenParticles, gcands);

  //for (auto gidx = 0u; gidx<gcands->size(); ++gidx) {
  //  const auto &gc = gcands->at(gidx);
  //  cout << gc.pt() << endl;
  //}
}

//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(ProcessedHadrons);
