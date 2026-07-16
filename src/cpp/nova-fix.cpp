// Na função evaluateExpression, na parte que processa métodos:

// Métodos de array: array.push(), array.pop(), array.length
if (pos < tokens.size() && tokens[pos].type == TOKEN_DOT) {
    pos++;
    if (pos < tokens.size() && tokens[pos].type == TOKEN_IDENTIFIER) {
        string method = tokens[pos].value;
        pos++;
        
        // Verifica se tem parênteses
        bool hasParens = false;
        if (pos < tokens.size() && tokens[pos].type == TOKEN_LPAREN) {
            hasParens = true;
            pos++; // Skip (
            if (pos < tokens.size() && tokens[pos].type == TOKEN_RPAREN) {
                pos++; // Skip )
            }
        }
        
        string arrayName = varName;
        if (arrays.find(arrayName) != arrays.end()) {
            if (method == "push" && hasParens) {
                // push precisa de argumento
                // Volta para processar o argumento
                pos--; // Volta o (
                // ... processa argumento
            } else if (method == "pop" && hasParens) {
                if (!arrays[arrayName].empty()) {
                    result = arrays[arrayName].back();
                    arrays[arrayName].pop_back();
                    variables[arrayName] = "[array]";
                } else {
                    result = "null";
                }
            } else if (method == "length" || method == "length()") {
                result = to_string(arrays[arrayName].size());
            }
        }
    }
}
