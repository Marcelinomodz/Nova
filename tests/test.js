const fs = require('fs');
const path = require('path');
const { Lexer } = require('../src/lexer');
const { Parser } = require('../src/parser');
const { Interpreter } = require('../src/interpreter');

function runTest(name, code, expected) {
  console.log(`\nTesting: ${name}`);
  console.log('Code:', code);
  
  try {
    const lexer = new Lexer(code);
    const tokens = lexer.tokenize();
    const parser = new Parser(tokens);
    const ast = parser.parse();
    const interpreter = new Interpreter();
    const result = interpreter.interpret(ast);
    
    console.log('Result:', result);
    console.log('Output:', interpreter.output);
    
    if (expected !== undefined) {
      const passed = result === expected || interpreter.output.join('\n') === expected;
      console.log(passed ? '✓ PASSED' : '✗ FAILED');
    } else {
      console.log('✓ PASSED');
    }
  } catch (error) {
    console.log('✗ FAILED:', error.message);
  }
}

// Run tests
console.log('Nova Language Tests\n');

runTest('Hello World', 'say "Hello"', 'Hello');
runTest('Variables', 'let x = 10\nsay x', '10');
runTest('Numbers', 'say 42', '42');
runTest('Operations', 'say 10 + 20', '30');
runTest('Condition', 'if 10 > 5 { say "yes" } else { say "no" }', 'yes');

console.log('\nTests complete!');
