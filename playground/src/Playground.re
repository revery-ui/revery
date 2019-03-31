/* open Revery; */
/* ignore(Revery.App.start); */

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

let postfix = "\nPlaygroundLib.Worker.setRenderFunction(render);";

let execute2: Js.t(Js.js_string) => Js.t(Js.js_string) = code => {
  let code = Js.to_string(code) ++ postfix;
  let buffer = Buffer.create(100);
  let formatter = Format.formatter_of_buffer(buffer);
  JsooTop.execute(true, formatter, code);
  let result = Buffer.contents(buffer)
  Js.string(result);
};

let reasonSyntax = () => {
  open Reason_toolchain.From_current;
  let wrap = (f, g, fmt, x) => g(fmt, f(x));
  Toploop.parse_toplevel_phrase :=
    Reason_util.correctly_catch_parse_errors(x =>
      Reason_toolchain.To_current.copy_toplevel_phrase(
        Reason_toolchain.RE.toplevel_phrase(x),
      )
    );
  Toploop.parse_use_file :=
    Reason_util.correctly_catch_parse_errors(x =>
      List.map(
        Reason_toolchain.To_current.copy_toplevel_phrase,
        Reason_toolchain.RE.use_file(x),
      )
    );
  Toploop.print_out_value :=
    wrap(copy_out_value, Reason_oprint.print_out_value);
  Toploop.print_out_type := wrap(copy_out_type, Reason_oprint.print_out_type);
  Toploop.print_out_class_type :=
    wrap(copy_out_class_type, Reason_oprint.print_out_class_type);
  Toploop.print_out_module_type :=
    wrap(copy_out_module_type, Reason_oprint.print_out_module_type);
  Toploop.print_out_type_extension :=
    wrap(copy_out_type_extension, Reason_oprint.print_out_type_extension);
  Toploop.print_out_sig_item :=
    wrap(copy_out_sig_item, Reason_oprint.print_out_sig_item);
  Toploop.print_out_signature :=
    wrap(List.map(copy_out_sig_item), Reason_oprint.print_out_signature);
  Toploop.print_out_phrase :=
    wrap(copy_out_phrase, Reason_oprint.print_out_phrase);
};

let _ = {
  reasonSyntax();
  JsooTop.initialize();
  /* PlaygroundLib.startPlayground(); */
};

/* let startRenderer = PlaygroundLib.Renderer.start; */
/* let updateRenderer = PlaygroundLib.Renderer.update; */
/* let startWorker = PlaygroundLib.Worker.start; */

let () = Js.export_all(
  [%js {
    val execute = execute;
    val execute2 = execute2;
    /* val startRenderer = startRenderer; */
    /* val updateRenderer = updateRenderer; */
  }]
);
