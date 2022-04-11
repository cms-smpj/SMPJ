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

  if not DTMode:
    config.General.workArea = 'runs_mc'
    IsEarly = RunYear[2:]=='e' # For 16 and 18, we have 'early' and 'late' campaigns (MC only)
    RunYear = RunYear[:2] # Stripping the early/late tags
    config.Data.splitting = 'FileBased'
    config.Data.unitsPerJob = 10

    # Neutrino Gun settings
    add = ""
    split = ""
    if RunYear=='16':
      split = "_PreVFP" if IsEarly else "_PostVFP" 
      add = "PreVFP" if IsEarly else ""
    elif RunYear=='18': split = "" if IsEarly else "_HEM"
    config.JobType.psetName = 'cfg/genj'+RunYear+MCType+add+'.py'
    config.General.requestName = 'GJetsRun'+RunYear+MCType+split+Tag
    if RunYear=='16':
      if MCType=='nu':
        Good=False
      elif MCType=='py': # Not implemented
        ##### Pythia 8 Flat
        if IsEarly:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL16MiniAODAPVv2-EpsilonPU_106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
          #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
        else:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL16MiniAODv2-EpsilonPU_106X_mcRun2_asymptotic_v17-v1/MINIAODSIM'
          #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v1/MINIAODSIM'
      elif MCType=='hw': # Not implemented
        ##### Herwig7 Flat
        if IsEarly:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v4/MINIAODSIM'
        else:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'
        Good=False
      elif MCType=='mg': # Not implemented
        Good=False
    elif RunYear=='17':
      # The files presented here are produced on CMSSW_9_2_8-9_4_6
      if MCType=='nu': 
        Good=False
      elif MCType=='py':
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL17MiniAODv2-EpsilonPU_106X_mc2017_realistic_v9-v1/MINIAODSIM'
        #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM'
      elif MCType=='hw':
        # Herwig7 Flat
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        Good=False
      elif MCType=='mg':
        Good=False
    elif RunYear=='18':
      if MCType=='nu':
        Good=False
      elif MCType=='py':
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL18MiniAODv2-EpsilonPU_106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM'
        #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM'
      elif MCType=='hw':
        # Herwig7 Flat
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        Good=False
      elif MCType=='mg':
        Good=False

  if Good:
    print(config.Data.inputDataset)
    submit(config)
