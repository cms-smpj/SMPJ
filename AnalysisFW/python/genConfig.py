from CRABClient.UserUtilities import config
config = config()

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

  def submit(config):
    try:
      crabCommand('submit', config = config)
    except HTTPException as hte:
      print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
      print "Failed submitting task: %s" % (cle)

  config.General.workArea = 'runs_mc'
  config.Data.splitting = 'FileBased'
  config.Data.unitsPerJob = 10

  config.JobType.psetName = 'cfg/gen.py'

  config.General.requestName = 'GJetsRunUL16'
  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL16GENAPV-106X_mcRun2_asymptotic_preVFP_v8-v2/GEN'

  #config.General.requestName = 'GJetsRunUL17'
  #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL17GEN-106X_mc2017_realistic_v6-v1/GEN'

  #config.General.requestName = 'GJetsRunUL18'
  #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL18GEN-106X_upgrade2018_realistic_v4-v2/GEN'

  print(config.Data.inputDataset)
  submit(config)
