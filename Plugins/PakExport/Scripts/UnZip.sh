#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

$SCRIPTPATH/Cleanup.sh

UtilsDir="$SCRIPTPATH/../Utils"

"$UtilsDir/7-Zip-Mac/7zz" x "$UtilsDir/PakExport.7z" -o"$UtilsDir"
