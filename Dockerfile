FROM ubuntu:17.10

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y wget python3

WORKDIR /root/
RUN wget http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitARMupdate.pl \
    && chmod +x devkitARMupdate.pl \
    && ./devkitARMupdate.pl /opt/devkitPro \
    && rm /root/devkitARMupdate.pl

ENV DEVKITPRO /opt/devkitPro
ENV DEVKITARM $DEVKITPRO/devkitARM
ENV PATH $PATH:$DEVKITARM/bin

RUN mkdir /root/cheat
ADD include/ /root/cheat/include/
ADD lib/     /root/cheat/lib/
ADD source/  /root/cheat/source/
ADD 3ds.ld   /root/cheat/
ADD build.py /root/cheat/
RUN mkdir /root/cheat/pokeplg/
