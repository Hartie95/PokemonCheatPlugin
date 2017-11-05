FROM ubuntu:17.10

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y wget python3

WORKDIR /root/
RUN wget http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitARMupdate.pl \
    && chmod +x devkitARMupdate.pl \
    && ./devkitARMupdate.pl /opt/devkitPro \
    && echo "export DEVKITPRO=/opt/devkitPro" >> .bashrc \
    && echo "export DEVKITARM=\$DEVKITPRO/devkitARM" >> .bashrc \
    && echo "export PATH=\$PATH:\$DEVKITARM/bin" >> .bashrc \
    && /bin/bash -c "source .bashrc" \
    && rm /root/devkitARMupdate.pl

RUN mkdir /root/cheat

ADD include/ /root/cheat/include/
ADD lib/     /root/cheat/lib/
ADD source/  /root/cheat/source/
ADD 3ds.ld   /root/cheat/
ADD build.py /root/cheat/

# For debugging
RUN apt-get install -y emacs

WORKDIR /root/cheat/
CMD while true; do sleep 1000; done
