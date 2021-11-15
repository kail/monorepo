# TODO: Use atlantis: https://www.runatlantis.io/
terraform {
  required_providers {
    aws = {
      # This can be the full URL as well, like registry.terraform.io/hashicorp/aws
      source  = "hashicorp/aws"
      version = "~> 3.65"
    }
  }
  required_version = ">= 0.14.9"

  # TODO: get backend state working.
  # NOTE: vars can't be used here, so this must match variables.tf!
  backend "s3" {
    bucket = "skobovm-terraform-state"
    key    = "global/s3/terraform.tfstate"
    region = "us-west-2"

    dynamodb_table = "terraform-state-lock"
    encrypt        = true
  }
}

# TODO: Create a terraform IAM user
# https://registry.terraform.io/providers/hashicorp/aws/latest/docs#argument-reference
provider "aws" {
  profile = "skobovm"
  region  = "us-west-2"
}

#
# Infra modules
#
module "state" {
  source = "./state"
}

module "blog" {
  source = "./blog"
}

module "server" {
  source = "./server"
}
