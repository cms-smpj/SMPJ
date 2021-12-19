# This is a handy handle for producing crab parameter files
# -*- coding: utf-8 -*-

#########
# Lists #
#########

# Global tags
from gtaglists import *
# MET filters are fetched
from filterlists import *
# Triggers are fetched
from triggerlists import *

#############
# Main text #
#############

importer = [
  '# -*- coding: utf-8 -*-',
  'import FWCore.ParameterSet.Config as cms\n', 
  'from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets',
  'from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters',
  'from RecoJets.JetProducers.PFJetParameters_cfi import *',
  'from RecoJets.JetProducers.GenJetParameters_cfi import *',
  'from RecoJets.JetProducers.AnomalousCellParameters_cfi import *',
  'from RecoJets.JetProducers.QGTagger_cfi import QGTagger',
  'from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *',
  'from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets',
  'from PhysicsTools.PatAlgos.tools.jetTools import *',
  'from PhysicsTools.PatAlgos.patSequences_cff import *',
  'from PhysicsTools.PatAlgos.patTemplate_cfg import *',
  'from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos',
  'import sys\n',
  'from gtaglists import *',
  'from triggerlists import *',
  'from filterlists import *',
  'from filelists import *\n',
]

process = [
  'process = cms.Process("Ntuplizer")',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  '#! Conditions',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  'process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")',
  'from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag',
  'process.load("Configuration.EventContent.EventContent_cff")',
  'process.load("Configuration.StandardSequences.GeometryRecoDB_cff")',
  'process.load("Configuration.StandardSequences.MagneticField_38T_cff")',
  'process.load("RecoJets.Configuration.GenJetParticles_cff")',
  'process.load("RecoJets.Configuration.RecoGenJets_cff")',
  'process.load("RecoJets.JetProducers.TrackJetParameters_cfi")',
  'process.load("RecoJets.JetProducers.PileupJetIDParams_cfi")',
  'process.load("PhysicsTools.PatAlgos.patSequences_cff")\n',
  '##-------------------- Import the JEC services -----------------------',
  'process.load("JetMETCorrections.Configuration.DefaultJEC_cff")\n',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  '#! Input',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
]

inputs = [
  'process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5000))\n',
  'process.source = cms.Source("PoolSource", fileNames = inFiles )\n',
  'process.load("CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi")',
  'process.load("CommonTools.RecoAlgos.HBHENoiseFilter_cfi")\n',
  'process.HBHENoiseFilterResultProducerNoMinZ = process.HBHENoiseFilterResultProducer.clone(minZeros = cms.int32(99999))\n',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  '#! Services',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  'process.load("FWCore.MessageLogger.MessageLogger_cfi")',
  'process.MessageLogger.cerr.FwkReport.reportEvery = 100',
  'process.MessageLogger.cerr.threshold = cms.untracked.string("ERROR")',
  'process.MessageLogger = cms.Service("MessageLogger",',
  '        destinations = cms.untracked.vstring(',
  '                "errors"',
  '        ),',
  '        errors = cms.untracked.PSet(',
  '                threshold = cms.untracked.string("ERROR")',
  '        ),',
  ')',
  'process.load("CommonTools.UtilAlgos.TFileService_cfi")',
  'process.TFileService.fileName=cms.string("DATA.root")',
]

flavors = [
  'process.load("PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi")',
  'process.selectedHadronsAndPartons.particles = "prunedGenParticles"',
  'process.selectedHadronsAndPartons.partonMode = cms.string("Auto")',
  'process.selectedHadronsAndPartons.src = "generator"',
  'process.physDefHadronsAndPartons = process.selectedHadronsAndPartons.clone( fullChainPhysPartons = cms.bool(False) )\n',
  'process.jetFlavs = ak4JetFlavourInfos.clone( jets = gjetname,',
  '                                             partons = cms.InputTag("selectedHadronsAndPartons","physicsPartons") )',
  'process.jetFlavsPD = process.jetFlavs.clone( partons = cms.InputTag("physDefHadronsAndPartons","physicsPartons") )',
]

logging = [
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  '#! Output and Log',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  'process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )',
  'process.options.allowUnscheduled = cms.untracked.bool(True)',
]

