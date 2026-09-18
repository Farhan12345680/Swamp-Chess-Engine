import createSwamp from "./swampGreedy.js";

let Module = null;

const modulePromise = createSwamp();

self.onmessage = async (event) => {
    const { id, type, data } = event.data;

    try {
        Module = await modulePromise;

        let result;

        switch (type) {

            case "initialize":
                Module._init();
                result = true;
                break;

            case "initFromFen": {
                const size = Module.lengthBytesUTF8(data) + 1;
                const ptr = Module._malloc(size);

                Module.stringToUTF8(data, ptr, size);
                Module._initFromFen(ptr);

                Module._free(ptr);

                result = true;
                break;
            }

            case "doMove":
                Module._doMove(data);
                result = true;
                break;

            case "search": {
                const ptr = Module._goSearchNextBestMove();
                const move = Module.UTF8ToString(ptr);

                Module._free(ptr);

                result = move;
                break;
            }

            case "isValidMove":
                result = Module._isValidMove(data) !== 0;
                break;

            case "getFen": {
                const ptr = Module._getFenFromBoard();
                result = Module.UTF8ToString(ptr);
                break;
            }

            default:
                throw new Error(`Unknown command: ${type}`);
        }

        self.postMessage({
            id,
            success: true,
            result
        });

    } catch (err) {

        self.postMessage({
            id,
            success: false,
            error: err.message
        });
    }
};
