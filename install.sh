#!/bin/bash

echo "🚀 Instalando Nova Language by MARCELINO MODZ"

# Verificar se é Termux
if [ -d "/data/data/com.termux" ]; then
    echo "📱 Termux detectado"
    
    # Instalar dependências
    pkg update -y
    pkg install -y g++ make
    
    # Compilar
    echo "🔨 Compilando..."
    g++ -std=c++11 -Wall -O2 -o nova src/cpp/nova.cpp
    
    if [ -f "nova" ]; then
        mv nova /data/data/com.termux/files/usr/bin/
        chmod +x /data/data/com.termux/files/usr/bin/nova
        echo "✅ Nova instalado em /data/data/com.termux/files/usr/bin/nova"
    else
        echo "❌ Erro na compilação"
        exit 1
    fi
else
    # Linux/outros
    echo "🐧 Linux detectado"
    sudo apt update 2>/dev/null || echo ""
    sudo apt install -y g++ make 2>/dev/null || echo ""
    
    g++ -std=c++11 -Wall -O2 -o nova src/cpp/nova.cpp
    
    if [ -f "nova" ]; then
        sudo mv nova /usr/local/bin/
        sudo chmod +x /usr/local/bin/nova
        echo "✅ Nova instalado em /usr/local/bin/nova"
    else
        echo "❌ Erro na compilação"
        exit 1
    fi
fi

echo ""
echo "✅ Nova Language instalado com sucesso!"
echo ""
nova --version
echo ""
echo "Teste rápido:"
echo 'say "Olá Mundo"' > test.nv
nova test.nv
rm -f test.nv
