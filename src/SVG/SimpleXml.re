[@deriving show({with_path: false})]
type t =
  | Element(string, list((string, string)), list(t))
  | Text(string);

let trim =
  Markup.filter(
    fun
    | `Text(strs) => strs |> String.concat("") |> String.trim != ""
    | _ => true,
  );

let simplify = stream =>
  stream
  |> trim
  |> Markup.tree(
       ~text=strings => Text(strings |> String.concat("")),
       ~element=
         ((_ns, name), attrs, children) => {
           let attrs =
             attrs |> List.map((((_ns, name), value)) => (name, value));
           Element(name, attrs, children);
         },
     );

let fromFile = path =>
  Markup.file(path) |> fst |> Markup.parse_xml |> Markup.signals |> simplify;

let fromString = str =>
  Markup.string(str) |> Markup.parse_xml |> Markup.signals |> simplify;
