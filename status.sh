#!/bin/bash

echo "=== Nova Language Status ==="
echo ""

echo "📦 Versão: $(nova --version 2>/dev/null || echo 'Não instalado')"
echo ""

echo "📁 Localização:"
which nova 2>/dev/null || echo "  Não encontrado"

echo ""
echo "🧪 Teste rápido:"
echo 'say "Nova está funcionando!"' | nova 2>/dev/null || echo "  ❌ Falha"

echo ""
echo "📂 Exemplos disponíveis:"
ls -1 examples/*.nv 2>/dev/null | head -5

echo ""
echo "✅ Status: $(nova --version 2>/dev/null && echo 'OK' || echo '❌ Não instalado')"
