from ubuntu:latest

run apt update
run apt upgrade -y
run apt install -y curl git gcc xz-utils
run curl -L https://github.com/chrissxYT/tools/raw/master/bootstrap | sh
run uname -a | grep x86 && curl -Lo /root/armgcc.tar.xz 'https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf.tar.xz?revision=79f65c42-1a1b-43f2-acb7-a795c8427085&la=en&hash=3BEE623628664E6E1736ABBE7CE5AD76B65B51EA' || curl -Lo /root/armgcc.tar.xz 'https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-aarch64-aarch64-none-elf.tar.xz?revision=9611f84a-41b8-4eae-9c30-045c830f5337&la=en&hash=109566BAD01B49B8BED6E60956DAB59ADFABAE62'
run cd /root ; tar xvfJ armgcc.tar.xz
run echo 'PATH="$PATH:/root/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin"' >> /root/.bashrc
run echo 'PATH="$PATH:/root/gcc-arm-10.2-2020.11-aarch64-aarch64-none-elf/bin"' >> /root/.bashrc

volume /root/anos
workdir /root/anos
