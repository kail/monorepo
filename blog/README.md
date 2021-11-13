# skobovm.com 
Publicly available dumping ground of opinions


This static site is generated with [Hugo](https://gohugo.io/) and the theme is based on a fork of [Cactus](https://themes.gohugo.io/hugo-theme-cactus/)

## Setup

```
# Install Hugo (mac)
brew install hugo

# Fetch submodules
git submodule update --init --recursive

# If it's a new machine, configure `skobovm` [AWS CLI profile](https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-quickstart.html)
aws configure --profile skobovm
```

## Build/Deploy
```
# Work locally
make debug

# Generate content and deploy to S3
make upload
```
