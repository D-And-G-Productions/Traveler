FROM debian:latest


RUN apt-get update
RUN apt-get -y install \
  curl \
  gnupg \
  ca-certificates \
  sudo \
  postgresql


ARG DOCKER_USER=postgres
USER ${DOCKER_USER}
WORKDIR /home/${DOCKER_USER}

CMD ["bash"]

