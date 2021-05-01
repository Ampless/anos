FROM chrissx/gcc-aarch64-none-elf:11

RUN apt install -y curl git gcc xz-utils
RUN curl -L https://github.com/chrissxYT/tools/raw/master/bootstrap | sh

VOLUME /anos
WORKDIR /anos
