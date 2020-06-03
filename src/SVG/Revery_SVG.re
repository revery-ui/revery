module Model = Model;

let fromFile = path => SimpleXml.fromFile(path) |> Option.map(Parser.svg);

let fromString = str => SimpleXml.fromString(str) |> Option.map(Parser.svg);

let render = SkiaRenderer.render;
