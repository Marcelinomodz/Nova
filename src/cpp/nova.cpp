#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cmath>

using namespace std;

// Token Types
enum TokenType {
    TOKEN_SAY, TOKEN_LET, TOKEN_IF, TOKEN_ELSE, TOKEN_LOOP,
    TOKEN_FUNC, TOKEN_RETURN, TOKEN_IMPORT, TOKEN_IDENTIFIER,
    TOKEN_NUMBER, TOKEN_STRING, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LBRACKET, TOKEN_RBRACKET,
    TOKEN_ASSIGN, TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY,
    TOKEN_DIVIDE, TOKEN_EQ, TOKEN_NEQ, TOKEN_LT, TOKEN_GT,
    TOKEN_LTE, TOKEN_GTE, TOKEN_AND, TOKEN_OR, TOKEN_NOT,
    TOKEN_TRUE, TOKEN_FALSE, TOKEN_COMMA, TOKEN_DOT,
    TOKEN_COLON, TOKEN_EOF, TOKEN_UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    int line;
    int col;
};

// Lexer (mesmo código)
class Lexer {
private:
    string source;
    int pos;
    int line;
    int col;
    
    char peek() { if (pos >= source.length()) return '\0'; return source[pos]; }
    char peekNext() { if (pos + 1 >= source.length()) return '\0'; return source[pos + 1]; }
    void advance() { if (peek() == '\n') { line++; col = 1; } else { col++; } pos++; }
    bool isWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
    bool isDigit(char c) { return c >= '0' && c <= '9'; }
    bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
    bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

public:
    Lexer(const string& src) : source(src), pos(0), line(1), col(1) {}
    
