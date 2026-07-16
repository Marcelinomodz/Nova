#!/bin/bash

echo "📦 Publicando Nova Language no NPM..."

# Verificar se está logado no npm
if ! npm whoami &> /dev/null; then
    echo "⚠️  Você precisa fazer login no npm primeiro:"
    echo "  npm login"
    exit 1
fi

# Atualizar versão
echo "📦 Atualizando versão..."
npm version patch

# Publicar
echo "📦 Publicando..."
npm publish

echo ""
echo "✅ Nova Language publicado com sucesso!"
echo ""
echo "Para instalar:"
echo "  npm install -g nova-lang"
echo ""
echo "Ou no Termux:"
echo "  pkg install nodejs"
echo "  npm install -g nova-lang"
