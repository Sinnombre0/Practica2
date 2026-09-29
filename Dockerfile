FROM fedora:44
RUN dnf install -y gcc make iproute tcpdump && dnf clean all
WORKDIR /app
COPY . /app
RUN make