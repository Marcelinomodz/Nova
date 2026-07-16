class StandardLibrary {
  constructor() {
    this.functions = {
      'print': this.print,
      'length': this.length,
      'toNumber': this.toNumber,
      'toString': this.toString,
      'typeOf': this.typeOf,
      'random': this.random,
      'sleep': this.sleep,
      'now': this.now
    };
  }

  print(...args) {
    console.log(...args);
    return args[args.length - 1];
  }

  length(value) {
    if (typeof value === 'string' || Array.isArray(value)) {
      return value.length;
    }
    return 0;
  }

  toNumber(value) {
    return Number(value);
  }

  toString(value) {
    return String(value);
  }

  typeOf(value) {
    return typeof value;
  }

  random(min = 0, max = 1) {
    return Math.random() * (max - min) + min;
  }

  sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  now() {
    return Date.now();
  }

  getFunction(name) {
    return this.functions[name] || null;
  }

  getFunctions() {
    return Object.keys(this.functions);
  }
}

module.exports = { StandardLibrary };
