#!/bin/bash

for dir in runs_dt/crab_*; do
  crab status -d $dir
done

for dir in runs_mc/crab_*; do
  crab status -d $dir
done
