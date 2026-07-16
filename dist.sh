#!/bin/bash

echo "📦 Criando pacote de distribuição..."

# Criar diretório de distribuição
mkdir -p dist

# Copiar arquivos
cp -r bin dist/
cp -r src dist/
cp -r examples dist/
cp package.json dist/
cp README.md dist/
cp LICENSE dist/

# Criar arquivo de instalação
cat > dist/install.sh << 'EOL'
#!/bin/bash
echo "Instalando Nova Language..."
npm install -g .
EOL

chmod +x dist/install.sh

# Compactar
cd dist
tar -czf nova-lang.tar.gz *
cd ..

echo "✅ Pacote criado em dist/nova-lang.tar.gz"
echo ""
echo "Para instalar:"
echo "  tar -xzf dist/nova-lang.tar.gz"
echo "  cd dist"
echo "  ./install.sh"
