const Bytecode = {
  PUSH: 1,
  POP: 2,
  ADD: 3,
  SUB: 4,
  MUL: 5,
  DIV: 6,
  CMP: 7,
  JMP: 8,
  JZ: 9,
  CALL: 10,
  RET: 11,
  SAY: 12,
  LOAD: 13,
  STORE: 14,
  HALT: 15,
  FUNC: 16,
  RETURN: 17
};

class Compiler {
  constructor() {
    this.bytecode = [];
    this.constants = [];
    this.symbols = new Map();
    this.labels = new Map();
    this.currentLabel = 0;
    this.functions = new Map();
  }

  compile(ast) {
    this.bytecode = [];
    this.constants = [];
    this.symbols = new Map();
    this.labels = new Map();
    this.currentLabel = 0;
    this.functions = new Map();
    
    this.compileNode(ast);
    this.bytecode.push(Bytecode.HALT);
    
    return {
      bytecode: this.bytecode,
      constants: this.constants,
      symbols: this.symbols
    };
  }

  compileNode(node) {
    switch (node.type) {
      case 'Program':
        return this.compileProgram(node);
      case 'SayStatement':
        return this.compileSay(node);
      case 'LetStatement':
        return this.compileLet(node);
      case 'IfStatement':
        return this.compileIf(node);
      case 'LoopStatement':
        return this.compileLoop(node);
      case 'FunctionStatement':
        return this.compileFunction(node);
      case 'ReturnStatement':
        return this.compileReturn(node);
      case 'ExpressionStatement':
        return this.compileExpressionStatement(node);
      case 'BinaryExpression':
        return this.compileBinary(node);
      case 'NumberLiteral':
        return this.compileNumber(node);
      case 'StringLiteral':
        return this.compileString(node);
      case 'Identifier':
        return this.compileIdentifier(node);
      case 'FunctionCall':
        return this.compileFunctionCall(node);
      default:
        throw new Error(`Unknown node type: ${node.type}`);
    }
  }

  compileProgram(node) {
    for (const statement of node.body) {
      this.compileNode(statement);
    }
  }

  compileExpressionStatement(node) {
    this.compileNode(node.expression);
    this.bytecode.push(Bytecode.POP);
  }

  compileSay(node) {
    this.compileNode(node.value);
    this.bytecode.push(Bytecode.SAY);
  }

  compileLet(node) {
    this.compileNode(node.value);
    this.bytecode.push(Bytecode.STORE);
    if (!this.symbols.has(node.name)) {
      this.symbols.set(node.name, this.symbols.size);
    }
  }

  compileIf(node) {
    this.compileNode(node.condition);
    const elseLabel = this.createLabel();
    const endLabel = this.createLabel();
    
    this.bytecode.push(Bytecode.JZ);
    this.bytecode.push(elseLabel);
    
    for (const statement of node.body) {
      this.compileNode(statement);
    }
    
    this.bytecode.push(Bytecode.JMP);
    this.bytecode.push(endLabel);
    this.addLabel(elseLabel);
    
    if (node.elseBody) {
      for (const statement of node.elseBody) {
        this.compileNode(statement);
      }
    }
    
    this.addLabel(endLabel);
  }

