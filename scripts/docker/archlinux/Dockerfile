FROM archlinux:latest

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm base base-devel git npm

RUN npm install --verbose --global --unsafe-perm=true esy@0.6.4
