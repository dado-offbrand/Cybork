from pathlib import Path
import subprocess
import shutil
import os

libs = "scipy numpy asyncio moteus" #is scipy needed?

def run(*args, **kwargs):
    print('running command: ' + args[0])
    subprocess.check_call(*args, shell=True, **kwargs)

def setup():
    # move updater.py to proper location
    cwd = os.getcwd()
    updater_location = os.path.join(cwd, 'updater.py')
    preroot = Path(cwd).parents[1]
    if os.path.exists(updater_location):
        shutil.move(updater_location, Path(cwd).parents[1])
    else:
        print("Ignoring initial updater.py setup because file was already moved / doesn't exist.")

    # install necessary libraries for robot code
    run(f'sudo apt-get install {libs}')
    print("Did that work? Ctrl+C to quit, ENTER to continue")
    input()
    
    # attempt to connect to controller AP

    # insert other setup stuff here

    print("Setup complete! Please reboot.") # for testing, automatic rebooting will have to be done alter

if __name__=="__main__":
    setup()