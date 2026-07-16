#!/bin/bash

echo "📦 Criando release no GitHub..."

# Verificar se o pacote existe
if [ ! -f "nova-0.2.0.deb" ]; then
    echo "📦 Criando pacote..."
    ./create-deb-simple.sh
fi

# Verificar se o pacote foi criado
if [ ! -f "nova-0.2.0.deb" ]; then
    echo "❌ Pacote não encontrado"
    exit 1
fi

echo ""
echo "📌 Agora crie a release manualmente:"
echo ""
echo "1. Acesse: https://github.com/Marcelinomodz/Nova/releases/new"
echo ""
echo "2. Preencha:"
echo "   Tag: v0.2.0"
echo "   Title: Nova Language v0.2.0 - by MARCELINO MODZ"
echo ""
echo "3. Descrição:"
echo ""
cat << 'DESC'
🚀 Nova Language v0.2.0

Uma linguagem de programação simples, moderna e poderosa!

## ✨ Características
- ✅ Linguagem independente (sem Node.js)
- ✅ Binário nativo em C++
- ✅ Suporte a Termux e Linux
- ✅ Pacote .deb para instalação fácil
- ✅ Sintaxe simples e intuitiva
- ✅ Funções, loops, condicionais
- ✅ Exemplos inclusos

## 📦 Instalação

### Termux/Android
```bash
# Baixar o pacote
curl -L -o nova.deb https://github.com/Marcelinomodz/Nova/releases/latest/download/nova-0.2.0.deb

# Instalar
dpkg -i nova.deb

# Verificar
nova --version
