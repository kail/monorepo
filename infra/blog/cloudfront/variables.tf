variable "s3_origin_id" {
  description = "S3 origin ID"
  type        = string
}

variable "domain_name" {
  description = "S3 domain name"
  type        = string
}

variable "alias" {
  description = "Alias"
  type        = string
}

variable "acm_certificate_arn" {
  description = "SSL cert ARN"
  type        = string
}

variable "logging_bucket" {
  description = "Logging bucket"
  type        = string
}

variable "logging_prefix" {
  description = "Log key prefix"
  type        = string
}
