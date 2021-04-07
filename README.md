# anOS
ANother Operating System, currently only designed to run on RasPi 3 and 4
(when the 5 comes out we will support it fairly quickly). x86/… support is
"planned" (writing the boot code for raspis was already tricky enough).

## Building
1. Make sure you have [`sm`](https://github.com/chrissxYT/tools) and
[Docker](https://www.docker.com) installed
2. Git clone / Download and extract a ZIP / ...
3. `cd /path/to/anos/buildenvs`
4. `sm raspi` (start the Docker container)
5. `sm` (build the image in the Docker container)
6. (optionally) Exit the docker container and test using qemu

## Why not Rust?
Rust is a good programming language, but redoxOS already exists.

## TODO
anOS already does some great things, but there's still a lot to do:

### Kernel
* Memory Management, obviously
* Some File System (maybe custom, IDK)
* Processes, System Calls, IPC

### Userspace
* `aport`
* Ports for `zsh`, `vim`, maybe some compiler, ...
* Display Server, Window Manager, SDL port
* Ports for that\_terminal, Emacs, some browser (maybe surf), 90s games, ...
* Networking
* Sound
