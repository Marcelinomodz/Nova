#!/bin/bash

echo "📦 Criando pacote Nova para Termux (pkg)..."

# Instalar dependências
pkg update -y
pkg install -y nodejs git dpkg

# Criar estrutura do pacote
PKG_NAME="nova"
PKG_VERSION="0.2.0"
PKG_MAINTAINER="MARCELINO MODZ"
PKG_DIR="nova-termux-pkg"
DEB_DIR="$PKG_DIR/DEBIAN"
BIN_DIR="$PKG_DIR/data/data/com.termux/files/usr/bin"
LIB_DIR="$PKG_DIR/data/data/com.termux/files/usr/lib/node_modules/nova-lang"

# Criar diretórios
mkdir -p $DEB_DIR
mkdir -p $BIN_DIR
mkdir -p $LIB_DIR

# Criar arquivo de controle
cat > $DEB_DIR/control << EOL
Package: nova
Version: $PKG_VERSION
Architecture: all
Maintainer: $PKG_MAINTAINER
Depends: nodejs (>= 12.0.0)
Description: Nova Language - A simple and modern programming language
 Homepage: https://github.com/marcelino/nova
 License: MIT
EOL

# Copiar arquivos do Nova
echo "📦 Copiando arquivos..."
cp -r bin/* $BIN_DIR/
cp -r src $LIB_DIR/
cp -r examples $LIB_DIR/
cp package.json $LIB_DIR/
cp README.md $LIB_DIR/
cp LICENSE $LIB_DIR/
cp BRANDING.md $LIB_DIR/

# Criar script de instalação
cat > $DEB_DIR/postinst << 'EOL'
#!/bin/bash
echo "🚀 Instalando Nova Language by MARCELINO MODZ..."
cd /data/data/com.termux/files/usr/lib/node_modules/nova-lang
npm install --production 2>/dev/null || true
chmod +x /data/data/com.termux/files/usr/bin/nova
echo "✅ Nova Language instalado com sucesso!"
echo ""
echo "Versão: $(nova --version 2>/dev/null || echo 'v0.2.0')"
echo ""
echo "Exemplo:"
echo "  echo 'say \"Olá Mundo\"' > hello.nv"
echo "  nova hello.nv"
EOL

chmod +x $DEB_DIR/postinst

# Criar script de remoção
cat > $DEB_DIR/postrm << 'EOL'
#!/bin/bash
echo "🗑️  Removendo Nova Language..."
rm -rf /data/data/com.termux/files/usr/lib/node_modules/nova-lang
rm -f /data/data/com.termux/files/usr/bin/nova
echo "✅ Nova Language removido!"
EOL

chmod +x $DEB_DIR/postrm

# Criar script de instalação local
cat > install-nova-termux.sh << 'EOL'
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
EOL

chmod +x install-nova-termux.sh

# Criar o pacote .deb
echo "📦 Criando pacote .deb..."
dpkg-deb --build $PKG_DIR

if [ $? -eq 0 ]; then
    echo "✅ Pacote criado: ${PKG_DIR}.deb"
    echo ""
    echo "📦 Tamanho: $(du -h ${PKG_DIR}.deb | cut -f1)"
    echo ""
    echo "Para instalar no Termux:"
    echo "  ./install-nova-termux.sh"
    echo ""
    echo "Ou:"
    echo "  dpkg -i ${PKG_DIR}.deb"
    echo ""
    echo "Para desinstalar:"
    echo "  dpkg -r nova"
else
    echo "❌ Erro ao criar pacote"
    exit 1
fi