    vector<Token> tokenize() {
        vector<Token> tokens;
        
        while (pos < source.length()) {
            char c = peek();
            
            if (isWhitespace(c)) { advance(); continue; }
            
            if (c == '/' && peekNext() == '/') {
                while (peek() != '\n' && pos < source.length()) advance();
                continue;
            }
            
            if (isDigit(c)) {
                string value = "";
                int startLine = line, startCol = col;
                while (isDigit(peek())) { value += peek(); advance(); }
                if (peek() == '.') { value += peek(); advance(); while (isDigit(peek())) { value += peek(); advance(); } }
                tokens.push_back({TOKEN_NUMBER, value, startLine, startCol});
                continue;
            }
            
            if (c == '"' || c == '\'') {
                char quote = c;
                string value = "";
                int startLine = line, startCol = col;
                advance();
                while (peek() != quote && pos < source.length()) {
                    if (peek() == '\\') { advance(); switch (peek()) { case 'n': value += '\n'; break; default: value += peek(); } advance(); }
                    else { value += peek(); advance(); }
                }
                if (peek() == quote) advance();
                tokens.push_back({TOKEN_STRING, value, startLine, startCol});
                continue;
            }
            
            if (isAlpha(c)) {
                string value = "";
                int startLine = line, startCol = col;
                while (isAlphaNumeric(peek())) { value += peek(); advance(); }
                
                TokenType type = TOKEN_IDENTIFIER;
                if (value == "say") type = TOKEN_SAY;
                else if (value == "let") type = TOKEN_LET;
                else if (value == "if") type = TOKEN_IF;
                else if (value == "else") type = TOKEN_ELSE;
                else if (value == "loop") type = TOKEN_LOOP;
                else if (value == "func") type = TOKEN_FUNC;
                else if (value == "return") type = TOKEN_RETURN;
                else if (value == "import") type = TOKEN_IMPORT;
                else if (value == "true") type = TOKEN_TRUE;
                else if (value == "false") type = TOKEN_FALSE;
                
                tokens.push_back({type, value, startLine, startCol});
                continue;
            }
            
            int startLine = line, startCol = col;
            switch (c) {
                case '=': if (peekNext() == '=') { advance(); advance(); tokens.push_back({TOKEN_EQ, "==", startLine, startCol}); }
                          else { advance(); tokens.push_back({TOKEN_ASSIGN, "=", startLine, startCol}); } break;
                case '!': if (peekNext() == '=') { advance(); advance(); tokens.push_back({TOKEN_NEQ, "!=", startLine, startCol}); }
                          else { advance(); tokens.push_back({TOKEN_NOT, "!", startLine, startCol}); } break;
                case '<': if (peekNext() == '=') { advance(); advance(); tokens.push_back({TOKEN_LTE, "<=", startLine, startCol}); }
                          else { advance(); tokens.push_back({TOKEN_LT, "<", startLine, startCol}); } break;
                case '>': if (peekNext() == '=') { advance(); advance(); tokens.push_back({TOKEN_GTE, ">=", startLine, startCol}); }
                          else { advance(); tokens.push_back({TOKEN_GT, ">", startLine, startCol}); } break;
                case '&': if (peekNext() == '&') { advance(); advance(); tokens.push_back({TOKEN_AND, "&&", startLine, startCol}); } break;
                case '|': if (peekNext() == '|') { advance(); advance(); tokens.push_back({TOKEN_OR, "||", startLine, startCol}); } break;
                case '+': advance(); tokens.push_back({TOKEN_PLUS, "+", startLine, startCol}); break;
                case '-': advance(); tokens.push_back({TOKEN_MINUS, "-", startLine, startCol}); break;
                case '*': advance(); tokens.push_back({TOKEN_MULTIPLY, "*", startLine, startCol}); break;
                case '/': advance(); tokens.push_back({TOKEN_DIVIDE, "/", startLine, startCol}); break;
                case '(': advance(); tokens.push_back({TOKEN_LPAREN, "(", startLine, startCol}); break;
                case ')': advance(); tokens.push_back({TOKEN_RPAREN, ")", startLine, startCol}); break;
                case '{': advance(); tokens.push_back({TOKEN_LBRACE, "{", startLine, startCol}); break;
                case '}': advance(); tokens.push_back({TOKEN_RBRACE, "}", startLine, startCol}); break;
                case '[': advance(); tokens.push_back({TOKEN_LBRACKET, "[", startLine, startCol}); break;
                case ']': advance(); tokens.push_back({TOKEN_RBRACKET, "]", startLine, startCol}); break;
                case ',': advance(); tokens.push_back({TOKEN_COMMA, ",", startLine, startCol}); break;
                case '.': advance(); tokens.push_back({TOKEN_DOT, ".", startLine, startCol}); break;
                case ':': advance(); tokens.push_back({TOKEN_COLON, ":", startLine, startCol}); break;
                default: advance(); tokens.push_back({TOKEN_UNKNOWN, string(1, c), startLine, startCol});
            }
        }
        
        tokens.push_back({TOKEN_EOF, "EOF", line, col});
        return tokens;
    }
};

// Interpreter
class Interpreter {
private:
    map<string, string> variables;
    map<string, vector<string>> arrays;
    map<string, map<string, string>> objects;
    map<string, vector<string>> funcParams;
    map<string, vector<Token>> funcBody;
    map<string, string> modules;
    vector<string> output;
    int loopIndex;
    string returnValue;
    bool inFunctionCall;
    
    string getValue(const string& name) {
        if (variables.find(name) != variables.end()) {
            return variables[name];
        }
        return name;
    }
    
    string getArrayString(const string& name) {
        if (arrays.find(name) != arrays.end()) {
            string result = "[";
            for (size_t i = 0; i < arrays[name].size(); i++) {
                result += arrays[name][i];
                if (i < arrays[name].size() - 1) result += ", ";
            }
            result += "]";
            return result;
        }
        return "[array]";
    }
    
    bool isNumber(const string& s) {
        if (s.empty()) return false;
        for (char c : s) {
            if (!isdigit(c) && c != '.') return false;
        }
        return true;
    }
    
    string getObjectProperty(const string& objName, const string& propName) {
        if (objects.find(objName) != objects.end() && 
            objects[objName].find(propName) != objects[objName].end()) {
            return objects[objName][propName];
        }
        return "null";
    }
    
