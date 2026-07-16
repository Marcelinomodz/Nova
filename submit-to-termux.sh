#!/bin/bash

echo "📤 Submetendo Nova Language para o Termux..."
echo ""

cd termux-packages-temp

# Configurar git
git config user.name "Marcelinomodz"
git config user.email "marcelinodomingosartur2@gmail.com"

# Verificar se já existe a branch
git checkout -b add-nova-package 2>/dev/null || git checkout add-nova-package 2>/dev/null

# Adicionar arquivos
git add packages/nova

# Verificar se há mudanças
if git diff --cached --quiet; then
    echo "⚠️  Nenhuma mudança para commit"
else
    # Commit
    git commit -m "Add Nova Language package

Nova is a simple, modern and powerful programming language
created by MARCELINO MODZ.

Features:
- ✅ Independent binary (no dependencies)
- ✅ C++ native compilation
- ✅ Simple syntax
- ✅ Functions, loops, conditionals
- ✅ Termux support

Homepage: https://github.com/Marcelinomodz/Nova
License: MIT"
    
    echo "✅ Commit criado!"
fi

echo ""
echo "📌 Para submeter ao repositório oficial:"
echo ""
echo "1. Faça fork do repositório:"
echo "   https://github.com/termux/termux-packages"
echo ""
echo "2. Adicione seu fork como remote:"
echo "   git remote add myfork https://github.com/Marcelinomodz/termux-packages.git"
echo ""
echo "3. Envie para seu fork:"
echo "   git push -u myfork add-nova-package"
echo ""
echo "4. Crie o Pull Request:"
echo "   https://github.com/termux/termux-packages/compare/main...Marcelinomodz:termux-packages:add-nova-package"
echo ""
echo "Pressione ENTER quando terminar..."
read

echo ""
echo "✅ Submissão concluída!"
echo ""
echo "📌 Após aprovação, os usuários poderão instalar com:"
echo "   pkg install nova"
