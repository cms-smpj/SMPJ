from CRABClient.UserUtilities import config
config = config()

## Configurable parameters begin
import sys

args = sys.argv
if len(args)<3:
  sys.exit()

Tag=''
# Choose 16/17/18 (for MC, 16e/16l and 18e/18l)
RunYear=args[1]
# Era or MC Type
Era=args[2]

# True for DT, False for MC
DTMode=True
# Choose py/hw/nu/mg (MC only, only nu is implemented)
MCType='nu'
if Era=='nu' or Era=='py' or Era=='hw' or Era=='mg':
  DTMode = False
  MCType = Era

config.General.transferOutputs = True
config.General.transferLogs = False

config.Data.inputDBS = 'global'
config.Data.outLFNDirBase = '/store/user/hsiikone'
config.Data.publication = False

config.JobType.pluginName = 'Analysis'
config.Site.storageSite = 'T2_FI_HIP'

if __name__ == '__main__':
  from CRABAPI.RawCommand import crabCommand
  from CRABClient.ClientExceptions import ClientException
  from httplib import HTTPException
  Good=True

  def submit(config):
    try:
      crabCommand('submit', config = config)
    except HTTPException as hte:
      print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
      print "Failed submitting task: %s" % (cle)

  if DTMode:
    config.General.workArea = 'runs_dt'
    config.Data.splitting = 'LumiBased'
    config.Data.unitsPerJob = 100 
    config.JobType.psetName = 'cfg/had'+RunYear+'.py'
    config.General.requestName = 'PartsRun'+RunYear+Era+Tag
    Ver = '1'
    Campaign = ''
    # UL ReReco
    # UL ReReco
    if RunYear=='16':
      # CMSSW 10_6_8_patch1: Era == Be/Bl/C/D/E/Fe/Fl/G/H
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt'
      if Era=='Be':
        Campaign = '21Feb2020_ver1_UL2016'
        Era = 'B'
      elif Era=='Bl':
        Campaign = '21Feb2020_ver2_UL2016'
        Era = 'B'
      else:
        Campaign = '21Feb2020_UL2016'
      # HIP issue mitigation
      if Era=='B' or Era=='C' or Era=='D' or Era=='E' or Era=='Fe':
        Campaign += '_HIPM'
      # Late F produced without HIP mitigation
      if Era=='Fe': Era = 'F'
      if Era=='Fl': Era = 'F'
    elif RunYear=='17':
      # CMSSW 10_6_2: Era == B/C/D/E/F
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSONmod.txt'
      Campaign = '09Aug2019_UL2017'
      if Era=='C': Campaign += '_MiniAODv2'
      elif Era=='F': config.Data.unitsPerJob = 80
    elif RunYear=='18':
      # CMSSW 10_6_4_patch1: Era == A/B/C/D
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'
      Ver = '2'
      Campaign = '12Nov2019_UL2018'
      if Era=='A':   config.Data.unitsPerJob = 100
      elif Era=='D': config.Data.unitsPerJob = 160
      if Era=='C' or Era=='D':
        Campaign += '_rsb'
        Ver = '1'
    # For now, only ZeroBias is used
    config.Data.inputDataset = '/ZeroBias/Run20'+RunYear+Era+'-'+Campaign+'-v'+Ver+'/MINIAOD'
  else:
    config.General.workArea = 'runs_mc'
    IsEarly = RunYear[2:]=='e' # For 16 and 18, we have 'early' and 'late' campaigns (MC only)
    RunYear = RunYear[:2] # Stripping the early/late tags
    config.Data.splitting = 'FileBased'
    config.Data.unitsPerJob = 1

    # Neutrino Gun settings
    add = ""
    split = ""
    if RunYear=='16':
      split = "_PreVFP" if IsEarly else "_PostVFP" 
      add = "PreVFP" if IsEarly else ""
    elif RunYear=='18': split = "" if IsEarly else "_HEM"
    config.JobType.psetName = 'cfg/had'+RunYear+MCType+add+'.py'
    config.General.requestName = 'PartsRun'+RunYear+MCType+split+Tag
    if RunYear=='16':
      if MCType=='nu':
        ##### Neutrino Gun
        if IsEarly:
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8_ext3-v2/MINIAODSIM'
        else:
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'
      elif MCType=='py': # Not implemented
        ##### Pythia 8 Flat
        if IsEarly:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8_ext1-v2/MINIAODSIM'
        else:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'
        Good=False
      elif MCType=='hw': # Not implemented
        ##### Herwig7 Flat
        if IsEarly:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v4/MINIAODSIM'
        else:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'
        Good=False
      elif MCType=='mg': # Not implemented
        ##### Madgraph + Pythia8 HT slices
        if IsEarly:
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'
        else:
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'
        Good=False
    elif RunYear=='17':
      # The files presented here are produced on CMSSW_9_2_8-9_4_6
      if MCType=='nu': 
        config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
      elif MCType=='py':
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        Good=False
      elif MCType=='hw':
        # Herwig7 Flat
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        Good=False
      elif MCType=='mg':
        ##### Madgraph + Pythia8 HT slices
        config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        Good=False
    elif RunYear=='18':
      if MCType=='nu':
        if IsEarly:
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
        else:
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-FlatPU0to70_UL18HEMreReco_106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
      elif MCType=='py':
        if IsEarly:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        else:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_UL18HEMreReco_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        Good=False
      elif MCType=='hw':
        # Herwig7 Flat
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        Good=False
      elif MCType=='mg':
        ##### Madgraph + Pythia8 HT slices
        config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        Good=False

  if Good:
    print(config.Data.inputDataset)
    submit(config)
