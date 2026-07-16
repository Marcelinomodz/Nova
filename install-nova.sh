#!/bin/bash

echo "╔═══════════════════════════════════════════╗"
echo "║                                           ║"
echo "║    ███╗   ██╗ ██████╗ ██╗   ██╗ █████╗   ║"
echo "║    ████╗  ██║██╔═══██╗██║   ██║██╔══██╗  ║"
echo "║    ██╔██╗ ██║██║   ██║██║   ██║███████║  ║"
echo "║    ██║╚██╗██║██║   ██║╚██╗ ██╔╝██╔══██║  ║"
echo "║    ██║ ╚████║╚██████╔╝ ╚████╔╝ ██║  ██║  ║"
echo "║    ╚═╝  ╚═══╝ ╚═════╝   ╚═══╝  ╚═╝  ╚═╝  ║"
echo "║                                           ║"
echo "║         Nova Language v0.2.0              ║"
echo "║         by MARCELINO MODZ                 ║"
echo "╚═══════════════════════════════════════════╝"
echo ""

# Verificar Termux
if [ ! -d "/data/data/com.termux" ]; then
    echo "❌ Este script é apenas para Termux"
    exit 1
fi

# Instalar dependências
echo "📦 Instalando dependências..."
pkg update -y
pkg install -y g++ make

# Baixar e compilar
echo "📦 Baixando código fonte..."
if [ ! -d "nova-src" ]; then
    git clone https://github.com/marcelino/nova.git nova-src
fi

cd nova-src

echo "📦 Compilando..."
g++ -std=c++11 -Wall -O2 -static -o nova src/cpp/nova.cpp

if [ $? -eq 0 ]; then
    echo "📦 Instalando..."
    mv nova /data/data/com.termux/files/usr/bin/
    chmod +x /data/data/com.termux/files/usr/bin/nova
    
    echo ""
    echo "✅ Nova Language instalado com sucesso!"
    echo ""
    echo "Versão: $(nova --version)"
    echo ""
    echo "Teste rápido:"
    echo 'say "Olá Termux!"' > test.nv
    nova test.nv
    rm -f test.nv
else
    echo "❌ Erro na compilação"
    exit 1
fi

cd ..
rm -rf nova-src

echo ""
echo "🎉 Nova Language by MARCELINO MODZ está pronto!"
echo ""
echo "Comandos:"
echo "  nova --help        # Ajuda"
echo "  nova --version     # Versão"
echo "  nova arquivo.nv    # Executar programa"
