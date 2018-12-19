/* Mouse Input */
open Revery_Math;

open NodeEvents;

module Cursor {

    /* State needed to track on the cursor */
    type t = {
        x: ref(float),
        y: ref(float),
    };

    let make = () => {
        let ret: t = {
            x: ref(0.),
            y: ref(0.),
        };
        ret;
    };
}

let getPositionFromMouseEvent = (evt: mouseEvent) => {
    switch (evt) {
    | MouseDown(c) => Vec2.create(c.mouseX, c.mouseY)
    | MouseMove(c) => Vec2.create(c.mouseX, c.mouseY)
    | MouseUp(c) => Vec2.create(c.mouseX, c.mouseY)
    }
}

let dispatch = (_cursor: Cursor.t, evt: mouseEvent, node: Node.node('a)) => {
   let pos =getPositionFromMouseEvent(evt);

   let isNodeImpacted = (n) => n#hitTest(pos); 

   let nodes: ref(list(Node.node('a))) = ref([]);
   let collect = (n) => {
       if (isNodeImpacted(n)) {
        nodes := List.append(nodes^, [n]);
       }
   };

   Node.iter(collect, node);

   print_endline ("dispatch - " ++ string_of_int(List.length(nodes^)) ++ " impacted");

   List.iter((n) => n#handleEvent(evt), nodes^);
};

