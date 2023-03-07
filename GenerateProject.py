import argparse
import subprocess
import logging

parser = argparse.ArgumentParser(description = "Generate project files for Crystal Eye Rendering Engine")

parser.add_argument("--clear-build-folder", "-c", action = "store_true",
                    default = False, help = "Remove everything in the build folder")

parser.add_argument("--compile-shaders", "-sh", action = "store_true",
                    default = True, help = "Compile shader files to spv binary")


if __name__ == '__main__':

    args = parser.parse_args()

    if args.clear_build_folder:
        subprocess.run(["del", "/s", "/q", ".\Build\*"], shell=True)

    
    logging.basicConfig(
        #filename='GenerateProject.log',
        level=logging.DEBUG,
        format='[%(asctime)s.%(msecs)03d] [%(levelname)s] : %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S',)

    logging.info("Generating project files for Crystal Eye Engine")
    subprocess.run(["cmake", "-S", ".", "-B", "Build"])