    string executeFunction(const string& name, vector<string> args) {
        if (funcBody.find(name) == funcBody.end()) {
            return "null";
        }
        
        map<string, string> oldVars = variables;
        map<string, vector<string>> oldArrays = arrays;
        map<string, map<string, string>> oldObjects = objects;
        string oldReturn = returnValue;
        bool oldInFunction = inFunctionCall;
        
        inFunctionCall = true;
        returnValue = "";
        
        vector<string> params = funcParams[name];
        for (size_t i = 0; i < params.size() && i < args.size(); i++) {
            variables[params[i]] = args[i];
        }
        
        vector<Token> body = funcBody[name];
        for (size_t i = 0; i < body.size(); i++) {
            Token t = body[i];
            if (t.type == TOKEN_RETURN) {
                i++;
                if (i < body.size()) {
                    int pos = i;
                    returnValue = evaluateExpression(body, pos, loopIndex);
                    i = pos;
                }
                break;
            } else if (t.type == TOKEN_SAY) {
                i++;
                if (i < body.size()) {
                    int pos = i;
                    string result = evaluateExpression(body, pos, loopIndex);
                    i = pos;
                    cout << result << endl;
                    output.push_back(result);
                }
            } else if (t.type == TOKEN_LET) {
                i++;
                if (i < body.size() && body[i].type == TOKEN_IDENTIFIER) {
                    string varName = body[i].value;
                    i++;
                    if (i < body.size() && body[i].type == TOKEN_ASSIGN) {
                        i++;
                        int pos = i;
                        string value = evaluateExpression(body, pos, loopIndex);
                        i = pos;
                        variables[varName] = value;
                    }
                }
            }
        }
        
        string result = returnValue;
        
        variables = oldVars;
        arrays = oldArrays;
        objects = oldObjects;
        returnValue = oldReturn;
        inFunctionCall = oldInFunction;
        
        return result;
    }
    
