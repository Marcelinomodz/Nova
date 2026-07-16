#!/bin/bash

echo "🔗 Criando link manual para Nova..."

# Criar link
ln -sf $(pwd)/bin/nova /data/data/com.termux/files/usr/bin/nova

if [ $? -eq 0 ]; then
    echo "✅ Link criado com sucesso!"
    echo ""
    echo "Testando:"
    nova --version
    nova examples/hello.nv
else
    echo "❌ Erro ao criar link"
    echo "Tente com sudo ou como root"
fi
