import subprocess
import os
import shutil

repo_url = "https://github.com/dado-offbrand/Cybork.git"

script_dir = os.path.dirname(os.path.abspath(__file__))
cybork_dir = os.path.dirname(script_dir)

temp_dir = os.path.join(cybork_dir, "temp_clone")
subprocess.run(["git", "clone", repo_url, temp_dir])

def copy_files(src_dir, dest_dir, exclude_file):
    for item in os.listdir(src_dir):
        src_path = os.path.join(src_dir, item)
        dest_path = os.path.join(dest_dir, item)

        if src_path == exclude_file:
            continue

        if os.path.isdir(src_path):
            if os.path.exists(dest_path):
                shutil.rmtree(dest_path)
            shutil.copytree(src_path, dest_path)
        else:
            shutil.copy2(src_path, dest_path)

copy_files(temp_dir, cybork_dir, os.path.join(script_dir, "updater.py"))
shutil.rmtree(temp_dir)

# untested code, running updater.py will pull the repo and update all code excluding itself