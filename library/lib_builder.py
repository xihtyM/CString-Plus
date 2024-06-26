import os

def clear_and_create_object_files():
    for entry in os.scandir():
        if entry.name.endswith((".lib", ".o")):
            os.remove(entry.name)
    
    for entry in os.scandir("../impl"):
        os.system("gcc ../impl/" + entry.name + " -c")
    

def clear_object_files_and_create_library():
    command = "ar rcs libcstringplus.lib "

    for entry in os.scandir():
        if entry.name.endswith(".o"):
            command += entry.name + " "
    
    os.system(command)

    for entry in os.scandir():
        if entry.name.endswith(".o"):
            os.remove(entry.name)

if __name__ == "__main__":
    clear_and_create_object_files()
    clear_object_files_and_create_library()