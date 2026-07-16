const { Runtime } = require('../runtime');
const { StandardLibrary } = require('../stdlib');

class Interpreter {
  constructor() {
    this.runtime = new Runtime();
    this.stdlib = new StandardLibrary();
    this.output = [];
    this.returnValue = null;
    this.functions = new Map();
    this.inFunctionCall = false;
    
    this.loadStandardLibrary();
  }

  loadStandardLibrary() {
    for (const name of this.stdlib.getFunctions()) {
      this.runtime.declareVariable(name, this.stdlib.getFunction(name));
    }
  }

  interpret(ast) {
    this.output = [];
    this.returnValue = null;
    this.functions = new Map();
    this.inFunctionCall = false;
    return this.evaluate(ast);
  }

  evaluate(node, scope = null) {
    if (!node) return null;
    
    if (!scope) {
      scope = this.runtime.currentScope;
    }
    
    switch (node.type) {
      case 'Program':
        return this.evaluateProgram(node, scope);
      case 'SayStatement':
        return this.evaluateSay(node, scope);
      case 'LetStatement':
        return this.evaluateLet(node, scope);
      case 'IfStatement':
        return this.evaluateIf(node, scope);
      case 'LoopStatement':
        return this.evaluateLoop(node, scope);
      case 'FunctionStatement':
        return this.evaluateFunction(node, scope);
      case 'ReturnStatement':
        return this.evaluateReturn(node, scope);
      case 'ImportStatement':
        return this.evaluateImport(node, scope);
      case 'TaskStatement':
        return this.evaluateTask(node, scope);
      case 'ExpressionStatement':
        return this.evaluate(node.expression, scope);
      case 'BinaryExpression':
        return this.evaluateBinary(node, scope);
      case 'NumberLiteral':
        return node.value;
      case 'StringLiteral':
        return node.value;
      case 'BooleanLiteral':
        return node.value;
      case 'Identifier':
        return this.evaluateIdentifier(node, scope);
      case 'FunctionCall':
        return this.evaluateFunctionCall(node, scope);
      default:
        throw new Error(`Unknown node type: ${node.type}`);
    }
  }

  evaluateProgram(node, scope) {
    let result = null;
    for (const statement of node.body) {
      result = this.evaluate(statement, scope);
    }
    return result;
  }

  evaluateSay(node, scope) {
    const value = this.evaluate(node.value, scope);
    const displayValue = this.getValue(value);
    console.log(displayValue);
    this.output.push(displayValue);
    return displayValue;
  }

  getValue(value) {
    if (value === null || value === undefined) return 'null';
    if (typeof value === 'string' && value.startsWith('var_')) {
      // Se for um ID de variável, tenta buscar o valor
      try {
        const actualValue = this.runtime.getVariableByValue(value);
        if (actualValue !== undefined) {
          return actualValue;
        }
      } catch (e) {
        return value;
      }
    }
    return value;
  }

  evaluateLet(node, scope) {
    const value = this.evaluate(node.value, scope);
    this.runtime.declareVariable(node.name, value);
    return value;
  }

  evaluateIf(node, scope) {
    const condition = this.evaluate(node.condition, scope);
    if (condition) {
      return this.evaluate({ type: 'Program', body: node.body }, scope);
    } else if (node.elseBody) {
      return this.evaluate({ type: 'Program', body: node.elseBody }, scope);
    }
    return null;
  }

  evaluateLoop(node, scope) {
    const count = this.evaluate(node.count, scope);
    let result = null;
    for (let i = 0; i < count; i++) {
      const loopScope = new Map(scope);
      loopScope.set('loop_index', i);
      result = this.evaluate({ type: 'Program', body: node.body }, loopScope);
    }
    return result;
  }

