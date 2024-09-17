from pathlib import Path
import subprocess
import shutil
import os

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

    # insert other setup stuff here

    print("Setup complete!")

if __name__=="__main__":
    setup()