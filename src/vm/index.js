const { Bytecode } = require('../compiler');

class VM {
  constructor() {
    this.stack = [];
    this.callStack = [];
    this.pc = 0;
    this.running = false;
    this.bytecode = [];
    this.constants = [];
    this.symbols = new Map();
    this.variables = [];
    this.functions = new Map();
  }

  load(program) {
    this.bytecode = program.bytecode;
    this.constants = program.constants;
    this.symbols = program.symbols;
    this.variables = new Array(this.symbols.size).fill(null);
    this.stack = [];
    this.callStack = [];
    this.pc = 0;
    this.functions = new Map();
    
    // Load functions from constants
    for (let i = 0; i < this.constants.length; i++) {
      const constVal = this.constants[i];
      if (constVal && typeof constVal === 'object' && constVal.type === 'function') {
        this.functions.set(constVal.name, {
          params: constVal.params,
          body: constVal.body,
          index: i
        });
      }
    }
  }

  run() {
    this.running = true;
    let result = null;
    
    while (this.running && this.pc < this.bytecode.length) {
      const opcode = this.bytecode[this.pc++];
      result = this.execute(opcode);
    }
    
    return result;
  }

  execute(opcode) {
    switch (opcode) {
      case Bytecode.PUSH:
        const index = this.bytecode[this.pc++];
        this.stack.push(this.constants[index]);
        break;
      case Bytecode.POP:
        this.stack.pop();
        break;
      case Bytecode.ADD:
        const b = this.stack.pop();
        const a = this.stack.pop();
        this.stack.push(a + b);
        break;
      case Bytecode.SUB:
        const d = this.stack.pop();
        const c = this.stack.pop();
        this.stack.push(c - d);
        break;
      case Bytecode.MUL:
        const f = this.stack.pop();
        const e = this.stack.pop();
        this.stack.push(e * f);
        break;
      case Bytecode.DIV:
        const h = this.stack.pop();
        const g = this.stack.pop();
        this.stack.push(g / h);
        break;
      case Bytecode.CMP:
        const j = this.stack.pop();
        const i = this.stack.pop();
        this.stack.push(i === j ? 1 : 0);
        break;
      case Bytecode.JMP:
        const label = this.bytecode[this.pc];
        this.pc = label;
        break;
      case Bytecode.JZ:
        const value = this.stack.pop();
        const target = this.bytecode[this.pc];
        if (value === 0 || value === false || value === null) {
          this.pc = target;
        } else {
          this.pc++;
        }
        break;
      case Bytecode.SAY:
        const output = this.stack.pop();
        console.log(output);
        break;
      case Bytecode.LOAD:
        const varIndex = this.bytecode[this.pc++];
        this.stack.push(this.variables[varIndex]);
        break;
      case Bytecode.STORE:
        const storeIndex = this.bytecode[this.pc++];
        const storeValue = this.stack.pop();
        this.variables[storeIndex] = storeValue;
        break;
      case Bytecode.CALL:
        const argCount = this.bytecode[this.pc++];
        const func = this.stack.pop();
        
        if (typeof func === 'function') {
          // Built-in function
          const args = [];
          for (let i = 0; i < argCount; i++) {
            args.unshift(this.stack.pop());
          }
          const result = func(...args);
          this.stack.push(result);
        } else if (func && typeof func === 'object' && func.call) {
          // User function
          const args = [];
          for (let i = 0; i < argCount; i++) {
            args.unshift(this.stack.pop());
          }
          const result = func.call(args);
          this.stack.push(result);
        } else if (func && typeof func === 'object' && func.params) {
          // Function from constants
          const args = [];
          for (let i = 0; i < argCount; i++) {
            args.unshift(this.stack.pop());
          }
          
          // Simple function execution using interpreter-like approach
          const oldVars = this.variables.slice();
          
          // Set up parameters
          for (let i = 0; i < func.params.length; i++) {
            const paramName = func.params[i];
            if (this.symbols.has(paramName)) {
              this.variables[this.symbols.get(paramName)] = args[i] || null;
            }
          }
          
          // Save current position
          const savedPc = this.pc;
          const savedStack = this.stack.slice();
          
          // Execute function body (simplified)
          let result = null;
          let returned = false;
          
          for (const statement of func.body) {
            // We need to compile and execute the body
            // For now, return null
            if (statement.type === 'ReturnStatement') {
              result = statement.value;
              returned = true;
              break;
            }
          }
          
          // Restore state
          this.variables = oldVars;
          this.pc = savedPc;
          this.stack = savedStack;
          
          this.stack.push(result || 0);
        } else {
          this.stack.push(0);
        }
        break;
      case Bytecode.RETURN:
        const returnValue = this.stack.pop();
        if (this.callStack.length > 0) {
          const state = this.callStack.pop();
          this.pc = state.pc;
          this.variables = state.variables;
        }
        this.stack.push(returnValue || 0);
        break;
      case Bytecode.HALT:
        this.running = false;
        break;
      default:
        throw new Error(`Unknown opcode: ${opcode}`);
    }
  }
}

module.exports = { VM };
