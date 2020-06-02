FROM archlinux:latest

RUN pacman -Syu --noconfirm

RUN pacman -S --noconfirm base base-devel git npm

RUN mkdir "${HOME}/.npm-packages"
RUN npm config set prefix "${HOME}/.npm-packages"
RUN echo export PATH=\"\$PATH:\${HOME}/.npm-packages/bin\" >> .bashrc
RUN source $HOME/.bashrc
RUN npm install -g esy

RUN npm --version
RUN git --version
RUN esy --version