    string evaluateExpression(const vector<Token>& tokens, int& pos, int loopIdx = -1) {
        string result = "";
        
        if (pos >= tokens.size()) return "";
        
        if (tokens[pos].type == TOKEN_STRING) {
            result = tokens[pos].value;
            pos++;
        } else if (tokens[pos].type == TOKEN_NUMBER) {
            result = tokens[pos].value;
            pos++;
        } else if (tokens[pos].type == TOKEN_TRUE) {
            result = "true";
            pos++;
        } else if (tokens[pos].type == TOKEN_FALSE) {
            result = "false";
            pos++;
        } else if (tokens[pos].type == TOKEN_IDENTIFIER) {
            string varName = tokens[pos].value;
            
            if (varName == "loop_index" && loopIdx >= 0) {
                result = to_string(loopIdx);
            } else if (arrays.find(varName) != arrays.end()) {
                result = getArrayString(varName);
            } else if (objects.find(varName) != objects.end()) {
                result = varName;
            } else {
                result = getValue(varName);
            }
            pos++;
            
            // Verifica se é acesso a objeto: objeto.propriedade
            if (pos < tokens.size() && tokens[pos].type == TOKEN_DOT) {
                pos++;
                if (pos < tokens.size() && tokens[pos].type == TOKEN_IDENTIFIER) {
                    string propName = tokens[pos].value;
                    pos++;
                    string objName = result;
                    result = getObjectProperty(objName, propName);
                }
            }
            
            // Chamada de função: nome(args)
            if (pos < tokens.size() && tokens[pos].type == TOKEN_LPAREN) {
                pos++;
                vector<string> args;
                while (pos < tokens.size() && tokens[pos].type != TOKEN_RPAREN) {
                    if (tokens[pos].type == TOKEN_COMMA) {
                        pos++;
                        continue;
                    }
                    int tempPos = pos;
                    string arg = evaluateExpression(tokens, tempPos, loopIdx);
                    pos = tempPos;
                    args.push_back(arg);
                }
                if (pos < tokens.size() && tokens[pos].type == TOKEN_RPAREN) {
                    pos++;
                }
                result = executeFunction(varName, args);
            }
            
            // Métodos de array: array.push(), array.pop(), array.length
            if (pos < tokens.size() && tokens[pos].type == TOKEN_DOT) {
                pos++;
                if (pos < tokens.size() && tokens[pos].type == TOKEN_IDENTIFIER) {
                    string method = tokens[pos].value;
                    pos++;
                    
                    string arrayName = varName;
                    if (arrays.find(arrayName) != arrays.end()) {
                        if (method == "push") {
                            if (pos < tokens.size() && tokens[pos].type == TOKEN_LPAREN) {
                                pos++;
                                string value = evaluateExpression(tokens, pos, loopIdx);
                                if (pos < tokens.size() && tokens[pos].type == TOKEN_RPAREN) {
                                    pos++;
                                    arrays[arrayName].push_back(value);
                                    variables[arrayName] = "[array]";
                                    result = getArrayString(arrayName);
                                }
                            }
                        } else if (method == "pop") {
                            if (pos < tokens.size() && tokens[pos].type == TOKEN_LPAREN) {
                                pos++;
                                if (pos < tokens.size() && tokens[pos].type == TOKEN_RPAREN) {
                                    pos++;
                                    if (!arrays[arrayName].empty()) {
                                        result = arrays[arrayName].back();
                                        arrays[arrayName].pop_back();
                                        variables[arrayName] = "[array]";
                                    } else {
                                        result = "null";
                                    }
                                }
                            }
                        } else if (method == "length" || method == "length()") {
                            result = to_string(arrays[arrayName].size());
                        }
                    }
                    // Métodos de string
                    else if (variables.find(arrayName) != variables.end()) {
                        string str = variables[arrayName];
                        if (method == "length") {
                            result = to_string(str.length());
                        } else if (method == "upper") {
                            string upper = str;
                            transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
                            result = upper;
                        } else if (method == "lower") {
                            string lower = str;
                            transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                            result = lower;
                        }
                    }
                }
            }
            
            // Acesso a array: array[0]
            if (pos < tokens.size() && tokens[pos].type == TOKEN_LBRACKET) {
                pos++;
                string indexStr = evaluateExpression(tokens, pos, loopIdx);
                if (pos < tokens.size() && tokens[pos].type == TOKEN_RBRACKET) {
                    pos++;
                    string arrayName = varName;
                    if (arrays.find(arrayName) != arrays.end() && isNumber(indexStr)) {
                        int idx = stoi(indexStr);
                        if (idx >= 0 && idx < (int)arrays[arrayName].size()) {
                            result = arrays[arrayName][idx];
                        } else {
                            result = "null";
                        }
                    } else {
                        result = "null";
                    }
                }
            }
        } else if (tokens[pos].type == TOKEN_LPAREN) {
            pos++;
            result = evaluateExpression(tokens, pos, loopIdx);
            if (pos < tokens.size() && tokens[pos].type == TOKEN_RPAREN) {
                pos++;
            }
        } else {
            return "";
        }
        
        // Processa operadores
        while (pos < tokens.size()) {
            if (tokens[pos].type == TOKEN_PLUS) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                if (isNumber(result) && isNumber(right)) {
                    double num1 = stod(result);
                    double num2 = stod(right);
                    result = to_string(num1 + num2);
                } else {
                    result = result + right;
                }
            } else if (tokens[pos].type == TOKEN_MINUS) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                double num1 = stod(result);
                double num2 = stod(right);
                result = to_string(num1 - num2);
            } else if (tokens[pos].type == TOKEN_MULTIPLY) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                double num1 = stod(result);
                double num2 = stod(right);
                result = to_string(num1 * num2);
            } else if (tokens[pos].type == TOKEN_DIVIDE) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                double num1 = stod(result);
                double num2 = stod(right);
                result = to_string(num1 / num2);
            } else if (tokens[pos].type == TOKEN_EQ) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                result = (result == right) ? "true" : "false";
            } else if (tokens[pos].type == TOKEN_NEQ) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                result = (result != right) ? "true" : "false";
            } else if (tokens[pos].type == TOKEN_LT) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                if (isNumber(result) && isNumber(right)) {
                    result = (stod(result) < stod(right)) ? "true" : "false";
                } else {
                    result = (result < right) ? "true" : "false";
                }
            } else if (tokens[pos].type == TOKEN_GT) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                if (isNumber(result) && isNumber(right)) {
                    result = (stod(result) > stod(right)) ? "true" : "false";
                } else {
                    result = (result > right) ? "true" : "false";
                }
            } else if (tokens[pos].type == TOKEN_LTE) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                if (isNumber(result) && isNumber(right)) {
                    result = (stod(result) <= stod(right)) ? "true" : "false";
                } else {
                    result = (result <= right) ? "true" : "false";
                }
            } else if (tokens[pos].type == TOKEN_GTE) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                if (isNumber(result) && isNumber(right)) {
                    result = (stod(result) >= stod(right)) ? "true" : "false";
                } else {
                    result = (result >= right) ? "true" : "false";
                }
            } else if (tokens[pos].type == TOKEN_AND) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                bool leftBool = (result == "true" || result == "1");
                bool rightBool = (right == "true" || right == "1");
                result = (leftBool && rightBool) ? "true" : "false";
            } else if (tokens[pos].type == TOKEN_OR) {
                pos++;
                string right = evaluateExpression(tokens, pos, loopIdx);
                bool leftBool = (result == "true" || result == "1");
                bool rightBool = (right == "true" || right == "1");
                result = (leftBool || rightBool) ? "true" : "false";
            } else if (tokens[pos].type == TOKEN_NOT) {
                pos++;
                bool val = (result == "true" || result == "1");
                result = val ? "false" : "true";
            } else {
                break;
            }
        }
        
        return result;
    }

