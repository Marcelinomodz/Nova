const fs = require('fs');
const path = require('path');
const { Lexer } = require('../src/lexer');
const { Parser } = require('../src/parser');
const { Compiler } = require('../src/compiler');
const { VM } = require('../src/vm');

function testVM(code, expected) {
  console.log('\nTesting VM with:', code);
  
  try {
    const lexer = new Lexer(code);
    const tokens = lexer.tokenize();
    const parser = new Parser(tokens);
    const ast = parser.parse();
    const compiler = new Compiler();
    const program = compiler.compile(ast);
    const vm = new VM();
    vm.load(program);
    const result = vm.run();
    
    console.log('Result:', result);
    console.log('PASSED ✓');
  } catch (error) {
    console.log('FAILED ✗:', error.message);
  }
}

console.log('Nova VM Tests\n');

testVM('say "Hello"');
testVM('say 42');
testVM('say 10 + 20');

console.log('\nVM tests complete!');
