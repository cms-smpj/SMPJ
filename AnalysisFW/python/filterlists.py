import FWCore.ParameterSet.Config as cms

# These should be reviewed from time to time. See the listings at:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2

fltlist = {
  '16' : { # 8_0_X and 9_4_X MINIAOD (which originates from 8_0_X AOD) 
    'dt' : cms.vstring(
      "Flag_goodVertices","Flag_globalSuperTightHalo2016Filter","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter",
      "Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_BadPFMuonFilter",
      "Flag_eeBadScFilter"
    ),
    'mc' : cms.vstring(
      "Flag_goodVertices","Flag_globalSuperTightHalo2016Filter","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter",
      "Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_BadPFMuonFilter"
    )
  },
  '17' : { # 9_4_0 and further
    'dt' : cms.vstring(
      "Flag_goodVertices","Flag_globalSuperTightHalo2016Filter","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter",
      "Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_BadPFMuonFilter",
      "Flag_eeBadScFilter"#,
#      "Flag_ecalBadCalibReducedMINIAODFilter"
    ),
    'mc' : cms.vstring(
      "Flag_goodVertices","Flag_globalSuperTightHalo2016Filter","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter",
      "Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_BadPFMuonFilter"#,
#      "Flag_ecalBadCalibReducedMINIAODFilter"
    )
  },
  '18' : { # 10_1_X and further
    'dt' : cms.vstring(
      "Flag_goodVertices","Flag_globalSuperTightHalo2016Filter","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter",
      "Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_BadPFMuonFilter",
      "Flag_eeBadScFilter"#,
#      "Flag_ecalBadCalibReducedMINIAODFilter"
    ),
    'mc' : cms.vstring(
      "Flag_goodVertices","Flag_globalSuperTightHalo2016Filter","Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter",
      "Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_BadPFMuonFilter"#,
#      "Flag_ecalBadCalibReducedMINIAODFilter"
    )
  }
}
