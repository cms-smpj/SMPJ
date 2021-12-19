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

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "PhysicsTools/PatUtils/interface/bJetSelector.h"
#include "PhysicsTools/PatExamples/interface/BTagPerformance.h"
#include "PhysicsTools/PatExamples/interface/PatBTagCommonHistos.h"

#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "SMPJ/AnalysisFW/interface/QCDEvent.h"
#include "SMPJ/AnalysisFW/interface/QCDEventHdr.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJet.h"
#include "SMPJ/AnalysisFW/interface/QCDMET.h"

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
using std::string;
// Some compilers do not automatically understand this.
using std::size_t;

class ProcessedTreeProducerBTag : public edm::EDAnalyzer
{
public:
  using LorentzVector = reco::Particle::LorentzVector;
  // Public functions for the ED analyzer
  explicit ProcessedTreeProducerBTag(edm::ParameterSet const& cfg);
  virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
  virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
  void beginJob() {
    // Generic boolean indicators
    mSatisfactory = true;
    mNewTrigs = false;
    mRedoPhysDef = false;
    mRedoAlgoDef = false;
    if (mIsMCarlo) {
      if (mMCType==1) { // Herwig++
        if (mRunYear=="2016") {
          mRedoPhysDef = true;
          mRedoAlgoDef = true;
        }
      }
    }
  }
  void endJob() { cout << "Happily finished processing! :)" << endl; }
  void beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& set) {
    if (mIsMCarlo and mMCType==0) {
      edm::Handle<GenLumiInfoHeader> header;
      iLumi.getByToken(mLumiHeaderToken_, header);
      cout << header->configDescription() << endl;
      const auto &lheHead = header->lheHeaders();
      for (const auto &head : lheHead) cout << " " << head.first << " " << head.second << endl;
  
      const auto &wNames = header->weightNames();
      for (const auto &wn : wNames) mPSWeightHisto->Fill(wn.c_str(),1);
    }
  }

  ~ProcessedTreeProducerBTag() {}
