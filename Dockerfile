FROM amd64/ubuntu:24.10 as mapf-base

WORKDIR /tmp

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update -q \
    && apt install -yq --no-install-recommends \
    zsh \
    && rm -rf /var/lib/apt/lists/* \
    && apt clean

ENV SHELL /bin/zsh
SHELL ["/bin/zsh", "-c"]

RUN apt update -q \
    && apt install -yq --no-install-recommends \
    ca-certificates \
    curl \
    git \
    tar \
    wget \
    && rm -rf /var/lib/apt/lists/* \
    && apt clean

RUN sh -c "$(wget https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh -O -)" -x

WORKDIR /mapf
ENTRYPOINT ["/bin/zsh", "-lc"]
CMD ["trap : TERM INT; sleep infinity & wait"]