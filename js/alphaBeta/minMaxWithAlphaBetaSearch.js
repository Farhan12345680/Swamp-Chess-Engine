import {
    initialize,
    initFromFen,
    doMove,
    isValidMove,
    goSearchNextBestMove,
    getFenStringFromGame
} from "./SwampEngineMinMaxAlphaBeta.js";


const START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


const pieceImages = {
    wp: "./images/wp.png",
    wr: "./images/wr.png",
    wn: "./images/wn.png",
    wb: "./images/wb.png",
    wq: "./images/wq.png",
    wk: "./images/wk.png",

    bp: "./images/bp.png",
    br: "./images/br.png",
    bn: "./images/bn.png",
    bb: "./images/bb.png",
    bq: "./images/bq.png",
    bk: "./images/bk.png"
};


const fenPieces = {
    P: "wp",
    R: "wr",
    N: "wn",
    B: "wb",
    Q: "wq",
    K: "wk",

    p: "bp",
    r: "br",
    n: "bn",
    b: "bb",
    q: "bq",
    k: "bk"
};


let selectedSrc = "";
let selectedPiece = "";
let engineThinking = false;
let humanSide = "w";


class GameNode {
    constructor(fenString) {
        this.fenString = fenString;
        this.prev = null;
        this.next = null;
    }
}


let currentNode = null;


function createPiece(pieceName) {

    const child = document.createElement("img");

    child.src = pieceImages[pieceName];

    return child;
}


function renderFen(fenString) {

    const board = fenString.split(" ")[0];
    const ranks = board.split("/");

    document.querySelectorAll(".square").forEach(e => {
        e.innerHTML = "";
        e.piece = "es";
        e.classList.remove("selected");
    });

    for (let rank = 7; rank >= 0; rank--) {

        const fenRank = ranks[7 - rank];

        let file = 0;

        for (const character of fenRank) {

            if (character >= "1" && character <= "8") {
                file += Number(character);
                continue;
            }

            const pos =
                String.fromCharCode("a".charCodeAt(0) + file) +
                (rank + 1);

            const square = [...document.querySelectorAll(".square")]
                .find(e => e.pos === pos);

            const pieceName = fenPieces[character];

            square.piece = pieceName;
            square.appendChild(createPiece(pieceName));

            file++;
        }
    }
}


function createInitialChessGame() {
    renderFen(START_FEN);
}


function squareToIndex(square) {

    const file = square.charCodeAt(0) - "a".charCodeAt(0);
    const rank = Number(square.charAt(1)) - 1;

    return rank * 8 + file;
}


function encodeMove(moveString, pieceName) {

    const src = squareToIndex(moveString.substring(0, 2));
    const dest = squareToIndex(moveString.substring(2, 4));

    let promotion = 0;

    if (moveString.length === 5) {

        if (moveString[4] === "q") {
            promotion = 1;
        }
        else if (moveString[4] === "r") {
            promotion = 2;
        }
        else if (moveString[4] === "b") {
            promotion = 3;
        }
        else if (moveString[4] === "n") {
            promotion = 4;
        }
    }
    else if (
        pieceName === "wp" &&
        moveString.charAt(3) === "8"
    ) {
        promotion = 1;
    }
    else if (
        pieceName === "bp" &&
        moveString.charAt(3) === "1"
    ) {
        promotion = 1;
    }

    return src | (dest << 6) | (promotion << 12);
}


function clearSelection() {

    document.querySelectorAll(".square").forEach(e => {
        e.classList.remove("selected");
    });

    selectedSrc = "";
    selectedPiece = "";
}


function getSquarePieceColor(pieceName) {

    if (!pieceName || pieceName === "es") {
        return null;
    }

    if (pieceName.charAt(0) === "w") {
        return "w";
    }

    return "b";
}


