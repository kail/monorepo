#
# Main server
#
resource "aws_instance" "monoserver" {
  # Vanilla Ubuntu 20.04 AMI
  ami           = "ami-036d46416a34a611c"
  
  # Custom shitty AMI (ubuntu 20.04)
  # TODO: make new AMI
  # ami           = "ami-034608683a8bb1a93"
  instance_type = "m6i.large"

  tags = {
    Name = var.monoserver_instance_name
  }

  # Default SSH key
  key_name = "aws-skobovm"

  # Security group
  vpc_security_group_ids = [
    aws_security_group.monoserver-sg.id
  ]

  # IAM Role
  iam_instance_profile = "${aws_iam_instance_profile.monoserver_profile.name}"

  # TODO: configure ebs_block_device?
}

# SSH key (aws_skobovm)
resource "aws_key_pair" "ssh-key" {
  key_name   = "aws-skobovm"
  public_key = file("static/ssh.pub")
}

# Security group
resource "aws_security_group" "monoserver-sg" {
  name        = "monoserver-security-group"
  description = "Allow HTTP, HTTPS and SSH traffic"

  ingress {
    description = "SSH"
    from_port   = 22
    to_port     = 22
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    description = "HTTPS"
    from_port   = 443
    to_port     = 443
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    description = "HTTP"
    from_port   = 80
    to_port     = 80
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }

  tags = {
    Name = "monoserver"
  }
}

# Elastic IP
resource "aws_eip" "monoserver" {
  instance = aws_instance.monoserver.id
  vpc      = true
}

# Route53 zone already existed
data "aws_route53_zone" "main" {
  name = "skobovm.com"
}
resource "aws_route53_record" "api" {
  zone_id = data.aws_route53_zone.main.zone_id
  name    = "api.${data.aws_route53_zone.main.name}"
  type    = "A"
  ttl     = "300"
  records = [aws_eip.monoserver.public_ip]
}

# IAM Role
resource "aws_iam_instance_profile" "monoserver_profile" {
  name = "monoserver_profile"
  role = "MonoserverEC2"
}
