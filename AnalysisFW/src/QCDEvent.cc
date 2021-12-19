#include "SMPJ/AnalysisFW/interface/QCDEvent.h"
//---------------------------------------------------
QCDEvent::QCDEvent()
{
  HLTObj_.clear();
  L1Obj_.clear();
  PFJetsCHS_.clear();
  GenJets_.clear();
}
//---------------------------------------------------
QCDEvent::~QCDEvent() {}
//---------------------------------------------------
void QCDEvent::setPFJetsCHS(const std::vector<QCDPFJet>& fPFJetsCHS)
{
  PFJetsCHS_.clear();
  for(unsigned i=0;i<fPFJetsCHS.size();i++) {
    PFJetsCHS_.push_back(fPFJetsCHS[i]);
  }
}
//---------------------------------------------------
void QCDEvent::setGenJets(const std::vector<LorentzVector>& fGenJets)
{
  GenJets_.clear();
  for(unsigned i=0;i<fGenJets.size();i++) {
    GenJets_.push_back(fGenJets[i]);
  }
}
//---------------------------------------------------
void QCDEvent::setL1Obj(const std::vector<std::vector<LorentzVector> >& fL1Obj)
{
  L1Obj_.clear();
  for(unsigned i=0;i<fL1Obj.size();i++) {
    std::vector<LorentzVector> vv;
    for(unsigned j=0;j<fL1Obj[i].size();j++) {
      vv.push_back(fL1Obj[i][j]);
    }
    L1Obj_.push_back(vv);
  }
}
//---------------------------------------------------
void QCDEvent::setHLTObj(const std::vector<std::vector<LorentzVector> >& fHLTObj)
{
  HLTObj_.clear();
  for(unsigned i=0;i<fHLTObj.size();i++) {
    std::vector<LorentzVector> vv;
    for(unsigned j=0;j<fHLTObj[i].size();j++) {
      vv.push_back(fHLTObj[i][j]);
    }
    HLTObj_.push_back(vv);
  }
}
//---------------------------------------------------
int QCDEvent::nGoodJets(int unc, int id, float ymax, float ptmin, std::vector<QCDJet> jets)
{
  // unc defines the uncertainty
  // id defines the jet id
  // ymax defines the maximum rapidity
  // ptmin defines the minimum jet pt
  int sign(0),counter(0);
  if (unc > 0)
    sign = 1;
  if (unc < 0)
    sign = -1;
  for(unsigned i=0;i<jets.size();i++) {
    bool passID(true);
    if (id == 1 && !jets[i].looseID())
      passID = false;
    if (id == 2 && !jets[i].tightID())
      passID = false;
    if (passID) {
      if (fabs(jets[i].y()) <= ymax && jets[i].ptCor()*(1+sign*jets[i].unc()) >= ptmin)
        counter++;
    }
  }
  return counter;
}
//---------------------------------------------------
float QCDEvent::genmjj()
{
  if (GenJets_.size() < 2)
    return 0.0;
  else {
    return (GenJets_[0]+GenJets_[1]).mass();
  }
}
//---------------------------------------------------
float QCDEvent::pfmjj()
{
  if (PFJetsCHS_.size() < 2)
    return 0.0;
  else {
    const LorentzVector& P0 = PFJetsCHS_[0].p4();
    const LorentzVector& P1 = PFJetsCHS_[1].p4();
    return (P0+P1).mass();
  }
}
//---------------------------------------------------
float QCDEvent::pfchsmjjcor(int k)
{
  int sign(0);
  if (PFJetsCHS_.size() < 2)
    return 0.0;
  else {
    if (k>0)
      sign = 1;
    if (k<0)
      sign = -1;
    const LorentzVector& P0 = PFJetsCHS_[0].p4();
    const LorentzVector& P1 = PFJetsCHS_[1].p4();
    double unc0 = PFJetsCHS_[0].unc();
    double unc1 = PFJetsCHS_[1].unc();
    return ((1+sign*unc0)*P0+(1+sign*unc1)*P1).mass();
  }
}
//---------------------------------------------------
float QCDEvent::pfchsmjjcor(int k,int src)
{
  int sign(0);
  if (PFJetsCHS_.size() < 2)
    return 0.0;
  else {
    if (k>0)
      sign = 1;
    if (k<0)
      sign = -1;
    const LorentzVector& P0 = PFJetsCHS_[0].p4();
    const LorentzVector& P1 = PFJetsCHS_[1].p4();
    double unc0 = PFJetsCHS_[0].uncSrc(src);
    double unc1 = PFJetsCHS_[1].uncSrc(src);
    return ((1+sign*unc0)*P0+(1+sign*unc1)*P1).mass();
  }
}
