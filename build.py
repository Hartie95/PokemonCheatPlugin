#!/usr/bin/python
import sys
import os
import ftplib
import glob

# the path plugin was copied to
COPYTOPATH = 'cheat.plg'

CC = "arm-none-eabi-gcc"
CP = "arm-none-eabi-g++"
OC = "arm-none-eabi-objcopy"
LD = "arm-none-eabi-ld"
CTRULIB = '../libctru'
DEVKITARM = os.environ['DEVKITARM']
LIBPATH = '-L ' + DEVKITARM + '/lib/gcc/arm-none-eabi/5.3.0/' + ' -L ' + DEVKITARM + '/arm-none-eabi/lib/ -L obj'


def allFile(pattern):
    s = "";
    for file in glob.glob(pattern):
        s += file + " ";
    return s;

def run(cmd):
	#print(cmd);
	os.system(cmd)

cwd = os.getcwd()

if os.path.exists('obj'):
    run("rm obj/*.o")
else:
    run("mkdir obj") # We need this directory

if os.path.exists(''):
    run("rm bin/*.elf")
else:
    run("mkdir bin")

run(CC + 
    " -Os -s  -g -I include -I include/libntrplg " + 
    allFile('source/libntrplg/*.c')                +
    allFile('source/ns/*.c')                       +
    allFile('source/*.c')                          +
    allFile('source/battle/*.c')                   +
    allFile('source/rng/*.c')                      +
    allFile('source/libctru/*.c')                  +
    " -c  -march=armv6 -mlittle-endian")

run(CC                              +
    "  -Os "                        +
    allFile('source/libntrplg/*.s') +
    allFile('source/ns/*.s')        +
    allFile('source/*.s')           +
    allFile('source/libctru/*.s')   +
    " -c -s -march=armv6 -mlittle-endian ")

run(LD + ' ' + LIBPATH + " -pie --print-gc-sections  -T 3ds.ld -Map=homebrew.map " + 
    allFile("*.o") + " "     + 
    allFile("lib/*.o") + " " + 
    allFile("lib/*.a")       + 
    " -lc --nostdlib")

run("cp -r *.o obj/ ")
run("cp a.out bin/homebrew.elf ")
run(OC+" -O binary a.out payload.bin -S")
run("rm *.o")
run("rm *.out")
run('cp payload.bin ' + COPYTOPATH);
