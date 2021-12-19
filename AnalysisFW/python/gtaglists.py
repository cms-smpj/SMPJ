import FWCore.ParameterSet.Config as cms

# The global tags should be checked from time to time.
# See: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions#Global_Tags_for_2016_legacy_data
# However, the info on this page is not always up-to-date.
# If in doubt, use the browser https://cms-conddb.cern.ch/cmsDbBrowser/index/Prod
# Ultra Legacy summary: https://twiki.cern.ch/twiki/bin/view/CMS/PdmVRun2LegacyAnalysisSummaryTable
GTags = {
  '16' : {
    'dt' : "106X_dataRun2_v32",
    'mc' : "106X_mcRun2_asymptotic_v15",
    'mcPreVFP' : "106X_mcRun2_asymptotic_preVFP_v9"
  },
  '17' : {
    'dt' : "106X_dataRun2_v32",
    'mc' : "106X_mc2017_realistic_v8"
  },
  '18' : {
    'dt' : "106X_dataRun2_v32", 
    'mc' : "106X_upgrade2018_realistic_v15_L1v1"
  }
}
