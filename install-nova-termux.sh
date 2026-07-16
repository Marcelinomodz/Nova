#!/bin/bash
echo "📦 Instalando Nova Language no Termux..."
dpkg -i nova-termux-pkg.deb
if [ $? -eq 0 ]; then
    echo "✅ Nova Language instalado com sucesso!"
    echo ""
    nova --version
else
    echo "❌ Falha na instalação"
    echo "Tentando instalar dependências..."
    pkg install -y nodejs
    dpkg -i nova-termux-pkg.deb
fi