  evaluateFunction(node, scope) {
    this.functions.set(node.name, {
      name: node.name,
      params: node.params,
      body: node.body,
      scope: scope
    });
    
    const func = (...args) => {
      const funcScope = new Map();
      for (let i = 0; i < node.params.length; i++) {
        funcScope.set(node.params[i], args[i] || null);
      }
      
      this.runtime.scopeStack.push(funcScope);
      this.runtime.currentScope = funcScope;
      
      this.inFunctionCall = true;
      let result = null;
      for (const stmt of node.body) {
        result = this.evaluate(stmt, funcScope);
        if (this.returnValue !== null) {
          result = this.returnValue;
          this.returnValue = null;
          break;
        }
      }
      
      this.runtime.scopeStack.pop();
      this.runtime.currentScope = this.runtime.scopeStack[this.runtime.scopeStack.length - 1];
      this.inFunctionCall = false;
      
      return result;
    };
    
    this.runtime.declareVariable(node.name, func);
    return func;
  }

  evaluateReturn(node, scope) {
    this.returnValue = this.evaluate(node.value, scope);
    return this.returnValue;
  }

  evaluateImport(node, scope) {
    const path = this.evaluate(node.path, scope);
    try {
      const module = require(path);
      return module;
    } catch (error) {
      throw new Error(`Cannot import module: ${path}`);
    }
  }

  evaluateTask(node, scope) {
    console.log(`Task '${node.name}' defined`);
    return null;
  }

  evaluateBinary(node, scope) {
    const left = this.evaluate(node.left, scope);
    const right = this.evaluate(node.right, scope);
    
    // Se for string, converte para string legível
    const leftDisplay = typeof left === 'string' && left.startsWith('var_') ? this.runtime.getVariableByValue(left) : left;
    const rightDisplay = typeof right === 'string' && right.startsWith('var_') ? this.runtime.getVariableByValue(right) : right;
    
    const leftVal = leftDisplay !== undefined ? leftDisplay : left;
    const rightVal = rightDisplay !== undefined ? rightDisplay : right;
    
    switch (node.operator) {
      case 'PLUS':
        return leftVal + rightVal;
      case 'MINUS':
        return leftVal - rightVal;
      case 'MULTIPLY':
        return leftVal * rightVal;
      case 'DIVIDE':
        return leftVal / rightVal;
      case 'LT':
        return leftVal < rightVal;
      case 'GT':
        return leftVal > rightVal;
      case 'LTE':
        return leftVal <= rightVal;
      case 'GTE':
        return leftVal >= rightVal;
      case 'EQ':
        return leftVal === rightVal;
      case 'NEQ':
        return leftVal !== rightVal;
      case 'ASSIGN':
        if (node.left.type === 'Identifier') {
          this.runtime.setVariable(node.left.name, rightVal);
          return rightVal;
        }
        throw new Error('Invalid assignment target');
      default:
        throw new Error(`Unknown operator: ${node.operator}`);
    }
  }

  evaluateIdentifier(node, scope) {
    if (scope && scope.has && scope.has(node.name)) {
      const value = scope.get(node.name);
      return value;
    }
    return this.runtime.getVariable(node.name);
  }

  evaluateFunctionCall(node, scope) {
    let func = this.runtime.getVariable(node.name);
    
    if (typeof func === 'function') {
      const args = node.args.map(arg => this.evaluate(arg, scope));
      return func(...args);
    }
    
    const funcDef = this.functions.get(node.name);
    if (!funcDef) {
      throw new Error(`Undefined function: ${node.name}`);
    }
    
    const args = node.args.map(arg => this.evaluate(arg, scope));
    const funcScope = new Map();
    for (let i = 0; i < funcDef.params.length; i++) {
      funcScope.set(funcDef.params[i], args[i] || null);
    }
    
    this.runtime.scopeStack.push(funcScope);
    this.runtime.currentScope = funcScope;
    
    let result = null;
    this.returnValue = null;
    for (const stmt of funcDef.body) {
      result = this.evaluate(stmt, funcScope);
      if (this.returnValue !== null) {
        result = this.returnValue;
        this.returnValue = null;
        break;
      }
    }
    
    this.runtime.scopeStack.pop();
    this.runtime.currentScope = this.runtime.scopeStack[this.runtime.scopeStack.length - 1];
    
    return result;
  }

  getOutput() {
    return this.output;
  }

  getMemoryStats() {
    return this.runtime.getMemoryStats();
  }
}

module.exports = { Interpreter };
