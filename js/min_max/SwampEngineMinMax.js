const worker = new Worker(
    new URL("./SwampEngineWorkerMinMax.js", import.meta.url),
    {
        type: "module"
    }
);

let requestId = 0;

const pendingRequests = new Map();

worker.addEventListener("message", (event) => {

    const { id, success, result, error } = event.data;

    const request = pendingRequests.get(id);

    if (!request) {
        return;
    }

    pendingRequests.delete(id);

    if (success) {
        request.resolve(result);
    } else {
        request.reject(new Error(error));
    }
});


function sendRequest(type, data = null) {

    return new Promise((resolve, reject) => {

        const id = requestId++;

        pendingRequests.set(id, {
            resolve,
            reject
        });

        worker.postMessage({
            id,
            type,
            data
        });
    });
}


export function initialize() {
    return sendRequest("initialize");
}


export function initFromFen(fenString) {
    return sendRequest("initFromFen", fenString);
}


export function doMove(move) {
    return sendRequest("doMove", move);
}


export function goSearchNextBestMove() {
    return sendRequest("search");
}


export function isValidMove(move) {
    return sendRequest("isValidMove", move);
}


export function getFenStringFromGame() {
    return sendRequest("getFen");
}
