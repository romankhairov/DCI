# Copyright 2024 3D Source, Inc. All Rights Reserved.

import unreal
import argparse

argparser = argparse.ArgumentParser(add_help=False)
argparser.add_argument('Guid',
                       help='Guid ')
argparser.add_argument('GenerateCollision',
                       help='GenerateCollision ')
args = argparser.parse_args()

unreal.PakExportEditorUtility.move_assets(args.Guid, args.GenerateCollision)
