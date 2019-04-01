open Revery;
open Revery.UI;

type nodeType =
  | View
  | Text
  | Image;

type updates =
  | RootNode(int)
  | NewNode(int, nodeType)
  | SetStyle(int, Style.t)
  | AddChild(int, int)
  | RemoveChild(int, int)
  | SetText(int, string)
  | SetImageSrc(int, string);

let show = (u: updates) => {
  switch (u) {
  | RootNode(_) => "rootnode"
  | NewNode(_) => "newnode"
  | _ => "Unknown"
  };
};

let showAll = (u: list(updates)) => {
  List.iter(v => print_endline("- " ++ show(v)), u);
};
