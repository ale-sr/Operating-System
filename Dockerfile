# Operating system
FROM ubuntu:16.04

#Set bash as shell
SHELL ["/bin/bash", "-c"]

# Install set up tools
RUN apt-get update && \
    DEBIAN_FRONTEND=noninterative \
        apt-get install -y --no-install-recommends \
	gdb qemu-system-x86 \
	vim build-essential \
	ctags cgdb \
	cscope clang

# Fix the qemu path
#RUN ln -s /bin/qemu-system-i386 /bin/qemu

# Clean up apt-get's files
RUN apt-get clean autoclean && \
    rm -rf /var/lib/apt/* /var/lib/cache/* /var/lib/log/*


# Create working directory
RUN mkdir -p /pintos/src
COPY ./src /pintos/src
WORKDIR /pintos/src

# Add Pintos to PATH
ENV PINTOS_HOME=/pintos
ENV PATH=/pintos/src/utils:$PATH
