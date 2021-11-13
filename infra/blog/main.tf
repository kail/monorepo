provider "aws" {
  profile = "skobovm"
  region  = "us-west-2"
}

# Main blog bucket
resource "aws_s3_bucket" "blog" {
  bucket = var.blog_domain
  acl    = "public-read"

  website {
    index_document = "index.html"
    error_document = "index.html"
  }

  logging {
    target_bucket = aws_s3_bucket.log_bucket.id
    target_prefix = "${var.blog_domain}/"
  }
}

# Redirect bucket
resource "aws_s3_bucket" "www_blog" {
  bucket = "www.${var.blog_domain}"
  acl    = "public-read"

  website {
    redirect_all_requests_to = var.blog_domain
  }

  logging {
    target_bucket = aws_s3_bucket.log_bucket.id
    target_prefix = "${var.blog_domain}/"
  }
}

# Public bucket policy
data "template_file" "blog_bucket_policy" {
  template = file("blog/public_bucket_policy.json")
  vars = {
    bucket = aws_s3_bucket.blog.id
  }
}
resource "aws_s3_bucket_policy" "blog_bucket" {
  bucket = aws_s3_bucket.blog.id
  policy = data.template_file.blog_bucket_policy.rendered
}

# Access logs bucket
resource "aws_s3_bucket" "log_bucket" {
  # Keeping the name generic in case it's used for other logging.
  bucket = "${var.blog_domain}-logs"
  acl    = "log-delivery-write"

  # Don't need to store logs indefinitely.
  lifecycle_rule {
    id      = "log"
    enabled = true
    tags = {
      rule      = "log"
      autoclean = "true"
    }
    transition {
      days          = 30
      storage_class = "STANDARD_IA"
    }
    expiration {
      days = 90
    }
  }
}

# Block public access to logs
resource "aws_s3_bucket_public_access_block" "log_bucket" {
  bucket                  = aws_s3_bucket.log_bucket.id
  block_public_acls       = true
  block_public_policy     = true
  ignore_public_acls      = true
  restrict_public_buckets = true
}
