class MemoryManager {
  constructor() {
    this.heap = new Map();
    this.stack = [];
    this.gcThreshold = 1000;
    this.allocations = 0;
    this.idCounter = 0;
  }

  allocate(value) {
    const id = 'var_' + (this.idCounter++);
    this.heap.set(id, {
      value: value,
      refCount: 1,
      marked: false
    });
    this.allocations++;
    
    if (this.allocations > this.gcThreshold) {
      this.garbageCollect();
    }
    
    return id;
  }

  get(id) {
    if (!this.heap.has(id)) {
      throw new Error(`Memory error: Invalid reference ${id}`);
    }
    return this.heap.get(id).value;
  }

  set(id, value) {
    if (!this.heap.has(id)) {
      throw new Error(`Memory error: Invalid reference ${id}`);
    }
    this.heap.get(id).value = value;
  }

  incrementRef(id) {
    if (this.heap.has(id)) {
      this.heap.get(id).refCount++;
    }
  }

  decrementRef(id) {
    if (this.heap.has(id)) {
      const ref = this.heap.get(id);
      ref.refCount--;
      if (ref.refCount <= 0) {
        this.heap.delete(id);
      }
    }
  }

  pushStack(value) {
    this.stack.push(value);
  }

  popStack() {
    return this.stack.pop();
  }

  garbageCollect() {
    for (const [id, ref] of this.heap) {
      if (ref.refCount > 0) {
        ref.marked = true;
      }
    }
    
    for (const [id, ref] of this.heap) {
      if (!ref.marked) {
        this.heap.delete(id);
      }
    }
    
    this.allocations = 0;
  }

  getStats() {
    return {
      heapSize: this.heap.size,
      stackSize: this.stack.length,
      allocations: this.allocations
    };
  }
}

class Runtime {
  constructor() {
    this.memory = new MemoryManager();
    this.globals = new Map();
    this.currentScope = this.globals;
    this.scopeStack = [this.globals];
  }

  pushScope() {
    const scope = new Map();
    this.scopeStack.push(scope);
    this.currentScope = scope;
    return scope;
  }

  popScope() {
    this.scopeStack.pop();
    this.currentScope = this.scopeStack[this.scopeStack.length - 1];
    return this.currentScope;
  }

  declareVariable(name, value) {
    const id = this.memory.allocate(value);
    this.currentScope.set(name, id);
    return id;
  }

  getVariable(name) {
    for (let i = this.scopeStack.length - 1; i >= 0; i--) {
      if (this.scopeStack[i].has(name)) {
        const id = this.scopeStack[i].get(name);
        return this.memory.get(id);
      }
    }
    throw new Error(`Undefined variable: ${name}`);
  }

  setVariable(name, value) {
    for (let i = this.scopeStack.length - 1; i >= 0; i--) {
      if (this.scopeStack[i].has(name)) {
        const id = this.scopeStack[i].get(name);
        this.memory.set(id, value);
        return;
      }
    }
    throw new Error(`Undefined variable: ${name}`);
  }

  getVariableByValue(value) {
    for (const [name, id] of this.currentScope) {
      try {
        const val = this.memory.get(id);
        if (val === value || String(val) === String(value)) {
          return val;
        }
      } catch (e) {}
    }
    return undefined;
  }

  getMemoryStats() {
    return this.memory.getStats();
  }
}

module.exports = { Runtime, MemoryManager };
