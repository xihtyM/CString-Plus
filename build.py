import os, sys
MAIN_FILE = sys.argv[1]
COMPILER = "gcc" if MAIN_FILE.endswith(".c") else "g++"

if not sys.argv[2:]:
    std = "c2x" if COMPILER == "gcc" else "c++23"
else:
    std = sys.argv[2]

def build_command(path: str):
    command = ""
    for entry in os.scandir(path):
        if entry.is_dir():
            command += build_command(entry.path)
        
        if not entry.is_file() or not entry.name.endswith((".cc", ".c", ".cpp")):
            continue
        
        command += "\"%s\" " % entry.path
    
    return command

def build_project(name: str):
    command = COMPILER + " -flto -std=%s -o %s.exe -O2 -s " % (std, name)

    command += build_command("impl")
    
    command += MAIN_FILE
    
    os.system(command)

build_project("string")
