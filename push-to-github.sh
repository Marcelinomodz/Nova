#!/bin/bash

echo "📤 Enviando Nova Language para o GitHub..."

# Verificar se o remote existe
if ! git remote get-url origin &> /dev/null; then
    git remote add origin https://github.com/Marcelinomodz/Nova.git
fi

# Solicitar token
echo ""
echo "🔑 Digite seu token do GitHub:"
echo "(Crie em: https://github.com/settings/tokens)"
read -r GITHUB_TOKEN

# Configurar remote com token
git remote set-url origin "https://$GITHUB_TOKEN@github.com/Marcelinomodz/Nova.git"

# Adicionar todos os arquivos
echo "📦 Adicionando arquivos..."
git add .

# Commit
echo "📝 Commitando..."
git commit -m "✨ Nova Language v0.2.0 - by MARCELINO MODZ

🚀 Primeira versão da linguagem Nova

✅ Lexer completo
✅ Parser completo  
✅ Interpretador
✅ Compilador C++
✅ Suporte a Termux
✅ Pacote .deb

by MARCELINO MODZ ❤️"

# Push
echo "📤 Enviando..."
git branch -M main
git push -u origin main --force

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Código enviado com sucesso!"
    echo ""
    echo "📌 Repositório: https://github.com/Marcelinomodz/Nova"
    echo "📌 Releases: https://github.com/Marcelinomodz/Nova/releases"
else
    echo ""
    echo "❌ Erro ao enviar"
    echo "Tente manualmente:"
    echo "  git push -u origin main --force"
fi
