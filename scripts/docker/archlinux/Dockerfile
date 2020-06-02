FROM archlinux:latest

ENV NPM_PATH=$HOME/.npm-packages
ENV PATH=$NPM_PATH:$PATH
RUN echo $NPM_PATH

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm base base-devel git npm

RUN mkdir $NPM_PATH
RUN npm config set prefix $NPM_PATH

RUN npm install -g esy

RUN npm --version
RUN git --version
RUN esy --version
