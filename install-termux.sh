#!/bin/bash

echo "📦 Instalando Nova Language no Termux"

# Verificar Node.js
if ! command -v node &> /dev/null; then
    echo "📦 Instalando Node.js..."
    pkg install nodejs -y
fi

# Ir para o diretório do Nova
cd ~/Nova

# Remover instalação anterior
rm -f /data/data/com.termux/files/usr/bin/nova

# Instalar
echo "📦 Instalando Nova..."
npm install -g . 2>/dev/null || npm link 2>/dev/null

# Se falhar, criar link manual
if [ ! -f /data/data/com.termux/files/usr/bin/nova ]; then
    echo "📦 Criando link manual..."
    ln -sf ~/Nova/bin/nova /data/data/com.termux/files/usr/bin/nova
fi

# Verificar
echo ""
echo "✅ Nova Language instalado!"
echo ""
nova --version
echo ""
echo "Exemplo:"
echo "  echo 'say \"Olá Mundo\"' > hello.nv"
echo "  nova hello.nv"
