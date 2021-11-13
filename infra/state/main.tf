#
# Terraform state infra
#
provider "aws" {
  profile = "skobovm"
  region  = "us-west-2"
}

# S3 containing terraform state
resource "aws_s3_bucket" "terraform_state" {
  bucket = var.terraform_state_bucket

  # See the full revision history of our state files.
  versioning {
    enabled = true
  }

  # Enable server-side encryption by default.
  server_side_encryption_configuration {
    rule {
      apply_server_side_encryption_by_default {
        sse_algorithm = "AES256"
      }
    }
  }
}

# Dynamo instance for locking state access.
# Do I really need this? No. But it's useful to have
# for posterity, completeness, and anyone browsing the code.
resource "aws_dynamodb_table" "terraform_locks" {
  name         = var.terraform_lock_dynamo
  billing_mode = "PAY_PER_REQUEST"
  hash_key     = "LockID"

  attribute {
    name = "LockID"
    type = "S"
  }
}

