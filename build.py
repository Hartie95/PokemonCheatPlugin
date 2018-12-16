#!/usr/bin/python
import sys
import os
import ftplib
import glob

# the path plugin was copied to
COPYTOPATH = 'cheat.plg'

# get devkitarm environment variable 
DEVKITARM = os.environ['DEVKITARM']
# set paths to local compilers
CC = DEVKITARM + "/bin/arm-none-eabi-gcc"
CP = DEVKITARM + "/bin/arm-none-eabi-g++"
OC = DEVKITARM + "/bin/arm-none-eabi-objcopy"
LD = DEVKITARM + "/bin/arm-none-eabi-ld"
CTRULIB = '../libctru'
LIBPATH = '-L ' + DEVKITARM + '/lib/gcc/arm-none-eabi/5.3.0/' + ' -L ' + DEVKITARM + '/arm-none-eabi/lib/ -L obj'

# define function to get all files in a folder based on pattern
def allFile(pattern):
    s = "";
    for file in glob.glob(pattern):
        s += file + " ";
    return s;

def run(cmd):
	#print(cmd);
	os.system(cmd)

# check if required folders exist
if not os.path.exists("obj"):
    os.makedirs("obj");
if not os.path.exists("bin"):
    os.makedirs("bin");


# actually build the thing
cwd = os.getcwd()
# clean up files from previous build
run("rm obj/*.o")
run("rm bin/*.elf")
run(CC+  " -Os -s  -g -I include -I include/libntrplg " + allFile('source/libntrplg/*.c') + allFile('source/ns/*.c') + allFile('source/*.c') + allFile('source/battle/*.c') + allFile('source/rng/*.c') + allFile('source/libctru/*.c') + " -c  -march=armv6 -mlittle-endian ");
run(CC+"  -Os " + allFile('source/libntrplg/*.s') +  allFile('source/ns/*.s')  + allFile('source/*.s') + allFile('source/libctru/*.s') + " -c -s -march=armv6 -mlittle-endian ");

run(LD + ' ' + LIBPATH + " -pie --print-gc-sections  -T 3ds.ld -Map=homebrew.map " + allFile("*.o") + " " + allFile("lib/*.o") + " " + allFile("lib/*.a") + " -lc --nostdlib")
run("cp -r *.o obj/ ")
run("cp a.out bin/homebrew.elf ")
run(OC+" -O binary a.out payload.bin -S")
run("rm *.o")
run("rm *.out")
run('cp payload.bin ' + COPYTOPATH);
