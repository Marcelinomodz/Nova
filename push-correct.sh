#!/bin/bash

echo "📤 Enviando para o GitHub..."

echo ""
echo "🔑 Cole seu token do GitHub:"
echo "(Crie em: https://github.com/settings/tokens)"
echo "Token deve começar com: github_pat_"
read -r GITHUB_TOKEN

# Verificar se o token é válido
if [[ ! "$GITHUB_TOKEN" =~ ^github_pat_ ]]; then
    echo "⚠️  Token inválido! Deve começar com 'github_pat_'"
    echo "Crie um token classic em: https://github.com/settings/tokens"
    exit 1
fi

# Configurar remote com token
git remote set-url origin "https://$GITHUB_TOKEN@github.com/Marcelinomodz/Nova.git"

# Adicionar arquivos
echo "📦 Adicionando arquivos..."
git add .
git add -f nova 2>/dev/null || true
git add -f *.deb 2>/dev/null || true

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
✅ Binário independente

by MARCELINO MODZ ❤️" 2>/dev/null || echo "Nada para commit"

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
    echo ""
    echo "Tente manualmente:"
    echo "  git push https://$GITHUB_TOKEN@github.com/Marcelinomodz/Nova.git main --force"
fi
