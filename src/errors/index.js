class NovaError extends Error {
  constructor(message, line, column, type = 'RuntimeError') {
    super(message);
    this.name = 'NovaError';
    this.type = type;
    this.line = line;
    this.column = column;
  }

  toString() {
    return `[${this.type}] ${this.message} at line ${this.line}, column ${this.column}`;
  }
}

class LexerError extends NovaError {
  constructor(message, line, column) {
    super(message, line, column, 'LexerError');
  }
}

class ParserError extends NovaError {
  constructor(message, line, column) {
    super(message, line, column, 'ParserError');
  }
}

class RuntimeError extends NovaError {
  constructor(message, line, column) {
    super(message, line, column, 'RuntimeError');
  }
}

class TypeError extends NovaError {
  constructor(message, line, column) {
    super(message, line, column, 'TypeError');
  }
}

class MemoryError extends NovaError {
  constructor(message, line, column) {
    super(message, line, column, 'MemoryError');
  }
}

module.exports = {
  NovaError,
  LexerError,
  ParserError,
  RuntimeError,
  TypeError,
  MemoryError
};
