# Github SSH key for cloning private repos
resource "aws_secretsmanager_secret" "github-ssh-key" {
  name = "monorepo/github/ssh"
}
