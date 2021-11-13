# NOTE: These must match the backend declaration!
variable "terraform_state_bucket" {
  description = "Terraform state bucket name"
  type        = string
  default     = "skobovm-terraform-state"
}

variable "terraform_lock_dynamo" {
  description = "Terraform DynamoDB table for locking state"
  type        = string
  default     = "terraform-state-lock"
}
