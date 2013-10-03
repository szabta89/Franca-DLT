#!/bin/sh

# start simulated target application
# (it will send DLT traces to the dlt-daemon)

# the trace-generating application
TRACEGEN="../../org.franca.tools.contracts.tracegen.dlt/bin/tracegen"

# the folder where the traces are
TRACES="traces"

echo starting IF01
$TRACEGEN STA IF01 $TRACES/robotarm_trace_01_good.txt 2500 &
sleep 16

echo starting IF02
$TRACEGEN STA IF02 $TRACES/robotarm_trace_02_bad.txt 4000 &
sleep 10

echo starting IF03
$TRACEGEN STA IF03 $TRACES/robotarm_trace_03_bad.txt 7000 &

