#!/bin/bash

echo "📦 Criando pacote Nova para distribuição..."

# Compilar primeiro
echo "🔨 Compilando..."
g++ -std=c++11 -Wall -O2 -static -o nova src/cpp/nova.cpp 2>/dev/null || g++ -std=c++11 -Wall -O2 -o nova src/cpp/nova.cpp

if [ ! -f "nova" ]; then
    echo "❌ Erro na compilação"
    exit 1
fi

# Criar estrutura do pacote
PKG_NAME="nova"
PKG_VERSION="0.2.0"
PKG_DIR="$PKG_NAME-$PKG_VERSION"
DEB_DIR="$PKG_DIR/DEBIAN"
BIN_DIR="$PKG_DIR/usr/local/bin"
SHARE_DIR="$PKG_DIR/usr/share/nova"

mkdir -p $DEB_DIR
mkdir -p $BIN_DIR
mkdir -p $SHARE_DIR

# Copiar binário
cp nova $BIN_DIR/
chmod +x $BIN_DIR/nova

# Copiar exemplos
cp -r examples $SHARE_DIR/

# Criar controle
cat > $DEB_DIR/control << 'EOL'
Package: nova
Version: 0.2.0
Architecture: all
Maintainer: MARCELINO MODZ <marcelino@modz.com>
Depends: 
Description: Nova Language - A simple and modern programming language
 Nova é uma linguagem de programação simples, moderna e poderosa
 criada por MARCELINO MODZ.
Homepage: https://github.com/marcelino/nova
License: MIT
EOL

# Criar script de pós-instalação
cat > $DEB_DIR/postinst << 'EOL'
#!/bin/bash
echo ""
echo "╔═══════════════════════════════════════════╗"
echo "║                                           ║"
echo "║         Nova Language v0.2.0              ║"
echo "║         by MARCELINO MODZ                 ║"
echo "╚═══════════════════════════════════════════╝"
echo ""
echo "✅ Nova Language instalado com sucesso!"
echo ""
echo "📝 Comandos:"
echo "  nova --help        # Ajuda"
echo "  nova --version     # Versão"
echo "  nova arquivo.nv    # Executar programa"
echo ""
echo "📚 Exemplos em: /usr/share/nova/examples/"
echo ""
echo "💡 Criar seu primeiro programa:"
echo "  echo 'say \"Olá Mundo\"' > hello.nv"
echo "  nova hello.nv"
EOL

chmod +x $DEB_DIR/postinst

# Criar script de pós-remoção
cat > $DEB_DIR/postrm << 'EOL'
#!/bin/bash
echo "🗑️  Nova Language removido!"
EOL

chmod +x $DEB_DIR/postrm

# Criar .deb
echo "📦 Criando pacote .deb..."
dpkg-deb --build $PKG_DIR 2>/dev/null

if [ $? -eq 0 ]; then
    echo "✅ Pacote criado: ${PKG_DIR}.deb"
    echo ""
    echo "📦 Tamanho: $(du -h ${PKG_DIR}.deb | cut -f1)"
    echo ""
    echo "Para instalar no Termux:"
    echo "  dpkg -i ${PKG_DIR}.deb"
    echo ""
    echo "Para instalar no Linux:"
    echo "  sudo dpkg -i ${PKG_DIR}.deb"
else
    echo "❌ Erro ao criar pacote"
    echo "Tentando sem compressão..."
    dpkg-deb -b $PKG_DIR
fi

# Limpar
rm -rf $PKG_DIR

echo ""
echo "✅ Pacote criado com sucesso!"
