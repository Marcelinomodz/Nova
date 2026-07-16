#!/bin/bash

echo "📦 Publicando Nova Language no NPM..."

# Verificar login
if ! npm whoami &> /dev/null; then
    echo "⚠️  Faça login no NPM primeiro:"
    echo "  npm login"
    exit 1
fi

# Atualizar versão
echo "📌 Versão atual: $(node -p "require('./package.json').version")"
echo "Deseja atualizar a versão? (s/N)"
read -r answer
if [[ "$answer" =~ ^[Ss]$ ]]; then
    echo "Tipo de atualização:"
    echo "  1) patch (0.2.0 -> 0.2.1)"
    echo "  2) minor (0.2.0 -> 0.3.0)"
    echo "  3) major (0.2.0 -> 1.0.0)"
    read -r version_type
    case $version_type in
        1) npm version patch ;;
        2) npm version minor ;;
        3) npm version major ;;
        *) echo "Opção inválida" && exit 1 ;;
    esac
fi

# Publicar
echo "📦 Publicando..."
npm publish

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Nova Language publicado com sucesso!"
    echo ""
    echo "Para instalar:"
    echo "  npm install -g nova-lang"
    echo ""
    echo "Ou no Termux:"
    echo "  pkg install nodejs"
    echo "  npm install -g nova-lang"
else
    echo "❌ Falha na publicação"
fi
