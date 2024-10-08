from pathlib import Path
import subprocess
import shutil
import os

libs = "openai pyaudio scipy numpy webrtcvad adafruit-circuitpython-ads1x15"
# ai is on controller for now, can easily be swapped
# also AP setup is done on controller because it sends commands to the slave (robot)

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

    # set up AP with borrowed script (will this even work?)
    run('sudo chmod +x setup.sh')
    run('./setup.sh')

    # insert other setup stuff here

    print("Setup complete! Please reboot.")  # for testing, automatic rebooting will have to be done alter


if __name__ == "__main__":
    setup()