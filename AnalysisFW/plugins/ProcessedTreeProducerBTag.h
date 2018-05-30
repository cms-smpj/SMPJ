#ifndef ProcessedTreeProducerBTag_h
#define ProcessedTreeProducerBTag_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "SMPJ/AnalysisFW/interface/QCDEvent.h"
#include "SMPJ/AnalysisFW/interface/QCDEventHdr.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJet.h"
#include "SMPJ/AnalysisFW/interface/QCDMET.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"

#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/PatCandidates/interface/IsolatedTrack.h"

//Hadron level definition
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"              
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace edm;
using namespace reco;
using namespace std;
using namespace trigger;

class ProcessedTreeProducerBTag : public edm::EDAnalyzer
{
  public:
    typedef reco::Particle::LorentzVector LorentzVector;
    explicit ProcessedTreeProducerBTag(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    virtual ~ProcessedTreeProducerBTag();
  private:
    void buildTree();
    static bool sort_pfjets(QCDPFJet j1, QCDPFJet j2) {
      return j1.ptCor() > j2.ptCor();
    }
    unsigned genorder(double pt, Handle<GenJetCollection> gjets, unsigned blw, unsigned abv) {
      unsigned nxt = (abv+blw)/2;
      if (nxt==blw) return nxt;
      double nxtpt = gjets->at(nxt).p4().Pt();
      if (nxtpt<=pt)
        return genorder(pt,gjets,blw,nxt);
      else
        return genorder(pt,gjets,nxt,abv);
    }
    unsigned partonorder(double pt, edm::Handle<reco::GenParticleCollection> prtns, unsigned blw, unsigned abv)     {
      unsigned nxt = (abv+blw)/2;
      if (nxt==blw) return nxt;
      double nxtpt = prtns->at(nxt).p4().Pt();
      if (nxtpt>pt)
        return partonorder(pt,prtns,blw,nxt);
      else
        return partonorder(pt,prtns,nxt,abv);
    }

    //---- configurable parameters --------
    float  mULimCEF; // Not to be set by the user
    float  mULimNEF; // Not to be set by the user
    float  mULimNHF; // Not to be set by the user
    float  mLLimNEF; // Not to be set by the user
    float  mLLimNHF; // Not to be set by the user
    bool   mSaveWeights; // Not to be set by the user
    bool   mAK4;
    bool   mPrintTriggerMenu;
    bool   mIsPFJecUncSet; // Not to be set by the user
    bool   mUseLegacyTag; // Not to be set by the user
    int    mGoodVtxNdof;
    double mGoodVtxZ;
    int    mMinNPFJets;
    double mMinPFPt,mMinPFPtThirdJet,mMinGenPt,mMaxY,mMinJJMass;
    string mPFPayloadName;
    string mRunYear; 
    string mPFJetPUID;

    // unc file for CHS jet //
    string                                   mPFJECUncSrc;
    vector<string>                           mPFJECUncSrcNames;
    edm::EDGetTokenT<reco::VertexCollection> mOfflineVertices;
    edm::EDGetTokenT<reco::BeamSpot>         mBeamSpot;
    edm::EDGetTokenT<edm::View<pat::Jet> >   mPFJetsName;
    edm::EDGetTokenT<double>                 mSrcCaloRho;
    edm::EDGetTokenT<double>                 mSrcPFRho;
    // MET //
    edm::EDGetTokenT<pat::METCollection> mPFMETt1;
    edm::EDGetTokenT<pat::METCollection> mPFMETt0pc;
    edm::EDGetTokenT<pat::METCollection> mPFMETt0pct1;
    // GEN //
    bool                                                     mIsMCarlo;
    bool                                                     mUseGenInfo;
    int                                                      mMCType;
    edm::EDGetTokenT<GenJetCollection>                       mGenJetsName;
    edm::EDGetTokenT<reco::GenParticleCollection>            mGenParticles;
    edm::EDGetTokenT<GenEventInfoProduct>                    mEventInfo;
    edm::EDGetTokenT<std::vector<PileupSummaryInfo> >        mSrcPU;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosToken;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosTokenPhysicsDef;
    edm::EDGetTokenT<GenEventInfoProduct>                    mGenEvtInfoToken;
    edm::Handle<GenEventInfoProduct>                         mGenEvtInfo;
    // TRIGGER // 
    std::string                                              mProcessName;
    std::vector<std::string>                                 mTriggerNames;
    std::vector<unsigned int>                                mTriggerIndex;
    edm::EDGetTokenT<edm::TriggerResults>                    mTriggerResultsTag;
    edm::EDGetTokenT<trigger::TriggerEvent>                  mTriggerEventTag;
    edm::Handle<edm::TriggerResults>                         mTriggerResultsHandle;
    edm::Handle<trigger::TriggerEvent>                       mTriggerEventHandle;
    edm::EDGetTokenT<edm::TriggerResults>                    mTriggerBits;
    edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> mTriggerObjects;
    edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescales;
    edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Min;
    edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Max;
    TH1F *mTriggerPassHisto,*mTriggerNamesHisto;
    // CORRECTORS //
    JetCorrectionUncertainty *mPFUnc;
    std::vector<JetCorrectionUncertainty*> mPFUncSrc;
//    edm::EDGetTokenT<pat::IsolatedTrackCollection> mIsolatedTracks;
    // MISC //
    edm::EDGetTokenT<pat::PackedCandidateCollection> mCands;
    edm::EDGetTokenT<edm::ValueMap<float>> mQGToken;
    HLTConfigProvider nHltConfig;

    edm::Service<TFileService> fs;
    TTree *mTree;
    // TREE variables //
    QCDEvent *mEvent;

    int getMatchedPartonGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);
    int getMatchedHadronGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);

    HLTPrescaleProvider hltPrescale_;
};

#endif
