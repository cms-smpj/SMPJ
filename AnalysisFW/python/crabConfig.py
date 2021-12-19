from CRABClient.UserUtilities import config
config = config()

## Configurable parameters begin
import sys

if len(sys.argv)<5:
  sys.exit()
args = sys.argv

# Choose 16/17/18
RunYear=args[1]
# Era
Era=args[2]
# Choose ak4/ak8, zb (DT only)
Mode=args[3]

# True for DT, False for MC
DTMode=False
# Choose py/pylong/hw/nu/mg (MC only)
MCType='py'
if args[4]=='dt': DTMode=True
else: MCType = args[4]

# Run extensions (MC)
ExtMode=False
if not DTMode and len(Era)>1: ExtMode=True

# In case of pylong (0-14) or mg (0-8)
RangeIdx = 0 
if len(sys.argv)>5 and not DTMode:
  if MCType=='pylong' or MCType=='mg': RangeIdx = int(args[5])

# Extra identifier
TagTag=''
if len(sys.argv)>6: TagTag = args[6]

print(RunYear,Era,Mode,DTMode,MCType,RangeIdx)
## Configurable parameters end

PtRanges = ['15to30','30to50','50to80','80to120','120to170','170to300','300to470','470to600','600to800','800to1000','1000to1400','1400to1800','1800to2400','2400to3200','3200toInf']
HTRanges = ['HT50to100','HT100to200','HT200to300','HT300to500','HT500to700','HT700to1000','HT1000to1500','HT1500to2000','HT2000toInf']

Tag=''
DTLoc='/JetHT'
if Mode=='ak8':
  Tag='_AK8'
elif Mode=='zb':
  if DTMode:
    Tag='_ZB'
    DTLoc='/ZeroBias'
  else:
    Mode='ak4'

config.General.transferOutputs = True
config.General.transferLogs = False

