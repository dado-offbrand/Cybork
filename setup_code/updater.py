import subprocess
import shutil
import pathlib
import os

def remove_readonly(func, path, exc_info):
    os.chmod(path, 0o777)
    func(path)

def update():
    if pathlib.Path.cwd().name == "debug":
        print("Move the script to be on the same level as the root folder, then try again.")
        return

    repo_url = "https://github.com/dado-offbrand/Cybork.git"
    script_dir = os.path.dirname(os.path.abspath(__file__))
    destination_path = os.path.join(script_dir, "Cybork")

    if os.path.exists(destination_path):
        shutil.rmtree(destination_path, onerror=remove_readonly)
    subprocess.run(["git", "clone", repo_url, destination_path])

if __name__=="__main__":
    update()