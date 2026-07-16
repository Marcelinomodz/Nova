#!/bin/bash

echo "📦 Configurando Git e enviando para o GitHub..."

# Verificar se git está instalado
if ! command -v git &> /dev/null; then
    echo "📦 Instalando git..."
    pkg install git -y
fi

# Configurar git
echo "Digite seu nome de usuário do GitHub:"
read -r GITHUB_USER

echo "Digite seu email do GitHub:"
read -r GITHUB_EMAIL

git config --global user.name "$GITHUB_USER"
git config --global user.email "$GITHUB_EMAIL"

# Inicializar repositório
echo "📦 Inicializando repositório..."
git init

# Criar .gitignore
cat > .gitignore << 'EOL'
node_modules/
*.deb
*.tar.gz
dist/
*.log
*.tmp
.DS_Store
EOL

# Adicionar todos os arquivos
echo "📦 Adicionando arquivos..."
git add .
git commit -m "✨ Nova Language v0.2.0 - by MARCELINO MODZ"

# Criar repositório no GitHub
echo ""
echo "📦 Criando repositório no GitHub..."
echo "Acesse: https://github.com/new"
echo "Crie um repositório chamado: nova"
echo ""
echo "Depois de criar, execute:"
echo "  git remote add origin https://github.com/$GITHUB_USER/nova.git"
echo "  git branch -M main"
echo "  git push -u origin main"
echo ""
echo "Ou use este script para continuar:"
echo "  ./push-github.sh"
