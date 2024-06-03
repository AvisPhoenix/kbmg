# Kubernetes Manager
## About
Kubernetes Manager is a Command Line Interface application to let you do the more commons actions in a more easy way than kubectl (by using it).
Currently is working with az (the CLI Azure), to change between environments.
The idea is reduce to one line to get information of your kubernetes pods, for example:
```bash
kbmg dev micro dlog user-data
```
Means: go to dev environment, use the micro namespace and download the logs for all the  pods which include on the name "user-data".

Don´t get me wrong, Kubectl is much more powerful, but some repetitive daily tasks could be more easy and quick to do with this tool.

## How to configure
You will need to create a file named "config.yaml" and add the configurations you want.
Currently support:
- Define log name and path
- Alias for name environments
- Alias for namespaces
- Alias for pod names
- Assign rules to use SH or Bash for execute command.

For more details run the help command:
```bash
kbmg -howConf
```
The file config.yaml is an example
## How to use
Command form use:
```bash
kbmg <environment> <namespace> <action> <pod>
```
<pod> you don´t need to add a pod name for the action call ¨pods¨.

Supported actions:
- **pods**: Get all the pods for that namespace and environment.
- **log**: Get the logs commands for all the pods which has name as part of his name, for that namespace and environment.
- **dlog**: Download all the logs of all the pods which has name as part of his name, for that namespace and environment.
- **exec** :Get the exec command for all the pods which has name as part of his name, for that namespace and environment. (this will follows the rules of config file, by default bash)

For more details run the help command:
```bash
kbmg -help
```
## How to compile
### What do you need before
In order to use kbmg you will need to have:
- [kubectl](https://kubernetes.io/docs/tasks/tools/).
- [az](https://learn.microsoft.com/en-us/cli/azure/).
- zip command

Note: kubectl is mandatory, because kbmg is just a shortcut to kubectl, az is the platform I use, I hope I could add support to other platforms to change between environments, or does not use anything to change environments.

### Dev dependencies
- CMake 3.5
- C++ (support with C++17)
- PStream ([you can install it on your system](https://pstreams.sourceforge.net/download/) or uncomment the line 47 of cmake, we already include a copy of version 1.0.3)
- YAML CPP (include with CMake, you don´t need to add it manually)

### Build
```bash
mkdir build
cd build
cmake ..
make
```
## To Do
- Add support to other platforms to change environments (like AWS)
- Add UI menus ([FTXUI](https://github.com/ArthurSonzogni/FTXUI/tree/main) looks great!!)
- Add support to more commands (like describe).
- Add support to prefix and suffix for environments names.
- Add support to define the config path.
- Let execute some commands without write the command. Currently kbmg generate the kubectl command to execute or display the log, but does nto execute it for you.
- Add password support for the zip files
- Let the user define the name of downloaded logs files.
- Compatibility with windows

# Contributors
<a class="AppHeader-logo Button--invisible Button--medium Button Button--invisible-noVisuals color-bg-transparent p-0">  <span class="Button-content" href="https://github.com/AvisPhoenix">
    <span class="Button-label"><img src="https://avatars.githubusercontent.com/u/49039545?v=4" alt="" size="32" height="32" width="32" class="avatar circle"></span>
  </span>
</a>

# License

This file is part of Kubernetes Manager (kbmg).

Kubernetes Manager is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Kubernetes Manager is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Kubernetes Manager (gpl-3.0.txt). If not, see <https://www.gnu.org/licenses/>.
