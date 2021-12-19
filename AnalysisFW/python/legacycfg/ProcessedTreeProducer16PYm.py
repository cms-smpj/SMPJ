# -*- coding: utf-8 -*-

import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.patSequences_cff import *
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from RecoJets.JetProducers.QGTagger_cfi import QGTagger
from filelists import *
from filterlists import *

import FWCore.ParameterSet.Config as cms

process = cms.Process("Ntuplizer")
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Conditions
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoJets.Configuration.RecoGenJets_cff')
process.load('RecoJets.JetProducers.TrackJetParameters_cfi')
process.load('RecoJets.JetProducers.PileupJetIDParams_cfi')
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.GlobalTag.globaltag = "80X_mcRun2_asymptotic_2016_TrancheIV_v6"

##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10000))

triggers = cms.vstring()
follows = cms.vstring()
inFiles = QCD16Mor17P8M1
filters = fltlist['16']['mc']

ak4flag = False
zbflag = False

process.source = cms.Source("PoolSource", fileNames = inFiles )

process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

process.HBHENoiseFilterResultProducerNoMinZ = process.HBHENoiseFilterResultProducer.clone(minZeros = cms.int32(99999))

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = cms.untracked.string("ERROR")
process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring(
                'errors'
        ),
        errors = cms.untracked.PSet(
                threshold = cms.untracked.string( 'ERROR' )
        ),
)
process.load('CommonTools.UtilAlgos.TFileService_cfi')
process.TFileService.fileName=cms.string('DATA.root')

genParticleCollection = 'prunedGenParticles'
#jetname='slimmedJets'
#gjetname='slimmedGenJets'
jetname='slimmedJetsAK8'
gjetname='slimmedGenJetsAK8'

# Other options: see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
#process.load('RecoJets.JetProducers.QGTagger_cfi')
#process.QGTagger.srcJets = cms.InputTag(jetname)
#process.QGTagger.jetsLabel  = cms.string('QGL_AK4PFchs')

process.load('PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi')
process.selectedHadronsAndPartons.particles = genParticleCollection
process.physDefHadronsAndPartons = process.selectedHadronsAndPartons.clone( fullChainPhysPartons = cms.bool(False) )

from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
process.jetFlavs = ak4JetFlavourInfos.clone( jets = gjetname,
                                             partons = cms.InputTag("selectedHadronsAndPartons","algorithmicPartons") )
process.jetFlavsPD = process.jetFlavs.clone( partons = cms.InputTag("physDefHadronsAndPartons","physicsPartons") )

process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.ak8 =  cms.EDAnalyzer('ProcessedTreeProducerBTag',
  ## jet collections ###########################
  pfjetschs       = cms.InputTag(jetname),
  pfchsjetpuid    = cms.string('pileupJetId:fullDiscriminant'),
  runYear         = cms.untracked.string("2016"),
  ## MET collection ####
  pfmetT1         = cms.InputTag('slimmedMETs'),
  genjets         = cms.untracked.InputTag(gjetname),
  ## database entry for the uncertainties ######
  PFPayloadName   = cms.string(''),
  jecUncSrc       = cms.untracked.string(''),
  jecUncSrcNames  = cms.vstring(''),
  ## set the conditions for good Vtx counting ##
  offlineVertices = cms.InputTag('offlineSlimmedPrimaryVertices'),
  beamSpot        = cms.InputTag('offlineBeamSpot'),
  goodVtxNdof     = cms.double(4),
  goodVtxZ        = cms.double(24),
  ## rho #######################################
  srcCaloRho      = cms.InputTag('fixedGridRhoFastjetAllCalo'),
  srcPFRho        = cms.InputTag('fixedGridRhoFastjetAll'),
  srcPULabel      = cms.untracked.InputTag('slimmedAddPileupInfo'),
  ## preselection cuts #########################
  maxEta          = cms.double(5.0),
  minPFPt         = cms.double(15.0),
  minPFPtThirdJet = cms.double(5.0),
#  minNPFJets      = cms.uint32(1), #AK4
  minNPFJets      = cms.uint32(0), #AK8
  minGenPt        = cms.untracked.double(15.0),
  isMCarlo        = cms.untracked.bool(True),
  useGenInfo      = cms.untracked.bool(True),
  AK4             = cms.untracked.bool(ak4flag),
  ZB              = cms.untracked.bool(zbflag),
  ## trigger ###################################
  printTriggerMenu= cms.untracked.bool(False),
  processName     = cms.untracked.string('HLT'),
  filterName      = filters,
  triggerName     = triggers,
  triggerFollow   = follows,
  filterFlags     = cms.untracked.InputTag("TriggerResults","","RECO"),
  triggerResults  = cms.untracked.InputTag("TriggerResults","","HLT"),
  triggerHLTObjs  = cms.untracked.InputTag("selectedPatTrigger"),
  triggerL1Objs   = cms.untracked.InputTag("caloStage2Digis","Jet"),
  triggerL1HTObjs = cms.untracked.InputTag("caloStage2Digis","EtSum"),
  #triggerAllObjs   = cms.untracked.InputTag("gtStage2Digis","GlobalAlgBlk"),
  prescales       = cms.InputTag("patTrigger"),
  prescalesL1Min  = cms.InputTag("patTrigger","l1min"),
  prescalesL1Max  = cms.InputTag("patTrigger","l1max"),
  ## jec services ##############################
  EventInfo       = cms.untracked.InputTag("generator"),
  GenParticles    = cms.untracked.InputTag(genParticleCollection),
  jetFlavInfos    = cms.untracked.InputTag("jetFlavs"),
  jetFlavInfosPD  = cms.untracked.InputTag("jetFlavsPD"),
  saveWeights     = cms.bool(False)                      
)

#Try scheduled processs
process.path = cms.Path(process.ak8)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)
