FROM archlinux:latest

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm base base-devel git npm perl
#perl for shasum binary

# Revery special deps
RUN pacman -S --noconfirm libpng libxcursor libxi libxinerama libxrandr harfbuzz glu gtk3 fontconfig nasm python2
ENV PATH=$PATH:/usr/bin/core_perl

# workaround hack while ocaml-secondary-compiler@4.08.1 work well with gcc 10
RUN mv /usr/bin/gcc /usr/bin/gcc-orig
RUN echo -en '#!/usr/bin/sh\n/usr/bin/gcc-orig -fcommon "$@"' > /usr/bin/gcc
RUN chmod u+x /usr/bin/gcc

RUN npm install --verbose --global --unsafe-perm=true esy@0.6.4
