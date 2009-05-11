#!/bin/bash

if [ "$1" = "--help" ]; then
  echo This is a wrapper to convert potential to gromacs
  echo Usage: ${0##*/} 
  echo Needs: \$SOURCE_WRAPPER, run_or_exit
  exit 0
fi

table_to_xvg="$($SOURCE_WRAPPER convert_potential xvg)" || die "$SOURCE_WRAPPER convert_potential xvg failed" 

name=$($csg_get name)
input="${name}.pot.cur" 
#gromacs want '_' !
output="$($csg_get gromacs.table)" 
log "Convert $input to $output"

r_cut=$($csg_get max)
gromacs_bins="$(get_sim_property gromacs.table_bins)"

run_or_exit csg_resample --in ${input} --out smooth_${input} --grid 0:${gromacs_bins}:${r_cut} 
run_or_exit ${table_to_xvg} smooth_${input} ${output}