# jettype: 'ak4', 'ak8', 'zb'
def producer(RunYear,era,jettype,Mode):
  add='PreVFP' if len(Mode)!=2 else ''
  fname="cfg/"+jettype+RunYear+('' if era=='dt' else era)+add+".py"
  with open(fname, 'w') as f:
    # Import lines
    for line in importer:
      f.write(line+'\n')
    f.write('\n')
    # Process.load lines
    for line in process:
      f.write(line+'\n')
    f.write('\n')
    # Global tag is set
    GTag = GTags[RunYear][Mode]
    Mode=Mode[:2]
    f.write('process.GlobalTag.globaltag = "'+GTag+'"\n')
    if GTag=='DEFAULT':
      f.write('process.GlobalTag = GlobalTag(process.GlobalTag, "auto:run2_' + ('data' if Mode=='dt' else 'mc') + '" )\n')
    f.write('\n')
    # Triggers:
    ## Active
    f.write('triggers=')
    if Mode=='dt':
      f.write('trglist["'+RunYear+'"]["dt"]["'+jettype+'"]\n\n')
    else:
      f.write('trglist["'+RunYear+'"]["mc"]["'+jettype+'"]\n\n')
    ## Followed
    f.write('follows=')
    if Mode=='dt' and jettype!='zb':
      f.write('trglist["'+RunYear+'"]["dt"]["ak'+('4' if jettype=='ak8' else '8')+'"]\n\n')
    else:
      f.write('cms.vstring()\n\n')
    # MET filters
    f.write('filters=fltlist["'+RunYear+'"]["'+Mode+'"]\n\n')
    # Testing input files
    f.write('inFiles=')
    if RunYear=='16':
      if jettype=='zb':
        f.write('ZBD16\n')
      elif Mode=='dt':
        f.write('JHTG16\n')
      elif MC=='py':
        f.write('QCD16Mor17P8M1\n')
      elif MC=='hw':
        f.write('QCD16Mor17HS1\n')
      elif MC=='mg':
        f.write('QCD16MG\n')
      elif MC=='nu':
        f.write('QCD16NU\n')
      else:
        f.write('cms.untracked.vstring()\n')
    elif RunYear=='17':
      if jettype=='zb':
        f.write('ZBDUL17\n')
      elif Mode=='dt':
        f.write('JHTDUL17\n')
      elif MC=='py':
        f.write('QCDUL17P8CP5\n')
      elif MC=='nu':
        f.write('QCDUL17NU\n')
      elif MC=='hw':
        f.write('QCD17HS1\n')
      elif MC=='mg':
        f.write('QCD17MG\n')
      else:
        f.write('cms.untracked.vstring()\n')
    elif RunYear=='18':
      if jettype=='zb':
        f.write('ZBAUL18\n')
      elif Mode=='dt':
        f.write('JHTAUL18\n')
      elif MC=='py':
        f.write('QCDUL18P8CP5\n')
        #f.write('QCD18P8CP5\n')
      elif MC=='mg':
        f.write('QCD18MG\n')
      else:
        f.write('cms.untracked.vstring()\n')
    else:
      f.write('cms.untracked.vstring()\n')
    # Remaining input stuff   
    for line in inputs:
      f.write(line+'\n')
    # Jet collections
    f.write("jetname='slimmedJets"+("AK8" if jettype=='ak8' else "")+"'\n")
    f.write("gjetname='slimmedGenJets"+("AK8" if jettype=='ak8' else "")+"'\n\n")
    # QGL: Other options: see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
    if jettype!='ak8':
      f.write("process.load('RecoJets.JetProducers.QGTagger_cfi')\n")
      f.write("process.QGTagger.srcJets = cms.InputTag(jetname)\n")
      f.write("process.QGTagger.jetsLabel  = cms.string('QGL_AK4PFchs')\n\n")
    if Mode=='mc':
      for line in flavors:
        f.write(line+'\n')
    # EDAnalyzer for PatJets
    f.write('process.load("PhysicsTools.PatAlgos.patSequences_cff")\n')
    f.write("process.ak"+("8" if jettype=='ak8' else "4")+" =  cms.EDAnalyzer('ProcessedTreeProducerBTag',\n")
    f.write("  ## jet collections ###########################\n")
    f.write("  pfjetschs       = cms.InputTag(jetname),\n")
    f.write("  pfchsjetpuid    = cms.string('pileupJetId:fullDiscriminant'),\n")
    f.write("  runYear         = cms.untracked.string('20"+RunYear+"'),\n")
    f.write("  ## MET collection ####\n")
    f.write("  pfmetT1         = cms.InputTag('slimmedMETs'),\n")
    f.write("  genjets         = cms.untracked.InputTag(gjetname),\n")
    f.write("  ## database entry for the uncertainties ######\n")
    f.write("  PFPayloadName   = cms.string(''),\n")
    f.write("  jecUncSrc       = cms.untracked.string(''),\n")
    f.write("  jecUncSrcNames  = cms.vstring(''),\n")
    f.write("  ## set the conditions for good Vtx counting ##\n")
    f.write("  offlineVertices = cms.InputTag('offlineSlimmedPrimaryVertices'),\n")
    f.write("  beamSpot        = cms.InputTag('offlineBeamSpot'),\n")
    f.write("  goodVtxNdof     = cms.double(4),\n")
    f.write("  goodVtxZ        = cms.double(24),\n")
    f.write("  ## rho #######################################\n")
    f.write("  srcCaloRho      = cms.InputTag('fixedGridRhoFastjetAllCalo'),\n")
    f.write("  srcPFRho        = cms.InputTag('fixedGridRhoFastjetAll'),\n")
    f.write("  srcPULabel      = cms.untracked.InputTag('slimmedAddPileupInfo'),\n")
    f.write("  ## preselection cuts #########################\n")
    f.write("  maxEta          = cms.double(5.0),\n")
    f.write("  minPFPt         = cms.double(15.0),\n")
    f.write("  minPFPtThirdJet = cms.double(5.0),\n")
    f.write("  minNPFJets      = cms.uint32("+repr(0 if jettype=='ak8' else 1)+"),\n")
    f.write("  minGenPt        = cms.untracked.double(15.0),\n")
    f.write("  isMCarlo        = cms.untracked.bool("+("False" if Mode=="dt" else "True")+"),\n")
    f.write("  useGenInfo      = cms.untracked.bool("+("False" if Mode=="dt" else "True")+"),\n")
    f.write("  AK4             = cms.untracked.bool("+("False" if jettype=='ak8' else "True")+"),\n")
    f.write("  ZB              = cms.untracked.bool("+("True" if jettype=='zb' else "False")+"),\n")
    f.write("  ## trigger ###################################\n")
    f.write("  printTriggerMenu= cms.untracked.bool(False),\n")
    f.write("  discardFilter   = cms.untracked.bool(False),\n")
    f.write("  trigSimple      = cms.untracked.bool(True),\n")
    f.write("  trigObjs        = cms.untracked.bool(False),\n")
    f.write("  filterName      = filters,\n")
    f.write("  triggerName     = triggers,\n")
    f.write("  triggerFollow   = follows,\n")
    f.write("  triggerHLTObjs  = cms.untracked.InputTag('"+("selected" if RunYear=='16' else "slimmed")+"PatTrigger'),\n")
    f.write("  triggerL1Objs   = cms.untracked.InputTag('caloStage2Digis','Jet'),\n")
    f.write("  triggerL1HTObjs = cms.untracked.InputTag('caloStage2Digis','EtSum'),\n")
    f.write("  #triggerAllObjs = cms.untracked.InputTag('gtStage2Digis','GlobalAlgBlk'),\n")
    f.write("  prescales       = cms.InputTag('patTrigger'),\n")
    f.write("  prescalesL1Min  = cms.InputTag('patTrigger','l1min'),\n")
    f.write("  prescalesL1Max  = cms.InputTag('patTrigger','l1max'),\n")
    if Mode=="mc":
      f.write("  ## gen ##############################\n")
      f.write("  EventInfo       = cms.untracked.InputTag('generator'),\n")
      f.write("  GenParticles    = cms.untracked.InputTag('prunedGenParticles'),\n")
      f.write("  jetFlavInfos    = cms.untracked.InputTag('jetFlavs'),\n")
      f.write("  jetFlavInfosPD  = cms.untracked.InputTag('jetFlavsPD'),\n")
      if MC!='py':
        f.write("  mcType          = cms.untracked.int32(1),\n")
    else:
      f.write("  HBHENoiseFilterResultNoMinZLabel = cms.untracked.InputTag('HBHENoiseFilterResultProducerNoMinZ', 'HBHENoiseFilterResult'),\n")
    f.write("  saveWeights     = cms.bool(False)\n")
    f.write(")\n\n")
    f.write("process.path = cms.Path(")
    if jettype!='ak8':
      f.write("process.QGTagger*\n                        ")
    if Mode=="dt":
      f.write("process.HBHENoiseFilterResultProducerNoMinZ*\n                        ")
    else:
      f.write("process.selectedHadronsAndPartons*\n                        ")
      f.write("process.jetFlavs*\n                        ")
      f.write("process.physDefHadronsAndPartons*\n                        ")
      f.write("process.jetFlavsPD*\n                        ")
    f.write("process.ak"+("8" if jettype=='ak8' else "4")+")\n\n")
    for line in logging:
      f.write(line+'\n')

# Produce the run files
for RunYear in ['16','17','18']:
  for MC in ['py','nu','hw','mg']:
    producer(RunYear,MC,'ak4','mc')
    producer(RunYear,MC,'ak8','mc')
    if RunYear=='16':
      producer(RunYear,MC,'ak4','mcPreVFP')
      producer(RunYear,MC,'ak8','mcPreVFP')
  producer(RunYear,'dt','zb','dt')
  producer(RunYear,'dt','ak4','dt')
  producer(RunYear,'dt','ak8','dt')
