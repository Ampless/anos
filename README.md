# anOS
**Deprecated in favor of [AmplOS2/AmplOS2](https://github.com/AmplOS2/AmplOS2)**

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

## TODO / Roadmap
anOS already does some great things, but there's still a lot to do:

* Some File System (probably [FAT](https://github.com/Ampless/afat))
* `aport` on macOS and Linux
* Memory Management, obviously
* Processes, System Calls, IPC
* Port a C library (probably `dietlibc`) and
compiler (`clang`, `gcc`, `lcc`, ...)
* `aport` on anOS
* Port a shell (`ash`, `dash`, `ksh`, ...) and
editor (`nano`, `ed`, `that_editor`, ...)
* Port [`doas`](https://github.com/Ampless/doas), instead of reinventing the
wheel with `peb`
* Port `zsh` and `vim`
* A temporary unstable API for full-screen graphics from Userspace
* Port `genericdoom`
* Display Server, Window Manager, SDL port
* Port modern programming languages (Dart (+ Flutter), Go, Rust, ...) and
apps (`that_terminal`, Emacs, some browser (maybe surf), 90s games, ...)
* Networking
* Sound
* SMP
* x86/RISC-V/...
