# Infrastructure

## Dependencies
```
# terraform >= 1.011
brew tap hashicorp/tap
brew install hashicorp/tap/terraform
brew update
brew upgrade hashicorp/tap/terraform
```

## Local Dev
```
# If it's a new machine, configure `skobovm` [AWS CLI profile](https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-quickstart.html)
aws configure --profile skobovm
```

## Running Terraform
```
# Download the plugins/providers
terraform init

# Lint and validate the config
terraform fmt
terraform validate

# Create the infrastructure
terraform apply

# Inspect the current state of the world
terraform show
```

## Tired of blowing away money?
```
terraform destroy
```

## Troubleshooting

Invalid Token Issues? Try running these:
```
export AWS_PROFILE="skobovm"
unset AWS_SESSION_TOKEN

# Validate that this works
aws sts get-caller-identity --region us-west-2
```
