# Personal Monorepo

## Description
Reducing friction is the easiest way to make faster progress -- for me. I was tired of
boilerplate for working on new projects, so I set up a framework that allows me to
quickly get started on anything from embedded products to websites.

## Contents
 * [blog](blog/): A [Hugo](https://gohugo.io/) based static website hosted on S3
 * [hardware](hw/): Hardware projects containing FW and CAD files
 * [infra](infra/): Personal AWS infra setup using [Terraform](https://www.terraform.io/)
 * [server](server/): Microservices and routing configurations

## Getting Started
1. Clone `git clone git@github.com:kail/monorepo.git --recurse-submodules`

## TODO List
- [ ] Create base docker image
- [ ] Add firewall and other security
- [ ] Create machine image with packer/ami builder
- [ ] Set up observability tools such as Grafana
- [ ] Consider using (or play with) [MicroK8s](https://microk8s.io/)
