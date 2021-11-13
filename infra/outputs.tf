# Monorepo server outputs
output "monoserver_id" {
  description = "ID of the monoserver instance"
  value       = aws_instance.monoserver.id
}

output "monoserver_dns" {
  description = "DNS name of the monoserver instance"
  value       = aws_instance.monoserver.public_dns
}

output "monoserver_public_ip" {
  description = "Public IP address of the monoserver instance"
  value       = aws_instance.monoserver.public_ip
}

# State module output
output "state_module_output" {
  value = module.state
}

# Blog module output
output "blog_module_output" {
  value = module.blog
}
