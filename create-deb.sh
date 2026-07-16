#!/bin/bash

echo "📦 Criando pacote .deb para Termux..."

# Compilar primeiro
./build-standalone.sh

# Criar estrutura
PKG_NAME="nova"
PKG_VERSION="0.2.0"
PKG_DIR="nova-pkg"
DEB_DIR="$PKG_DIR/DEBIAN"
BIN_DIR="$PKG_DIR/data/data/com.termux/files/usr/bin"

mkdir -p $DEB_DIR
mkdir -p $BIN_DIR

# Copiar binário
cp nova $BIN_DIR/
chmod +x $BIN_DIR/nova

# Criar controle
cat > $DEB_DIR/control << 'EOL'
Package: nova
Version: 0.2.0
Architecture: all
Maintainer: MARCELINO MODZ
Depends: 
Description: Nova Language - A simple and modern programming language
 Homepage: https://github.com/marcelino/nova
 License: MIT
EOL

# Criar script de instalação
cat > $DEB_DIR/postinst << 'EOL'
#!/bin/bash
echo "✅ Nova Language by MARCELINO MODZ instalado!"
echo ""
echo "Versão: $(nova --version 2>/dev/null || echo 'v0.2.0')"
echo ""
echo "Exemplo:"
echo "  echo 'say \"Olá Mundo\"' > hello.nv"
echo "  nova hello.nv"
EOL

chmod +x $DEB_DIR/postinst

# Criar .deb
dpkg-deb --build $PKG_DIR

if [ $? -eq 0 ]; then
    echo "✅ Pacote criado: ${PKG_DIR}.deb"
    echo ""
    echo "Para instalar:"
    echo "  dpkg -i ${PKG_DIR}.deb"
    echo ""
    echo "Ou no Termux:"
    echo "  pkg install ./${PKG_DIR}.deb"
else
    echo "❌ Erro ao criar pacote"
    exit 1
fi
