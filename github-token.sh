#!/bin/bash

echo "🔑 Configurando token do GitHub..."

echo ""
echo "📌 Para criar um token de acesso:"
echo "1. Acesse: https://github.com/settings/tokens"
echo "2. Clique em 'Generate new token (classic)'"
echo "3. Nome: nova-language"
echo "4. Selecionar: repo, workflow, write:packages"
echo "5. Clique em 'Generate token'"
echo "6. Copie o token gerado"
echo ""
echo "Depois de criar o token, execute:"
echo "  git remote set-url origin https://SEU_TOKEN@github.com/Marcelinomodz/Nova.git"
echo ""
echo "Exemplo:"
echo "  git remote set-url origin https://ghp_xxxxxxxxxxxx@github.com/Marcelinomodz/Nova.git"
