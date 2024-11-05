FROM amd64/ubuntu:24.10 AS mapf-base

WORKDIR /tmp

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update -q \
    && apt install -yq --no-install-recommends \
    zsh \
    && rm -rf /var/lib/apt/lists/* \
    && apt clean

ENV SHELL /bin/zsh
SHELL ["/bin/zsh", "-c"]

# INSTALL COMMON

RUN apt update -q \
    && apt install -yq --no-install-recommends \
    ca-certificates \
    curl \
    htop \
    git \
    gpg \
    neovim \
    software-properties-common \
    sudo \
    tar \
    tmux \
    wget \
    && rm -rf /var/lib/apt/lists/* \
    && apt clean

RUN sh -c "$(wget https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh -O -)" -x

# INSTALL LLVM

ENV FLAGS="-O3 -Wall -Wextra -fsanitize=address"
ENV CLANG_VERSION=19
ENV CXX="clang++-${CLANG_VERSION}"
ENV CC="clang-${CLANG_VERSION}"
ENV FLAGS="${FLAGS}"
ENV CFLAGS="${FLAGS} -std=c23"
ENV CXXFLAGS="${FLAGS} -fsized-deallocation -stdlib=libc++ -std=c++23"

RUN echo "Build info:" \
    && echo "  CC=${CC}" \
    && echo "  CXX=${CXX}" \
    && echo "  CFLAGS=${CFLAGS}" \
    && echo "  CXXFLAGS=${CXXFLAGS}"

RUN wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && sudo ./llvm.sh ${CLANG_VERSION} \
    && rm -rf /tmp/*

RUN apt-get update -q \
    && apt-get install -yq --no-install-recommends \
    clang-${CLANG_VERSION} \
    clang-format-${CLANG_VERSION} \
    clang-tidy-${CLANG_VERSION} \
    lldb-${CLANG_VERSION} \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

RUN printf "export CC='${CC}'\n" >> /root/.zshrc \
    && printf "export CXX='${CXX}'\n" >> /root/.zshrc \
    && printf "export CFLAGS='${CFLAGS}'\n" >> /root/.zshrc \
    && printf "export CXXFLAGS='${CXXFLAGS}'\n" >> /root/.zshrc \
    && ln -sf ${CC} /usr/bin/clang \
    && ln -sf ${CXX} /usr/bin/clang++ \
    && ln -sf /usr/bin/clang-format-${CLANG_VERSION} /usr/bin/clang-format

# INSTALL BAZEL

ARG TARGETARCH
RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.23.0/bazelisk-linux-${TARGETARCH} \
    && chmod +x bazelisk-linux-${TARGETARCH} \
    && mv bazelisk-linux-amd64 /usr/bin/bazel

WORKDIR /mapf
ENTRYPOINT ["/bin/zsh", "-lc"]
CMD ["trap : TERM INT; sleep infinity & wait"]