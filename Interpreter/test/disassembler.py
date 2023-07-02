# loop through all the files in the test directory and disassemble them using "mmvm -d <filename>"
#the files that are going to e disassembled ar the ones that have the extension .out
# the output of the disassembler is going to be saved in a file with the same name as the input file but with the extension .dis
# the files are in the root of the project (this program has to be run inside the test folder)
# the cmd has to run by the system call

import os

for file in os.listdir("."):
    if file.endswith(".out"):
        os.system("mmvm -d " + file + " > " + file[:-4] + ".dis 2>&1")
        