print_endline ("Hello, world");

/* open Revery; */
/* open Revery.Draw; */
/* open Revery.Math; */
/* open Revery.UI; */
/* open Js_of_ocaml; */
/* open PlaygroundLib; */
/* open PlaygroundLib.Types; */


/* let rootNode: ref(option(viewNode)) = ref(None); */

/* let createNode = (nodeType) => switch(nodeType) { */
/* | View => (new viewNode)() */
/* /1* | _ => (new viewNode)() *1/ */
/* }; */

/* let idToNode: Hashtbl.t(int, node) = Hashtbl.create(100); */

/* let nodeFromId = (id: int) => { */
/*     let item = Hashtbl.find_opt(idToNode, id); */
/*     switch (item) { */
/*     | Some(v) => v */
/*     | None => failwith("Cannot find node: " ++ string_of_int(id)) */
/*     } */
/* }; */

/* let visitUpdate = u => switch(u) { */
/*     | RootNode(id) => { */
/*         print_endline ("update: rootnode: " ++string_of_int(id)); */
/*         let node = nodeFromId(id); */
/*         rootNode := Obj.magic(node); */
        
/*     } */
/*     | NewNode(id, nodeType) => { */
/*         print_endline ("update: newnode: " ++string_of_int(id)); */
/*         let node = createNode(nodeType); */
/*         Hashtbl.add(idToNode, id, node); */
/*     } */
/*     | SetStyle(id, style) => { */
/*         print_endline ("update: setstyle: " ++string_of_int(id)); */
/*         /1* let node = nodeFromId(id); *1/ */
/*         /1* node#setStyle(style); *1/ */
/*     } */
/*     /1* | AddChild(parentId, childId) => { *1/ */
/*     /1*     print_endline ("update: addchild: " ++string_of_int(parentId)); *1/ */
/*     /1*    let parentNode = nodeFromId(parentId); *1/ */ 
/*     /1*    let childNode = nodeFromId(childId); *1/ */
/*     /1*    parentNode#addChild(childNode); *1/ */
/*     /1* } *1/ */
/*     /1* | RemoveChild(parentId, childId) => { *1/ */
/*     /1*     print_endline ("update: removechild: " ++string_of_int(parentId)); *1/ */
/*     /1*    let parentNode = nodeFromId(parentId); *1/ */ 
/*     /1*    let childNode = nodeFromId(childId); *1/ */
/*     /1*    parentNode#removeChild(childNode); *1/ */
/*     /1* } *1/ */
/*     | _ => () */
/*     }; */

/* let update = (v: list(updates)) => { */
/*     List.iter(visitUpdate, v); */
/* }; */

/* let start = () => { */
/*     let init = app => { */

/*         let win = */ 
/*             App.createWindow(app, "Welcome to Revery", ~createOptions={ */
/*                 ...Window.defaultCreateOptions, */
/*                 maximized: true, */
/*             }); */


/*         let _projection = Mat4.create(); */

/*         let render = () => switch(rootNode^) { */
/*         | None => () */
/*         | Some(rootNode) => { */

/*             let size = Window.getSize(win); */
/*             let pixelRatio = Window.getDevicePixelRatio(win); */
/*             let scaleFactor = Window.getScaleFactor(win); */
/*             let adjustedHeight = size.height / scaleFactor; */
/*             let adjustedWidth = size.width / scaleFactor; */

/*             print_endline ("rendering - width: " ++ string_of_int(adjustedWidth) ++ " | height: " ++ string_of_int(adjustedHeight)); */

/*             rootNode#setStyle( */
/*                 Style.make( */
/*                     ~position=LayoutTypes.Relative, */
/*                     ~width=adjustedWidth, */
/*                     ~height=adjustedHeight, */
/*                     (), */
/*                 ), */
/*             ); */

/*             let layoutNode = rootNode#toLayoutNode(~force=false, ()); */
/*             Layout.printCssNode(layoutNode); */

/*             /1* Layout.layout(~force=true, rootNode); *1/ */
/*             /1* rootNode#recalculate(); *1/ */
/*             /1* rootNode#flushCallbacks(); *1/ */

/*             /1* Mat4.ortho( *1/ */
/*             /1*     _projection, *1/ */
/*             /1*     0.0, *1/ */
/*             /1*     float_of_int(adjustedWidth), *1/ */
/*             /1*     float_of_int(adjustedHeight), *1/ */
/*             /1*     0.0, *1/ */
/*             /1*     1000.0, *1/ */
/*             /1*     -1000.0, *1/ */
/*             /1* ); *1/ */

/*             /1* let drawContext = NodeDrawContext.create(~zIndex=0, ~opacity=1.0, ()); *1/ */

/*             /1* /2* Render all geometry that requires an alpha *2/ *1/ */
/*             /1* RenderPass.startAlphaPass( *1/ */
/*             /1*     ~pixelRatio, *1/ */
/*             /1*     ~scaleFactor, *1/ */
/*             /1*     ~screenHeight=adjustedHeight, *1/ */
/*             /1*     ~screenWidth=adjustedWidth, *1/ */
/*             /1*     ~projection=_projection, *1/ */
/*             /1*     (), *1/ */
/*             /1* ); *1/ */
/*             /1* rootNode#draw(drawContext); *1/ */
/*             /1* RenderPass.endAlphaPass(); *1/ */

/*             /1* (renderFunction^)(); *1/ */
/*         } */
/*         }; */

/*         Window.setRenderCallback(win, render); */
/*         Window.setShouldRenderCallback(win, () => true); */

/*         Window.maximize(win); */

/*         /1* UI.start(win, render); *1/ */
/*     }; */

/*     App.start(init); */
/* }; */


/* let () = start(); */
