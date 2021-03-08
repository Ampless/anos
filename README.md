# anOS
ANother Operating System, currently only designed to run on RasPi 3 and 4
(when the 5 comes out we will support it fairly quickly). x86/… support is
"planned" (writing the boot code for raspis was already tricky enough).

## building
1. Make sure you have [`sm`](https://github.com/chrissxYT/tools) installed
2. Git clone / Download and extract a ZIP / ...
3. `cd /path/to/anos/buildenvs`
4. `sm raspi` (start the Docker container)
5. `sm` (build the image in the Docker container)
6. (optionally) Exit the docker container and test using qemu

## why not rust?
Rust is a good programming language, but redoxOS already exists.