public:
    Interpreter() : loopIndex(0), returnValue(""), inFunctionCall(false) {}
    
    void execute(const vector<Token>& tokens) {
        int i = 0;
        
        while (i < tokens.size()) {
            Token token = tokens[i];
            
            if (token.type == TOKEN_SAY) {
                i++;
                if (i < tokens.size()) {
                    int tempPos = i;
                    string result = evaluateExpression(tokens, tempPos, loopIndex);
                    i = tempPos;
                    cout << result << endl;
                    output.push_back(result);
                }
            }
            else if (token.type == TOKEN_LET) {
                i++;
                if (i < tokens.size() && tokens[i].type == TOKEN_IDENTIFIER) {
                    string varName = tokens[i].value;
                    i++;
                    if (i < tokens.size() && tokens[i].type == TOKEN_ASSIGN) {
                        i++;
                        
                        if (i < tokens.size() && tokens[i].type == TOKEN_LBRACKET) {
                            vector<string> arrayValues;
                            i++;
                            while (i < tokens.size() && tokens[i].type != TOKEN_RBRACKET) {
                                if (tokens[i].type == TOKEN_STRING || tokens[i].type == TOKEN_NUMBER ||
                                    tokens[i].type == TOKEN_TRUE || tokens[i].type == TOKEN_FALSE) {
                                    arrayValues.push_back(tokens[i].value);
                                    i++;
                                } else if (tokens[i].type == TOKEN_COMMA) {
                                    i++;
                                } else {
                                    i++;
                                }
                            }
                            if (i < tokens.size() && tokens[i].type == TOKEN_RBRACKET) {
                                i++;
                            }
                            arrays[varName] = arrayValues;
                            variables[varName] = "[array]";
                        }
                        else if (i < tokens.size() && tokens[i].type == TOKEN_LBRACE) {
                            map<string, string> obj;
                            i++;
                            while (i < tokens.size() && tokens[i].type != TOKEN_RBRACE) {
                                if (tokens[i].type == TOKEN_IDENTIFIER) {
                                    string key = tokens[i].value;
                                    i++;
                                    if (i < tokens.size() && tokens[i].type == TOKEN_COLON) {
                                        i++;
                                        if (i < tokens.size()) {
                                            string value = "";
                                            if (tokens[i].type == TOKEN_STRING) {
                                                value = tokens[i].value;
                                                i++;
                                            } else if (tokens[i].type == TOKEN_NUMBER) {
                                                value = tokens[i].value;
                                                i++;
                                            } else if (tokens[i].type == TOKEN_TRUE) {
                                                value = "true";
                                                i++;
                                            } else if (tokens[i].type == TOKEN_FALSE) {
                                                value = "false";
                                                i++;
                                            }
                                            obj[key] = value;
                                        }
                                    }
                                } else {
                                    i++;
                                }
                                if (i < tokens.size() && tokens[i].type == TOKEN_COMMA) {
                                    i++;
                                }
                            }
                            if (i < tokens.size() && tokens[i].type == TOKEN_RBRACE) {
                                i++;
                            }
                            objects[varName] = obj;
                            variables[varName] = "[object]";
                        }
                        else {
                            int tempPos = i;
                            string value = evaluateExpression(tokens, tempPos, loopIndex);
                            i = tempPos;
                            variables[varName] = value;
                        }
                    }
                }
            }
            else if (token.type == TOKEN_LOOP) {
                i++;
                int count = 0;
                if (i < tokens.size()) {
                    int tempPos = i;
                    string countStr = evaluateExpression(tokens, tempPos, loopIndex);
                    i = tempPos;
                    count = stoi(countStr);
                }
                
                if (i < tokens.size() && tokens[i].type == TOKEN_LBRACE) {
                    i++;
                    for (int loop = 0; loop < count; loop++) {
                        int bodyPos = i;
                        loopIndex = loop;
                        while (bodyPos < tokens.size() && tokens[bodyPos].type != TOKEN_RBRACE) {
                            if (tokens[bodyPos].type == TOKEN_SAY) {
                                bodyPos++;
                                if (bodyPos < tokens.size()) {
                                    int tempPos = bodyPos;
                                    string result = evaluateExpression(tokens, tempPos, loop);
                                    bodyPos = tempPos;
                                    cout << result << endl;
                                    output.push_back(result);
                                }
                            } else {
                                bodyPos++;
                            }
                        }
                    }
                    while (i < tokens.size() && tokens[i].type != TOKEN_RBRACE) i++;
                    if (i < tokens.size()) i++;
                }
            }
            else if (token.type == TOKEN_IF) {
                i++;
                int tempPos = i;
                string condition = evaluateExpression(tokens, tempPos, loopIndex);
                i = tempPos;
                
                bool isTrue = (condition == "true" || condition == "1");
                
                if (i < tokens.size() && tokens[i].type == TOKEN_LBRACE) {
                    i++;
                    if (isTrue) {
                        int bodyPos = i;
                        while (bodyPos < tokens.size() && tokens[bodyPos].type != TOKEN_RBRACE) {
                            if (tokens[bodyPos].type == TOKEN_SAY) {
                                bodyPos++;
                                if (bodyPos < tokens.size()) {
                                    int exprPos = bodyPos;
                                    string result = evaluateExpression(tokens, exprPos, loopIndex);
                                    bodyPos = exprPos;
                                    cout << result << endl;
                                    output.push_back(result);
                                }
                            } else {
                                bodyPos++;
                            }
                        }
                    }
                    while (i < tokens.size() && tokens[i].type != TOKEN_RBRACE) i++;
                    if (i < tokens.size()) i++;
                }
                
                if (i < tokens.size() && tokens[i].type == TOKEN_ELSE) {
                    i++;
                    if (i < tokens.size() && tokens[i].type == TOKEN_LBRACE) {
                        i++;
                        if (!isTrue) {
                            int bodyPos = i;
                            while (bodyPos < tokens.size() && tokens[bodyPos].type != TOKEN_RBRACE) {
                                if (tokens[bodyPos].type == TOKEN_SAY) {
                                    bodyPos++;
                                    if (bodyPos < tokens.size()) {
                                        int exprPos = bodyPos;
                                        string result = evaluateExpression(tokens, exprPos, loopIndex);
                                        bodyPos = exprPos;
                                        cout << result << endl;
                                        output.push_back(result);
                                    }
                                } else {
                                    bodyPos++;
                                }
                            }
                        }
                        while (i < tokens.size() && tokens[i].type != TOKEN_RBRACE) i++;
                        if (i < tokens.size()) i++;
                    }
                }
            }
            else if (token.type == TOKEN_FUNC) {
                i++;
                if (i < tokens.size() && tokens[i].type == TOKEN_IDENTIFIER) {
                    string funcName = tokens[i].value;
                    i++;
                    
                    vector<string> params;
                    if (i < tokens.size() && tokens[i].type == TOKEN_LPAREN) {
                        i++;
                        while (i < tokens.size() && tokens[i].type != TOKEN_RPAREN) {
                            if (tokens[i].type == TOKEN_IDENTIFIER) {
                                params.push_back(tokens[i].value);
                            }
                            i++;
                            if (i < tokens.size() && tokens[i].type == TOKEN_COMMA) {
                                i++;
                            }
                        }
                        if (i < tokens.size() && tokens[i].type == TOKEN_RPAREN) {
                            i++;
                        }
                    }
                    
                    if (i < tokens.size() && tokens[i].type == TOKEN_LBRACE) {
                        i++;
                        vector<Token> body;
                        int braceCount = 1;
                        while (i < tokens.size() && braceCount > 0) {
                            if (tokens[i].type == TOKEN_LBRACE) braceCount++;
                            if (tokens[i].type == TOKEN_RBRACE) braceCount--;
                            if (braceCount > 0) {
                                body.push_back(tokens[i]);
                            }
                            i++;
                        }
                        funcParams[funcName] = params;
                        funcBody[funcName] = body;
                        variables[funcName] = "[function]";
                    }
                }
            }
            else if (token.type == TOKEN_IMPORT) {
                i++;
                if (i < tokens.size()) {
                    string module = "";
                    if (tokens[i].type == TOKEN_STRING) {
                        module = tokens[i].value;
                        i++;
                    }
                    modules[module] = "loaded";
                    cout << "📦 Importando módulo: " << module << endl;
                    output.push_back("import: " + module);
                }
            }
            else {
                i++;
            }
        }
    }
    
    vector<string> getOutput() {
        return output;
    }
};

