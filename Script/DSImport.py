import unreal
import argparse

argparser = argparse.ArgumentParser(add_help=False)
argparser.add_argument('DS_path',
                       help='Path to datasmith file. Example: d:\\tmp\\3DS\\Christine.step')
args = argparser.parse_args()

DS_path = args.DS_path
imported_path = "/Game/ImportedDS"

print("DS load started...")

ds_scene_in_memory = unreal.DatasmithSceneElement.construct_datasmith_scene_from_file(DS_path)

if ds_scene_in_memory is None:
    print("Scene loading failed.")
    quit()

import_options = ds_scene_in_memory.get_options(unreal.DatasmithImportOptions)
import_options.base_options.scene_handling = unreal.DatasmithImportScene.ASSETS_ONLY

result = ds_scene_in_memory.import_scene(imported_path)

if not result.import_succeed:
    print("Importing failed.")
    quit()

ds_scene_in_memory.destroy_scene()

unreal.EditorAssetLibrary.save_directory(imported_path)

print("Custom import process complete!")
