FROM chrissx/gcc-aarch64-none-elf:10.2

RUN apt install -y nasm xorriso grub-pc-bin grub-common curl git gcc xz-utils
RUN curl -L https://github.com/chrissxYT/tools/raw/master/bootstrap | sh

VOLUME /anos
WORKDIR /anos