int main(int argc, char* argv[]) {
    cout << "╔═══════════════════════════════════════════╗" << endl;
    cout << "║                                           ║" << endl;
    cout << "║    ███╗   ██╗ ██████╗ ██╗   ██╗ █████╗   ║" << endl;
    cout << "║    ████╗  ██║██╔═══██╗██║   ██║██╔══██╗  ║" << endl;
    cout << "║    ██╔██╗ ██║██║   ██║██║   ██║███████║  ║" << endl;
    cout << "║    ██║╚██╗██║██║   ██║╚██╗ ██╔╝██╔══██║  ║" << endl;
    cout << "║    ██║ ╚████║╚██████╔╝ ╚████╔╝ ██║  ██║  ║" << endl;
    cout << "║    ╚═╝  ╚═══╝ ╚═════╝   ╚═══╝  ╚═╝  ╚═╝  ║" << endl;
    cout << "║                                           ║" << endl;
    cout << "║         Nova Language v0.6.0              ║" << endl;
    cout << "║         by MARCELINO MODZ                 ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;
    cout << endl;
    
    if (argc < 2) {
        cout << "Usage: nova <file.nv>" << endl;
        cout << "       nova --help" << endl;
        cout << "       nova --version" << endl;
        return 0;
    }
    
    string arg = argv[1];
    
    if (arg == "--help") {
        cout << "Nova Language v0.6.0 by MARCELINO MODZ" << endl;
        cout << "Usage: nova <file.nv> [options]" << endl;
        return 0;
    }
    
    if (arg == "--version") {
        cout << "Nova Language v0.6.0 by MARCELINO MODZ" << endl;
        return 0;
    }
    
    ifstream file(arg);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << arg << endl;
        return 1;
    }
    
    string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();
    
    Interpreter interpreter;
    interpreter.execute(tokens);
    
    return 0;
}
