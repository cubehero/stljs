#!/bin/bash

STL=$1
TMP=$2
START_DIR=`dirname $0`
BASEFILE=`echo "$STL" | sed -e 's/\.stl//g'`
OUTFILE=${BASEFILE}.png
POVFILE=${TMP}/`basename ${BASEFILE}.pov`
INCFILE=${TMP}/`basename ${BASEFILE}.inc`

#${START_DIR}/../py-stl-3.1/stl2pov.py "$STL" "${INCFILE}"
${START_DIR}/../stl2pov-2.5.0/stl2pov "$STL" > "${INCFILE}"
MODELNAME=`grep "#declare" "${INCFILE}" | cut -d\  -f2`
echo $INCFILE
cat ${START_DIR}/pov_layout.tmpl | sed -e "s@{{INCLUDE_FILE}}@${INCFILE}@g" -e "s@{{MODELNAME}}@${MODELNAME}@g" -e "s@{{BASE_PATH}}@${START_DIR}@g" > "${POVFILE}"
	   
# cat "${POVFILE}"
# povray -s -i"${POVFILE}" +FN +W1600 +H1200 -o"${OUTFILE}" +Q9 +AM2 +A2
povray -s -i"${POVFILE}" +FN +W800 +H600 -o"${OUTFILE}" +Q9 +AM2 +A2
echo "OUTFILE: ${OUTFILE}"
