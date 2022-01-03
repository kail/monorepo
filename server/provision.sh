#!/bin/bash

# Exit when a command fails
set -e
# Echo commands before executing
set -x
# Don't allow use of unset variables
set -u

# This script is used for provisioning the monoserver. In reality,
# this should be done via packer or ami builder. This needs to be run
# under sudo, since some commands require root privileges.

USER=ubuntu
HOME=/home/$USER

install() {
  # Update everything installed
  sudo apt-get update
  sudo apt-get upgrade -y

  # Required for adding PPAs (personal package archives)
  sudo apt-get install -y software-properties-common

  # Install basic essentials
  sudo apt-get install -y \
      build-essential \
      vim \
      curl \
      jq \
      libssl-dev \
      git \
      mercurial \
      default-jdk \
      ca-certificates \
      gnupg \
      lsb-release \
      tmux \
      awscli \
      openssh-client

  # Many python versions
  sudo add-apt-repository -y ppa:deadsnakes/ppa

  # Add Docker packages
  rm -f /usr/share/keyrings/docker-archive-keyring.gpg
  curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
  echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

  # Update package lists
  sudo apt-get update

  # Install python
  sudo apt-get install -y \
      python3.10 \
      python3-pip \
      python3-dev \
      python3-virtualenv

  # Install docker
  # https://docs.docker.com/engine/install/ubuntu/
  sudo apt-get install -y \
      docker-ce=5:20.10.12~3-0~ubuntu-focal \
      docker-ce-cli=5:20.10.12~3-0~ubuntu-focal \
      containerd.io=1.4.12-1

  # Install docker compose
  sudo curl -L "https://github.com/docker/compose/releases/download/1.29.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
  sudo chmod +x /usr/local/bin/docker-compose

  # Install nginx
  sudo apt-get install -y nginx
}

configure_aws() {
  mkdir -p ${HOME}/.aws
  touch ${HOME}/.aws/config

  # Invoke as a bash command, because otherwise the heredoc
  # breaks syntax highlighting.
  bash -c 'cat <<EOF > ${HOME}/.aws/config
[default]
region = us-west-2
EOF'

  # Test that we can list secrets
  SECRETS_LIST=$(aws secretsmanager list-secrets 2> /dev/null | jq .SecretList)
  echo $SECRETS_LIST
  if [[ -z "$SECRETS_LIST" ]]; then
    echo AWS failed to list secrets
    exit 1
  fi
}

configure_repo() {
  # This should already exist, but do it just in case
  mkdir -p ${HOME}/.ssh

  # Get the key from secrets manager
  aws secretsmanager get-secret-value --secret-id monorepo/github/ssh | jq -r .SecretString > ${HOME}/.ssh/id_ed25519_github

  # Restrict to read-only
  chmod 400 ${HOME}/.ssh/id_ed25519_github

  # Start the agent and add the key
  eval "$(ssh-agent -s)"
  ssh-add ${HOME}/.ssh/id_ed25519_github

  # Clone the repo or get latest
  if [ -d ${HOME}/monorepo ]; then
    git -C ${HOME}/monorepo pull origin master
  else
    git clone git@github.com:kail/monorepo.git ${HOME}/monorepo
  fi
}

configure_nginx() {
  # Remove the default configuration (this is a symlink; file is in sites-available)
  sudo rm -f /etc/nginx/sites-enabled/default

  # Install the monorepo conf
  sudo rm -f /etc/nginx/sites-available/monorepo.conf
  cp ${HOME}/monorepo/server/conf/monorepo.conf /etc/nginx/sites-available

  # Create a symlink to the available site. This can point to the repo,
  # but having the extra step makes it harder to break.
  ln -sf /etc/nginx/sites-available/monorepo.conf /etc/nginx/sites-enabled/monorepo.conf

  # Restart nginx
  sudo systemctl restart nginx
}

install

configure_aws

configure_repo

configure_nginx
