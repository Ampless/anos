FROM chrissx/gcc-x86_64-none-elf:11

RUN apt install -y nasm xorriso grub-pc-bin grub-common curl git gcc xz-utils
RUN curl -L https://github.com/chrissxYT/tools/raw/master/bootstrap | sh

VOLUME /anos
WORKDIR /anos
