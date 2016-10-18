#!/bin/bash

gcc -std=c++14 output.cpp -ooutput -I../../src -L../.. -lstdc++ -lpharmmlcpp $(xml2-config --cflags --libs) || exit
rm -rf results
mkdir -p results

TESTFILES_DIR="../mdl-roundtrip-test/161011-1351.0f4fc6b/pass1/Product5.1" # FIXME: How to handle test models long term?

# Define colours
RED=$(tput setaf 1)
GRE=$(tput setaf 2)
YEL=$(tput setaf 3)
BLU=$(tput setaf 4)
PUR=$(tput setaf 5)
CYA=$(tput setaf 6)
NOR=$(tput sgr0)

if [ ! -d "$TESTFILES_DIR" ]; then
  echo "${RED} testfile directory ${PUR}${TESTFILES_DIR}${RED} does not exist${NOR}"
  exit 1
fi

WD="$(pwd)"
cd "$TESTFILES_DIR"
TESTFILES=(`find -type f -name "*.xml" | sed "s|^\./||" | sort -V`)
cd "$WD"

numtotalfails=0
numtotaldiff=0

for i in "${!TESTFILES[@]}"; do
    file="${TESTFILES[$i]}"
    path="$TESTFILES_DIR/$file"
    original_lines=$(wc -l $path)
    outfile_dir="results/$(dirname $file)"

    echo -n $BLU $file $NOR
    mkdir -p $outfile_dir
    ./output "$TESTFILES_DIR/$file" >/dev/null 2>&1 |:
    if [[ -e output.xml ]]; then
        numdiff=$(xmldiff output.xml "$TESTFILES_DIR/$file" | wc -l)
        if [ "$numdiff" -gt "0" ]; then
            numtotaldiff=$((numtotaldiff + numdiff))
            echo "$RED (FAIL) $numdiff lines $NOR"
        fi
        mv output.xml "results/$file"
    else
        numtotalfails=$((numtotalfails + 1))
        echo "$RED (FAIL) no output $NOR"
    fi
done

echo "Total number of crashed models: " $numtotalfails
echo "Total number of difflines: " $numtotaldiff
