// Provides: caml_open_sync_raw
function caml_open_sync_raw(fileName) {

    var f = caml_js_from_string(fileName);
    console.log(f);
    var data = new Uint8Array([17, 211, 2]);

    var result = null;

    function reqListener () {
        console.log('loaded');

        var res = new Uint8Array(oReq.response);
        console.dir(res);
    }

    var oReq = new XMLHttpRequest();
    oReq.addEventListener("load", reqListener);
    oReq.responseType = "arraybuffer"
    oReq.open("GET", "binary.dat", true);
    oReq.send();

    // var str = oReq.response;

    var ab = new ArrayBuffer(5);
    var bufView = new Uint8Array(ab);
    var ret = new Uint8Array(ab);

    console.dir(typeof oReq.response);

    console.dir(ret);

    return caml_ba_create_from(ret, null, 0, 0, 0, [ret.length])
}
