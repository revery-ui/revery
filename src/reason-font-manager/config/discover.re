module Configurator = Configurator.V1;

type feature = {
  flags: list(string),
  c_flags: list(string),
  cxx_flags: list(string),
  test: Configurator.t => bool,
};

let root = Sys.getenv("cur__root");

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];

let check_headers = (headers, conf) => {
  let code = String.concat("\n", headers);
  Configurator.c_test(conf, code ++ "\nint main() { return 0; }");
};

let feature = {test: _ => true, flags: [], c_flags: [], cxx_flags: []};
let apple_core_text = {
  ...feature,
  flags:
    []
    @ ccopt("-framework CoreText")
    @ ccopt("-framework Foundation")
    @ cclib("-lstdc++"),
  cxx_flags: ["-x", "objective-c++", "-lstdc++"],
  test:
    check_headers([
      "#include <Foundation/Foundation.h>",
      "#include <CoreText/CoreText.h>",
    ]),
};
let linux_fontconfig = {
  ...feature,
  flags: [] @ cclib("-lfontconfig") @ cclib("-lstdc++"),
  cxx_flags: ["-fPIC", "-lstdc++"],
  test: check_headers(["#include <fontconfig/fontconfig.h>"]),
};
let windows_direct_write = {
  ...feature,
  flags: [] @ cclib("-ldwrite") @ cclib("-lstdc++"),
  cxx_flags: ["-fno-exceptions", "-fno-rtti", "-lstdc++"],
  test: check_headers(["#include <dwrite.h>", "#include <dwrite_1.h>"]),
};

let generate_configs = conf => {
  let has_core_text = apple_core_text.test(conf);
  let has_fontconfig = linux_fontconfig.test(conf);
  let has_direct_write = windows_direct_write.test(conf);

  let (define, {flags, c_flags, cxx_flags, _}) =
    switch (has_core_text, has_fontconfig, has_direct_write) {
    | (true, _, _) => ("USE_APPLE_CORE_TEXT", apple_core_text)
    | (_, true, _) => ("USE_LINUX_FONTCONFIG", linux_fontconfig)
    | (_, _, true) => ("USE_WINDOWS_DIRECT_WRITE", windows_direct_write)
    | _ => failwith("missing a font manager")
    };

  Configurator.C_define.gen_header_file(
    ~fname="config.h",
    conf,
    [(define, Switch(true))],
  );
  Configurator.Flags.write_sexp("c_flags.sexp", c_flags);
  Configurator.Flags.write_sexp("cxx_flags.sexp", cxx_flags);
  Configurator.Flags.write_sexp("flags.sexp", flags);
};
Configurator.main(~name="reason-font-manager", generate_configs);
