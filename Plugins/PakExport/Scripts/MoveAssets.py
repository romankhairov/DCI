# 

import unreal
import argparse

argparser = argparse.ArgumentParser(add_help=False)
argparser.add_argument('Guid',
                       help='Guid ')
args = argparser.parse_args()

unreal.PakExportEditorUtility.move_assets(args.Guid)
