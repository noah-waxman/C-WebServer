FROM codercom/code-server
USER root
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gdb \
    libcurl4-openssl-dev \
    pkg-config \
    vim
USER coder
