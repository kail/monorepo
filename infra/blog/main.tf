provider "aws" {
  profile = "skobovm"
  region  = "us-west-2"
}

# This provider is required for generating the SSL Cert
# which needs to be in us-east-1 to work.
provider "aws" {
  alias  = "us-east-1"
  region = "us-east-1"
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

#
# CDN (Cloudfront)
#

# Name for the origin
locals {
  s3_origin_id = "s3-web-${aws_s3_bucket.blog.id}"
  s3_www_origin_id = "s3-web-${aws_s3_bucket.www_blog.id}"
}

# SSL Cert
resource "aws_acm_certificate" "blog" {
  provider                  = aws.us-east-1
  domain_name               = var.blog_domain
  subject_alternative_names = ["www.${var.blog_domain}"]
  validation_method         = "DNS"
}

# CF distribution
module "cf" {
  source = "./cloudfront"
  s3_origin_id = local.s3_origin_id
  domain_name = aws_s3_bucket.blog.website_endpoint
  alias = var.blog_domain
  acm_certificate_arn = aws_acm_certificate.blog.arn
  logging_bucket = aws_s3_bucket.log_bucket.bucket_domain_name
  logging_prefix = "${var.blog_domain}/cf/"
}

module "www_cf" {
  source = "./cloudfront"
  s3_origin_id = local.s3_www_origin_id
  domain_name = aws_s3_bucket.www_blog.website_endpoint
  alias = "www.${var.blog_domain}"
  acm_certificate_arn = aws_acm_certificate.blog.arn
  logging_bucket = aws_s3_bucket.log_bucket.bucket_domain_name
  logging_prefix = "${var.blog_domain}/cf/"
}
