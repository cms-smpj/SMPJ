//Author K. Kousouris
//Modified by: S. Ganguly

#ifndef QCDJet_h
#define QCDJet_h
#include "DataFormats/JetReco/interface/Jet.h"
//-------- Generic Jet class for QCD analyses ---------------
class QCDJet 
{
   public:
     typedef reco::Particle::LorentzVector LorentzVector;
     //------------ Constructor ------------------------------
     QCDJet() {}
     //------------ Destructor -------------------------------
     ~QCDJet() {}
     //------------ Sett methods -----------------------------
  //   void clear() {pfParticles_.clear();}
     void setP4(LorentzVector fP4)               { P4_ = fP4; }
     void setGen(int fIdx, float fgenR)          { genIdx_ = fIdx; genR_ = fgenR; }
     void setCor(float fCor)                     { cor_  = fCor; }
     void setUnc(float fUnc)                     { unc_  = fUnc; }
     void setUncSrc(std::vector<float> fUncSrc)  { uncSrc_ = fUncSrc; }
     void setJecLabels(std::vector<double> fJecLabels) { jecLabels_ = fJecLabels; }
     void setArea(float fArea)                   { area_ = fArea; }
     void setLooseID(bool fLooseID)              { looseID_ = fLooseID; } 
     void setTightID(bool fTightID)              { tightID_ = fTightID; }
//     void setPFParticles(std::vector<LorentzVector> fpfFParticles) {pfParticles_ = fpfFParticles;}
     //------------ Get methods ------------------------------
     const LorentzVector& p4()    const {return P4_;}
     float pt()                   const {return P4_.pt()/cor_;}
     int genidx()                 const {return genIdx_;}
     float genR()                 const {return genR_;} 
     float ptCor()                const {return P4_.pt();}
     float e()                    const {return P4_.energy()/cor_;}
     float eCor()                 const {return P4_.energy();}
     float eta()                  const {return P4_.eta();}
     float y()                    const {return P4_.Rapidity();}
     float phi()                  const {return P4_.phi();}
     float mass()                 const {return P4_.mass();}
     float cor()                  const {return cor_;}
     float unc()                  const {return unc_;} 
     float uncSrc(int i)          const {return uncSrc_[i];}
     double jecLabels(int i)      const {return jecLabels_[i];}
     float area()                 const {return area_;} 
     bool  looseID()              const {return looseID_;}
     bool  tightID()              const {return tightID_;}
  //   int  nParticles()            const {return pfParticles_.size();}
  //   const LorentzVector& getPFParticles(int i) const {return pfParticles_[i];}
   private:
     //------ jet 4-momentum vector------------------
     LorentzVector P4_;
     //------ matched genjet index-------------------
     int genIdx_;
     //------ matching radius -----------------------
     float genR_;
     //------ jec factor ----------------------------
     float cor_;
     // ----- All components of JEC Factor ----------
     std::vector<double> jecLabels_;
     //------ jec uncertainty -----------------------
     float unc_;
     //------ jec uncertainty sources ---------------
     std::vector<float> uncSrc_;
     //------ jet area ------------------------------
     float area_;
     //------ loose ID flag -------------------------
     bool  looseID_;
     //------ tight ID flag -------------------------
     bool  tightID_;
 };
#endif
