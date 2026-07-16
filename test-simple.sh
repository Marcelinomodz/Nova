#!/bin/bash

echo "🧪 Testando Nova Language..."

# Criar arquivo de teste
cat > test.nv << 'EOL'
// Teste básico
say "=== Nova Language Test ==="

let nome = "Nova"
say "Bem vindo ao " + nome

let x = 10
let y = 20
let soma = x + y
say "10 + 20 = " + soma

if soma > 15 {
    say "Soma é maior que 15"
}

loop 3 {
    say "Loop: " + loop_index
}

func quadrado(n) {
    return n * n
}

say "Quadrado de 5: " + quadrado(5)

say "✅ Teste concluído!"
EOL

# Executar
nova test.nv

# Limpar
rm -f test.nv
