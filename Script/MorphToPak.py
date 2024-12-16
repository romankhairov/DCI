import unreal
import argparse

argparser = argparse.ArgumentParser(add_help=False)
argparser.add_argument('Length',
                       help='Morph length')
argparser.add_argument('Width',
                       help='Morph width')
argparser.add_argument('Edge',
                       help='Morph edge')
argparser.add_argument('SizeX',
                       help='Size X')
argparser.add_argument('SizeY',
                       help='Size Y')
argparser.add_argument('Gap',
                       help='Gap')
argparser.add_argument('Styling',
                       help='Styling')
args = argparser.parse_args()

morphs = unreal.Array(unreal.MorphTargetData)
morphs.append(unreal.MorphTargetData('width', float(args.Width)));
morphs.append(unreal.MorphTargetData('length', float(args.Length)));
morphs.append(unreal.MorphTargetData('edge', float(args.Edge)));

print("Morph to static mesh started...")

unreal.PakExportUtility.make_bunch_from_morphs('/Game/T_plank_morph2', '/Game/mat_test/01/MS_01_Inst', '/Game/mat_test/03/MS_02_Inst1', '/Game/mat_test/04/MS_02_Inst1', '/Game/Meshes/StaticMesh', morphs, unreal.Vector2D(float(args.SizeX), float(args.SizeY)), float(args.Gap), args.Styling, True)

print("Morph to static mesh complete!")
