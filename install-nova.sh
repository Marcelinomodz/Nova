#!/bin/bash

echo "🚀 Instalando Nova Language by MARCELINO MODZ"

# Verificar sistema
if [ -d "/data/data/com.termux" ]; then
    echo "📱 Termux detectado"
    
    # Baixar e instalar
    echo "📦 Baixando pacote..."
    curl -L -o nova.deb https://github.com/Marcelinomodz/Nova/releases/latest/download/nova-0.2.0.deb
    
    if [ -f "nova.deb" ]; then
        echo "📦 Instalando..."
        dpkg -i nova.deb
        rm -f nova.deb
        
        echo ""
        echo "✅ Nova Language instalado com sucesso!"
        echo ""
        nova --version
    else
        echo "❌ Falha ao baixar pacote"
        echo "Instale manualmente: https://github.com/Marcelinomodz/Nova/releases"
    fi
else
    echo "🐧 Linux detectado"
    
    # Baixar e instalar
    echo "📦 Baixando pacote..."
    wget -O nova.deb https://github.com/Marcelinomodz/Nova/releases/latest/download/nova-0.2.0.deb
    
    if [ -f "nova.deb" ]; then
        echo "📦 Instalando..."
        sudo dpkg -i nova.deb
        rm -f nova.deb
        
        echo ""
        echo "✅ Nova Language instalado com sucesso!"
        echo ""
        nova --version
    else
        echo "❌ Falha ao baixar pacote"
        echo "Instale manualmente: https://github.com/Marcelinomodz/Nova/releases"
    fi
fi

echo ""
echo "🎉 Nova Language by MARCELINO MODZ"
