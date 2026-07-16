class Parser {
  constructor(tokens) {
    this.tokens = tokens;
    this.position = 0;
  }

  peek() {
    return this.tokens[this.position] || { type: 'EOF', value: 'EOF' };
  }

  advance() {
    return this.tokens[this.position++];
  }

  expect(type) {
    const token = this.peek();
    if (token.type === type) {
      return this.advance();
    }
    throw new Error(`Expected ${type} but got ${token.type} at line ${token.line}, column ${token.column}`);
  }

  parse() {
    const statements = [];
    while (this.peek().type !== 'EOF') {
      statements.push(this.parseStatement());
    }
    return { type: 'Program', body: statements };
  }

  parseStatement() {
    const token = this.peek();
    
    switch (token.type) {
      case 'SAY':
        return this.parseSay();
      case 'LET':
        return this.parseLet();
      case 'IF':
        return this.parseIf();
      case 'LOOP':
        return this.parseLoop();
      case 'FUNC':
        return this.parseFunction();
      case 'RETURN':
        return this.parseReturn();
      case 'IMPORT':
        return this.parseImport();
      case 'TASK':
        return this.parseTask();
      default:
        return this.parseExpressionStatement();
    }
  }

  parseSay() {
    this.expect('SAY');
    const value = this.parseExpression();
    return { type: 'SayStatement', value };
  }

  parseLet() {
    this.expect('LET');
    const name = this.expect('IDENTIFIER').value;
    this.expect('ASSIGN');
    const value = this.parseExpression();
    return { type: 'LetStatement', name, value };
  }

  parseIf() {
    this.expect('IF');
    const condition = this.parseExpression();
    this.expect('LBRACE');
    const body = this.parseBlock();
    this.expect('RBRACE');
    
    let elseBody = null;
    if (this.peek().type === 'ELSE') {
      this.expect('ELSE');
      this.expect('LBRACE');
      elseBody = this.parseBlock();
      this.expect('RBRACE');
    }
    
    return { type: 'IfStatement', condition, body, elseBody };
  }

  parseLoop() {
    this.expect('LOOP');
    const count = this.parseExpression();
    this.expect('LBRACE');
    const body = this.parseBlock();
    this.expect('RBRACE');
    return { type: 'LoopStatement', count, body };
  }

  parseFunction() {
    this.expect('FUNC');
    const name = this.expect('IDENTIFIER').value;
    this.expect('LPAREN');
    const params = [];
    
    // Check if there are parameters
    if (this.peek().type !== 'RPAREN') {
      do {
        const param = this.expect('IDENTIFIER');
        params.push(param.value);
      } while (this.peek().type === 'COMMA' && this.advance());
    }
    
    this.expect('RPAREN');
    this.expect('LBRACE');
    const body = this.parseBlock();
    this.expect('RBRACE');
    return { type: 'FunctionStatement', name, params, body };
  }

  parseReturn() {
    this.expect('RETURN');
    const value = this.parseExpression();
    return { type: 'ReturnStatement', value };
  }

  parseImport() {
    this.expect('IMPORT');
    const path = this.parseExpression();
    return { type: 'ImportStatement', path };
  }

  parseTask() {
    this.expect('TASK');
    const name = this.expect('IDENTIFIER').value;
    this.expect('LBRACE');
    const body = this.parseBlock();
    this.expect('RBRACE');
    return { type: 'TaskStatement', name, body };
  }

  parseBlock() {
    const statements = [];
    while (this.peek().type !== 'RBRACE' && this.peek().type !== 'EOF') {
      statements.push(this.parseStatement());
    }
    return statements;
  }

  parseExpressionStatement() {
    const expr = this.parseExpression();
    return { type: 'ExpressionStatement', expression: expr };
  }

  parseExpression() {
    return this.parseBinaryExpression();
  }

  parseBinaryExpression(precedence = 0) {
    let left = this.parsePrimary();
    
    const operators = {
      'PLUS': 1, 'MINUS': 1,
      'MULTIPLY': 2, 'DIVIDE': 2,
      'LT': 3, 'GT': 3, 'LTE': 3, 'GTE': 3,
      'EQ': 4, 'NEQ': 4,
      'ASSIGN': 5
    };
    
    while (true) {
      const token = this.peek();
      const opPrecedence = operators[token.type];
      
      if (!opPrecedence || opPrecedence <= precedence) {
        break;
      }
      
      this.advance();
      const operator = token.type;
      const right = this.parseBinaryExpression(opPrecedence);
      
      left = {
        type: 'BinaryExpression',
        operator,
        left,
        right
      };
    }
    
    return left;
  }

  parsePrimary() {
    const token = this.peek();
    
    switch (token.type) {
      case 'NUMBER':
        this.advance();
        return { type: 'NumberLiteral', value: token.value };
      case 'STRING':
        this.advance();
        return { type: 'StringLiteral', value: token.value };
      case 'TRUE':
        this.advance();
        return { type: 'BooleanLiteral', value: true };
      case 'FALSE':
        this.advance();
        return { type: 'BooleanLiteral', value: false };
      case 'IDENTIFIER':
        this.advance();
        // Check if it's a function call
        if (this.peek().type === 'LPAREN') {
          return this.parseFunctionCall(token.value);
        }
        return { type: 'Identifier', name: token.value };
      case 'LPAREN':
        this.advance();
        const expr = this.parseExpression();
        this.expect('RPAREN');
        return expr;
      default:
        throw new Error(`Unexpected token: ${token.type} at line ${token.line}, column ${token.column}`);
    }
  }

  parseFunctionCall(name) {
    this.expect('LPAREN');
    const args = [];
    
    if (this.peek().type !== 'RPAREN') {
      do {
        args.push(this.parseExpression());
      } while (this.peek().type === 'COMMA' && this.advance());
    }
    
    this.expect('RPAREN');
    return {
      type: 'FunctionCall',
      name,
      args
    };
  }
}

module.exports = { Parser };
