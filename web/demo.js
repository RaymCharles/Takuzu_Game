var canvas = document.getElementById('mycanvas');
var restartButton = document.getElementById("restart");
var undoButton = document.getElementById("undo");
var redoButton = document.getElementById("redo");
var solveButton = document.getElementById("solve");

var immutable_zero = new Image();
immutable_zero.src = "annexe/panpan3.png"
var zero = new Image();
zero.src = "annexe/panpan3.png"

var error_zero = new Image();
error_zero.src = "annexe/panpansad.png"

var immutable_one = new Image();
immutable_one.src = "annexe/bambi2.png"
var one = new Image();
one.src = "annexe/bambi2.png"

var error_one = new Image();
error_one.src = "annexe/bambisad.png"


const SquareImages = [];

var ctx = canvas.getContext('2d');
var width_canvas = canvas.width;
var height_canvas = canvas.height;
var size_grid = 6;   //start with a 6 size grid       
var size_square = canvas.width / size_grid;
var g;




function start() {
    Module.onRuntimeInitialized = () => { start(); }
    console.log("window load");
    drawCanvas();
    //g = Module._new_random(size_grid,size_grid,false,false,false);
    g = Module._new_default();
    initCanvas(g);
}


function choiceSize() {
    var select = document.getElementById("choicesize");
    var selectedOption = select.options[select.selectedIndex];
    var size = selectedOption.value;
    size_grid = size;
    //g = Module._new_random(size_grid,size_grid,false,false,false);
    g = Module._new_default();
    size_square = width_canvas / size_grid;
    initCanvas(g);
    printGame(g);
  }  

function drawCanvas() { 
    for (let i = 0; i < size_grid; i++) {
        for (let j = 0; j < size_grid; j++) {
          const x = i * size_square;
          const y = j * size_square;
          ctx.strokeRect(x, y, size_square, size_square); 
        }
    }
}


function initCanvas(g){
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var number = Module._get_number(g, row, col);
            var immutable = Module._is_immutable(g, row, col);
            var empty = Module._is_empty(g, row, col);
            var has_error = Module._has_error(g, row, col);
            const x = col * size_square;
            const y = row * size_square;
            const squareIndex = row * size_grid + col;
            if (empty){
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                SquareImages[squareIndex] = 1;
                drawCanvas();
            }
            else if (immutable && number == 0) { 
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(immutable_zero, x, y, size_square, size_square);
                SquareImages[squareIndex] = immutable_zero;
                drawCanvas();
            }
            else if (immutable && number == 1) {
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(immutable_one, x, y, size_square, size_square);
                SquareImages[squareIndex] = immutable_one;
                drawCanvas();
            }
            else if ((one||immutable_one) && has_error ) {
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(immutable_one, x, y, size_square, size_square);
                SquareImages[squareIndex] = error_one;
                drawCanvas();
            }
            else if ((zero||immutable_zero) && has_error ) {
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(immutable_one, x, y, size_square, size_square);
                SquareImages[squareIndex] = error_zero;
                drawCanvas();
            }
        }
    }
}


function update(g){
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var number = Module._get_number(g, row, col);
            var immutable = Module._is_immutable(g, row, col);
            var empty = Module._is_empty(g, row, col);
            var has_error = Module._has_error(g, row , col);
            const x = col * size_square;
            const y = row * size_square;
            const squareIndex = row * size_grid + col;

            if (empty){
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                SquareImages[squareIndex] = 1;
                drawCanvas();
            }


            else if (has_error && !immutable && number == 0) { 
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(error_zero, x, y, size_square, size_square);
                SquareImages[squareIndex] = error_zero;
                drawCanvas();
            }
            else if (has_error && !immutable && number == 1) { 
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(error_one, x, y, size_square, size_square);
                SquareImages[squareIndex] = error_one;
                drawCanvas();
            }



            else if (immutable && number == 0) { 
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(immutable_zero, x, y, size_square, size_square);
                SquareImages[squareIndex] = immutable_zero;
                drawCanvas();
            }
            else if (immutable && number == 1) {
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(immutable_one, x, y, size_square, size_square);
                SquareImages[squareIndex] = immutable_one;
                drawCanvas();
            }
            else if (!immutable && number == 0) {  
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(zero, x, y, size_square, size_square);
                SquareImages[squareIndex] = zero;
                drawCanvas();
            }
            else if (!immutable && number == 1) {
                ctx.fillStyle = "#FFFFFF";
                ctx.fillRect(x, y, size_square, size_square);
                ctx.drawImage(one, x, y, size_square, size_square);
                SquareImages[squareIndex] = one;
                drawCanvas();
            }
        }
    }
}


let test1 = false;
let test2 = false;

Module.onRuntimeInitialized = () => {
  test1 = true;
  checkFlags();
};

