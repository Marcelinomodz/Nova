#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>

using namespace std;

// Token Types
enum TokenType {
    TOKEN_SAY,
    TOKEN_LET,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_LOOP,
    TOKEN_FUNC,
    TOKEN_RETURN,
    TOKEN_IMPORT,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LTE,
    TOKEN_GTE,
    TOKEN_COMMA,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    int line;
    int col;
};

struct Function {
    string name;
    vector<string> params;
    vector<string> body;
    bool isBuiltin;
};

// Lexer
class Lexer {
private:
    string source;
    int pos;
    int line;
    int col;
    
    char peek() {
        if (pos >= source.length()) return '\0';
        return source[pos];
    }
    
    char peekNext() {
        if (pos + 1 >= source.length()) return '\0';
        return source[pos + 1];
    }
    
    void advance() {
        if (peek() == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
        pos++;
    }
    
    bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }
    
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    
    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }
    
    bool isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }

public:
    Lexer(const string& src) : source(src), pos(0), line(1), col(1) {}
    
    vector<Token> tokenize() {
        vector<Token> tokens;
        
        while (pos < source.length()) {
            char c = peek();
            
            if (isWhitespace(c)) {
                advance();
                continue;
            }
            
            // Comments
            if (c == '/' && peekNext() == '/') {
                while (peek() != '\n' && pos < source.length()) {
                    advance();
                }
                continue;
            }
            
            // Numbers
            if (isDigit(c)) {
                string value = "";
                int startLine = line;
                int startCol = col;
                
                while (isDigit(peek())) {
                    value += peek();
                    advance();
                }
                
                if (peek() == '.') {
                    value += peek();
                    advance();
                    while (isDigit(peek())) {
                        value += peek();
                        advance();
                    }
                }
                
                tokens.push_back({TOKEN_NUMBER, value, startLine, startCol});
                continue;
            }
            
            // Strings
            if (c == '"' || c == '\'') {
                char quote = c;
                string value = "";
                int startLine = line;
                int startCol = col;
                
                advance();
                
                while (peek() != quote && pos < source.length()) {
                    if (peek() == '\\') {
                        advance();
                        switch (peek()) {
                            case 'n': value += '\n'; break;
                            case 't': value += '\t'; break;
                            case '"': value += '"'; break;
                            case '\'': value += '\''; break;
                            case '\\': value += '\\'; break;
                            default: value += peek();
                        }
                        advance();
                    } else {
                        value += peek();
                        advance();
                    }
                }
                
                if (peek() == quote) {
                    advance();
                }
                
                tokens.push_back({TOKEN_STRING, value, startLine, startCol});
                continue;
            }
            
            // Identifiers and keywords
            if (isAlpha(c)) {
                string value = "";
                int startLine = line;
                int startCol = col;
                
                while (isAlphaNumeric(peek())) {
                    value += peek();
                    advance();
                }
                
                TokenType type = TOKEN_IDENTIFIER;
                if (value == "say") type = TOKEN_SAY;
                else if (value == "let") type = TOKEN_LET;
                else if (value == "if") type = TOKEN_IF;
                else if (value == "else") type = TOKEN_ELSE;
                else if (value == "loop") type = TOKEN_LOOP;
                else if (value == "func") type = TOKEN_FUNC;
                else if (value == "return") type = TOKEN_RETURN;
                else if (value == "import") type = TOKEN_IMPORT;
                
                tokens.push_back({type, value, startLine, startCol});
                continue;
            }
            
            // Operators
            int startLine = line;
            int startCol = col;
            
            switch (c) {
                case '=':
                    if (peekNext() == '=') {
                        advance();
                        advance();
                        tokens.push_back({TOKEN_EQ, "==", startLine, startCol});
                    } else {
                        advance();
                        tokens.push_back({TOKEN_ASSIGN, "=", startLine, startCol});
                    }
                    break;
                case '!':
                    if (peekNext() == '=') {
                        advance();
                        advance();
                        tokens.push_back({TOKEN_NEQ, "!=", startLine, startCol});
                    } else {
                        advance();
                        tokens.push_back({TOKEN_UNKNOWN, "!", startLine, startCol});
                    }
                    break;
                case '<':
                    if (peekNext() == '=') {
                        advance();
                        advance();
                        tokens.push_back({TOKEN_LTE, "<=", startLine, startCol});
                    } else {
                        advance();
                        tokens.push_back({TOKEN_LT, "<", startLine, startCol});
                    }
                    break;
                case '>':
                    if (peekNext() == '=') {
                        advance();
                        advance();
                        tokens.push_back({TOKEN_GTE, ">=", startLine, startCol});
                    } else {
                        advance();
                        tokens.push_back({TOKEN_GT, ">", startLine, startCol});
                    }
                    break;
                case '+':
                    advance();
                    tokens.push_back({TOKEN_PLUS, "+", startLine, startCol});
                    break;
                case '-':
                    advance();
                    tokens.push_back({TOKEN_MINUS, "-", startLine, startCol});
                    break;
                case '*':
                    advance();
                    tokens.push_back({TOKEN_MULTIPLY, "*", startLine, startCol});
                    break;
                case '/':
                    advance();
                    tokens.push_back({TOKEN_DIVIDE, "/", startLine, startCol});
                    break;
                case '(':
                    advance();
                    tokens.push_back({TOKEN_LPAREN, "(", startLine, startCol});
                    break;
                case ')':
                    advance();
                    tokens.push_back({TOKEN_RPAREN, ")", startLine, startCol});
                    break;
                case '{':
                    advance();
                    tokens.push_back({TOKEN_LBRACE, "{", startLine, startCol});
                    break;
                case '}':
                    advance();
                    tokens.push_back({TOKEN_RBRACE, "}", startLine, startCol});
                    break;
                case ',':
                    advance();
                    tokens.push_back({TOKEN_COMMA, ",", startLine, startCol});
                    break;
                default:
                    advance();
                    tokens.push_back({TOKEN_UNKNOWN, string(1, c), startLine, startCol});
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
    map<string, Function> functions;
    map<string, string> builtins;
    vector<string> output;
    int loopIndex;
    
    string getValue(const string& name) {
        if (variables.find(name) != variables.end()) {
            return variables[name];
        }
        return name;
    }
    
    bool isNumber(const string& s) {
        return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
    }
    
    string executeFunction(const string& name, const vector<string>& args) {
        if (name == "print") {
            for (const auto& arg : args) {
                cout << arg;
            }
            cout << endl;
            return "";
        }
        if (name == "length") {
            if (!args.empty()) {
                return to_string(args[0].length());
            }
            return "0";
        }
        if (name == "random") {
            return to_string(rand() % 100);
        }
        return "0";
    }

public:
    Interpreter() : loopIndex(0) {
        // Built-in functions
        builtins["print"] = "print";
        builtins["length"] = "length";
        builtins["random"] = "random";
    }
    
    void execute(const vector<Token>& tokens) {
        int i = 0;
        
        while (i < tokens.size()) {
            Token token = tokens[i];
            
            if (token.type == TOKEN_SAY) {
                i++;
                if (i < tokens.size()) {
                    if (tokens[i].type == TOKEN_STRING) {
                        cout << tokens[i].value << endl;
                    } else if (tokens[i].type == TOKEN_IDENTIFIER) {
                        string varName = tokens[i].value;
                        if (variables.find(varName) != variables.end()) {
                            cout << variables[varName] << endl;
                        }
                    }
                }
                i++;
            }
            else if (token.type == TOKEN_LET) {
                i++;
                if (i < tokens.size() && tokens[i].type == TOKEN_IDENTIFIER) {
                    string varName = tokens[i].value;
                    i++;
                    if (i < tokens.size() && tokens[i].type == TOKEN_ASSIGN) {
                        i++;
                        if (i < tokens.size()) {
                            if (tokens[i].type == TOKEN_STRING || tokens[i].type == TOKEN_NUMBER) {
                                variables[varName] = tokens[i].value;
                                i++;
                            } else if (tokens[i].type == TOKEN_IDENTIFIER) {
                                string valName = tokens[i].value;
                                if (variables.find(valName) != variables.end()) {
                                    variables[varName] = variables[valName];
                                }
                                i++;
                            }
                        }
                    }
                }
            }
            else if (token.type == TOKEN_IF) {
                i++;
                // Simple if - just skip for now
                int braceCount = 0;
                while (i < tokens.size()) {
                    if (tokens[i].type == TOKEN_LBRACE) braceCount++;
                    if (tokens[i].type == TOKEN_RBRACE) {
                        braceCount--;
                        if (braceCount == 0) {
                            i++;
                            break;
                        }
                    }
                    i++;
                }
            }
            else if (token.type == TOKEN_LOOP) {
                i++;
                int count = 0;
                if (i < tokens.size() && tokens[i].type == TOKEN_NUMBER) {
                    count = stoi(tokens[i].value);
                    i++;
                }
                
                if (i < tokens.size() && tokens[i].type == TOKEN_LBRACE) {
                    i++; // Skip {
                    // Execute loop body
                    for (int loop = 0; loop < count; loop++) {
                        int bodyPos = i;
                        while (bodyPos < tokens.size() && tokens[bodyPos].type != TOKEN_RBRACE) {
                            if (tokens[bodyPos].type == TOKEN_SAY) {
                                bodyPos++;
                                if (bodyPos < tokens.size()) {
                                    if (tokens[bodyPos].type == TOKEN_STRING) {
                                        cout << tokens[bodyPos].value << endl;
                                    } else if (tokens[bodyPos].type == TOKEN_IDENTIFIER) {
                                        string varName = tokens[bodyPos].value;
                                        if (varName == "loop_index") {
                                            cout << loop << endl;
                                        } else if (variables.find(varName) != variables.end()) {
                                            cout << variables[varName] << endl;
                                        }
                                    }
                                }
                                bodyPos++;
                            } else {
                                bodyPos++;
                            }
                        }
                    }
                    // Skip to closing brace
                    while (i < tokens.size() && tokens[i].type != TOKEN_RBRACE) {
                        i++;
                    }
                    if (i < tokens.size()) i++; // Skip }
                }
            }
            else if (token.type == TOKEN_FUNC) {
                i++;
                if (i < tokens.size() && tokens[i].type == TOKEN_IDENTIFIER) {
                    string funcName = tokens[i].value;
                    Function func;
                    func.name = funcName;
                    func.isBuiltin = false;
                    functions[funcName] = func;
                    
                    // Skip function body for now
                    while (i < tokens.size() && tokens[i].type != TOKEN_LBRACE) {
                        i++;
                    }
                    if (i < tokens.size()) i++; // Skip {
                    int braceCount = 1;
                    while (i < tokens.size() && braceCount > 0) {
                        if (tokens[i].type == TOKEN_LBRACE) braceCount++;
                        if (tokens[i].type == TOKEN_RBRACE) braceCount--;
                        i++;
                    }
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

// Main
int main(int argc, char* argv[]) {
    // Welcome message
    cout << "╔═══════════════════════════════════════════╗" << endl;
    cout << "║                                           ║" << endl;
    cout << "║    ███╗   ██╗ ██████╗ ██╗   ██╗ █████╗   ║" << endl;
    cout << "║    ████╗  ██║██╔═══██╗██║   ██║██╔══██╗  ║" << endl;
    cout << "║    ██╔██╗ ██║██║   ██║██║   ██║███████║  ║" << endl;
    cout << "║    ██║╚██╗██║██║   ██║╚██╗ ██╔╝██╔══██║  ║" << endl;
    cout << "║    ██║ ╚████║╚██████╔╝ ╚████╔╝ ██║  ██║  ║" << endl;
    cout << "║    ╚═╝  ╚═══╝ ╚═════╝   ╚═══╝  ╚═╝  ╚═╝  ║" << endl;
    cout << "║                                           ║" << endl;
    cout << "║         Nova Language v0.2.0              ║" << endl;
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
        cout << "Nova Language v0.2.0 by MARCELINO MODZ" << endl;
        cout << "Usage: nova <file.nv> [options]" << endl;
        cout << "Options:" << endl;
        cout << "  --help     Show this help" << endl;
        cout << "  --version  Show version" << endl;
        return 0;
    }
    
    if (arg == "--version") {
        cout << "Nova Language v0.2.0 by MARCELINO MODZ" << endl;
        return 0;
    }
    
    // Read file
    ifstream file(arg);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << arg << endl;
        return 1;
    }
    
    string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    // Tokenize
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();
    
    // Execute
    Interpreter interpreter;
    interpreter.execute(tokens);
    
    return 0;
}
