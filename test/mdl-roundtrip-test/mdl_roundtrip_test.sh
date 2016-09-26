#!/bin/bash

# Path variables
TESTFILES_DIR="testfiles"
DDMORE_CONVERTER="converter-toolbox-distribution/converter-toolbox/convert.sh"
PHARMML2MDL="../../mdl"
CACHE_DIR="cache"

# Define colours:
RED=$(tput setaf 1)
GRE=$(tput setaf 2)
YEL=$(tput setaf 3)
BLU=$(tput setaf 4)
PUR=$(tput setaf 5)
CYA=$(tput setaf 6)
NOR=$(tput sgr0)

# Check directory existence and get list of testfiles
if [ ! -d "$TESTFILES_DIR" ]; then
  echo "${RED}MDL testfile directory ${PUR}${TESTFILES_DIR}${RED} does not exist${NOR}"
  exit 1
fi
WD="$(pwd)"
cd "$TESTFILES_DIR"
TESTFILES=(`find -type f -name "*.mdl" | sed "s|^\./||" | sort -V`)
cd "$WD"

# Check if cache directory (of MDL->PharmML conversions) exist and load cache
declare -a CACHE_FILES
declare -a CACHE_HASHES
if [ -d "$CACHE_DIR" ]; then
    # Get all (MDL) hashes (32 char MD5) and cached (PharmML) files
    cd "$CACHE_DIR"
    cache_commit=$(cat "last_commit")
    CACHE_FILES=($(find `pwd` -type f -regextype sed -regex ".*/[a-f0-9]\{32\}\.xml"))
    if (( ${#CACHE_FILES[@]} > 0 )); then
        echo "${YEL}${#CACHE_FILES[@]}${NOR} MDL->PharmML conversions cached (last commit: ${PUR}$cache_commit${NOR})"
        CACHE_HASHES=("${CACHE_FILES[@]##*/}") # Strip path
        CACHE_HASHES=("${CACHE_HASHES[@]%.*}") # Strip file extension
    fi
else
    mkdir "$CACHE_DIR"
fi
cd "$WD"

# Check converter availability
if [ ! -x "$DDMORE_CONVERTER" ]; then
  echo "${RED}DDMoRe converter ${PUR}${DDMORE_CONVERTER}${RED} does not exist or is not executable${NOR}"
  exit 1
fi
if [ ! -x "$PHARMML2MDL" ]; then
  echo "${RED}pharmml2mdl converter ${PUR}${PHARMML2MDL}${RED} does not exist or is not executable${NOR}"
  exit 1
fi

# Get some Git information
HASH="$(git rev-parse --short HEAD)"
TIME="$(git log -1 --date=format:"%y%m%d-%H%M" --format=%ad)"
LONG_TIME="$(git log -1 --date=local --format=%ad)"
WD="${TIME}.${HASH}"

# Save commit information to cache directory
echo "${TIME}.${HASH}" > "$CACHE_DIR/last_commit"

# Output some information
echo "Last Git commit: ${PUR}$HASH${NOR} ($LONG_TIME)"
echo "Conversion results will be saved in ${PUR}$WD${NOR} folder"

# Declare cache lookup function (true and echoes path if cache exists)
function get_cached_file() {
    local file="$1"
    local file_hash=$(md5sum "$file" | awk '{ print $1 }')
    for cached_hash in "${CACHE_HASHES[@]}"; do
        if [[ "$cached_hash" == "$file_hash" ]]; then
            for path in "${CACHE_FILES[@]}"; do
                if [[ "${path##*/}" == "${cached_hash%.*}.xml" ]]; then
                    echo "$path"
                    return 0;
                fi
            done
        fi
    done
    return 1
}

####################################################
# Initialize pass 1 (successful conversion) arrays #
####################################################
PASS1_PHARMML=()
PASS1_MDL=()

# Run pass 1A: MDL->PharmML (via DDMoRe P5.1 converter)
echo
echo "Running pass 1A: MDL->PharmML (via DDMoRe P5.1 converter)"
for i in "${!TESTFILES[@]}"; do
    file="${TESTFILES[$i]}"
    filenum=$(($i+1))
    num_files=${#TESTFILES[@]}

    # Get file paths
    in_file="$TESTFILES_DIR/$file"
    out_file="$WD/pass1/${file%.mdl}.xml"
    out_file_dir="$WD/pass1/${file%/*}"
    log_file="${out_file}.log"
    mkdir -p "$out_file_dir"
    echo -n "Converting ${BLU}$file${NOR} [${YEL}$filenum/$num_files${NOR}] "

    cache_path=$(get_cached_file "$in_file")
    success=1
    if [ -z "$cache_path" ]; then
        # Convert file and output logfile
        $DDMORE_CONVERTER "$in_file" "$out_file_dir" "MDL" "8" "PharmML" "0.8.1" 1>"${log_file}" 2>&1
        success=$?
    else
        # Skip conversion if cached (copy that file instead)
        echo -n "(CACHED) "
        cp "$cache_path" "$out_file"
        if [ -s "$out_file" ]; then
            success=0
        fi
    fi

    # Output success/failure status
    checksum=$(md5sum "$in_file" | awk '{ print $1 }')
    if [[ $success == 0 ]] && [[ -f "$out_file" ]]; then
        echo "(${GRE}SUCCESS${NOR})"
        PASS1_PHARMML+=("$out_file")

        # Cache for next execution
        cp "$out_file" "$CACHE_DIR/$checksum.xml"
    else
        echo "(${RED}FAIL${NOR})"

        # Cache empty file (representing failed conversion) for next execution
        touch "$CACHE_DIR/$checksum.xml"
    fi
done

# Run pass 1B: PharmML->MDL (via pharmml2mdl)
echo
echo "Running pass 1B: PharmML->MDL (via pharmml2mdl)"
for i in "${!PASS1_PHARMML[@]}"; do
    file="${PASS1_PHARMML[$i]}"
    filenum=$(($i+1))
    num_files=${#PASS1_PHARMML[@]}

    # Get file paths
    in_file="$file"
    out_file="${file%.xml}.mdl"
    out_file_dir="${file%/*}"
    log_file="${out_file}.log"
    echo -n "Converting ${BLU}$file${NOR} [${YEL}$filenum/$num_files${NOR}] "

    # Convert file
    warnings=$((
    (
    $PHARMML2MDL "$in_file"
    ) 1>$out_file
    ) 2>&1)

    # Output success/failure status
    if [[ $? == 0 ]] && [[ -f "$out_file" ]]; then
        echo "(${GRE}SUCCESS${NOR})"
        PASS1_MDL+=("$out_file")
    else
        echo "(${RED}FAIL${NOR})"
    fi

    # Output logfile
    echo "$warnings" > $log_file
done

####################################################
# Initialize pass 2 (successful conversion) arrays #
####################################################
PASS2_PHARMML=()
PASS2_MDL=()

# Run pass 2A: MDL->PharmML (via DDMoRe P5.1 converter)
echo
echo "Running pass 2A: MDL->PharmML (via DDMoRe P5.1 converter)"
for i in "${!PASS1_MDL[@]}"; do
    file="${PASS1_MDL[$i]}"
    filenum=$(($i+1))
    num_files=${#PASS1_MDL[@]}

    # Get file paths
    in_file="$file"
    out_file=$(echo $in_file | sed "s|pass1|pass2|")
    out_file="${out_file%.mdl}.xml"
    out_file_dir="${out_file%/*}"
    log_file="${out_file}.log"
    mkdir -p "$out_file_dir"
    echo -n "Converting ${BLU}$file${NOR} [${YEL}$filenum/$num_files${NOR}] "

    cache_path=$(get_cached_file "$in_file")
    success=1
    if [ -z "$cache_path" ]; then
        # Convert file and output logfile
        $DDMORE_CONVERTER "$in_file" "$out_file_dir" "MDL" "8" "PharmML" "0.8.1" 1>"${log_file}" 2>&1
        success=$?
    else
        # Skip conversion if cached (copy that file instead)
        echo -n "(CACHED) "
        cp "$cache_path" "$out_file"
        if [ -s "$out_file" ]; then
            success=0
        fi
    fi

    # Output success/failure status
    checksum=$(md5sum "$in_file" | awk '{ print $1 }')
    if [[ $success == 0 ]] && [[ -f "$out_file" ]]; then
        echo "(${GRE}SUCCESS${NOR})"
        PASS2_PHARMML+=("$out_file")

        # Cache for next execution
        cp "$out_file" "$CACHE_DIR/$checksum.xml"
    else
        echo "(${RED}FAIL${NOR})"

        # Cache empty file (representing failed conversion) for next execution
        touch "$CACHE_DIR/$checksum.xml"
    fi
done

# Run pass 2B: PharmML->MDL (via pharmml2mdl)
echo
echo "Running pass 2B: PharmML->MDL (via pharmml2mdl)"
for i in "${!PASS2_PHARMML[@]}"; do
    file="${PASS2_PHARMML[$i]}"
    filenum=$(($i+1))
    num_files=${#PASS2_PHARMML[@]}

    # Get file paths
    in_file="$file"
    out_file="${file%.xml}.mdl"
    out_file_dir="${file%/*}"
    log_file="${out_file}.log"
    echo -n "Converting ${BLU}$file${NOR} [${YEL}$filenum/$num_files${NOR}] "

    # Convert file
    warnings=$((
    (
    $PHARMML2MDL "$in_file"
    ) 1>$out_file
    ) 2>&1)

    # Output success/failure status
    if [[ $? == 0 ]] && [[ -f "$out_file" ]]; then
        echo "(${GRE}SUCCESS${NOR})"
        PASS2_MDL+=("$out_file")
    else
        echo "(${RED}FAIL${NOR})"
    fi

    # Output logfile
    echo "$warnings" > $log_file
done

###################
# Validation pass #
###################
PHARMML_EQ=()
MDL_EQ=()

# Compare PharmML's from 1A and 2A (via sdiff)
echo
echo "Comparing PharmML generated in pass 1A and 2A"
for file in "${PASS2_PHARMML[@]}"; do
    # Get file paths
    pass2_pharmml="$file"
    pass1_pharmml=$(echo $pass2_pharmml | sed "s|pass2|pass1|")
    basename=${file##*pass2/}
    echo -n "Analyzing ${BLU}$basename${NOR} "

    # Check diff of PharmML files
    diff=$(sdiff -B -b -s -d "$pass1_pharmml" "$pass2_pharmml")

    # Output success/failure status
    if [[ $diff =~ ^\ *$ ]] ; then
        echo "(${GRE}100 % EQUIVALENT${NOR})"
        PHARMML_EQ+=("$basename")
    else
        sort_diff=$(sdiff -B -b -s -d <(sort "$pass1_pharmml") <(sort "$pass2_pharmml"))
        if [[ $sort_diff =~ ^\ *$ ]] ; then
            echo "(${YEL}100 % SORTED EQUIVALENT${NOR})"
            PHARMML_EQ+=("$basename")
        else
            sort_diff_lines=$(echo "$sort_diff" | wc -l)
            echo "(${RED}$sort_diff_lines (SORTED) LINES DIFFER${NOR})"
            if [[ $sort_diff_lines < 11 ]]; then
                echo "$sort_diff"
            fi
        fi
    fi
done

# Compare MDL's from 1B and 2B (via sdiff)
echo
echo "Comparing MDL generated in pass 1B and 2B"
for file in "${PASS2_MDL[@]}"; do
    # Get file paths
    pass2_mdl="$file"
    pass1_mdl=$(echo $pass2_mdl | sed "s|pass2|pass1|")
    basename=${file##*pass2/}
    echo -n "Analyzing ${BLU}$basename${NOR} "

    # Check diff of MDL files
    diff=$(sdiff -B -b -s -d "$pass1_mdl" "$pass2_mdl")

    # Output success/failure status
    if [[ $diff =~ ^\ *$ ]] ; then
        echo "(${GRE}100 % EQUIVALENT${NOR})"
        MDL_EQ+=("$basename")
    else
        sort_diff=$(sdiff -B -b -s -d <(sort "$pass1_mdl") <(sort "$pass2_mdl"))
        if [[ $sort_diff =~ ^\ *$ ]] ; then
            echo "(${YEL}100 % SORTED EQUIVALENT${NOR})"
            MDL_EQ+=("$basename")
        else
            sort_diff_lines=$(echo "$sort_diff" | wc -l)
            echo "(${RED}$sort_diff_lines (SORTED) LINES DIFFER:${NOR})"
            echo "$sort_diff"
        fi
    fi
done

#####################
# Summarize results #
#####################

# Output number of files through the sieve
echo
echo "Number of files sieved in passes and passing equivalence validation:"
   passes=(""               "[1A]"               "[1B]"           "[2A]"               "[2B]"           "[1A & 2A]"       "[1B & 2B]")
   labels=("MDL"            "MDL->PharmML"       "PharmML->MDL"   "MDL->PharmML"       "PharmML->MDL"   "100 % PharmML"   "100 % MDL")
num_files=(${#TESTFILES[@]} ${#PASS1_PHARMML[@]} ${#PASS1_MDL[@]} ${#PASS2_PHARMML[@]} ${#PASS2_MDL[@]} ${#PHARMML_EQ[@]} ${#MDL_EQ[@]})
for((i=0;i<7;i++)); do
    printf "%10s %18s %s\n" "${passes[$i]}" "${labels[$i]} " "${num_files[$i]}"
done

# Output results table
resfile="$WD/results.csv"
echo
echo "Saving results table file ${PUR}${resfile}${NOR}"
fileIn () {
    local e
    for e in "${@:2}"; do
        [[ "$e" =~ "${1%.mdl}." ]] && echo "PASS" && return 0
    done
    echo "" && return 1
}
format='"%s","%s","%s","%s","%s","%s","%s"'"\n"
printf "$format" "Testfile name" "[1A] MDL->PharmML" "[1B] PharmML->MDL" "[2A] MDL->PharmML" "[2B] PharmML->MDL" "[1A & 2A] 100 % PharmML" "[1B & 2B] 100 % MDL" > "$resfile"
for file in "${TESTFILES[@]}"; do
    printf "$format" \
        "$file" \
        "$(fileIn "$file" "${PASS1_PHARMML[@]}")" \
        "$(fileIn "$file" "${PASS1_MDL[@]}")" \
        "$(fileIn "$file" "${PASS2_PHARMML[@]}")" \
        "$(fileIn "$file" "${PASS2_MDL[@]}")" \
        "$(fileIn "$file" "${PHARMML_EQ[@]}")" \
        "$(fileIn "$file" "${MDL_EQ[@]}")" \
        >> "$resfile"
done
