class Interpreter {

    constructor(ast) {

        this.ast = ast;

        this.memory = {};

    }


    run() {


        for (const node of this.ast) {


            if (node.type === "VARIABLE") {


                this.memory[node.name] = node.value;


            }


            if (node.type === "SAY") {


                if (this.memory[node.value]) {

                    console.log(this.memory[node.value]);

                } else {

                    console.log(node.value);

                }


            }


        }


    }


}


module.exports = Interpreter;
