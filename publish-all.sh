#!/bin/bash

echo "📦 Publicando Nova Language - MARCELINO MODZ"
echo "════════════════════════════════════════════"

# 1. Configurar Git
echo ""
echo "1️⃣ Configurando Git..."
./setup-git.sh

# 2. Enviar para GitHub
echo ""
echo "2️⃣ Enviando para GitHub..."
./push-github.sh

# 3. Criar pacote
echo ""
echo "3️⃣ Criando pacote..."
./create-package.sh

# 4. Criar release
echo ""
echo "4️⃣ Criando release no GitHub..."
./create-release.sh

echo ""
echo "════════════════════════════════════════════"
echo "✅ Publicação concluída!"
echo ""
echo "📌 Links importantes:"
echo "  Repositório: https://github.com/marcelino/nova"
echo "  Releases: https://github.com/marcelino/nova/releases"
echo ""
echo "📦 Pacotes criados:"
ls -lh *.deb 2>/dev/null || echo "  Nenhum pacote encontrado"
echo ""
echo "Para instalar:"
echo "  pkg install nova  # Em breve"
echo "  ou"
echo "  dpkg -i nova-0.2.0.deb"
echo ""
echo "🎉 Nova Language by MARCELINO MODZ publicada!"
