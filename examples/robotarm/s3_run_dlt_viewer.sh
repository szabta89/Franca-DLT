#!/bin/sh

# start DLT viewer with contract validator plug-in

# the trace-generating application
DLT_VIEWER_DIR="$HOME/sw/dltviewer/debug"

cd $DLT_VIEWER_DIR
export LD_LIBRARY_PATH=.
./dlt_viewer

