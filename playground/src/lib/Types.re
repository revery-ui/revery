open Revery;
open Revery.UI;

type nodeType =
| View;


type updates =
| RootNode(int)
| NewNode(int, nodeType)
| SetStyle(int, Style.t)
| AddChild(int, int)
| RemoveChild(int, int);