window.addEventListener('load', () => {
  test2 = true;
  checkFlags();
});

function checkFlags() {
  if (test1 && test2) {
    start();
  }
}


canvas.addEventListener('click', canvasLeftClic);
restartButton.addEventListener("click", restart);
undoButton.addEventListener("click",undo);
redoButton.addEventListener("click",redo);
solveButton.addEventListener("click",solve);


function canvasLeftClic(event) { 
    const squareX = Math.floor(event.offsetX / size_square);
    const squareY = Math.floor(event.offsetY / size_square);
    const bambiX = squareX * size_square;
    const bambiY = squareY * size_square;
    const squareIndex = squareY * size_grid + squareX;

    if (SquareImages[squareIndex] == 1){//Empty -> panpan
        ctx.fillStyle = "#FFFFFF";
        ctx.fillRect(bambiX, bambiY, size_square, size_square);
        ctx.drawImage(zero, bambiX, bambiY, size_square, size_square);
        drawCanvas();
        SquareImages[squareIndex] = zero;
        Module._play_move(g,squareY,squareX,1);
        printGame(g);
        update(g);
        checkingError(squareY,squareX);
    }
    else if (SquareImages[squareIndex] == zero){ 
        ctx.fillStyle = "#FFFFFF";
        ctx.fillRect(bambiX, bambiY, size_square, size_square);
        ctx.drawImage(one, bambiX, bambiY, size_square, size_square);
        drawCanvas();
        SquareImages[squareIndex] = one; //panpan -> bambi
        Module._play_move(g,squareY,squareX,2);
        printGame(g);
        update(g);
        checkingError(squareY,squareX);
    }
    else if (SquareImages[squareIndex] == error_zero){//errpr_panpan -> bambi
        ctx.fillStyle = "#FFFFFF";
        ctx.fillRect(bambiX, bambiY, size_square, size_square);
        ctx.drawImage(one, bambiX, bambiY, size_square, size_square);
        drawCanvas();
        SquareImages[squareIndex] = one; //panpan -> bambi
        Module._play_move(g,squareY,squareX,2);
        printGame(g);
        update(g);
        checkingError(squareY,squareX);
    }
    else if (SquareImages[squareIndex] == one){ //bambi -> empty
        ctx.fillStyle = "#FFFFFF";
        ctx.fillRect(bambiX, bambiY, size_square, size_square);
        drawCanvas();
        SquareImages[squareIndex] = 1;
        Module._play_move(g,squareY,squareX,0);
        printGame(g);
        update(g);
        checkingError(squareY,squareX);
    }
    else if (SquareImages[squareIndex] == error_one){ //errpr_bambi -> empty
        ctx.fillStyle = "#FFFFFF";
        ctx.fillRect(bambiX, bambiY, size_square, size_square);
        drawCanvas();
        SquareImages[squareIndex] = 1;
        Module._play_move(g,squareY,squareX,0);
        printGame(g);
        update(g);
        checkingError(squareY,squareX);
    }
    
    gameOver(g);
}


function gameOver (g){
    var over = Module._is_over(g);
    if(over){
        document.getElementById("victory").textContent = "Victoire";
        console.log("game is over");
    } else {
        document.getElementById("victory").textContent = "";
    }
}


function checkingError(){ 
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    for (var x = 0; x < nb_rows; x++) {
        for (var y = 0; y < nb_cols; y++) {
            var hasError = Module._has_error(g,x,y);
            const squareIndex = x * size_grid + y;
            if (hasError){
                ctx.fillStyle = "#FF0000";
                ctx.fillRect(y*size_square, x*size_square, size_square, size_square);
                ctx.drawImage(SquareImages[squareIndex],y*size_square, x*size_square, size_square, size_square);
            }
        }
    }
}


function restart(){
    Module._restart(g);
    initCanvas(g);
    document.getElementById("victory").textContent = "";
}

function undo(){
    Module._undo(g);
    update(g);
}

function redo(){
    Module._redo(g);
    update(g);
}

function solve(){
    Module._solve(g);
    update(g);
    gameOver(g);
}

function printGame(gaming) {
    var text = "";
    var nb_rows = Module._nb_rows(gaming);
    var nb_cols = Module._nb_cols(gaming);
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var number = Module._get_number(gaming, row, col);
            var immutable = Module._is_immutable(gaming, row, col);
            var empty = Module._is_empty(gaming, row, col);
            if (empty)
                text += " ";
            else if (immutable && number == 0)
                text += "W";
            else if (immutable && number == 1)
                text += "B";
            else if (number == 0)
                text += "w";
            else if (number == 1)
                text += "b";
            else text += "?";
        }
        text += "\n";
    }
    console.log(text);
}
// EOF