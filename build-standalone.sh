#!/bin/bash

echo "🔨 Compilando Nova Language Standalone..."

# Verificar g++
if ! command -v g++ &> /dev/null; then
    echo "📦 Instalando g++..."
    pkg install g++ -y
fi

# Compilar
echo "📦 Compilando..."
g++ -std=c++11 -Wall -O2 -static -o nova src/cpp/nova.cpp

if [ $? -eq 0 ]; then
    echo "✅ Compilação bem sucedida!"
    echo ""
    echo "📦 Instalando..."
    
    # Instalar no sistema
    if [ -d "/data/data/com.termux" ]; then
        mv nova /data/data/com.termux/files/usr/bin/
        chmod +x /data/data/com.termux/files/usr/bin/nova
        echo "✅ Instalado em /data/data/com.termux/files/usr/bin/nova"
    else
        sudo mv nova /usr/local/bin/
        sudo chmod +x /usr/local/bin/nova
        echo "✅ Instalado em /usr/local/bin/nova"
    fi
    
    echo ""
    echo "🧪 Testando..."
    nova --version
    echo ""
    echo "📝 Teste rápido:"
    echo 'say "Olá Mundo"' > test.nv
    nova test.nv
    rm -f test.nv
else
    echo "❌ Erro na compilação"
    exit 1
fi