  compileLoop(node) {
    const startLabel = this.createLabel();
    const endLabel = this.createLabel();
    
    // Initialize loop counter
    const counterName = '__loop_counter';
    if (!this.symbols.has(counterName)) {
      this.symbols.set(counterName, this.symbols.size);
    }
    
    this.bytecode.push(Bytecode.PUSH);
    this.bytecode.push(this.addConstant(0));
    this.bytecode.push(Bytecode.STORE);
    this.bytecode.push(this.symbols.get(counterName));
    
    this.addLabel(startLabel);
    
    // Load counter
    this.bytecode.push(Bytecode.LOAD);
    this.bytecode.push(this.symbols.get(counterName));
    
    // Load count
    this.compileNode(node.count);
    this.bytecode.push(Bytecode.CMP);
    this.bytecode.push(Bytecode.JZ);
    this.bytecode.push(endLabel);
    
    // Set loop_index
    this.bytecode.push(Bytecode.LOAD);
    this.bytecode.push(this.symbols.get(counterName));
    if (!this.symbols.has('loop_index')) {
      this.symbols.set('loop_index', this.symbols.size);
    }
    this.bytecode.push(Bytecode.STORE);
    this.bytecode.push(this.symbols.get('loop_index'));
    
    // Execute body
    for (const statement of node.body) {
      this.compileNode(statement);
    }
    
    // Increment counter
    this.bytecode.push(Bytecode.LOAD);
    this.bytecode.push(this.symbols.get(counterName));
    this.bytecode.push(Bytecode.PUSH);
    this.bytecode.push(this.addConstant(1));
    this.bytecode.push(Bytecode.ADD);
    this.bytecode.push(Bytecode.STORE);
    this.bytecode.push(this.symbols.get(counterName));
    
    this.bytecode.push(Bytecode.JMP);
    this.bytecode.push(startLabel);
    this.addLabel(endLabel);
  }

  compileFunction(node) {
    // Store function metadata
    this.functions.set(node.name, {
      params: node.params,
      body: node.body
    });
    
    // Create function in constants
    const funcIndex = this.constants.length;
    this.constants.push({
      type: 'function',
      name: node.name,
      params: node.params,
      body: node.body
    });
    
    // Store function reference
    this.bytecode.push(Bytecode.PUSH);
    this.bytecode.push(funcIndex);
    this.bytecode.push(Bytecode.STORE);
    if (!this.symbols.has(node.name)) {
      this.symbols.set(node.name, this.symbols.size);
    }
  }

  compileReturn(node) {
    this.compileNode(node.value);
    this.bytecode.push(Bytecode.RETURN);
  }

  compileBinary(node) {
    this.compileNode(node.left);
    this.compileNode(node.right);
    
    const opMap = {
      'PLUS': Bytecode.ADD,
      'MINUS': Bytecode.SUB,
      'MULTIPLY': Bytecode.MUL,
      'DIVIDE': Bytecode.DIV,
      'EQ': Bytecode.CMP,
      'NEQ': Bytecode.CMP,
      'LT': Bytecode.CMP,
      'GT': Bytecode.CMP,
      'LTE': Bytecode.CMP,
      'GTE': Bytecode.CMP
    };
    
    this.bytecode.push(opMap[node.operator] || Bytecode.ADD);
  }

  compileNumber(node) {
    const index = this.addConstant(node.value);
    this.bytecode.push(Bytecode.PUSH);
    this.bytecode.push(index);
  }

  compileString(node) {
    const index = this.addConstant(node.value);
    this.bytecode.push(Bytecode.PUSH);
    this.bytecode.push(index);
  }

  compileIdentifier(node) {
    if (this.symbols.has(node.name)) {
      this.bytecode.push(Bytecode.LOAD);
      this.bytecode.push(this.symbols.get(node.name));
    } else {
      throw new Error(`Undefined variable: ${node.name}`);
    }
  }

  compileFunctionCall(node) {
    // Load function
    if (this.symbols.has(node.name)) {
      this.bytecode.push(Bytecode.LOAD);
      this.bytecode.push(this.symbols.get(node.name));
    } else {
      throw new Error(`Undefined function: ${node.name}`);
    }
    
    // Push arguments
    for (const arg of node.args) {
      this.compileNode(arg);
    }
    
    // Call function
    this.bytecode.push(Bytecode.CALL);
    this.bytecode.push(node.args.length);
  }

  addConstant(value) {
    const index = this.constants.indexOf(value);
    if (index === -1) {
      this.constants.push(value);
      return this.constants.length - 1;
    }
    return index;
  }

  createLabel() {
    return `label_${this.currentLabel++}`;
  }

  addLabel(label) {
    this.labels.set(label, this.bytecode.length);
  }

  getLabelPosition(label) {
    return this.labels.get(label) || 0;
  }
}

module.exports = { Compiler, Bytecode };