private:
  // Configurable parameters
  bool            mSaveWeights; // Not to be set by the user
  bool            mAK4;
  bool            mZB;
  bool            mPrintTriggerMenu;
  bool            mIsPFJecUncSet; // Not to be set by the user
  int             mGoodVtxNdof;
  double          mGoodVtxZ;
  unsigned int    mMinNPFJets;
  double          mMinPFPt,mMinPFPtThirdJet,mMinGenPt,mMaxEta;
  string          mPFPayloadName;
  string          mRunYear; 
  string          mPFJetPUID;
  bool            mSatisfactory;

  // unc file for CHS jet
  string                                   mPFJECUncSrc;
  vector<string>                           mPFJECUncSrcNames;
  edm::EDGetTokenT<reco::VertexCollection> mOfflineVertices;
  edm::EDGetTokenT<reco::BeamSpot>         mBeamSpot;
  edm::EDGetTokenT<edm::View<pat::Jet> >   mPFJetsName;
  edm::EDGetTokenT<double>                 mSrcCaloRho;
  edm::EDGetTokenT<double>                 mSrcPFRho;
  // MET //
  edm::EDGetTokenT<pat::METCollection> mPFMETt1;
  // GEN //
  bool                                                     mIsMCarlo;
  bool                                                     mUseGenInfo;
  bool                                                     mNewTrigs;
  int                                                      mMCType;
  edm::EDGetTokenT<reco::GenJetCollection>                 mGenJetsName;
  edm::EDGetTokenT<reco::GenParticleCollection>            mGenParticles;
  edm::Handle<reco::GenParticleCollection>                 mGenParts;
  edm::EDGetTokenT<GenEventInfoProduct>                    mEventInfo;
  edm::EDGetTokenT<vector<PileupSummaryInfo> >             mSrcPU;
  edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosToken;
  edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosTokenPhysicsDef;
  edm::EDGetTokenT<edm::ValueMap<float>>                   mQGLToken;
  edm::EDGetTokenT<edm::ValueMap<float>>                   mQGAx2Token;
  edm::EDGetTokenT<edm::ValueMap<int>>                     mQGMulToken;
  edm::EDGetTokenT<edm::ValueMap<float>>                   mQGPtDToken;

  edm::InputTag                                            mGenEventSrc_;
  edm::EDGetTokenT<GenEventInfoProduct>                    mGenEventToken_;
  edm::EDGetTokenT<GenLumiInfoHeader>                      mLumiHeaderToken_;

  edm::InputTag                                            mLHEEventSrc_;
  edm::EDGetTokenT<LHEEventProduct>                        mLHEEventToken_;
  edm::EDGetTokenT<LHERunInfoProduct>                      mLHEInfoToken_;

  // TRIGGER & FILTER // 
  bool                                                     mDiscardFilter;
  bool                                                     mTrigSimple;
  bool                                                     mTrigObjs;
  bool                                                     mFilterPAT;
  int                                                      mFilterEcalBad;
  vector<int>                                              mFilterMissing;
  const vector<string>                                     mFilterNames;
  vector<int>                                              mFilterIndex;
  edm::EDGetTokenT<bool>                                   mFilterBitEcal;
  edm::EDGetTokenT<edm::TriggerResults>                    mFilterBitsRECO;
  edm::EDGetTokenT<edm::TriggerResults>                    mFilterBitsPAT;
  const vector<string>                                     mTriggerNames;
  const vector<string>                                     mTriggerNamesFlw;
  map<string,vector<string>>                               mTriggerNamesMap;
  map<string,int>                                          mTriggerNamesIndexMap;
  vector<int>                                              mTriggerIndex;
  vector<int>                                              mTriggerIndexFlw;
  map<string,vector<int>>                                  mTriggerIndexMap;
  edm::EDGetTokenT<edm::TriggerResults>                    mTriggerBits;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> mTriggerHLTObjs;
  edm::EDGetTokenT<BXVector<l1t::Jet> >                    mTriggerL1Objs;
  edm::EDGetTokenT<BXVector<l1t::EtSum> >                  mTriggerL1HTObjs;
  edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescales;
  edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Min;
  edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Max;
  // CORRECTORS //
  JetCorrectionUncertainty                                *mPFUnc;
  vector<JetCorrectionUncertainty*>                        mPFUncSrc;
  // MISC //
  edm::EDGetTokenT<bool>                                   mHBHENoiseFilterResultNoMinZLabel;
  edm::EDGetTokenT<pat::PackedCandidateCollection>         mCands;
  HLTConfigProvider                                        mHLTConfig;
  HLTPrescaleProvider                                      mHLTPrescale;

  edm::Service<TFileService>                               fs;
  TTree                                                   *mTree;
  TH1F                                                    *mTriggerPassHisto,*mTriggerNamesHisto,*mFilterActiveHisto,*mPSWeightHisto;
  QCDEvent                                                *mEvent;

  vector<int>           mGenFlavour;
  vector<int>           mGenFlavourHadr;
  vector<int>           mGenFlavourPhys;
  vector<float>         mGenBPt;
  vector<LorentzVector> mGenJets;
  map<int,int>          mGenJetPhysFlav;
  map<int,int>          mGenJetAlgoFlav;

  bool                  mRedoPhysDef;
  bool                  mRedoAlgoDef;
    
  // Private functions for various purposes
  bool trigUpdate(const vector<string> &tNames, vector<int> &tIndex, bool active);
  bool trigCheck(const edm::TriggerNames &names, const vector<string> &tNames, vector<int> &tIndex, bool active);
  
  // Sort the QCDPFJet collection according to pt
  static bool sort_pfjets(QCDPFJet j1, QCDPFJet j2) { return j1.ptCor() > j2.ptCor(); }
  
  // Search for the index of the given pt, genjets
  unsigned gen_ptpos(double pt, unsigned blw, unsigned abv) {
    unsigned nxt = (abv+blw)/2; 
    if (nxt==blw) return nxt;
    double nxtpt = mGenJets[nxt].pt();
    if (nxtpt<=pt) return gen_ptpos(pt,blw,nxt);
    else           return gen_ptpos(pt,nxt,abv);
  } 

  bool is_bhadr(int pdgid) {
    int aid = abs(pdgid);
    if (aid/100 == 5 or aid/1000==5) {
      return true;
    }
    return false;
  }

  // Find the gen jet best matching to the current jet
  template<typename T>      
  pair<int,float> best_genjet(const T &jet) {
    int imin = -1;
    float rmin = 999.;

    int limit = mGenJets.size();
    if (limit>0) {
      float ratiouplim = (jet.pt()>20 ? 1.5 : 5.0);
      float ratiololim = (jet.pt()>20 ? 0.4 : 0.2);
      // Find approximately the closest pt and then iterate up and down
      // If a good match is found, iteration stops
      int dwn = (jet.pt()>20 ? gen_ptpos(jet.pt(),0,limit-1) : limit-1);
      int up = dwn+1;
      // Iterate the pt-ordered genjet set up & down
      while (dwn>=0 or up<limit) {
        // Take a step downwards (towards greater pt)
        if (dwn>=0) {
          if ((mGenJets[dwn].pt())/(jet.pt())<ratiouplim) {
            float deltaR = reco::deltaR(jet,mGenJets[dwn]);
            if (deltaR < rmin) {
              imin = dwn;
              rmin = deltaR;
              if (rmin<0.2) break; // Stop if the delta R match is good enough
            }
            --dwn;
          } else {
            // Ignore the "down" end when we reach the pt threshold
            dwn = -1;
          }
        }
        if (up<limit) {
          // Take a step upwards (towards smaller pt)
          if ((mGenJets[up].pt())/(jet.pt())>ratiololim) {
            float deltaR = reco::deltaR(jet,mGenJets[up]);
            if (deltaR < rmin) {
              imin = up;
              rmin = deltaR;
              if (rmin<0.2) break; // Stop if the delta R match is good enough
            }
            ++up;
          } else {
            // Ignore the "up" end when we reach the pt threshold
            up = limit;
          }
        }
      }
    }
    return make_pair(imin,rmin);
  }

  // Find the gen jet best matching to the current jet
  template<typename T>      
  pair<int,float> best_jet4prtn(const T &prtn) {
    int imin = -1;
    float rmin = 999.;

    // Iterate the pt-ordered genjet set up & down
    for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
      float deltaR = reco::deltaR(prtn,mGenJets[igen]);
      if (deltaR<rmin) {
        imin = igen;
        rmin = deltaR;
      }
    }
    return make_pair(imin,rmin);
  }

  void flavs_physdef() {
    mGenJetPhysFlav.clear();
    vector<int> hpParts;
    for (unsigned i = 0; i<mGenParts->size(); ++i) {
      if (hpParts.size()==2) break;
      const auto &gPart = mGenParts->at(i);
      int id = gPart.pdgId();
      int absId = abs(id);
      if (absId>10 and absId!=21) continue; // Non-partons
      if (fabs(gPart.eta())>5.0) continue; // Bad eta region
  
      if (mMCType==0) { // Pythia 8
        if (abs(gPart.pdgId())!=23) continue;
      } else if (mMCType==1) { // Herwig++
        bool hardProc = false;
        if (gPart.numberOfMothers()==1) {
          auto &moths = gPart.motherRefVector();
          auto *moth = moths.at(0).get();
          if (moth!=nullptr and moth->pdgId()!=2212 and fabs(moth->eta())>10.0) hardProc = true; 
        }
        if (!hardProc) continue;
      } else {
        return;
      }
      hpParts.push_back(i);
    }

    for (auto &ihp : hpParts) {
      auto &hp = mGenParts->at(ihp);
      float drmin = 999.;
      int imin = -1;
      for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
        auto &gjet = mGenJets[igen];
        float dR = reco::deltaR(gjet,hp);
        if (dR<drmin) {
          drmin = dR;
          imin = igen;
        }
      }
      if (imin!=-1 and drmin<0.4) mGenJetPhysFlav[imin] = hp.pdgId();
    }
  }

  void flavs_newalgodef() {
    mGenJetAlgoFlav.clear();
    map<int,int> jet2prtn;
    for (unsigned i = 0; i<mGenParts->size(); ++i) {
      const auto &gPart = mGenParts->at(i);
      int id = gPart.pdgId();
      int absId = abs(id);
      if (absId>10 and absId!=21) continue; // Non-partons
      if (fabs(gPart.eta())>5.0) continue; // Bad eta region
  
      if (mMCType!=0 and mMCType!=1) // Non- Pythia8 & Herwig++
        return;

      float drmin = 999.;
      int imin = -1;
      for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
        auto &gjet = mGenJets[igen];
        float dR = reco::deltaR(gjet,gPart);
        if (dR<drmin) {
          drmin = dR;
          imin = igen;
        }
      }
      if (imin!=-1 and drmin<0.4) {
        if (jet2prtn.find(imin)==jet2prtn.end()) {
          jet2prtn[imin] = i;
        } else {
          auto &currBest = mGenParts->at(jet2prtn[imin]);
          if (gPart.pt()>currBest.pt())
            jet2prtn[imin] = i;
        }
      }
    }
    for (auto &j2p : jet2prtn) {
      auto &bestPrtn = mGenParts->at(j2p.second);
      mGenJetAlgoFlav[j2p.first] = bestPrtn.pdgId();
    }
  }
};

