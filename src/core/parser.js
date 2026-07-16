class Parser {

    constructor(tokens) {
        this.tokens = tokens;
        this.position = 0;
    }


    parse() {

        const statements = [];


        while (this.position < this.tokens.length) {

            let token = this.tokens[this.position];


            // variável
            if (token.type === "LET") {

                this.position++;

                const name = this.tokens[this.position].value;

                this.position++;

                this.position++; // pula o =


                const value = this.tokens[this.position].value;


                statements.push({

                    type: "VARIABLE",

                    name,

                    value

                });


                this.position++;


            }


            // say
            else if (token.type === "SAY") {

                this.position++;


                const value = this.tokens[this.position].value;


                statements.push({

                    type: "SAY",

                    value

                });


                this.position++;

            }


            else {

                this.position++;

            }

        }


        return statements;

    }

}


module.exports = Parser;
