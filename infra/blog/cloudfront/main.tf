# CF distribution
resource "aws_cloudfront_distribution" "blog" {
  origin {
    origin_id   = var.s3_origin_id
    domain_name = var.domain_name

    # Custom origin with S3 website as source
    # This ensures subdirectories redirect to their associated index.html
    custom_origin_config {
      http_port              = "80"
      https_port             = "443"
      origin_protocol_policy = "http-only"
      origin_ssl_protocols   = ["TLSv1.2"]
    }
  }

  enabled         = true
  is_ipv6_enabled = true

  # US, CA, MX, EU, IL
  price_class     = "PriceClass_100"
  aliases = [
    var.alias
  ]
  default_root_object = "index.html"

  default_cache_behavior {
    allowed_methods  = ["GET", "HEAD"]
    cached_methods   = ["GET", "HEAD"]
    target_origin_id = var.s3_origin_id

    viewer_protocol_policy = "redirect-to-https"
    compress               = true

    forwarded_values {
      query_string = false

      cookies {
        forward = "none"
      }
    }
  }

  restrictions {
    geo_restriction {
      restriction_type = "none"
    }
  }

  viewer_certificate {
    acm_certificate_arn = var.acm_certificate_arn
    ssl_support_method  = "sni-only"
  }

  logging_config {
    include_cookies = false
    bucket          = var.logging_bucket
    prefix          = var.logging_prefix
  }
}