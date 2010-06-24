#! /bin/bash
#
# Copyright 2009 The VOTCA Development Team (http://www.votca.org)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

if [ "$1" = "--help" ]; then
cat <<EOF
${0##*/}, version %version%
This script implemtents scaling of the potential update (.dpot)

Usage: ${0##*/} infile outfile

USES: csg_get_interaction_property do_external log check_deps

NEEDS: name inverse.post_update_options.scale
EOF
   exit 0
fi

check_deps "$0"

[ -z "$2" ] && die "${0##*/}: Missing arguments"

[ -f "$2" ] && die "${0##*/}: $2 is already there"

name=$(csg_get_interaction_property name)
scale=$(csg_get_interaction_property inverse.post_update_options.scale 1.0)

log "scaling potential update by factor $scale"

do_external table linearop "$1" "$2" $scale 0
