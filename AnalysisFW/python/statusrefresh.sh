#!/bin/bash

CASES=4
RESTARTS=0
while [ $CASES -gt 0 ]; do
  echo ">>>>>>>>>>>>>>>>>>>>>>>>>>"
  echo "Starting a new iteration!"
  now=$(date +"%T")
  echo "Current time: $now"
  echo "Jobs refreshed: $RESTARTS"
  echo "<<<<<<<<<<<<<<<<<<<<<<<<<<"

  CASES=0
  echo "Data runs:"
  if [ -z "$(ls -A runs_dt)" ]; then
     echo "None!"
  else
    for dir in runs_dt/crab_*; do
      echo ">>>>>>"
      echo $dir
      JSON=$(crab status -d $dir --json | tail -n 2 | head -n 1 | python3 -c "import sys, json; states = [v['State'] for a,v in json.load(sys.stdin).items()]; print(' '.join(states));")
      all=$(echo $JSON | python3 -c "import sys; print(len(sys.stdin.read().rstrip().split(' ')));")
      queue=$(echo $JSON | python3 -c "import sys; states = list(filter(lambda a: a != 'finished', sys.stdin.read().rstrip().split(' '))); print(len(states))")
      fin=$((all-queue))
      if [ $queue -gt 0 ]; then
        CASES=$((CASES+1))
        echo "Finished $fin/$all jobs!"
        run=$(echo $JSON | tail -n 2 | head -n 1 | python3 -c "import sys; states = list(filter(lambda a: a == 'running', sys.stdin.read().rstrip().split(' '))); print(len(states))")
        fails=$(echo $JSON | tail -n 2 | head -n 1 | python3 -c "import sys; states = list(filter(lambda a: a == 'failed', sys.stdin.read().rstrip().split(' '))); print(len(states))")
        other=$((queue-run-fails))
        if [ $fails -gt 0 ]; then
          echo "Resubmitting failures: $fails"
          crab resubmit -d $dir
          RESTARTS=$((RESTARTS+1))
        fi
        if [ $run -gt 0 ]; then
          echo "Running jobs: $run"
        fi
        if [ $other -gt 0 ]; then
          echo "Other jobs in the mill: $other"
        fi
      else
        echo "Finished all $fin jobs!"
      fi
    done
    echo "<<<<<<"
  fi

  echo "MC runs:"
  if [ -z "$(ls -A runs_mc)" ]; then
     echo "None!"
  else
    for dir in runs_mc/crab_*; do
      echo ">>>>>>"
      echo $dir
      JSON=$(crab status -d $dir --json | tail -n 2 | head -n 1 | python3 -c "import sys, json; states = [v['State'] for a,v in json.load(sys.stdin).items()]; print(' '.join(states));")
      all=$(echo $JSON | python3 -c "import sys; print(len(sys.stdin.read().rstrip().split(' ')));")
      queue=$(echo $JSON | python3 -c "import sys; states = list(filter(lambda a: a != 'finished', sys.stdin.read().rstrip().split(' '))); print(len(states))")
      fin=$((all-queue))
      if [ $queue -gt 0 ]; then
        CASES=$((CASES+1))
        echo "Finished $fin/$all jobs!"
        run=$(echo $JSON | tail -n 2 | head -n 1 | python3 -c "import sys; states = list(filter(lambda a: a == 'running', sys.stdin.read().rstrip().split(' '))); print(len(states))")
        fails=$(echo $JSON | tail -n 2 | head -n 1 | python3 -c "import sys; states = list(filter(lambda a: a == 'failed', sys.stdin.read().rstrip().split(' '))); print(len(states))")
        other=$((queue-run-fails))
        if [ $fails -gt 0 ]; then
          echo "Resubmitting failures: $fails"
          crab resubmit -d $dir
          RESTARTS=$((RESTARTS+1))
        fi
        if [ $run -gt 0 ]; then
          echo "Running jobs: $run"
        fi
        if [ $other -gt 0 ]; then
          echo "Other jobs in the mill: $other"
        fi
      else
        echo "Finished all $fin jobs!"
      fi
    done
    echo "<<<<<<"
  fi

  echo "Loop finished, going to sleep!"
  sleep 4m
done