config.Data.inputDBS = 'global'
config.Data.outLFNDirBase = '/store/user/hsiikone'
config.Data.publication = False
# Temporary flag for UL16 datasets in production
config.Data.allowNonValidInputDataset = True

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

  Good = True
  if DTMode:
    config.General.workArea = 'runs_dt'
    config.Data.splitting = 'LumiBased'
    config.Data.unitsPerJob = 55 
    config.JobType.psetName = 'cfg/'+Mode+RunYear+'.py'
    config.General.requestName = 'Run'+RunYear+Era+Tag+TagTag
    Ver = '1'
    Campaign = ''
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
      if Era=='F': config.Data.unitsPerJob = 80
    elif RunYear=='18':
      # CMSSW 10_6_4_patch1: Era == A/B/C/D
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'
      Ver = '2'
      Campaign = '12Nov2019_UL2018'
      if Era=='A':   config.Data.unitsPerJob = 100
      elif Era=='D': config.Data.unitsPerJob = 160
      if Mode=='ak4':
        if Era=='C':
          Campaign += '_rsb'
          Ver = '1'
        if Era=='D': Ver = '4'
      elif Mode=='zb':
        if Era=='C' or Era=='D':
          Campaign += '_rsb'
          Ver = '1'
    config.Data.inputDataset = DTLoc+'/Run20'+RunYear+Era+'-'+Campaign+'-v'+Ver+'/MINIAOD'
  else:
    if not ExtMode:
      config.General.workArea = 'runs_mc'
    else:
      config.General.workArea = 'runs_extmc'
    DoPreVFP = RunYear=='16' and Era[0]=='e'
    config.Data.splitting = 'FileBased'
    config.Data.unitsPerJob = 5 
    config.JobType.psetName = 'cfg/'+Mode+RunYear+MCType[:2]+("PreVFP" if DoPreVFP else "")+'.py'
    if MCType=='nu': config.Data.unitsPerJob = 1
    elif MCType!='py' and MCType!='pylong' and MCType!='hw' and MCType!='mg':
      print "Unknown MC Type, ", MCType
      Good = False

    if RunYear=='16':
      if MCType=='nu':
        ##### Neutrino Gun
        if DoPreVFP:
          config.General.requestName = 'QCD16'+Tag+'_SingleNu_S19UL16_mcr2a_PreVFP_v8_e3_2'
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8_ext3-v2/MINIAODSIM'
        else:
          config.General.requestName = 'QCD16'+Tag+'_SingleNu_S19UL16_mcr2a_v13_2'
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'

      elif MCType=='hw':
        ##### Herwig7 Flat
        if DoPreVFP:
          config.General.requestName = 'QCD16'+Tag+'_CH3_15to7k_S19UL16_mcr2a_PreVFP_v8_4'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v4/MINIAODSIM'
        else:
          config.General.requestName = 'QCD16'+Tag+'_CH3_15to7k_S19UL16_mcr2a_v13_2'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'

      elif MCType=='py':
        ##### Pythia 8 Flat
        if DoPreVFP:
          if not ExtMode:
            config.General.requestName = 'QCD16'+Tag+'_CP5_15to7k_S19UL16_mcr2a_PreVFP_v8_e1_2'
            config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8_ext1-v2/MINIAODSIM'
          else:
            config.General.requestName = 'QCD16'+Tag+'_CP5_15to7k_S19UL16_mcr2a_PreVFP_v8_e1_2'
            config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8_ext1-v2/MINIAODSIM'
        else:
          if not ExtMode:
            config.General.requestName = 'QCD16'+Tag+'_CP5_15to7k_S19UL16_mcr2a_v13_2'
            config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'
          else:
            config.General.requestName = 'QCD16'+Tag+'_CP5_15to7k_S19UL16_mcr2a_v13_2'
            config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'

      elif MCType=='pylong':
        ##### Pythia 8 Pthat slices
        if DoPreVFP:
          config.General.requestName = 'QCD16'+Tag+'_CP5_'+PtRanges[RangeIdx]+'_S19UL16_mcr2a_PreVFP_v8_2'+TagTag
          config.Data.inputDataset = '/QCD_Pt_'+PtRanges[RangeIdx]+'_TuneCP5_13TeV_pythia8/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2/MINIAODSIM'
        else:
          config.General.requestName = 'QCD16'+Tag+'_CP5_'+PtRanges[RangeIdx]+'_S19UL16_mcr2a_v13_2'+TagTag
          config.Data.inputDataset = '/QCD_Pt_'+PtRanges[RangeIdx]+'_TuneCP5_13TeV_pythia8/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'

      elif MCType=='mg':
        ##### Madgraph + Pythia8 HT slices
        if DoPreVFP:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_'+HTRanges[RangeIdx]+'_S19UL16_mcr2a_PreVFP_v8_1'+TagTag
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1/MINIAODSIM'
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_'+HTRanges[RangeIdx]+'_S19UL16_mcr2a_v13_2'+TagTag
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'

    elif RunYear=='17':
      if MCType=='nu': 
        ##### Neutrino Gun
        config.General.requestName = 'QCD17'+Tag+'_SingleNu_PU2017_S19UL17_mc17r_v6_2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        else:
          Good = False

      elif MCType=='hw':
        # Herwig7 Flat
        config.General.requestName = 'QCD17'+Tag+'_CH3_15to7k_S19UL17_mc17r_v6_2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        else:
          Good = False

      elif MCType=='py':
        #config.General.requestName = 'QCD17'+Tag+'_CP5_15to7k_S19UL17_mc17r_v7_HSv1'
        #config.Data.inputDataset = '/RelValQCD_FlatPt_15_3000HS_13/CMSSW_10_6_14-PU25ns_106X_mc2017_realistic_v7_HS-v1/MINIAODSIM'
        config.General.requestName = 'QCD17'+Tag+'_CP5_15to7k_S19UL17_mc17r_v7_HSv1_BugFix'
        config.Data.inputDataset = '/RelValQCD_FlatPt_15_3000HS_13/CMSSW_10_6_14_Pyt8240BugFix-PU25ns_106X_mc2017_realistic_v7_HS-v1/MINIAODSIM'

        ##### Pythia 8 Flat
        #config.General.requestName = 'QCD17'+Tag+'_CP5_15to7k_S19UL17_mc17r_v6_2'+TagTag

        #if not ExtMode:
        #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        #else:
        #  config.General.requestName += '_e2'
        #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6_ext2-v2/MINIAODSIM'

      elif MCType=='pylong':
        ##### Pythia 8 Pthat slices
        config.General.requestName = 'QCD17'+Tag+'_CP5_'+PtRanges[RangeIdx]+'_S19UL17_mc17r_v6_2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/QCD_Pt_'+PtRanges[RangeIdx]+'_TuneCP5_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        else:
          Good = False

      elif MCType=='mg':
        ##### Madgraph + Pythia8 HT slices
        config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_'+HTRanges[RangeIdx]+'_S19UL17_mc17r_v6_2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
        else:
          Good = False

    elif RunYear=='18':
      if MCType=='nu':
        ##### Neutrino Gun
        config.General.requestName = 'QCD18'+Tag+'_SingleNu_S19UL18_mc18r_v11_L1v1_v2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
          #config.General.requestName += '_HEM'
          #config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-FlatPU0to70_UL18HEMreReco_106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
        else:
          Good = False

      elif MCType=='hw':
        # Herwig7 Flat
        config.General.requestName = 'QCD18'+Tag+'_CH3_15to7k_S19UL18_mc18r_v11_L1v1_v2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH3_Flat_13TeV_herwig7/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        else:
          Good = False

      elif MCType=='py':
        # Pythia 8 Flat
        config.General.requestName = 'QCD18'+Tag+'_CP5_15to7k_S19UL18_mc18r_v11_L1v1_v2'+TagTag
        
        if not ExtMode:
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          #config.General.requestName += '_HEM' 
          #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_UL18HEMreReco_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        else:
          config.General.requestName += '_e' 
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          #config.General.requestName += '_HEM' 
          #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_UL18HEMreReco_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'

      elif MCType=='pylong':
        ##### Pythia 8 Pthat slices
        config.General.requestName = 'QCD18'+Tag+'_CP5_'+PtRanges[RangeIdx]+'_S19UL18_mc18r_v11_L1v1_v2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/QCD_Pt_'+PtRanges[RangeIdx]+'_TuneCP5_13TeV_pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        else:
          Good = False

      elif MCType=='mg':
        ##### Madgraph + Pythia8 HT slices
        config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_'+HTRanges[RangeIdx]+'_S19UL18_mc18r_v11_L1v1_v2'+TagTag

        if not ExtMode:
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        else:
          Good = False

  if Good: submit(config)

