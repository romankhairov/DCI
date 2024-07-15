# Copyright 2024 3D Source, Inc. All Rights Reserved.

#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

Guid=$1

echo Guid=$Guid

$SCRIPTPATH/Cleanup.sh

UtilsDir="$SCRIPTPATH/../Utils"
PakExportDir="$UtilsDir/PakExport"
PakEDir="$PakExportDir/Plugins/${Guid}"

"$UtilsDir/7-Zip-Mac/7zz" x "$UtilsDir/PakExport/Plugins.7z" -o"$UtilsDir/PakExport"
"$UtilsDir/7-Zip-Mac/7zz" x "$UtilsDir/PakExport/Content.7z" -o"$UtilsDir/PakExport"

mv "$PakExportDir/Plugins/PakE" "$PakEDir"
mv "$PakEDir/PakE.uplugin" "$PakEDir/${Guid}.uplugin"
