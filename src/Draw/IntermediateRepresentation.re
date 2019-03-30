/*
 * IntermediateRepresentation.re
 *
 * Typing for API calls, such that the Nodes can issue a set of GFX commands,
 * and then have the actual rendering happen in another thread.
 */

/*
 * TODO: Text measurement?
 */

module DrawStringOptions {
    type t = {
        fontFamily: string,
        fontSize: string,
        gamma: float,
        color: Color.t,
        backgroundColor: Color.t,
        opacity: float,
        transform: Mat4.t,
        x: float,
        y: float,
        text: string,
    }
}

module DrawRectOptions {
    type t = {
        transform: Mat4.t,
        x: float,
        y: float,
        width: float,
        height: float,
        color: float,
    }
}

module DrawImageOptions {
    type t = {
       imagePath: string 
       transform: Mat4.t,
       x: float,
       y: float,
       width: float,
       height: float,
       opacity: float,
    }
}

type leaves =
| SetViewport(int, int, int, int)
| SetDepthTestEnabled(bool)
| Clear(float, float, float, float)
| DrawString(DrawStringOptions.t) 
| DrawRect(DrawRectOptions.t)
| DrawImage(DrawImageOptions.t)

type nodes =
| Scissor(int, int, int, int);

type renderTree = 
| Leaf(leaves)
| Node(nodes, list(renderTree));
