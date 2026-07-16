// Substitua a parte do acesso a objeto em evaluateExpression por:

// Acesso a objeto: objeto.propriedade
if (pos < tokens.size() && tokens[pos].type == TOKEN_DOT) {
    pos++; // Skip .
    if (pos < tokens.size() && tokens[pos].type == TOKEN_IDENTIFIER) {
        string propName = tokens[pos].value;
        pos++;
        string objName = varName;
        if (objects.find(objName) != objects.end() && 
            objects[objName].find(propName) != objects[objName].end()) {
            result = objects[objName][propName];
        } else {
            result = "null";
        }
    }
}
