#!/bin/bash

echo "📦 Criando release no GitHub..."

# Compilar e criar pacote
./create-package.sh

# Verificar se o pacote existe
if [ ! -f "nova-0.2.0.deb" ]; then
    echo "❌ Pacote não encontrado"
    exit 1
fi

# Instalar GitHub CLI
if ! command -v gh &> /dev/null; then
    echo "📦 Instalando GitHub CLI..."
    pkg install gh -y 2>/dev/null || sudo apt install gh -y 2>/dev/null
fi

# Fazer login
echo "🔑 Fazendo login no GitHub..."
gh auth login

# Criar release
echo "📦 Criando release v0.2.0..."
gh release create v0.2.0 \
    --title "Nova Language v0.2.0" \
    --notes "🚀 Nova Language by MARCELINO MODZ

## Novidades
- ✅ Linguagem independente (sem Node.js)
- ✅ Binário nativo em C++
- ✅ Instalação via pkg
- ✅ Suporte a Termux
- ✅ Exemplos inclusos

## Instalação
\`\`\`bash
pkg install nova
# ou
dpkg -i nova-0.2.0.deb
\`\`\`

## Comandos
\`\`\`bash
nova --help
nova --version
nova arquivo.nv
\`\`\`" \
    nova-0.2.0.deb

if [ $? -eq 0 ]; then
    echo "✅ Release criado com sucesso!"
    echo ""
    echo "📌 Link: https://github.com/marcelino/nova/releases"
else
    echo "❌ Erro ao criar release"
    echo "Crie manualmente em: https://github.com/marcelino/nova/releases/new"
fi