async function squareClick(e) {

    if (engineThinking) {
        return;
    }

    const square = e.currentTarget;
    const destination = square.pos;
    const destinationPiece = square.piece;

    if (!selectedSrc) {

        if (!destinationPiece || destinationPiece === "es") {
            return;
        }

        const pieceColor = getSquarePieceColor(destinationPiece);

        if (pieceColor !== humanSide) {
            return;
        }

        selectedSrc = destination;
        selectedPiece = destinationPiece;

        square.classList.add("selected");

        return;
    }

    if (destination === selectedSrc) {
        clearSelection();
        return;
    }

    const destinationColor = getSquarePieceColor(destinationPiece);

    if (destinationColor === humanSide) {

        clearSelection();

        selectedSrc = destination;
        selectedPiece = destinationPiece;

        square.classList.add("selected");

        return;
    }

    const moveString = selectedSrc + destination;
    const move = encodeMove(moveString, selectedPiece);

    try {

        const valid = await isValidMove(move);

        if (!valid) {
            clearSelection();
            return;
        }

        engineThinking = true;

        clearSelection();

        await doMove(move);

        const humanFen = await getFenStringFromGame();

        addGameNode(humanFen);

        renderFen(humanFen);
        console.log(humanFen);
        const engineMove = await goSearchNextBestMove();
        
        if (!engineMove || engineMove === "0000") {
            return;
        }

        const engineEncodedMove = encodeMove(
            engineMove,
            null
        );

        await doMove(engineEncodedMove);

        const engineFen = await getFenStringFromGame();

        addGameNode(engineFen);

        renderFen(engineFen);

    }
    catch (err) {

        console.log(err);

    }
    finally {

        engineThinking = false;
        clearSelection();
    }
}


function add_functionality() {

    document.querySelectorAll(".square").forEach(e => {

        e.style.cursor = "pointer";

        e.addEventListener("click", squareClick);
    });
}


function addGameNode(fenString) {

    const node = new GameNode(fenString);

    if (!currentNode) {
        currentNode = node;
        return;
    }

    currentNode.next = node;
    node.prev = currentNode;
    currentNode = node;
}


async function loadFen(fenString) {

    await initFromFen(fenString);

    renderFen(fenString);
}


async function restartGame() {

    if (engineThinking) {
        return;
    }

    engineThinking = true;

    try {

        humanSide = "w";

        await initialize();
        await initFromFen(START_FEN);

        currentNode = new GameNode(START_FEN);

        clearSelection();
        renderFen(START_FEN);

    }
    catch (err) {

        console.log(err);

    }
    finally {

        engineThinking = false;
        clearSelection();
    }
}


async function previousPosition() {

    if (engineThinking || !currentNode || !currentNode.prev) {
        return;
    }

    currentNode = currentNode.prev;

    clearSelection();

    await loadFen(currentNode.fenString);
}


async function nextPosition() {

    if (engineThinking || !currentNode || !currentNode.next) {
        return;
    }

    currentNode = currentNode.next;

    clearSelection();

    await loadFen(currentNode.fenString);
}


createInitialChessGame();
add_functionality();


const restart = document.querySelector("#restart");
const prev = document.querySelector("#prev");
const next = document.querySelector("#next");
const playBlack = document.querySelector("#playBlack");
const playWhite = document.querySelector("#playWhite");


restart.addEventListener("click", restartGame);
prev.addEventListener("click", previousPosition);
next.addEventListener("click", nextPosition);


await initialize();
await initFromFen(START_FEN);

currentNode = new GameNode(START_FEN);

renderFen(START_FEN);


playBlack.onclick = async () => {

    if (engineThinking) {
        return;
    }
    await restartGame();

    try {

        engineThinking = true;
        humanSide = "b";

        const engineMove = await goSearchNextBestMove();

        if (!engineMove || engineMove === "0000") {
            return;
        }

        const engineEncodedMove = encodeMove(
            engineMove,
            null
        );

        await doMove(engineEncodedMove);

        const engineFen = await getFenStringFromGame();

        addGameNode(engineFen);

        renderFen(engineFen);

    }
    catch (err) {

        console.log(err);

    }
    finally {

        engineThinking = false;
        clearSelection();
    }
};


playWhite.onclick = async () => {
    await restartGame();
};