ProcessedTreeProducerBTag::ProcessedTreeProducerBTag(edm::ParameterSet const& cfg):
  mSaveWeights(                                                                                      cfg.getParameter<bool>("saveWeights")),
  mAK4(                                                                                     cfg.getUntrackedParameter<bool>("AK4",false)),
  mZB(                                                                                      cfg.getUntrackedParameter<bool>("ZB",false)),
  mPrintTriggerMenu(                                                                        cfg.getUntrackedParameter<bool>("printTriggerMenu",false)),
  mIsPFJecUncSet(                                                                                                           false),
  // Cut params
  mGoodVtxNdof(                                                                                    cfg.getParameter<double>("goodVtxNdof")),
  mGoodVtxZ(                                                                                       cfg.getParameter<double>("goodVtxZ")),
  mMinNPFJets(                                                                               cfg.getParameter<unsigned int>("minNPFJets")),
  mMinPFPt(                                                                                        cfg.getParameter<double>("minPFPt")),
  mMinPFPtThirdJet(                                                                                cfg.getParameter<double>("minPFPtThirdJet")),
  mMinGenPt(                                                                              cfg.getUntrackedParameter<double>("minGenPt",20)),
  mMaxEta(                                                                                         cfg.getParameter<double>("maxEta")),
  // Misc
  mPFPayloadName(                                                                                  cfg.getParameter<string>("PFPayloadName")),
  mRunYear(                                                                               cfg.getUntrackedParameter<string>("runYear","2016")),
  mPFJetPUID(                                                                                      cfg.getParameter<string>("pfchsjetpuid")),
  mPFJECUncSrc(                                                                           cfg.getUntrackedParameter<string>("jecUncSrc","")),
  mPFJECUncSrcNames(                                                                      cfg.getParameter<vector<string> >("jecUncSrcNames")),
  mOfflineVertices(mayConsume<reco::VertexCollection>(                                      cfg.getParameter<edm::InputTag>("offlineVertices"))),
  mBeamSpot(mayConsume<reco::BeamSpot>(                                                     cfg.getParameter<edm::InputTag>("beamSpot"))),
  mPFJetsName(consumes<edm::View<pat::Jet>>(                                                cfg.getParameter<edm::InputTag>("pfjetschs"))),
  // Rho
  mSrcCaloRho(mayConsume<double>(                                                           cfg.getParameter<edm::InputTag>("srcCaloRho"))),
  mSrcPFRho(mayConsume<double>(                                                             cfg.getParameter<edm::InputTag>("srcPFRho"))),
  // MET
  mPFMETt1(mayConsume<pat::METCollection>(                                                  cfg.getParameter<edm::InputTag>("pfmetT1"))),
  // GEN
  mIsMCarlo(                                                                                cfg.getUntrackedParameter<bool>("isMCarlo",false)),
  mUseGenInfo(                                                                              cfg.getUntrackedParameter<bool>("useGenInfo",false)),
  mMCType(                                                                                   cfg.getUntrackedParameter<int>("mcType",0)), // 0 for Pythia, 1 for Herwig
  mGenJetsName(mayConsume<reco::GenJetCollection>(                                 cfg.getUntrackedParameter<edm::InputTag>("genjets",edm::InputTag("")))),
  mGenParticles(consumes<reco::GenParticleCollection>(                             cfg.getUntrackedParameter<edm::InputTag>("GenParticles",edm::InputTag("")))),
  mEventInfo(consumes<GenEventInfoProduct>(                                        cfg.getUntrackedParameter<edm::InputTag>("EventInfo",edm::InputTag("")))),
  mSrcPU(mayConsume<vector<PileupSummaryInfo> >(                                   cfg.getUntrackedParameter<edm::InputTag>("srcPULabel",edm::InputTag("")))),
  mJetFlavourInfosToken(consumes<reco::JetFlavourInfoMatchingCollection>(          cfg.getUntrackedParameter<edm::InputTag>("jetFlavInfos",edm::InputTag("")))),
  mJetFlavourInfosTokenPhysicsDef(consumes<reco::JetFlavourInfoMatchingCollection>(cfg.getUntrackedParameter<edm::InputTag>("jetFlavInfosPD",edm::InputTag("")))),
  // Trigger
  mDiscardFilter(                                                                           cfg.getUntrackedParameter<bool>("discardFilter",true)),
  mTrigSimple(                                                                              cfg.getUntrackedParameter<bool>("trigSimple",false)),
  mTrigObjs(                                                                                cfg.getUntrackedParameter<bool>("trigObjs",false)),
  mFilterNames(                                                                           cfg.getParameter<vector<string> >("filterName")),
  mFilterBitEcal(mayConsume<bool>(                                                                            edm::InputTag("ecalBadCalibReducedMINIAODFilter"))),
  mFilterBitsRECO(mayConsume<edm::TriggerResults>(                                                            edm::InputTag("TriggerResults","","RECO"))),
  mFilterBitsPAT(mayConsume<edm::TriggerResults>(                                                             edm::InputTag("TriggerResults","","PAT"))),
  mTriggerNames(                                                                          cfg.getParameter<vector<string> >("triggerName")),
  mTriggerNamesFlw(                                                                       cfg.getParameter<vector<string> >("triggerFollow")),
  mTriggerBits(mayConsume<edm::TriggerResults>(                                                               edm::InputTag("TriggerResults","","HLT"))),
  mTriggerHLTObjs(mayConsume<pat::TriggerObjectStandAloneCollection>(              cfg.getUntrackedParameter<edm::InputTag>("triggerHLTObjs",edm::InputTag("")))),
  mTriggerL1Objs(mayConsume<BXVector<l1t::Jet> >(                                  cfg.getUntrackedParameter<edm::InputTag>("triggerL1Objs",edm::InputTag("")))),
  mTriggerL1HTObjs(mayConsume<BXVector<l1t::EtSum> >(                              cfg.getUntrackedParameter<edm::InputTag>("triggerL1HTObjs",edm::InputTag("")))),
  mTriggerPrescales(consumes<pat::PackedTriggerPrescales>(                                  cfg.getParameter<edm::InputTag>("prescales"))),
  mTriggerPrescalesL1Min(consumes<pat::PackedTriggerPrescales>(                             cfg.getParameter<edm::InputTag>("prescalesL1Min"))),
  mTriggerPrescalesL1Max(consumes<pat::PackedTriggerPrescales>(                             cfg.getParameter<edm::InputTag>("prescalesL1Max"))),
  mHBHENoiseFilterResultNoMinZLabel(mayConsume<bool>(                              cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultNoMinZLabel",edm::InputTag("")))),
  mCands(mayConsume<pat::PackedCandidateCollection>(                                                          edm::InputTag("packedPFCandidates"))),
  mHLTPrescale(cfg, consumesCollector(), *this)
{
  if (mAK4) {
    mQGLToken   = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "qgLikelihood"));
    mQGAx2Token = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "axis2"));
    mQGMulToken = consumes<edm::ValueMap<int>>  (edm::InputTag("QGTagger", "mult"));
    mQGPtDToken = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "ptD"));
  }
  if (mIsMCarlo and mMCType==0) {
    mGenEventSrc_     = edm::InputTag("generator");
    mGenEventToken_   = mayConsume<GenEventInfoProduct>                       (mGenEventSrc_);
    mLumiHeaderToken_ = mayConsume<GenLumiInfoHeader, edm::BranchType::InLumi>(mGenEventSrc_);

    mLHEEventSrc_   = edm::InputTag("externalLHEProducer");
    mLHEEventToken_ = mayConsume<LHEEventProduct>                          (mLHEEventSrc_);
    mLHEInfoToken_  = mayConsume<LHERunInfoProduct, edm::BranchType::InRun>(mLHEEventSrc_);

    mPSWeightHisto = fs->make<TH1F>("PSWeights","PSWeights",1,0,1);
    mPSWeightHisto->SetBit(TH1::kUserContour);
  }

  mTree = fs->make<TTree>("ProcessedTree","ProcessedTree");
  mEvent = new QCDEvent();
  mTree->Branch("events","QCDEvent",&mEvent);
  mTriggerNamesHisto = fs->make<TH1F>("TriggerNames","TriggerNames",1,0,1);
  mTriggerNamesHisto->SetBit(TH1::kUserContour);  
  mTriggerPassHisto = fs->make<TH1F>("TriggerPass","TriggerPass",1,0,1);
  mTriggerPassHisto->SetBit(TH1::kUserContour);
  mFilterActiveHisto = fs->make<TH1F>("FilterActive","FilterActive",1,0,1); 
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup) {
  bool changed = true;
  if (!mHLTConfig.init(iRun,iSetup,"HLT",changed) or !mHLTPrescale.init(iRun, iSetup,"HLT",changed)) {
    cout << "ProcessedTreeProducerBTag::analyze: config extraction failure with process name HLT!" << endl;
    return;
  }
  if (!changed) return;
  cout << "Running on global tag " << mHLTConfig.globalTag() << "!!!" << endl;

  // Save MET filter pass information
  for (auto &fName : mFilterNames) {
    mFilterActiveHisto->Fill(fName.c_str(),0);
  }
  // As a last piece of info, we indicate if all filters are passed.
  mFilterActiveHisto->Fill("PassAll",0);

  // Additional steps to be performed first time in the analysis loop!
  // For MC, this means only updating MET Filter info
  mNewTrigs = true;

  cout << "New trigger menu found!!!" << endl;

  // The triggers that are actively and passively monitored will be set up according to the wishes of the user.
  // mTriggerNames(Flw) is the trigger name list set by the user.
  // We check mHLTConfig, which triggers of these are actually present.
  // Usage example for the passively monitored triggers: when AK4 and AK8 jets are saved into separate files, we still want the same events to be available in both.
  mSatisfactory = mSatisfactory and trigUpdate(mTriggerNames,mTriggerIndex,true) and trigUpdate(mTriggerNamesFlw,mTriggerIndexFlw,false); 

  if (mPrintTriggerMenu) {
    cout << "Available TriggerNames are: " << endl;
    mHLTConfig.dump("Triggers");
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
bool ProcessedTreeProducerBTag::trigUpdate(const vector<string> &tNames, vector<int> &tIndex, bool active) {
  tIndex.clear();
  const auto &hltNames = mHLTConfig.triggerNames();
  const unsigned int nHLTs = mHLTConfig.size();
  assert(hltNames.size()==nHLTs);

  if (mTrigSimple and active) {
    mTriggerNamesIndexMap.clear();
    mTriggerNamesMap.clear();
    mTriggerIndexMap.clear();
  }
  for (const auto &tName : tNames) {
    // For active triggers, we will add the base information in any case.
    if (active) {
      mTriggerPassHisto->Fill(tName.c_str(),0);
      mTriggerNamesHisto->Fill(tName.c_str(),0);
    }
    if (mTrigSimple) {
      // Create a temporary string and strip the zero from the end, in order to use this with regex.
      string tmpName = tName;
      tmpName.pop_back();
      regex trgversions(Form("%s([0-9]*)",tmpName.c_str()));
      if (tName.back()!='0' and tmpName.back()!='v') {
        cout << "In the 'simpleTrigs = True' mode the trigger names should end with 'v0'! Please correct!" << endl;
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
          if (active) mTriggerNamesHisto->Fill(tName.c_str(),1);
          cout << (active ? "Actively" : "Passively") << " monitored trigger " << hName << " " << mTriggerIndexMap[tName].back() << " exists" << endl;
        }
      }
      tIndex.push_back(counter==0 ? -1 : counter);
    } else {
      unsigned tIdx = mHLTConfig.triggerIndex(tName);
      bool accept = tIdx < nHLTs;
      cout << (active ? "Actively" : "Passively") << " monitored trigger " << tName << " " << tIdx << " ";
      cout << (accept ? "exists" : "does not exist in the current menu") << endl; 
      tIndex.push_back(accept ? tIdx : -1);
      if (active and accept) mTriggerNamesHisto->Fill(tName.c_str(),1);
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool ProcessedTreeProducerBTag::trigCheck(const edm::TriggerNames &names, const vector<string> &tNames, vector<int> &tIndex, bool active) {
  // Go trough all monitored triggers and check that these have the same names.
  assert(tIndex.size()==tNames.size());
  for (auto itrig = 0u; itrig<tIndex.size(); ++itrig) {
    int tIdx = tIndex[itrig];
    if (tIdx<0) continue; // Trigger not monitored or present, OK!
    const string &tName = tNames[itrig];

    if (mTrigSimple) {
      const auto &tNames2 = mTriggerNamesMap[tName];
      const auto &tIndex2 = mTriggerIndexMap[tName];
      assert(tNames2.size()==tIndex2.size());
      for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
        const string &tName2  = tNames2[itrig2];
        const auto &tIdx2 = tIndex2[itrig2];
        if (tName2!=names.triggerName(tIdx2)) {
          cout << "Mismatch in trigger names: " << tName << " " << tName2 << endl;
          return false;
        }
      }
    } else {
      const auto &tName2 = names.triggerName(tIdx);
      if (tName != tName2) {
        cout << "Mismatch in trigger names: " << tName << " " << tName2 << endl;
        return false;
      }
    }
  }
  cout << (active ? "Actively" : "Passively") << " monitored triggers checked!" << endl;
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::analyze(edm::Event const& event, edm::EventSetup const& iSetup) {
  // If the trigger information is not satisfactory, we should not enter the event analysis.
  if (!mSatisfactory) return;

  vector<QCDPFJet>      qPFJets;
  QCDEventHdr           qEvtHdr;
  QCDMET                qPFMet_t1, qPFMet_t0, qPFMet_t0t1;
  
  //-------------- Basic Event Info -----------------------------------
  qEvtHdr.setRun(event.id().run());
  qEvtHdr.setEvt(event.id().event());
  qEvtHdr.setLumi(event.luminosityBlock());
  qEvtHdr.setBunch(event.bunchCrossing());
  float refR = mAK4 ? 0.4 : 0.8;
  
  //-------------- Beam Spot ------------------------------------------
  edm::Handle<reco::BeamSpot> beamSpot;
  event.getByToken(mBeamSpot,beamSpot);
  if (beamSpot.isValid()) qEvtHdr.setBS(beamSpot->x0(),beamSpot->y0(),beamSpot->z0());
  else qEvtHdr.setBS(-999,-999,-999);

  //-------------- HCAL Noise Summary ---------------------------------
  if (mIsMCarlo) {
    qEvtHdr.setHCALNoiseNoMinZ(true);

    if (mMCType==0) {
      edm::Handle<GenEventInfoProduct> genEventInfo;
      event.getByToken(mGenEventToken_, genEventInfo);

      if (genEventInfo.isValid()) {
        const auto &ws = genEventInfo->weights();
        for (size_t i = 0; i < ws.size(); ++i) qEvtHdr.setPSWeight(i,ws[i]);
      }
    }
  } else {
    edm::Handle<bool> noiseSummary_NoMinZ;
    event.getByToken(mHBHENoiseFilterResultNoMinZLabel, noiseSummary_NoMinZ);
    qEvtHdr.setHCALNoiseNoMinZ(*noiseSummary_NoMinZ);
  }

  //-------------- Filter Info ---------------------------------------- 

  // Update the filter positions only when the trigger menu has changed.
  // These steps cannot be taken within "beginRun", as event.triggerNames() is not available there. 
  if (mNewTrigs) {
    //if (mIsMCarlo) mNewTrigs = false; // In data, this is set to false later.

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
    mFilterEcalBad = -1;
    if (mFilterMissing.size()>0) {
      for (auto &fltpos : mFilterMissing) {
        auto &flt = mFilterNames[fltpos];
        if (flt == "Flag_ecalBadCalibReducedMINIAODFilter") {
          cout << "MET filter " << flt << " requires special attention." << endl;
          mFilterEcalBad = fltpos;
        } else {
          cout << "MET filter " << flt << " missing :(" << endl;
        }
      }
    }
  }

  vector<int> Filtered;
  // Fetching data using tokens.
  edm::Handle<edm::TriggerResults> filterBits;
  event.getByToken(mFilterPAT ? mFilterBitsPAT : mFilterBitsRECO,filterBits); 
  const edm::TriggerNames &filterNames = event.triggerNames(*filterBits);

  // Go through the filters and check that all of them are good
  assert(mFilterIndex.size()==mFilterNames.size());
  bool passMET = true;
  for (auto iflt = 0u; iflt<mFilterIndex.size(); ++iflt) {
    int fltIdx = mFilterIndex[iflt];
    string flt2;
    bool reject = false;
    if (fltIdx<0) {
      if (mFilterEcalBad>=0 and iflt == abs(mFilterEcalBad)) {
        // This filter needs to be fetched separately.
        edm::Handle<bool> filterBitEcal;
        event.getByToken(mFilterBitEcal,filterBitEcal);
        reject =  !(*filterBitEcal);
        flt2 = "Flag_ecalBadCalibReducedMINIAODFilter";
      } else {
        continue; // If a filter was not found, we skip it
      }
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
    if (reject) {
      Filtered.push_back(iflt);
      passMET = false;
      mFilterActiveHisto->Fill(flt.c_str(),1);
    }
  }
  // We log the knowledge that all filters have passed. This knowledge is not saved on a per-event 
  // basis, as an empty vector 'Filtered' already indicates that no MET filters have been active.
  if (passMET) mFilterActiveHisto->Fill("PassAll",1);
  // A filter is allowed to reject the event
  if (mDiscardFilter and !passMET) return;

  mEvent->setFltDecision(Filtered);

  //-------------------------- Trigger Info --------------------------- 
  edm::Handle<edm::TriggerResults> triggerBits;
  event.getByToken(mTriggerBits,triggerBits);
  const edm::TriggerNames &names = event.triggerNames(*triggerBits);
  
  vector<int> L1Prescales, HLTPrescales, Fired;
  map<string, vector<LorentzVector> > vvHLT;

  // Fetching prescales
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  event.getByToken(mTriggerPrescales,      triggerPrescales);
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescalesL1Min;
  event.getByToken(mTriggerPrescalesL1Min, triggerPrescalesL1Min);
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescalesL1Max;
  event.getByToken(mTriggerPrescalesL1Max, triggerPrescalesL1Max);

  // When the triggers are changed, check that the info we got from hltConfig matches with that from event.triggerNames
  if (mNewTrigs) {
    mNewTrigs = false;
    if ((!trigCheck(names,mTriggerNames,mTriggerIndex,true) or !trigCheck(names,mTriggerNamesFlw,mTriggerIndexFlw,false))) {
      mSatisfactory = false;
      return;
    }
  }

  // Trigger counts
  unsigned fire = 0;
  // Primary triggers
  for (auto itrig = 0u; itrig<mTriggerNames.size(); ++itrig) {
    int tIdx = mTriggerIndex[itrig]; 
    if (tIdx<0) continue; // We skip the triggers not present (perfectly normal!)
    const string &tName = mTriggerNames[itrig];

    if (mTrigSimple) {
      const auto &tIndex2 = mTriggerIndexMap[tName];
      for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
        const auto &tIdx2 = tIndex2[itrig2];
        if (triggerBits->accept(tIdx2)) {
          L1Prescales.push_back(max(triggerPrescalesL1Max->getPrescaleForIndex(tIdx2),triggerPrescalesL1Min->getPrescaleForIndex(tIdx2)));
          HLTPrescales.push_back(triggerPrescales->getPrescaleForIndex(tIdx2));
          mTriggerPassHisto->Fill(tName.c_str(),1);
          Fired.push_back(itrig);
          ++fire;
          if (mTrigObjs and vvHLT.find(tName)==vvHLT.end()) vvHLT[tName] = vector<LorentzVector>();
        }
      } 
    } else {
      if (triggerBits->accept(tIdx)) {
        L1Prescales.push_back(max(triggerPrescalesL1Max->getPrescaleForIndex(tIdx),triggerPrescalesL1Min->getPrescaleForIndex(tIdx)));
        HLTPrescales.push_back(triggerPrescales->getPrescaleForIndex(tIdx));
        mTriggerPassHisto->Fill(tName.c_str(),1);
        Fired.push_back(itrig);
        ++fire;
        if (mTrigObjs and vvHLT.find(tName)==vvHLT.end()) vvHLT[tName] = vector<LorentzVector>();
      }
    }
  }

  // When the primary trigger has not fired, check if a "followed" secondary trigger has fired
  if (fire==0) {
    bool nofireoth = true;
    for (auto itrig = 0u; itrig<mTriggerIndexFlw.size(); ++itrig) {
      int tIdx = mTriggerIndexFlw[itrig]; 
      if (tIdx<0) continue; // Skip the triggers not present (perfectly normal!)

      if (mTrigSimple) {
        const auto &tIndex2 = mTriggerIndexMap[mTriggerNamesFlw[itrig]];
        for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
          const auto &tIdx2 = tIndex2[itrig2];
          if (triggerBits->accept(tIdx2)) {
            nofireoth = false;
            break;
          }
        }
        if (!nofireoth) break;
      } else {
        if (triggerBits->accept(tIdx)) {
          nofireoth = false;
          break;
        }
      }
    }
    // If none of the actively/passively monitored triggers have fired, skip this as a redundant event
    if (nofireoth) return;
  } // Primary trigger not fired

  // Saving the info to the event
  mEvent->setTrigDecision(Fired);
  mEvent->setPrescales(L1Prescales,HLTPrescales);

  //-------------- Trigger Object Info (exlusively Data) --------------- 
  // The trigger objects are saved only when the actively monitored trigger has fired (in ZB events we pay no attention)
  if (mTrigObjs and !mIsMCarlo and !mZB and fire>0) {
    vector<vector<LorentzVector> >      qL1Objs, qHLTObjs;  
    
    // Fetching data using tokens.
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerHLTObjs;
    event.getByToken(mTriggerHLTObjs,                   triggerHLTObjs);
    edm::Handle<BXVector<l1t::Jet> >                    triggerL1Objs;
    event.getByToken(mTriggerL1Objs,                    triggerL1Objs);
    edm::Handle<BXVector<l1t::EtSum> >                  triggerL1HTObjs;
    event.getByToken(mTriggerL1HTObjs,                  triggerL1HTObjs);

    // HLT sector
    regex pfjet(Form("HLT_%sPFJet([0-9]*)_v([0-9]*)",mAK4 ? "" : "AK8"));
    for (pat::TriggerObjectStandAlone obj : *triggerHLTObjs) { // note: not "const &" since we want to call unpackPathNames
      obj.unpackPathNames(names);
      vector<string> pathNamesAll  = obj.pathNames(false);
      vector<string> pathNamesLast = obj.pathNames(true);
      if (pathNamesAll.size()==0) continue; 
      
      for (unsigned hpn = 0, npn = pathNamesAll.size(); hpn < npn; ++hpn) {
        string tName = pathNamesAll[hpn];
        if (regex_match(tName,pfjet)) {
          TLorentzVector P4;
          P4.SetPtEtaPhiM(obj.pt(),obj.eta(),obj.phi(),obj.mass());
          string refName = tName;
          if (mTrigSimple) {
            if (mTriggerNamesIndexMap.count(tName)==0) {
              cout << "Trigger name not found in trigger name map!" << endl;
              mSatisfactory = false;
              return;
            }
            unsigned refIdx = mTriggerNamesIndexMap[tName];
            if (refIdx<mTriggerNames.size()) {
              refName = mTriggerNames[refIdx];
            } else {
              cout << "Trigger index too big in trigger name map!" << endl;
              mSatisfactory = false;
              return;
            }
          }
          vvHLT[refName].emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
        }
      }
    }
    // HLT objects are added collectively, after we have went through all the objects
    for (auto &trg : Fired) qHLTObjs.push_back(vvHLT[mTriggerNames[trg]]); 

    // L1 sector: jets
    vector<LorentzVector> vvL1;
    for (auto obj = triggerL1Objs->begin(0); obj != triggerL1Objs->end(0); ++obj) {
      TLorentzVector P4;
      P4.SetPtEtaPhiM(obj->pt(),obj->eta(),obj->phi(),obj->mass());
      vvL1.emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
    }
    qL1Objs.push_back(vvL1);

    mEvent->setL1Obj(qL1Objs);
    mEvent->setHLTObj(qHLTObjs);
  } // !mIsMCarlo mTrigObjs and !mZB
  
  //-------------- Vertex Info ----------------------------------------
  edm::Handle<reco::VertexCollection> recVtxs;
  event.getByToken(mOfflineVertices,recVtxs);
  
  //------------- reject events without reco vertices -----------------
  int VtxGood(0);
  bool isPVgood(false);
  float PVx(0),PVy(0),PVz(0),PVndof(0);
  for (auto ivtx = recVtxs->begin(); ivtx != recVtxs->end(); ++ivtx) {
    int index = ivtx-recVtxs->begin();
    if (index == 0) {
      PVx    = ivtx->x();
      PVy    = ivtx->y();
      PVz    = ivtx->z();
      PVndof = ivtx->ndof();
    }
    if (!(ivtx->isFake()) and ivtx->ndof() >= mGoodVtxNdof and fabs(ivtx->z()) <= mGoodVtxZ) {
      if (index == 0) isPVgood = true;
      ++VtxGood;
    }
  }
  qEvtHdr.setVertices(recVtxs->size(),VtxGood);
  qEvtHdr.setPV(isPVgood,PVndof,PVx,PVy,PVz);
  
  //-------------- Rho ------------------------------------------------
  edm::Handle<double> rhoCalo;
  event.getByToken(mSrcCaloRho,rhoCalo);
  edm::Handle<double> rhoPF;
  event.getByToken(mSrcPFRho,rhoPF);
  qEvtHdr.setRho(*rhoCalo,*rhoPF);

  //-------------- Generator (incl. simulated PU) Info ----------------
  if (mIsMCarlo and mUseGenInfo) {
    edm::Handle<GenEventInfoProduct> hEventInfo;
    edm::Handle<vector<PileupSummaryInfo> > PupInfo;
    event.getByToken(mEventInfo, hEventInfo);
    if (hEventInfo->hasBinningValues()) qEvtHdr.setPthat(hEventInfo->binningValues()[0]);
    else qEvtHdr.setPthat(0);
  
    qEvtHdr.setWeight(hEventInfo->weight());
    event.getByToken(mSrcPU, PupInfo);
    int nbx = PupInfo->size();
    int ootpuEarly(0),ootpuLate(0),intpu(0);
    float Tnpv = 0.; // new variable for computing pileup weight factor for the event
  
    for(auto PUI = PupInfo->begin(); PUI != PupInfo->end(); ++PUI) {
      if (PUI->getBunchCrossing() < 0) {
        ootpuEarly += PUI->getPU_NumInteractions();      
      } else if (PUI->getBunchCrossing() > 0) {
        ootpuLate += PUI->getPU_NumInteractions();
      } else {
        intpu += PUI->getPU_NumInteractions();
        Tnpv += PUI->getTrueNumInteractions();
      }
    }
    qEvtHdr.setPU(nbx,ootpuEarly,ootpuLate,intpu);
    qEvtHdr.setTrPu(Tnpv);
  } else {
    qEvtHdr.setPthat(0);
    qEvtHdr.setWeight(0);
    qEvtHdr.setPU(0,0,0,0);
    qEvtHdr.setTrPu(0);
  }
  mEvent->setEvtHdr(qEvtHdr);
  
  //---------------- GenJets ------------------------------------------
  if (mIsMCarlo) {
    // Setting up handy handles
    edm::Handle<reco::GenJetCollection> genJets;
    event.getByToken(mGenJetsName,genJets);
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfos;
    event.getByToken(mJetFlavourInfosToken, theJetFlavourInfos );
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfosPhys;
    event.getByToken(mJetFlavourInfosTokenPhysicsDef, theJetFlavourInfosPhys );
    event.getByToken(mGenParticles, mGenParts);
    // Genjet loop
    auto igen = genJets->begin();
    auto j = theJetFlavourInfos->begin();
    auto k = theJetFlavourInfosPhys->begin();
    mGenJets.clear();
    mGenFlavour.clear();
    mGenFlavourHadr.clear();
    mGenFlavourPhys.clear();
    mGenBPt.clear();
    for (;igen!=genJets->end() and j!=theJetFlavourInfos->end() and k!=theJetFlavourInfosPhys->end();++igen,++j,++k) {
      if (fabs(igen->eta()) > mMaxEta) continue;

      auto aInfo = j->second;
      auto bInfo = k->second;

      mGenJets.push_back(igen->p4());
      mGenFlavour.push_back(aInfo.getPartonFlavour());
      mGenFlavourHadr.push_back(aInfo.getHadronFlavour());
      mGenFlavourPhys.push_back(bInfo.getPartonFlavour());
    }
    map<int,float> jet2bpt;
    // Loop through all prunder particles and then the jets
    for (auto gidx = 0u; gidx < mGenParts->size(); ++gidx) {
      auto &gPart = mGenParts->at(gidx);
      int absId = abs(gPart.pdgId());
      if (absId<100) continue; 
      if (is_bhadr(absId)) {
        int imin = -1;
        float rmin = -1.;
        std::tie(imin,rmin) = best_jet4prtn(gPart);
        if (rmin<refR and (jet2bpt.find(imin)==jet2bpt.end() or jet2bpt[imin]<gPart.pt()))
          jet2bpt[imin] = gPart.pt();
      }
    }
    // Fill the best b hadron pt
    for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
      if (jet2bpt.find(igen)==jet2bpt.end()) mGenBPt.push_back(-1.);
      else mGenBPt.push_back(jet2bpt[igen]);
    }
    // Hand-made flavor definitions for when the built-in methods fail
    if (mRedoPhysDef) {
      flavs_physdef();
      for (auto &res : mGenJetPhysFlav) {
        int igen = res.first;
        if (mGenFlavourPhys[igen]==0 and res.second!=0)
          mGenFlavourPhys[igen] = res.second;
      }
    }
    if (mRedoAlgoDef) {
      flavs_newalgodef(); 
      for (auto &res : mGenJetAlgoFlav) {
        int igen = res.first;
        if (mGenFlavour[igen]==0)
          mGenFlavour[igen] = res.second;
      }
    }
  }
  
  //---------------- Jets ---------------------------------------------
  // Uncertainties
  edm::ESHandle<JetCorrectorParametersCollection> PFJetCorParColl;
  if (mPFPayloadName != "" and !mIsPFJecUncSet) {
    iSetup.get<JetCorrectionsRecord>().get(mPFPayloadName,PFJetCorParColl);
    JetCorrectorParameters const& PFJetCorPar = (*PFJetCorParColl)["Uncertainty"];
    mPFUnc = new JetCorrectionUncertainty(PFJetCorPar);
    if (mPFJECUncSrc != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++) {
        JetCorrectorParameters *parchs = new JetCorrectorParameters(mPFJECUncSrc,mPFJECUncSrcNames[isrc]);
        JetCorrectionUncertainty *tmpUnc = new JetCorrectionUncertainty(*parchs);
        mPFUncSrc.push_back(tmpUnc);
      } // Loop: uncertainty sources 
    } // If uncertainty sources are given
    mIsPFJecUncSet = true;
  } // PF payload is given and uncertainty sources not set
  
  // PFJets
  edm::Handle<edm::View<pat::Jet> > patJets;
  event.getByToken(mPFJetsName,patJets);
  // PF Candidates (for beta calculus)
  edm::Handle<pat::PackedCandidateCollection> cands;
  event.getByToken(mCands, cands);

  // Partons within the jet
  vector< vector<int> > jet2pu;
  if (mAK4) {
    for (auto jet=patJets->begin(); jet!=patJets->end(); ++jet) jet2pu.emplace_back(vector<int>());

    // Pick the PF candidates removed by CHS (fromPV==0)
    for (auto cidx = 0u; cidx<cands->size(); ++cidx) {
      auto &c = cands->at(cidx);
      if (c.fromPV()!=0) continue;
      float dRMin = 999.;
      int bestjet = -1;
      // Find the jet that best matches this candidate
      for (auto ijet=patJets->begin(); ijet!=patJets->end(); ++ijet) {
        float dR = reco::deltaR(*ijet,c);
        if (dR<dRMin) {
          dRMin = dR;
          bestjet = ijet-patJets->begin();
        }
      }
      // If the candidate is closer than the jet radius to the jet axis, this is a PU particle for the selected jet
      if (dRMin<refR) jet2pu[bestjet].push_back(cidx);
    }
  }

  // QG stuff needs to be taken as a separate entity for MiniAOD
  edm::Handle<edm::ValueMap<float>> qglHandle, qgax2Handle, qgptdHandle;
  edm::Handle<edm::ValueMap<int>> qgmulHandle;
  if (mAK4) {
    event.getByToken(mQGLToken, qglHandle);
    event.getByToken(mQGAx2Token, qgax2Handle);
    event.getByToken(mQGMulToken, qgmulHandle);
    event.getByToken(mQGPtDToken, qgptdHandle);
  }
  
  // Jet loop
  int maxGenMatch = -1;
  for (auto ijet=patJets->begin(); ijet!=patJets->end(); ++ijet) {
    // Preselection
    if (!ijet->isPFJet()) continue;
    if (fabs(ijet->eta()) > mMaxEta) continue;
    int jetNo = int(ijet-patJets->begin());
    if (ijet->pt() < (jetNo<=2 ? mMinPFPtThirdJet : mMinPFPt)) continue;
    double scale = 1./(ijet->jecSetsAvailable() ? ijet->jecFactor(0) : 1.);

    // Keep track of pt and energy for PU stuff
    //double che = 0.0, poe = 0.0;
    double pue = 0.0; 
    // Track parameters by Juska. Not-so-useful for chs jets.
    int mpuTrk(0), mlvTrk(0), mjtTrk(0); // # of pile-up tracks & lead-vertex tracks & all tracks ## Juska

    if (mAK4) {
      // Loop through the PF candidates within the jet.
      //vector<double> used;
      for (auto pidx = 0u; pidx < ijet->numberOfDaughters(); ++pidx) {
        auto dtr = dynamic_cast<const pat::PackedCandidate*>(ijet->daughter(pidx));
        if (dtr->charge()!=0) {
          //che += dtr->energy();
          
          ++mjtTrk; 
          if (dtr->fromPV()==0) {
            // Note: dtr->pvAssociationQuality() is the modern alternative, but fromPV is the one used for CHS.
            // Still for some reason, not all fromPV==0 cases are removed. These events fit the old "betaStar" definition (not-from-PV).
            // Due to CHS, the trailing betaStar is a vanishing fraction (1/10k), so we don't store it anymore.
            //++mpuTrk;
            //used.push_back(dtr->energy());
          } else {
            ++mlvTrk;
          }
        }
      }
      // Loop through the pileup PF candidates within the jet.
      for (auto &pidx : jet2pu[jetNo]) {
        auto dtr = cands->at(pidx);
        // We take the charged. candidates that have not appeared before: these were removed by CHS
        if (dtr.charge()!=0) {
          ++mpuTrk;
          pue += dtr.energy();
          //if (std::find(used.begin(),used.end(),dtr.energy())==used.end())
          //  pue += dtr.energy();
        }
      }
    }

    QCDPFJet qcdJet;

    // The fraction of CHS-removed PU (defined similarly as the other fractions, w.r.t. unscaled jet energy).
    float bPrime = (pue/ijet->energy())*scale; 
    qcdJet.setBetaPrime(bPrime);
     
    // JEC uncertainty
    double unc(0.0);
    vector<float> uncSrc(0);
    if (mPFPayloadName != "") {
      mPFUnc->setJetEta(ijet->eta());
      mPFUnc->setJetPt(ijet->pt());
      unc = mPFUnc->getUncertainty(true);
    }
    if (mPFJECUncSrc != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++) {
        mPFUncSrc[isrc]->setJetEta(ijet->eta());
        mPFUncSrc[isrc]->setJetPt(ijet->pt());
        float unc1 = mPFUncSrc[isrc]->getUncertainty(true);
        uncSrc.push_back(unc1);
      }
    }

    qcdJet.setP4(ijet->p4());
    qcdJet.setCor(scale);
    qcdJet.setUnc(unc);
    qcdJet.setUncSrc(uncSrc);
    qcdJet.setArea(ijet->jetArea());
    
    // This is a mixture of PF specific and more generic functions.
    // Not entirely logical (e.g. cemf instead of electron),
    // but these are the tags used by the jet ID's.
    double nhf   = ijet->neutralHadronEnergyFraction(); // includes hf hadrons
    double nemf  = ijet->neutralEmEnergyFraction();     // = phf + hfemf 
    double chf   = ijet->chargedHadronEnergyFraction(); // only for barrel
    double cemf  = ijet->chargedEmEnergyFraction();     // = elf
    double muf   = ijet->muonEnergyFraction();
    double hf_hf = ijet->HFHadronEnergyFraction();
    double hf_phf= ijet->HFEMEnergyFraction();
    int hf_hm    = ijet->HFHadronMultiplicity();
    int hf_phm   = ijet->HFEMMultiplicity();
    int chm      = ijet->chargedHadronMultiplicity();
    int nhm      = ijet->neutralHadronMultiplicity();
    int phm      = ijet->photonMultiplicity();
    int elm      = ijet->electronMultiplicity();
    int mum      = ijet->muonMultiplicity();

    float abseta = fabs(ijet->eta());
    int nm       = ijet->neutralMultiplicity();
    int cm       = ijet->chargedMultiplicity();
    int npr      = cm + nm;
    // See, https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
    bool tightID = true;

    // For the UL campaigns, the Jet IDs are provided in a purely hard-coded format:
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID13TeVUL
    if (abseta <= 2.6)
      tightID = cemf<0.8 and cm>0 and chf>0 and npr>1 and nemf<0.9 and muf<0.8 and nhf<0.9;
    else if (abseta <= 2.7)
      tightID = cemf<0.8 and cm>0 and nemf<0.99 and muf<0.8 and nhf<0.9;
    else if (abseta <= 3.0)
      tightID = nemf<0.99 and nemf>0.01 and nm>1;
    else
      tightID = nemf<0.90 and nm>10 and nhf>0.2;
    // Provided for backwards compatibility
    bool looseID = tightID;
     
    qcdJet.setLooseID(looseID);
    qcdJet.setTightID(tightID);
    qcdJet.setFrac(chf,nhf,nemf,cemf,muf);
    qcdJet.setMulti(npr,chm,nhm,phm,elm,mum,cm);
    qcdJet.setHFFrac(hf_hf,hf_phf);
    qcdJet.setHFMulti(hf_hm,hf_phm);
     
    double hof   = ijet->hoEnergyFraction(); // Juska
    qcdJet.setVtxInfo(mpuTrk,mlvTrk,mjtTrk);
    qcdJet.setHO(hof);
    
    // Check what is available
    //auto pdisc = ijet->getPairDiscri();
    //for (auto &disc : pdisc) cout << "  " << disc.first << endl;

    // Jet flavour tagging discriminators
    qcdJet.pfCombinedCvsL_ = ijet->bDiscriminator("pfCombinedCvsLJetTags");
    qcdJet.pfCombinedCvsB_ = ijet->bDiscriminator("pfCombinedCvsBJetTags");

    qcdJet.pfDeepCSVb_  = ijet->bDiscriminator("pfDeepCSVJetTags:probb");
    qcdJet.pfDeepCSVc_  = ijet->bDiscriminator("pfDeepCSVJetTags:probc");
    qcdJet.pfDeepCSVl_  = ijet->bDiscriminator("pfDeepCSVJetTags:probudsg");
    qcdJet.pfDeepCSVbb_ = ijet->bDiscriminator("pfDeepCSVJetTags:probbb");

    qcdJet.pfBTag_JetProb_ = ijet->bDiscriminator("pfJetProbabilityBJetTags");
    qcdJet.pfBTag_CombInclSecVtxV2_ = ijet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    qcdJet.pfBTag_CombMVAV2_ = ijet->bDiscriminator("pfCombinedMVAV2BJetTags");

    float QGL   = -1;
    float QGAx2 = -1;
    int QGMul   = -1;
    float QGPtD = -1;
    // QGL variables only relevant for AK4
    if (mAK4) {
      edm::RefToBase<pat::Jet> jetRef(edm::Ref<edm::View<pat::Jet> >(patJets, jetNo));
      QGL = (*qglHandle)[jetRef];
      QGAx2 = (*qgax2Handle)[jetRef];
      QGMul = (*qgmulHandle)[jetRef];
      QGPtD = (*qgptdHandle)[jetRef];
    }
    qcdJet.setQGTagger(QGL,QGAx2,QGMul,QGPtD);
    qcdJet.SetPUJetId((ijet->hasUserFloat(mPFJetPUID) ? ijet->userFloat(mPFJetPUID) : -999));
    
    if (mIsMCarlo) {
      float partonFlavour=0;
      float hadronFlavour=0;
      float partonFlavourPhys=0;

      if (mUseGenInfo) {
        partonFlavour = ijet->partonFlavour();
        hadronFlavour = ijet->hadronFlavour();
        if (ijet->genParton() != NULL) partonFlavourPhys = ijet->genParton()->pdgId();
      }
    
      float rmin;
      int imin;
      std::tie(imin,rmin) = best_genjet(*ijet);
      if (imin>maxGenMatch) maxGenMatch = imin;

      if (imin!=-1 and rmin<refR) {
        qcdJet.setGen(imin,rmin);
        if (mUseGenInfo) {
          // Patch the PF jet flavours if these were not found earlier
          if (partonFlavour==0)     partonFlavour     = mGenFlavour[imin]; 
          if (partonFlavourPhys==0) partonFlavourPhys = mGenFlavourPhys[imin];
          if (hadronFlavour==0)     hadronFlavour     = mGenFlavourHadr[imin];
          // Patch the gen flavours if these were not found earlier
          if (mGenFlavour[imin]==0)     mGenFlavour[imin]     = partonFlavour;
          if (mGenFlavourPhys[imin]==0) mGenFlavourPhys[imin] = partonFlavourPhys;
          if (mGenFlavourHadr[imin]==0) mGenFlavourHadr[imin] = hadronFlavour;
        }
      } else {
        // Empty gen jet if no match
        qcdJet.setGen(-1,-1.);
      }
      qcdJet.setFlavour(partonFlavour,hadronFlavour,partonFlavourPhys);
    } else {
      // Empty gen jet for data
      qcdJet.setGen(-1,-1.);
      qcdJet.setFlavour(0,0,0);
    }
    qPFJets.push_back(qcdJet);
  } // jet loop
  if (qPFJets.size()<mMinNPFJets) return;

  sort(qPFJets.begin(),qPFJets.end(),sort_pfjets);
  mEvent->setPFJetsCHS(qPFJets);

  if (mIsMCarlo) {
    // Find the first jet with too small pt (but include all matched gen jets)
    unsigned starter = (maxGenMatch==-1 ? 0 : maxGenMatch+1);
    unsigned limit = mGenJets.size();
    for (unsigned igen = starter; igen<mGenJets.size(); ++igen) {
      auto &genP = mGenJets[igen];
      if (genP.pt()<mMinGenPt) {
        limit = igen;
        break;
      }
    }
    // Remove all the trailing gen info
    while (mGenJets.size()>limit) {
      mGenJets.pop_back();
      mGenBPt.pop_back();
      mGenFlavour.pop_back();
      mGenFlavourHadr.pop_back();
      mGenFlavourPhys.pop_back();
    }
    // Save only the interesting gen info
    mEvent->setGenJets(mGenJets);
    mEvent->setGenBPt(mGenBPt);
    mEvent->setGenFlavour(mGenFlavour);
    mEvent->setGenFlavourHadron(mGenFlavourHadr);
    mEvent->setGenFlavourPhysicsDef(mGenFlavourPhys);
  }
  
  //---------------- MET ----------------------------------------------
  edm::Handle<pat::METCollection> pfmett1;
  event.getByToken(mPFMETt1, pfmett1);
  // MET T1 is available by default in MINIAOD
  const pat::MET &mett1 = pfmett1->front();
  qPFMet_t1.setVar(mett1.et(),mett1.sumEt(),mett1.phi());
  //auto mettt1 = mett1.corP2(pat::MET::Type1);
  // MET T0 is obtained through a custom patch
  auto mett0 = mett1.corP2(pat::MET::RawChs);
  qPFMet_t0.setVar(mett0.pt(),mett1.corSumEt(pat::MET::RawChs),mett0.phi());
  // MET T0T1 is found using standard routines
  auto mett0t1 = mett1.corP2(pat::MET::Type01);
  qPFMet_t0t1.setVar(mett0t1.pt(),mett1.corSumEt(pat::MET::Type01),mett0t1.phi());
  mEvent->setPFMET(qPFMet_t1,qPFMet_t0,qPFMet_t0t1);
  //cout << "Def:  " << mett1.px() << " " << mett1.py() << endl;
  //cout << "T1:   " << mettt1.px << " " << mettt1.py << endl;
  //cout << "T0:   " << mett0.px << " " << mett0.py << endl;
  //cout << "T0T1: " << mett0t1.px << " " << mett0t1.py << endl;
  
 
  //-------------- fill the tree --------------------------------------
  mTree->Fill();
}

//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(ProcessedTreeProducerBTag);
