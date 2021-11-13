output "blog_s3_bucket_arn" {
  value       = aws_s3_bucket.blog.arn
  description = "The ARN of the main blog S3 bucket"
}
