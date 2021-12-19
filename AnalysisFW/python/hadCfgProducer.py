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
  'process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",',
  '    ak4 = cms.PSet(',
  '        initialSeed = cms.untracked.uint32(81)',
  '    ),',
  ')',
  'process.randomEngineStateProducer = cms.EDProducer("RandomEngineStateProducer")',
]

logging = [
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  '#! Output and Log',
  '#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!',
  'process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )',
  'process.options.allowUnscheduled = cms.untracked.bool(True)',
]

# jettype: 'ak4', 'ak8', 'zb'
def producer(RunYear,era,Mode):
  if era=='mc' or era=='hw' or era=='mg':
    print("MC mode",era,"not implemented, exiting!")
    return
  add='' if era=='dt' else era
  add+='PreVFP' if len(Mode)!=2 else ''
  jettype='zb'
  fname="cfg/had"+RunYear+add+".py"
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
    # EDAnalyzer for PatJets
    f.write('process.load("PhysicsTools.PatAlgos.patSequences_cff")\n')
    f.write("process.ak4 =  cms.EDAnalyzer('ProcessedHadrons',\n")
    f.write("  isMCarlo        = cms.untracked.bool("+("False" if Mode=="dt" else "True")+"),\n")
    f.write("  filterName      = filters,\n")
    f.write("  triggerName     = triggers,\n")
    f.write(")\n\n")
    f.write("process.path = cms.Path(")
    f.write("process.ak4*\n")
    f.write("process.randomEngineStateProducer)\n")
    f.write("\n")
    for line in logging:
      f.write(line+'\n')

# Produce the run files
for RunYear in ['16','17','18']:
  #for MC in ['py','nu','hw','mg']:
  for MC in ['nu']:
    producer(RunYear,MC,'mc')
    if RunYear=='16':
      producer(RunYear,MC,'mcPreVFP')
  producer(RunYear,'dt','dt')
