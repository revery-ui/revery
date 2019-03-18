open Js_of_ocaml;
open Js_of_ocaml_toplevel;

let execute: Js.t(Js.js_string) => Js.t(Js.js_string) = code => {
  let code = Js.to_string(code);
  let buffer = Buffer.create(100);
  let formatter = Format.formatter_of_buffer(buffer);
  JsooTop.execute(true, formatter, code);
  let result = Buffer.contents(buffer)
  Js.string(result);
};


JsooTop.initialize();
let () = Js.export_all(
  [%js {
    val execute = execute
  }]
);
