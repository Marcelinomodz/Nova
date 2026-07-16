#!/bin/bash

echo "🔄 Reinstalando Nova Language..."

# Remover instalações anteriores
echo "Removendo instalações antigas..."
npm uninstall -g nova-lang 2>/dev/null
rm -f /data/data/com.termux/files/usr/bin/nova
rm -f /usr/local/bin/nova

# Limpar cache do npm
echo "Limpando cache..."
npm cache clean --force 2>/dev/null

# Instalar
echo "Instalando Nova..."
npm install -g .

# Verificar
echo ""
echo "✅ Nova Language reinstalado!"
echo ""
nova --version
nova examples/hello.nv
