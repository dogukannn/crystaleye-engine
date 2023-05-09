import argparse
import subprocess
import logging
import os

shader_directory = "Shaders"
sdl_path = "C:\Projects\External\SDL2-2.26.5" 

parser = argparse.ArgumentParser(description = "Generate project files for Crystal Eye Rendering Engine")

parser.add_argument("--clear-build-folder", "-c", action = "store_true",
                    default = False, help = "Remove everything in the build folder")

parser.add_argument("--compile-shaders", "-sh", action = "store_true",
                    default = True, help = "Compile shader files to spv binary")


def compile_shaders():
    for file in os.listdir(shader_directory):
        filepath = os.path.join(shader_directory, file)
        if os.path.isfile(filepath) and (filepath.endswith(".frag") or \
                filepath.endswith(".vert") or filepath.endswith(".comp")): 
            logging.info("Compiling shader " + filepath)
            subprocess.run(["glslc", filepath, "-o", filepath + ".spv"])
            
    

if __name__ == '__main__':

    args = parser.parse_args()

    if args.clear_build_folder:
        subprocess.run(["del", "/s", "/q", ".\Build\*"], shell=True)

    
    logging.basicConfig(
        #filename='GenerateProject.log',
        level=logging.DEBUG,
        format='[%(asctime)s.%(msecs)03d] [%(levelname)s] : %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S',)

    compile_shaders()

    logging.info("Deleting CMake cache")
    subprocess.run(["del", "/s", "/q", ".\Build\CMakeCache.txt"], shell=True)
   
    logging.info("Generating project files for Crystal Eye Engine")
    subprocess.run(["cmake", "--fresh", "-Dsdl2_DIR="+sdl_path, "-S", ".", "-B", "Build"])
