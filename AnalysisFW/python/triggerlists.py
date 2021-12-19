import FWCore.ParameterSet.Config as cms
# The trigger lists for each era follow this convention: AK4 - AK8 (incl. PFHT) - ZB
# Nesting: year - era - ak4/ak8/zb

trglist = {
  '16' : { 
    "mc" : {
      'ak4' : cms.vstring("HLT_ZeroBias_v0","HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0"),
      'ak8' : cms.vstring(),
      #'ak8' : cms.vstring("HLT_AK8PFJet40_v0","HLT_AK8PFJet60_v0","HLT_AK8PFJet80_v0","HLT_AK8PFJet140_v0","HLT_AK8PFJet200_v0","HLT_AK8PFJet260_v0","HLT_AK8PFJet320_v0","HLT_AK8PFJet400_v0","HLT_AK8PFJet450_v0","HLT_AK8PFJet500_v0")
      'zb' : cms.vstring("HLT_ZeroBias_v0","HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0")
    },
    "dt" : {
      'ak4' : cms.vstring("HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0"),
      'ak8' : cms.vstring(),
      #'ak8' : cms.vstring("HLT_AK8PFJet40_v0","HLT_AK8PFJet60_v0","HLT_AK8PFJet80_v0","HLT_AK8PFJet140_v0","HLT_AK8PFJet200_v0","HLT_AK8PFJet260_v0","HLT_AK8PFJet320_v0","HLT_AK8PFJet400_v0","HLT_AK8PFJet450_v0","HLT_AK8PFJet500_v0"),
      'zb' : cms.vstring("HLT_ZeroBias_v0")
    },
    "Be" : { # This does not pass the DATACERT and AK8 triggers are missing 
      'ak4' : cms.vstring("HLT_PFJet40_v4","HLT_PFJet60_v4","HLT_PFJet80_v4","HLT_PFJet140_v4","HLT_PFJet200_v4","HLT_PFJet260_v4","HLT_PFJet320_v4","HLT_PFJet400_v4","HLT_PFJet450_v4","HLT_PFJet500_v4"),
      'ak8' : cms.vstring(),
      'zb'  : cms.vstring("HLT_ZeroBias_v2")
    },
    "Bl" : {
      'ak4' : cms.vstring("HLT_PFJet40_v4","HLT_PFJet40_v5","HLT_PFJet60_v4","HLT_PFJet60_v5","HLT_PFJet80_v4","HLT_PFJet80_v5","HLT_PFJet140_v4","HLT_PFJet140_v5","HLT_PFJet200_v4","HLT_PFJet200_v5","HLT_PFJet260_v4","HLT_PFJet260_v5","HLT_PFJet320_v4","HLT_PFJet320_v5","HLT_PFJet400_v4","HLT_PFJet400_v5","HLT_PFJet450_v4","HLT_PFJet450_v5","HLT_PFJet500_v4","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v1","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_ZeroBias_v2","HLT_ZeroBias_v3")
    },
    "C" : {
      'ak4' : cms.vstring("HLT_PFJet40_v5","HLT_PFJet40_v6","HLT_PFJet60_v5","HLT_PFJet60_v6","HLT_PFJet80_v5","HLT_PFJet140_v5","HLT_PFJet200_v5","HLT_PFJet260_v5","HLT_PFJet320_v5","HLT_PFJet400_v5","HLT_PFJet450_v5","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v1","HLT_AK8PFJet40_v2","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_ZeroBias_v3","HLT_ZeroBias_v4")
    },
    "D" : {
      'ak4' : cms.vstring("HLT_PFJet40_v6","HLT_PFJet60_v6","HLT_PFJet80_v5","HLT_PFJet140_v5","HLT_PFJet200_v5","HLT_PFJet260_v5","HLT_PFJet320_v5","HLT_PFJet400_v5","HLT_PFJet450_v5","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v2","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_ZeroBias_v4")
    },
    "E" : {
      'ak4' : cms.vstring("HLT_PFJet40_v6","HLT_PFJet60_v6","HLT_PFJet80_v5","HLT_PFJet140_v5","HLT_PFJet200_v5","HLT_PFJet260_v5","HLT_PFJet320_v5","HLT_PFJet400_v5","HLT_PFJet450_v5","HLT_PFJet500_v5"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v2","HLT_AK8PFJet60_v1","HLT_AK8PFJet80_v1","HLT_AK8PFJet140_v1","HLT_AK8PFJet200_v1","HLT_AK8PFJet260_v1","HLT_AK8PFJet320_v1","HLT_AK8PFJet400_v1","HLT_AK8PFJet450_v1","HLT_AK8PFJet500_v1"),
      'zb' : cms.vstring("HLT_ZeroBias_v4")
    },
    "Fe" : {
      'ak4' : cms.vstring("HLT_PFJet40_v6","HLT_PFJet40_v7","HLT_PFJet60_v6","HLT_PFJet60_v7","HLT_PFJet80_v5","HLT_PFJet80_v6","HLT_PFJet140_v5","HLT_PFJet140_v6","HLT_PFJet200_v5","HLT_PFJet200_v6","HLT_PFJet260_v5","HLT_PFJet260_v7","HLT_PFJet320_v5","HLT_PFJet320_v7","HLT_PFJet400_v5","HLT_PFJet400_v7","HLT_PFJet450_v5","HLT_PFJet450_v7","HLT_PFJet500_v5","HLT_PFJet500_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v2","HLT_AK8PFJet40_v3","HLT_AK8PFJet60_v1","HLT_AK8PFJet60_v2","HLT_AK8PFJet80_v1","HLT_AK8PFJet80_v2","HLT_AK8PFJet140_v1","HLT_AK8PFJet140_v2","HLT_AK8PFJet200_v1","HLT_AK8PFJet200_v2","HLT_AK8PFJet260_v1","HLT_AK8PFJet260_v3","HLT_AK8PFJet320_v1","HLT_AK8PFJet320_v3","HLT_AK8PFJet400_v1","HLT_AK8PFJet400_v3","HLT_AK8PFJet450_v1","HLT_AK8PFJet450_v3","HLT_AK8PFJet500_v1","HLT_AK8PFJet500_v3"),
      'zb' : cms.vstring("HLT_ZeroBias_v4")
    },
    'Fl' : {
      'ak4' : cms.vstring("HLT_PFJet40_v7","HLT_PFJet60_v7","HLT_PFJet80_v6","HLT_PFJet140_v6","HLT_PFJet200_v6","HLT_PFJet260_v7","HLT_PFJet320_v7","HLT_PFJet400_v7","HLT_PFJet450_v7","HLT_PFJet500_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v3","HLT_AK8PFJet60_v2","HLT_AK8PFJet80_v2","HLT_AK8PFJet140_v2","HLT_AK8PFJet200_v2","HLT_AK8PFJet260_v3","HLT_AK8PFJet320_v3","HLT_AK8PFJet400_v3","HLT_AK8PFJet450_v3","HLT_AK8PFJet500_v3"),
      'zb' : cms.vstring("HLT_ZeroBias_v4")
    },
    'G' : {
      'ak4' : cms.vstring("HLT_PFJet40_v7","HLT_PFJet60_v7","HLT_PFJet80_v6","HLT_PFJet140_v6","HLT_PFJet200_v6","HLT_PFJet260_v7","HLT_PFJet320_v7","HLT_PFJet400_v7","HLT_PFJet450_v7","HLT_PFJet500_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v3","HLT_AK8PFJet60_v2","HLT_AK8PFJet80_v2","HLT_AK8PFJet140_v2","HLT_AK8PFJet200_v2","HLT_AK8PFJet260_v3","HLT_AK8PFJet320_v3","HLT_AK8PFJet400_v3","HLT_AK8PFJet450_v3","HLT_AK8PFJet500_v3"),
      'zb' : cms.vstring("HLT_ZeroBias_v4")
    },
    'H' : {
      'ak4' : cms.vstring("HLT_PFJet40_v9","HLT_PFJet60_v9","HLT_PFJet80_v8","HLT_PFJet140_v8","HLT_PFJet200_v8","HLT_PFJet260_v9","HLT_PFJet320_v9","HLT_PFJet400_v9","HLT_PFJet450_v9","HLT_PFJet500_v9"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v5","HLT_AK8PFJet60_v4","HLT_AK8PFJet80_v4","HLT_AK8PFJet140_v4","HLT_AK8PFJet200_v4","HLT_AK8PFJet260_v5","HLT_AK8PFJet320_v5","HLT_AK8PFJet400_v5","HLT_AK8PFJet450_v5","HLT_AK8PFJet500_v5"),
      'zb' : cms.vstring("HLT_ZeroBias_v4")
    }
  },
  '17' : {
    'mc' : {
      'ak4' : cms.vstring("HLT_ZeroBias_v0","HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0","HLT_PFJet550_v0"),
      'ak8' : cms.vstring(),
      #'ak8' : cms.vstring("HLT_AK8PFJet40_v0","HLT_AK8PFJet60_v0","HLT_AK8PFJet80_v0","HLT_AK8PFJet140_v0","HLT_AK8PFJet200_v0","HLT_AK8PFJet260_v0","HLT_AK8PFJet320_v0","HLT_AK8PFJet400_v0","HLT_AK8PFJet450_v0","HLT_AK8PFJet500_v0","HLT_AK8PFJet550_v0")
      'zb' : cms.vstring("HLT_ZeroBias_v0","HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0")
    },
    'dt' : {
      'ak4' : cms.vstring("HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0","HLT_PFJet550_v0"),
      'ak8' : cms.vstring(),
      #'ak8' : cms.vstring("HLT_AK8PFJet40_v0","HLT_AK8PFJet60_v0","HLT_AK8PFJet80_v0","HLT_AK8PFJet140_v0","HLT_AK8PFJet200_v0","HLT_AK8PFJet260_v0","HLT_AK8PFJet320_v0","HLT_AK8PFJet400_v0","HLT_AK8PFJet450_v0","HLT_AK8PFJet500_v0","HLT_AK8PFJet550_v0"),
      'zb' : cms.vstring("HLT_ZeroBias_v0")
    },
    'B' : {
      'ak4' : cms.vstring("HLT_PFJet40_v10","HLT_PFJet40_v11","HLT_PFJet40_v12","HLT_PFJet60_v10","HLT_PFJet60_v11","HLT_PFJet60_v12","HLT_PFJet80_v10","HLT_PFJet80_v11","HLT_PFJet80_v9","HLT_PFJet140_v10","HLT_PFJet140_v9","HLT_PFJet200_v10","HLT_PFJet200_v9","HLT_PFJet260_v10","HLT_PFJet260_v11","HLT_PFJet320_v10","HLT_PFJet320_v11","HLT_PFJet400_v10","HLT_PFJet400_v11","HLT_PFJet450_v10","HLT_PFJet450_v11","HLT_PFJet450_v12","HLT_PFJet500_v10","HLT_PFJet500_v11","HLT_PFJet500_v12","HLT_PFJet550_v1","HLT_PFJet550_v2"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v6","HLT_AK8PFJet40_v7","HLT_AK8PFJet60_v5","HLT_AK8PFJet60_v6","HLT_AK8PFJet80_v5","HLT_AK8PFJet80_v6","HLT_AK8PFJet140_v5","HLT_AK8PFJet140_v6","HLT_AK8PFJet200_v5","HLT_AK8PFJet200_v6","HLT_AK8PFJet260_v6","HLT_AK8PFJet260_v7","HLT_AK8PFJet320_v6","HLT_AK8PFJet320_v7","HLT_AK8PFJet400_v6","HLT_AK8PFJet400_v7","HLT_AK8PFJet450_v6","HLT_AK8PFJet450_v7","HLT_AK8PFJet500_v6","HLT_AK8PFJet500_v7","HLT_AK8PFJet550_v1","HLT_AK8PFJet550_v2"),
      'zb' : cms.vstring("HLT_ZeroBias_v5")
    },
    'C' : {
      'ak4' : cms.vstring("HLT_PFJet40_v13","HLT_PFJet40_v14","HLT_PFJet40_v15","HLT_PFJet40_v16","HLT_PFJet60_v13","HLT_PFJet60_v14","HLT_PFJet60_v15","HLT_PFJet60_v16","HLT_PFJet80_v12","HLT_PFJet80_v13","HLT_PFJet80_v14","HLT_PFJet80_v15","HLT_PFJet140_v11","HLT_PFJet140_v12","HLT_PFJet140_v13","HLT_PFJet140_v14","HLT_PFJet200_v11","HLT_PFJet200_v12","HLT_PFJet200_v13","HLT_PFJet200_v14","HLT_PFJet260_v12","HLT_PFJet260_v13","HLT_PFJet260_v14","HLT_PFJet260_v15","HLT_PFJet320_v12","HLT_PFJet320_v13","HLT_PFJet320_v14","HLT_PFJet320_v15","HLT_PFJet400_v12","HLT_PFJet400_v13","HLT_PFJet400_v14","HLT_PFJet400_v15","HLT_PFJet450_v13","HLT_PFJet450_v14","HLT_PFJet450_v15","HLT_PFJet450_v16","HLT_PFJet500_v13","HLT_PFJet500_v14","HLT_PFJet500_v15","HLT_PFJet500_v16","HLT_PFJet550_v3","HLT_PFJet550_v4","HLT_PFJet550_v5","HLT_PFJet550_v6"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v10","HLT_AK8PFJet40_v11","HLT_AK8PFJet40_v8","HLT_AK8PFJet40_v9","HLT_AK8PFJet60_v10","HLT_AK8PFJet60_v7","HLT_AK8PFJet60_v8","HLT_AK8PFJet60_v9","HLT_AK8PFJet80_v10","HLT_AK8PFJet80_v7","HLT_AK8PFJet80_v8","HLT_AK8PFJet80_v9","HLT_AK8PFJet140_v10","HLT_AK8PFJet140_v7","HLT_AK8PFJet140_v8","HLT_AK8PFJet140_v9","HLT_AK8PFJet200_v10","HLT_AK8PFJet200_v7","HLT_AK8PFJet200_v8","HLT_AK8PFJet200_v9","HLT_AK8PFJet260_v10","HLT_AK8PFJet260_v11","HLT_AK8PFJet260_v8","HLT_AK8PFJet260_v9","HLT_AK8PFJet320_v10","HLT_AK8PFJet320_v11","HLT_AK8PFJet320_v8","HLT_AK8PFJet320_v9","HLT_AK8PFJet400_v10","HLT_AK8PFJet400_v11","HLT_AK8PFJet400_v8","HLT_AK8PFJet400_v9","HLT_AK8PFJet450_v10","HLT_AK8PFJet450_v11","HLT_AK8PFJet450_v8","HLT_AK8PFJet450_v9","HLT_AK8PFJet500_v10","HLT_AK8PFJet500_v11","HLT_AK8PFJet500_v8","HLT_AK8PFJet500_v9","HLT_AK8PFJet550_v3","HLT_AK8PFJet550_v4","HLT_AK8PFJet550_v5","HLT_AK8PFJet550_v6"),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    },
    'D' : {
      'ak4' : cms.vstring("HLT_PFJet40_v16","HLT_PFJet40_v17","HLT_PFJet60_v16","HLT_PFJet60_v17","HLT_PFJet80_v15","HLT_PFJet80_v16","HLT_PFJet140_v14","HLT_PFJet140_v15","HLT_PFJet200_v14","HLT_PFJet200_v15","HLT_PFJet260_v15","HLT_PFJet260_v16","HLT_PFJet320_v15","HLT_PFJet320_v16","HLT_PFJet400_v15","HLT_PFJet400_v16","HLT_PFJet450_v16","HLT_PFJet450_v17","HLT_PFJet500_v16","HLT_PFJet500_v17","HLT_PFJet550_v6","HLT_PFJet550_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v11","HLT_AK8PFJet40_v12","HLT_AK8PFJet60_v10","HLT_AK8PFJet60_v11","HLT_AK8PFJet80_v10","HLT_AK8PFJet80_v11","HLT_AK8PFJet140_v10","HLT_AK8PFJet140_v11","HLT_AK8PFJet200_v10","HLT_AK8PFJet200_v11","HLT_AK8PFJet260_v11","HLT_AK8PFJet260_v12","HLT_AK8PFJet320_v11","HLT_AK8PFJet320_v12","HLT_AK8PFJet400_v11","HLT_AK8PFJet400_v12","HLT_AK8PFJet450_v11","HLT_AK8PFJet450_v12","HLT_AK8PFJet500_v11","HLT_AK8PFJet500_v12","HLT_AK8PFJet550_v6","HLT_AK8PFJet550_v7"),
      'zb' : cms.vstring("HLT_ZeroBias_v6","HLT_ZeroBias_part0_v6","HLT_ZeroBias_part1_v6","HLT_ZeroBias_part2_v6","HLT_ZeroBias_part3_v6","HLT_ZeroBias_part4_v6","HLT_ZeroBias_part5_v6","HLT_ZeroBias_part6_v6","HLT_ZeroBias_part7_v6")
    },
    'E' : {
      'ak4' : cms.vstring("HLT_PFJet40_v17","HLT_PFJet60_v17","HLT_PFJet80_v16","HLT_PFJet140_v15","HLT_PFJet200_v15","HLT_PFJet260_v16","HLT_PFJet320_v16","HLT_PFJet400_v16","HLT_PFJet450_v17","HLT_PFJet500_v17","HLT_PFJet550_v7"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v12","HLT_AK8PFJet60_v11","HLT_AK8PFJet80_v11","HLT_AK8PFJet140_v11","HLT_AK8PFJet200_v11","HLT_AK8PFJet260_v12","HLT_AK8PFJet320_v12","HLT_AK8PFJet400_v12","HLT_AK8PFJet450_v12","HLT_AK8PFJet500_v12","HLT_AK8PFJet550_v7"),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    },
    'F' : {
      'ak4' : cms.vstring("HLT_PFJet40_v17","HLT_PFJet40_v18","HLT_PFJet40_v19","HLT_PFJet60_v17","HLT_PFJet60_v18","HLT_PFJet60_v19","HLT_PFJet80_v16","HLT_PFJet80_v17","HLT_PFJet80_v18","HLT_PFJet140_v15","HLT_PFJet140_v16","HLT_PFJet140_v17","HLT_PFJet200_v15","HLT_PFJet200_v16","HLT_PFJet200_v17","HLT_PFJet260_v16","HLT_PFJet260_v17","HLT_PFJet260_v18","HLT_PFJet320_v16","HLT_PFJet320_v17","HLT_PFJet320_v18","HLT_PFJet400_v16","HLT_PFJet400_v17","HLT_PFJet400_v18","HLT_PFJet450_v17","HLT_PFJet450_v18","HLT_PFJet450_v19","HLT_PFJet500_v17","HLT_PFJet500_v18","HLT_PFJet500_v19","HLT_PFJet550_v7","HLT_PFJet550_v8","HLT_PFJet550_v9"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v12","HLT_AK8PFJet40_v13","HLT_AK8PFJet40_v14","HLT_AK8PFJet60_v11","HLT_AK8PFJet60_v12","HLT_AK8PFJet60_v13","HLT_AK8PFJet80_v11","HLT_AK8PFJet80_v12","HLT_AK8PFJet80_v13","HLT_AK8PFJet140_v11","HLT_AK8PFJet140_v12","HLT_AK8PFJet140_v13","HLT_AK8PFJet200_v11","HLT_AK8PFJet200_v12","HLT_AK8PFJet200_v13","HLT_AK8PFJet260_v12","HLT_AK8PFJet260_v13","HLT_AK8PFJet260_v14","HLT_AK8PFJet320_v12","HLT_AK8PFJet320_v13","HLT_AK8PFJet320_v14","HLT_AK8PFJet400_v12","HLT_AK8PFJet400_v13","HLT_AK8PFJet400_v14","HLT_AK8PFJet450_v12","HLT_AK8PFJet450_v13","HLT_AK8PFJet450_v14","HLT_AK8PFJet500_v12","HLT_AK8PFJet500_v13","HLT_AK8PFJet500_v14","HLT_AK8PFJet550_v7","HLT_AK8PFJet550_v8","HLT_AK8PFJet550_v9"),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    },
    'H' : {
      'ak4' : cms.vstring("HLT_HIAK4PFJet15_v1","HLT_HIPFJet25_v1","HLT_HIAK4PFJet40_v1","HLT_HIAK4PFJet60_v1","HLT_HIAK4PFJet80_v1","HLT_HIPFJet140_v1","HLT_HIPFJetFwd15_v1","HLT_HIPFJetFwd25_v1","HLT_HIAK4PFJet40FWD_v1","HLT_HIAK4PFJet60FWD_v1","HLT_HIAK4PFJet80FWD_v1","HLT_HIPFJetFwd140_v1"),
      'ak8' : cms.vstring("HLT_HIAK8PFJet15_v1","HLT_HIAK8PFJet25_v1","HLT_HIAK8PFJet40_v1","HLT_HIAK8PFJet60_v1","HLT_HIAK8PFJet80_v1","HLT_HIAK8PFJet140_v1","HLT_HIAK8PFJetFwd15_v1","HLT_HIAK8PFJetFwd25_v1","HLT_HIAK8PFJetFwd40_v1","HLT_HIAK8PFJetFwd60_v1","HLT_HIAK8PFJetFwd80_v1","HLT_HIAK8PFJetFwd140_v1"),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    },
  },
  '18' : {
    'mc' : {
      'ak4' : cms.vstring("HLT_ZeroBias_v0","HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0","HLT_PFJet550_v0"),
      'ak8' : cms.vstring(),
      #'ak8' : cms.vstring("HLT_AK8PFJet40_v0","HLT_AK8PFJet60_v0","HLT_AK8PFJet80_v0","HLT_AK8PFJet140_v0","HLT_AK8PFJet200_v0","HLT_AK8PFJet260_v0","HLT_AK8PFJet320_v0","HLT_AK8PFJet400_v0","HLT_AK8PFJet450_v0","HLT_AK8PFJet500_v0","HLT_AK8PFJet550_v0")
      'zb' : cms.vstring("HLT_ZeroBias_v0","HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0")
    },
    'dt' : {
      'ak4' : cms.vstring("HLT_PFJet40_v0","HLT_PFJet60_v0","HLT_PFJet80_v0","HLT_PFJet140_v0","HLT_PFJet200_v0","HLT_PFJet260_v0","HLT_PFJet320_v0","HLT_PFJet400_v0","HLT_PFJet450_v0","HLT_PFJet500_v0","HLT_PFJet550_v0"),
      'ak8' : cms.vstring(),
      #'ak8' : cms.vstring("HLT_AK8PFJet40_v0","HLT_AK8PFJet60_v0","HLT_AK8PFJet80_v0","HLT_AK8PFJet140_v0","HLT_AK8PFJet200_v0","HLT_AK8PFJet260_v0","HLT_AK8PFJet320_v0","HLT_AK8PFJet400_v0","HLT_AK8PFJet450_v0","HLT_AK8PFJet500_v0","HLT_AK8PFJet550_v0"),
      'zb' : cms.vstring("HLT_ZeroBias_v0")
    },
    'A' : {
      'ak4' : cms.vstring("HLT_PFJet40_v19","HLT_PFJet40_v20","HLT_PFJet40_v21","HLT_PFJet60_v19","HLT_PFJet60_v20","HLT_PFJet60_v21","HLT_PFJet80_v18","HLT_PFJet80_v19","HLT_PFJet80_v20","HLT_PFJet140_v17","HLT_PFJet140_v18","HLT_PFJet140_v19","HLT_PFJet200_v17","HLT_PFJet200_v18","HLT_PFJet200_v19","HLT_PFJet260_v18","HLT_PFJet260_v19","HLT_PFJet260_v20","HLT_PFJet320_v18","HLT_PFJet320_v19","HLT_PFJet320_v20","HLT_PFJet400_v18","HLT_PFJet400_v19","HLT_PFJet400_v20","HLT_PFJet450_v19","HLT_PFJet450_v20","HLT_PFJet450_v21","HLT_PFJet500_v19","HLT_PFJet500_v20","HLT_PFJet500_v21","HLT_PFJet550_v10","HLT_PFJet550_v11","HLT_PFJet550_v9"),
      'ak8' : cms.vstring("HLT_AK8PFJet40_v14","HLT_AK8PFJet40_v15","HLT_AK8PFJet40_v16","HLT_AK8PFJet60_v13","HLT_AK8PFJet60_v14","HLT_AK8PFJet60_v15","HLT_AK8PFJet80_v13","HLT_AK8PFJet80_v14","HLT_AK8PFJet80_v15","HLT_AK8PFJet140_v13","HLT_AK8PFJet140_v14","HLT_AK8PFJet140_v15","HLT_AK8PFJet200_v13","HLT_AK8PFJet200_v14","HLT_AK8PFJet200_v15","HLT_AK8PFJet260_v14","HLT_AK8PFJet260_v15","HLT_AK8PFJet260_v16","HLT_AK8PFJet320_v14","HLT_AK8PFJet320_v15","HLT_AK8PFJet320_v16","HLT_AK8PFJet400_v14","HLT_AK8PFJet400_v15","HLT_AK8PFJet400_v16","HLT_AK8PFJet450_v14","HLT_AK8PFJet450_v15","HLT_AK8PFJet450_v16","HLT_AK8PFJet500_v14","HLT_AK8PFJet500_v15","HLT_AK8PFJet500_v16","HLT_AK8PFJet550_v10","HLT_AK8PFJet550_v11","HLT_AK8PFJet550_v9"),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    },
    'B' : {
      'ak4' : cms.vstring("HLT_PFJet40_v21","HLT_PFJet60_v21","HLT_PFJet80_v20","HLT_PFJet140_v19","HLT_PFJet200_v19","HLT_PFJet260_v20","HLT_PFJet320_v20","HLT_PFJet400_v20","HLT_PFJet450_v21","HLT_PFJet500_v21","HLT_PFJet550_v11"),
      'ak8' : cms.vstring("HLT_AK8PFJet15_v3","HLT_AK8PFJet25_v3","HLT_AK8PFJet40_v16","HLT_AK8PFJet60_v15","HLT_AK8PFJet80_v15","HLT_AK8PFJet140_v15","HLT_AK8PFJet200_v15","HLT_AK8PFJet260_v16","HLT_AK8PFJet320_v16","HLT_AK8PFJet400_v16","HLT_AK8PFJet450_v16","HLT_AK8PFJet500_v16","HLT_AK8PFJet550_v11"),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    },
    'C' : {
      'ak4' : cms.vstring("HLT_PFJet40_v21","HLT_PFJet60_v21","HLT_PFJet80_v20","HLT_PFJet140_v19","HLT_PFJet200_v19","HLT_PFJet260_v20","HLT_PFJet320_v20","HLT_PFJet400_v20","HLT_PFJet450_v21","HLT_PFJet500_v21","HLT_PFJet550_v11"),
      'ak8' : cms.vstring(),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    },
    'D' : {
      'ak4' : cms.vstring("HLT_PFJet40_v21","HLT_PFJet60_v21","HLT_PFJet80_v20","HLT_PFJet140_v19","HLT_PFJet200_v19","HLT_PFJet260_v20","HLT_PFJet320_v20","HLT_PFJet400_v20","HLT_PFJet450_v21","HLT_PFJet500_v21","HLT_PFJet550_v11"),
      'ak8' : cms.vstring(),
      'zb' : cms.vstring("HLT_ZeroBias_v6")
    }
  }
}
