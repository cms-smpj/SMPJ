# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
import sys

process = cms.Process("Ntuplizer")
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Conditions
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.Configuration.RecoGenJets_cff")
process.load("RecoJets.JetProducers.TrackJetParameters_cfi")

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

process.GlobalTag.globaltag = "106X_upgrade2018_realistic_v16_L1v1"

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

#files= cms.untracked.vstring('file:toast.root')
files= cms.untracked.vstring(
# '/store/mc/RunIISummer20UL18GEN/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/GEN/106X_upgrade2018_realistic_v4-v2/2520000/040A5B53-2B91-8446-8246-538E79DC4D0D.root',
# '/store/mc/RunIISummer20UL18GEN/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/GEN/106X_upgrade2018_realistic_v4-v2/2520000/048D2590-79EF-B247-BBED-E21AA301CDAF.root',
# '/store/mc/RunIISummer20UL18GEN/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/GEN/106X_upgrade2018_realistic_v4-v2/2520000/08E37DD6-1E3E-404D-8F03-DC4FE1D3FE37.root',
 '/store/mc/RunIISummer20UL18GEN/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/GEN/106X_upgrade2018_realistic_v4-v2/2520000/0A959079-E71F-954B-AC7E-90213765717A.root',
# '/store/mc/RunIISummer20UL18GEN/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/GEN/106X_upgrade2018_realistic_v4-v2/2520000/0B368815-8A20-CD4E-B239-47FC82D28324.root',
)
process.source = cms.Source("PoolSource", fileNames = files)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = cms.untracked.string("ERROR")
process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring(
                "errors"
        ),
        errors = cms.untracked.PSet(
                threshold = cms.untracked.string("ERROR")
        ),
)
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName=cms.string("out.root")
process.jets =  cms.EDAnalyzer('MCJettify',
)

process.path = cms.Path(process.jets)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)
