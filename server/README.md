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
