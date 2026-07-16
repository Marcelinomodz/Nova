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

echo "📦 Instalando Nova Language..."

# Detectar sistema
if [ -d "/data/data/com.termux" ]; then
    echo "📱 Termux detectado"
    INSTALL_DIR="/data/data/com.termux/files/usr/bin"
    PKG_MANAGER="pkg"
elif command -v apt &> /dev/null; then
    echo "🐧 Debian/Ubuntu detectado"
    INSTALL_DIR="/usr/local/bin"
    PKG_MANAGER="apt"
elif command -v yum &> /dev/null; then
    echo "🐧 RHEL/CentOS detectado"
    INSTALL_DIR="/usr/local/bin"
    PKG_MANAGER="yum"
else
    echo "❌ Sistema não suportado"
    echo "Instale manualmente:"
    echo "  git clone https://github.com/Marcelinomodz/Nova"
    echo "  cd Nova"
    echo "  g++ -std=c++11 -Wall -O2 -o nova src/cpp/nova.cpp"
    echo "  sudo cp nova /usr/local/bin/"
    exit 1
fi

# Verificar se já está instalado
if command -v nova &> /dev/null; then
    echo "⚠️  Nova já está instalado!"
    echo "Versão: $(nova --version 2>/dev/null || echo 'desconhecida')"
    read -p "Deseja reinstalar? (s/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Ss]$ ]]; then
        echo "Instalação cancelada."
        exit 0
    fi
fi

# Instalar dependências
echo "📦 Instalando dependências..."
if [ "$PKG_MANAGER" = "pkg" ]; then
    pkg update -y
    pkg install -y git g++ make
elif [ "$PKG_MANAGER" = "apt" ]; then
    sudo apt update -y
    sudo apt install -y git g++ make
elif [ "$PKG_MANAGER" = "yum" ]; then
    sudo yum install -y git gcc-c++ make
fi

# Clonar ou atualizar
if [ -d "/tmp/nova-src" ]; then
    echo "📦 Atualizando código..."
    cd /tmp/nova-src
    git pull
else
    echo "📦 Clonando código..."
    git clone https://github.com/Marcelinomodz/Nova.git /tmp/nova-src
    cd /tmp/nova-src
fi

# Compilar
echo "🔨 Compilando Nova Language..."
if command -v g++ &> /dev/null; then
    g++ -std=c++11 -Wall -O2 -o nova src/cpp/nova.cpp
elif command -v clang++ &> /dev/null; then
    clang++ -std=c++11 -Wall -O2 -o nova src/cpp/nova.cpp
else
    echo "❌ Nenhum compilador C++ encontrado"
    exit 1
fi

# Instalar
echo "📦 Instalando..."
if [ -f "nova" ]; then
    if [ "$INSTALL_DIR" = "/data/data/com.termux/files/usr/bin" ]; then
        cp nova $INSTALL_DIR/
        chmod +x $INSTALL_DIR/nova
    else
        sudo cp nova $INSTALL_DIR/
        sudo chmod +x $INSTALL_DIR/nova
    fi
    echo "✅ Nova instalado em $INSTALL_DIR/nova"
else
    echo "❌ Erro na compilação"
    exit 1
fi

# Limpar
cd /
rm -rf /tmp/nova-src

echo ""
echo "✅ Nova Language instalado com sucesso!"
echo ""
echo "Versão: $(nova --version 2>/dev/null || echo 'v0.2.0')"
echo ""
echo "Teste rápido:"
echo 'say "Olá Mundo"' > /tmp/test.nv
nova /tmp/test.nv 2>/dev/null
rm -f /tmp/test.nv

echo ""
echo "📝 Comandos:"
echo "  nova --help        # Ajuda"
echo "  nova --version     # Versão"
echo "  nova arquivo.nv    # Executar programa"
echo ""
echo "💡 Exemplo:"
echo "  echo 'say \"Olá Mundo\"' > hello.nv"
echo "  nova hello.nv"
echo ""
echo "🎉 by MARCELINO MODZ"
