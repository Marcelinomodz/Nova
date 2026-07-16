class Lexer {

    constructor(code) {
        this.code = code;
        this.position = 0;
        this.tokens = [];
    }


    tokenize() {

        while (this.position < this.code.length) {

            let char = this.code[this.position];


            // Ignorar espaços
            if (/\s/.test(char)) {
                this.position++;
                continue;
            }


            // Texto entre aspas
            if (char === '"') {

                this.position++;

                let value = "";

                while (this.code[this.position] !== '"') {

                    value += this.code[this.position];
                    this.position++;

                }

                this.position++;

                this.tokens.push({
                    type: "STRING",
                    value
                });

                continue;
            }


            // Palavras
            if (/[a-zA-Z]/.test(char)) {

                let word = "";

                while (
                    /[a-zA-Z]/.test(this.code[this.position])
                ) {

                    word += this.code[this.position];
                    this.position++;

                }


                this.tokens.push({
    type: word.toUpperCase(),
    value: word
});

                continue;
            }


            this.position++;

        }


        return this.tokens;

    }

}


module.exports = Lexer;
