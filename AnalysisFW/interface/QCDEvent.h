//Author K. Kousouris
//Modified by: S. Ganguly

#ifndef QCDEvent_h
#define QCDEvent_h
#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "SMPJ/AnalysisFW/interface/QCDMET.h"
#include "SMPJ/AnalysisFW/interface/QCDCaloJet.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJet.h"
#include "SMPJ/AnalysisFW/interface/QCDEventHdr.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include <vector>

using std::vector;

class QCDEvent
{
    public:
      typedef reco::Particle::LorentzVector LorentzVector;
      //------------ Constructor ------------------------------
      QCDEvent();
      //------------ Destructor -------------------------------
      ~QCDEvent();
      //------------ Set methods ------------------------------
      void setPFMET(const QCDMET& fPFMET,const QCDMET& fPFMETt0, const QCDMET& fPFMETt0t1) {
        PFMet_ = fPFMET;
        PFMetT0_ = fPFMETt0;
        PFMetT0T1_ = fPFMETt0t1;
      }
      void setEvtHdr(const QCDEventHdr& fEvtHdr)                  {EvtHdr_ = fEvtHdr;}
      void setPFJetsCHS(const vector<QCDPFJet>& fPFJetsCHS);

      void setGenJets(const vector<LorentzVector>& fGenJets);
      void setL1Obj(const vector<vector<LorentzVector> >& fL1Obj);
      void setHLTObj(const vector<vector<LorentzVector> >& fHLTObj);
      void setPrescales(const vector<int>& fPreL1, const vector<int>& fPreHLT) {L1Prescale_ = fPreL1; HLTPrescale_ = fPreHLT;}
      void setTrigDecision(const vector<int>& fTrigDecision) {TriggerDecision_ = fTrigDecision;}
      void setFltDecision(const vector<int>& fFltDecision) {FilterDecision_ = fFltDecision;}

      void setGenFlavour(          const vector<int>& GenFlavour) {                genFlavour_= GenFlavour;}
      void setGenFlavourHadron(    const vector<int>& GenFlavourHadron) {          genFlavourHadron_= GenFlavourHadron;}
      void setGenFlavourPhysicsDef(const vector<int>& GenPartonFlavourPhysicsDef) {genFlavourPartonPhysicsDef_= GenPartonFlavourPhysicsDef;}
      void setGenBPt(              const vector<float>& fGenBPt) {                 genBPt_ = fGenBPt; }

      //------------ Get methods -------------------------------
      unsigned int nTriggers()                         const {return TriggerDecision_.size();}
      unsigned int nL1Obj(int i)                       const {return L1Obj_[i].size();}
      unsigned int nHLTObj(int i)                      const {return HLTObj_[i].size();}
      unsigned int nPFJetsCHS()                        const {return PFJetsCHS_.size();}
      unsigned int nGenJets()                          const {return GenJets_.size();}
      int nGoodJets(int unc, int id, float ymax, float ptmin, vector<QCDJet> jets);
      int fired(int i)                                 const {return TriggerDecision_[i];}
      int preL1(int i)                                 const {return L1Prescale_[i];}
      int preHLT(int i)                                const {return HLTPrescale_[i];}

      int GenJetFlavour(int i)                         const {return genFlavour_[i];}
      int GenHadronJetFlavour(int i)                   const {return genFlavourHadron_[i];}
      int GenPartonPhysicsDefJetFlavour(int i)         const {return genFlavourPartonPhysicsDef_[i];}

      float pfmjj();
      float genmjj();
      float pfchsmjjcor(int unc);
      float pfchsmjjcor(int unc,int src);
      float pfmjjgen();
      const QCDMET&        pfmet()                     const {return PFMet_;}
      const LorentzVector& hltobj(int itrig, int iobj) const {return (HLTObj_[itrig])[iobj];}
      const LorentzVector& l1obj(int itrig, int iobj)  const {return (L1Obj_[itrig])[iobj];}
      const LorentzVector& genjet(int i)               const {return GenJets_[i];}
      const QCDPFJet&      pfjetchs(int i)             const {return PFJetsCHS_[i];}
      const QCDEventHdr&   evtHdr()                    const {return EvtHdr_;}

      const vector<vector<LorentzVector>>& HLTObj() const {return HLTObj_;}
      const vector<LorentzVector>& hltObjsForPath(int i) const {return HLTObj_[i];}

    private:
      //---- event header (contains all the event info) --------------
      QCDEventHdr                              EvtHdr_;
      //---- PF met object -------------------------------------------
      QCDMET                                   PFMet_; // T1, implicit
      QCDMET                                   PFMetT0_;
      QCDMET                                   PFMetT0T1_;
      //---- trigger decision vector ---------------------------------
      vector<int>                         TriggerDecision_;
      //---- filter decision vector ---------------------------------
      vector<int>                         FilterDecision_;
      //---- L1 prescale vector --------------------------------------
      vector<int>                         L1Prescale_;
      //---- HLT prescale vector -------------------------------------
      vector<int>                         HLTPrescale_;
      //---- HLT objects ---------------------------------------------
      vector<vector<LorentzVector> > HLTObj_;
      //---- L1 objects ----------------------------------------------
      vector<vector<LorentzVector> > L1Obj_;
      //---- Genjets -------------------------------------------------
      vector<LorentzVector>               GenJets_;
      //---- PFJetsCHS -----------------------------------------------
      vector<QCDPFJet>                    PFJetsCHS_;
      vector<int>  genFlavour_;
      vector<int>  genFlavourHadron_;
      vector<int>  genFlavourPartonPhysicsDef_;
      vector<float> genBPt_;
};
#endif
