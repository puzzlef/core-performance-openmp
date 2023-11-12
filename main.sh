#!/usr/bin/env bash
src="core-performance-openmp"
out="$HOME/Logs/$src$1.log"
ulimit -s unlimited
printf "" > "$out"

# Download program
if [[ "$DOWNLOAD" != "0" ]]; then
  rm -rf $src
  git clone https://github.com/puzzlef/$src
  cd $src
fi

# Fixed config
: "${TYPE:=double}"
: "${MAX_THREADS:=64}"
: "${REPEAT_METHOD:=5}"
# Define macros (dont forget to add here)
DEFINES=(""
"-DTYPE=$TYPE"
"-DMAX_THREADS=$MAX_THREADS"
"-DREPEAT_METHOD=$REPEAT_METHOD"
)

# Run
g++ ${DEFINES[*]} -std=c++17 -O3 -fopenmp main.cxx
stdbuf --output=L ./a.out 1000000000 2>&1 | tee -a "$out"

# Signal completion
curl -X POST "https://maker.ifttt.com/trigger/puzzlef/with/key/${IFTTT_KEY}?value1=$src$1"
