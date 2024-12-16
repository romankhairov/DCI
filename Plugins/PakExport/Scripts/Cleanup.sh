# Copyright 2024 3D Source, Inc. All Rights Reserved.

#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

UtilsDir="$SCRIPTPATH/../Utils"
PakExportDir="$UtilsDir/PakExport"

rm -rf "$PakExportDir/Plugins"
rm -rf "$PakExportDir/Content"

"$UtilsDir/7-Zip-Mac/7zz" x "$PakExportDir/PakExport.7z" -o"$PakExportDir" -y
