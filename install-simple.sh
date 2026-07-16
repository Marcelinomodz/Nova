#!/bin/bash

echo "🚀 Instalando Nova Language..."

# Verificar se já está instalado
if [ -f /data/data/com.termux/files/usr/bin/nova ]; then
    echo "⚠️  Nova já está instalado!"
    echo "Versão atual: $(nova --version 2>/dev/null || echo 'desconhecida')"
    echo ""
    echo "Para reinstalar:"
    echo "  rm -f /data/data/com.termux/files/usr/bin/nova"
    echo "  npm install -g ."
    exit 0
fi

# Instalar
echo "📦 Instalando via NPM..."
npm install -g .

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Nova Language instalado com sucesso!"
    echo ""
    echo "Versão: $(nova --version)"
    echo ""
    echo "Teste rápido:"
    echo 'say "Olá Mundo"' > test.nv
    nova test.nv
    rm -f test.nv
else
    echo "❌ Falha na instalação"
    echo ""
    echo "Tente instalar manualmente:"
    echo "  1. npm link"
    echo "  2. Ou use o binário: ./bin/nova"
fi
