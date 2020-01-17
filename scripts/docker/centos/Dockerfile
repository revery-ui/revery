FROM centos:7

RUN yum -y update

RUN yum -y install centos-release-scl
RUN yum-config-manager --enable rhel-server-rhscl-7-rpms
RUN yum -y install llvm-toolset-7.0
RUN scl enable llvm-toolset-7.0 'clang -v'

RUN yum -y install gcc-c++ make sudo
RUN curl -sL https://rpm.nodesource.com/setup_10.x | sudo -E bash -
RUN yum -y install nodejs npm coreutils grep tar sed gawk diffutils autoconf

RUN yum -y install file fuse fuse-devel wget bzip2-devel libXt-devel libSM-devel libICE-devel ncurses-devel libacl-devel libxrandr-devel libXinerama-devel libXcursor-devel libXi-devel mesa-libGL-devel mesa-libGLU-devel gtk3-devel perl-Digest-SHA bzip2 m4 patch which cmake3 git nasm

RUN rpm -i https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/c/colm-0.13.0.4-2.el7.x86_64.rpm
RUN rpm -i https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/r/ragel-7.0.0.9-2.el7.x86_64.rpm

RUN node -v
RUN npm -v

RUN npm install --global --unsafe-perm=true esy@0.5.8

RUN yum -y install epel-release
RUN yum -y remove git
RUN rpm -U https://centos7.iuscommunity.org/ius-release.rpm
RUN yum -y install git2u

RUN yum -y install nasm
