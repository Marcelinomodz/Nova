class Lexer {
  constructor(code) {
    this.code = code;
    this.position = 0;
    this.tokens = [];
    this.line = 1;
    this.column = 1;
  }

  tokenize() {
    while (this.position < this.code.length) {
      const char = this.peek();
      
      // Skip whitespace
      if (this.isWhitespace(char)) {
        this.advance();
        continue;
      }
      
      // Skip comments
      if (char === '/' && this.peekNext() === '/') {
        this.skipLineComment();
        continue;
      }
      
      if (char === '/' && this.peekNext() === '*') {
        this.skipBlockComment();
        continue;
      }
      
      // Numbers
      if (this.isDigit(char)) {
        this.tokenizeNumber();
        continue;
      }
      
      // Strings
      if (char === '"' || char === "'") {
        this.tokenizeString(char);
        continue;
      }
      
      // Identifiers and keywords
      if (this.isAlpha(char)) {
        this.tokenizeIdentifier();
        continue;
      }
      
      // Operators and punctuation
      this.tokenizeOperator();
    }
    
    this.tokens.push({ type: 'EOF', value: 'EOF', line: this.line, column: this.column });
    return this.tokens;
  }

  peek() {
    return this.code[this.position] || '';
  }

  peekNext() {
    return this.code[this.position + 1] || '';
  }

  advance() {
    if (this.peek() === '\n') {
      this.line++;
      this.column = 1;
    } else {
      this.column++;
    }
    this.position++;
  }

  isWhitespace(char) {
    return char === ' ' || char === '\t' || char === '\n' || char === '\r';
  }

  isDigit(char) {
    return char >= '0' && char <= '9';
  }

  isAlpha(char) {
    return (char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z') || char === '_';
  }

  isAlphaNumeric(char) {
    return this.isAlpha(char) || this.isDigit(char);
  }

  skipLineComment() {
    while (this.peek() !== '\n' && this.position < this.code.length) {
      this.advance();
    }
  }

  skipBlockComment() {
    this.advance(); // Skip *
    while (!(this.peek() === '*' && this.peekNext() === '/') && this.position < this.code.length) {
      this.advance();
    }
    this.advance(); // Skip *
    this.advance(); // Skip /
  }

  tokenizeNumber() {
    let value = '';
    const startLine = this.line;
    const startColumn = this.column;
    
    while (this.isDigit(this.peek())) {
      value += this.peek();
      this.advance();
    }
    
    if (this.peek() === '.') {
      value += this.peek();
      this.advance();
      while (this.isDigit(this.peek())) {
        value += this.peek();
        this.advance();
      }
    }
    
    this.tokens.push({ 
      type: 'NUMBER', 
      value: parseFloat(value), 
      line: startLine, 
      column: startColumn 
    });
  }

  tokenizeString(quote) {
    let value = '';
    const startLine = this.line;
    const startColumn = this.column;
    
    this.advance(); // Skip opening quote
    
    while (this.peek() !== quote && this.position < this.code.length) {
      if (this.peek() === '\\') {
        this.advance();
        switch (this.peek()) {
          case 'n': value += '\n'; break;
          case 't': value += '\t'; break;
          case 'r': value += '\r'; break;
          case '"': value += '"'; break;
          case "'": value += "'"; break;
          case '\\': value += '\\'; break;
          default: value += this.peek();
        }
        this.advance();
      } else {
        value += this.peek();
        this.advance();
      }
    }
    
    if (this.peek() === quote) {
      this.advance(); // Skip closing quote
    }
    
    this.tokens.push({ 
      type: 'STRING', 
      value: value, 
      line: startLine, 
      column: startColumn 
    });
  }

  tokenizeIdentifier() {
    let value = '';
    const startLine = this.line;
    const startColumn = this.column;
    
    while (this.isAlphaNumeric(this.peek())) {
      value += this.peek();
      this.advance();
    }
    
    const keywords = {
      'say': 'SAY',
      'let': 'LET',
      'if': 'IF',
      'else': 'ELSE',
      'loop': 'LOOP',
      'func': 'FUNC',
      'return': 'RETURN',
      'import': 'IMPORT',
      'task': 'TASK',
      'every': 'EVERY',
      'true': 'TRUE',
      'false': 'FALSE',
      'bot': 'BOT',
      'command': 'COMMAND',
      'reply': 'REPLY'
    };
    
    const type = keywords[value] || 'IDENTIFIER';
    this.tokens.push({ 
      type, 
      value, 
      line: startLine, 
      column: startColumn 
    });
  }

  tokenizeOperator() {
    const char = this.peek();
    const startLine = this.line;
    const startColumn = this.column;
    
    switch (char) {
      case '=':
        if (this.peekNext() === '=') {
          this.advance();
          this.advance();
          this.tokens.push({ type: 'EQ', value: '==', line: startLine, column: startColumn });
        } else {
          this.advance();
          this.tokens.push({ type: 'ASSIGN', value: '=', line: startLine, column: startColumn });
        }
        break;
      case '!':
        if (this.peekNext() === '=') {
          this.advance();
          this.advance();
          this.tokens.push({ type: 'NEQ', value: '!=', line: startLine, column: startColumn });
        } else {
          this.advance();
          this.tokens.push({ type: 'NOT', value: '!', line: startLine, column: startColumn });
        }
        break;
      case '<':
        if (this.peekNext() === '=') {
          this.advance();
          this.advance();
          this.tokens.push({ type: 'LTE', value: '<=', line: startLine, column: startColumn });
        } else {
          this.advance();
          this.tokens.push({ type: 'LT', value: '<', line: startLine, column: startColumn });
        }
        break;
      case '>':
        if (this.peekNext() === '=') {
          this.advance();
          this.advance();
          this.tokens.push({ type: 'GTE', value: '>=', line: startLine, column: startColumn });
        } else {
          this.advance();
          this.tokens.push({ type: 'GT', value: '>', line: startLine, column: startColumn });
        }
        break;
      case '+':
        this.advance();
        this.tokens.push({ type: 'PLUS', value: '+', line: startLine, column: startColumn });
        break;
      case '-':
        this.advance();
        this.tokens.push({ type: 'MINUS', value: '-', line: startLine, column: startColumn });
        break;
      case '*':
        this.advance();
        this.tokens.push({ type: 'MULTIPLY', value: '*', line: startLine, column: startColumn });
        break;
      case '/':
        this.advance();
        this.tokens.push({ type: 'DIVIDE', value: '/', line: startLine, column: startColumn });
        break;
      case '(':
        this.advance();
        this.tokens.push({ type: 'LPAREN', value: '(', line: startLine, column: startColumn });
        break;
      case ')':
        this.advance();
        this.tokens.push({ type: 'RPAREN', value: ')', line: startLine, column: startColumn });
        break;
      case '{':
        this.advance();
        this.tokens.push({ type: 'LBRACE', value: '{', line: startLine, column: startColumn });
        break;
      case '}':
        this.advance();
        this.tokens.push({ type: 'RBRACE', value: '}', line: startLine, column: startColumn });
        break;
      case ',':
        this.advance();
        this.tokens.push({ type: 'COMMA', value: ',', line: startLine, column: startColumn });
        break;
      case ';':
        this.advance();
        this.tokens.push({ type: 'SEMICOLON', value: ';', line: startLine, column: startColumn });
        break;
      case ':':
        this.advance();
        this.tokens.push({ type: 'COLON', value: ':', line: startLine, column: startColumn });
        break;
      default:
        throw new Error(`Unexpected character: ${char} at line ${this.line}, column ${this.column}`);
    }
  }
}

module.exports = { Lexer };
