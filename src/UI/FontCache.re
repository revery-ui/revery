
type fontInfo = (string, int);
type t = Hashtbl.t(fontInfo, Fontkit.fk_face);

let _cache: t = Hashtbl.create(100);

let load = (fontName: string, size: int) => {
    switch (Hashtbl.find_opt(_cache, (fontName, size))) {
    | Some(fk) => fk
    | None => 
        print_endline ("Loading cached font: " ++ fontName);
        let fk = Fontkit.load(fontName, size)
        Hashtbl.add(_cache, (fontName, size), fk);
        fk
    }
}
