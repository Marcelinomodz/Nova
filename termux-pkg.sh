#!/bin/bash

echo "📦 Criando pacote para Termux..."

# Instalar dependências
pkg install -y nodejs git

# Criar diretório do pacote
mkdir -p $HOME/.termux/nova-package
cd $HOME/.termux/nova-package

# Criar script de instalação
cat > install << 'EOL'
#!/bin/bash
echo "Instalando Nova Language..."
npm install -g nova-lang
echo "✅ Nova instalado!"
EOL

chmod +x install

# Criar script de desinstalação
cat > remove << 'EOL'
#!/bin/bash
echo "Removendo Nova Language..."
npm uninstall -g nova-lang
rm -f /data/data/com.termux/files/usr/bin/nova
echo "✅ Nova removido!"
EOL

chmod +x remove

# Criar arquivo de metadados
cat > metadata << 'EOL'
Package: nova
Version: 0.2.0
Maintainer: Marcelino
Description: Nova Programming Language - A simple and modern programming language
Homepage: https://github.com/marcelino/nova
License: MIT
Architecture: all
Depends: nodejs
EOL

echo "✅ Pacote criado em ~/.termux/nova-package"
echo ""
echo "Para instalar:"
echo "  cd ~/.termux/nova-package"
echo "  ./install"
echo ""
echo "Ou para criar um pacote .deb:"
echo "  pkg install dpkg"
echo "  dpkg-deb -b ~/.termux/nova-package nova.deb"
