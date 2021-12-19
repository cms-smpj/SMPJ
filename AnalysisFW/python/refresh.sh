#!/bin/bash

for dir in runs_dt/crab_*; do
  crab resubmit -d $dir
done

for dir in runs_mc/crab_*; do
  crab resubmit -d $dir
done
