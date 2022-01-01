# MonoServer
A large-ish instance containing whatever I'd like to expose to the internet.

## Get the server address
```
aws configure --profile skobovm --region us-west-2
aws ec2 describe-instances --filters Name=tag:Name,Values=MonorepoServer Name=instance-state-name,Values=running --query Reservations[*].Instances[*].PublicDnsName
```

## Connect to the server
```
ssh -i ~/.ssh/aws_skobovm ubuntu@<address>
```

## Server setup steps (for documentation/creating an image later)
https://certbot.eff.org/instructions?ws=nginx&os=ubuntu-20
```
# Install nginx
sudo apt install nginx

# Install letsencrypt cert
sudo snap install core; sudo snap refresh core

# Install certbot
sudo snap install --classic certbot
sudo ln -s /snap/bin/certbot /usr/bin/certbot

# One of:
# Get and install the cert
sudo certbot --nginx

# Just get the cert
sudo certbot certonly --nginx

# Certificate is saved at: /etc/letsencrypt/live/api.skobovm.com/fullchain.pem
# Key is saved at:         /etc/letsencrypt/live/api.skobovm.com/privkey.pem

# Serve http
sudo bash -c 'echo OK > /var/www/html/index.html'

# Follow instructionns for github access
# https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent

```

TODO: certbot autoupdates keys, but need to figure out how to do this automatically.
Probably a script on startup to check for keys, generate new ones, etc.

## Adding a new service
1. Create new directory with a dockerfile
1. Add service name to `nginx/default.conf`
1. Add service config to `docker-compose.yml`
1. Test locally `docker-compose up`


## Troubleshooting

### Github key not recognized
https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent

```
ssh-add /home/ubuntu/.ssh/id_ed25519.github
```

### Docker compose permission error
Add user to docker group

```
sudo gpasswd -a $USER docker
newgrp docker
```

## Machine provisioning (manual for now)

This should be done via packer or ami-builder! For now, however, it is manual.

### Install docker
https://docs.docker.com/engine/install/ubuntu/
```
sudo apt-get update

sudo apt-get install \
    ca-certificates \
    curl \
    gnupg \
    lsb-release

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

sudo apt-get update
```

### Install Node
https://github.com/nodesource/distributions/blob/master/README.md#debinstall

```
curl -fsSL https://deb.nodesource.com/setup_17.x | sudo -E bash -
sudo apt-get install -y nodejs
```

### Node guidance
```
## You may also need development tools to build native addons:
     sudo apt-get install gcc g++ make
## To install the Yarn package manager, run:
     curl -sL https://dl.yarnpkg.com/debian/pubkey.gpg | gpg --dearmor | sudo tee /usr/share/keyrings/yarnkey.gpg >/dev/null
     echo "deb [signed-by=/usr/share/keyrings/yarnkey.gpg] https://dl.yarnpkg.com/debian stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
     sudo apt-get update && sudo apt-get install yarn
```

### Docker compose
```
sudo curl -L "https://github.com/docker/compose/releases/download/1.29.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose

sudo chmod +x /usr/local/bin/docker-compose
```