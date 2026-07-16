#!/bin/bash

echo "📤 Enviando para o GitHub..."

# Verificar se já tem remote
if ! git remote get-url origin &> /dev/null; then
    echo "Digite seu usuário do GitHub:"
    read -r GITHUB_USER
    echo "Digite o nome do repositório:"
    read -r REPO_NAME
    
    git remote add origin "https://github.com/$GITHUB_USER/$REPO_NAME.git"
fi

# Enviar
git branch -M main
git push -u origin main

if [ $? -eq 0 ]; then
    echo "✅ Código enviado com sucesso!"
    echo ""
    echo "📌 Seu repositório:"
    git remote get-url origin
else
    echo "❌ Erro ao enviar"
    echo "Tente: git push -u origin main --force"
fi
