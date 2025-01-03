#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

UEDir=$1
DestDir=$2
Guid=$3
GenerateCollision=$4

echo UEDir=$UEDir
echo DestDir=$DestDir
echo Guid=$Guid
echo GenerateCollision=$GenerateCollision

mkdir -p $SCRIPTPATH/Logs

exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>$SCRIPTPATH/Logs/CookAndPak.out 2>&1

UtilsDir="$SCRIPTPATH/../Utils"

PakExportDir="$UtilsDir/PakExport"
PakEDir="$PakExportDir/Plugins/${Guid}"
PakESavedDir="$PakEDir/Saved"
SourcePakPath="$PakESavedDir/StagedBuilds/Mac/PakExport/Plugins/${Guid}/Content/Paks/Mac/${Guid}PakExport-Mac.pak"

UECmdDir=$UEDir/Binaries/Mac/UnrealEditor-Cmd
ProjectFilePath="${PakExportDir}/PakExport.uproject"

#Move assets
#"$UEDir/Build/BatchFiles/Mac/Build.sh" "PakExportEditor" Mac Development "$ProjectFilePath"

while true
do
    result=$("$UECmdDir" "$ProjectFilePath" -IgnoreChangeList map=/Game/Minimal_Default.umap -UNATTENDED -RUNNINGUNATTENDEDSCRIPT -collision=True -Save -rebuild -nosourcecontrol -previewsurface=True -crashforuat -stdout -SkipSkinVerify -verbose=True -buildtexturestreaming -AllowCommandletRendering -rhithread -NOSCREENMESSAGES -run=MoveAssets -Guid=$3 -GenerateCollision=$4)

    if [ $result = 0 ]; then
        break
    fi
done

rm -rf "$PakExportDir/Source"
"$UtilsDir/7-Zip-Mac/7zz" x "$PakExportDir/PakExportNoBuild.7z" -o"$PakExportDir" -y

#cook game
$SCRIPTPATH/CookGame.sh $UEDir

#cook PakE help plugin
$SCRIPTPATH/CookPakE.sh $UEDir $Guid

#copy pak to destination dir
#mkdir -p "$d"
cp "$SourcePakPath" "$DestDir"
