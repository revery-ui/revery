open Reglfw;
open Revery_Shaders.Shader;

module Geometry = {
    type t = {
        vertexBuffers: list(VertexBuffer.t),
        indexBuffer: option(IndexBuffer.t),
        vertexCount: int
    };

    let draw = (geometry: t, shader: CompiledShader.t) => {
        /* Attach all vertex buffers that the shader supports */

        let attachBuffer = (vb) => VertexBuffer.attach(vb, shader);
        List.iter(attachBuffer, geometry.vertexBuffers);

        /* If no index buffer, draw full set of vertices */
        /* Otherwise, draw via the index buffer */

        switch (geometry.indexBuffer) {
        | Some(ib) => IndexBuffer.draw(ib)
        | None => Glfw.glDrawArrays(GL_TRIANGLES, 0, geometry.vertexCount)
        }
    };
};

module Builder = {
    type t = {
        vertexBuffers: ref(list(VertexBuffer.t)),
        indexBuffer: ref(option(IndexBuffer.t)),
        vertexCount: ref(int),
    };

    let create = () => {
        let builder: t = {
            vertexBuffers:ref([]),
            indexBuffer: ref(None),
            vertexCount: ref(0),
        };
        builder
    };


    let addVertexChannel = (builder: t, glType: Glfw.glType, elementCount: int, channel: VertexChannel.t, vertices: array(float)) => {
        /* TODO: Make sure we haven't added vertices already! */
        let vb = VertexBuffer.create(glType, elementCount, channel);
        VertexBuffer.setData(vb, vertices);

        let updatedList = List.append(builder.vertexBuffers^, [vb]);
        builder.vertexBuffers := updatedList;

        let vertexCount = Array.length(vertices) / elementCount;

        switch (builder.vertexCount^) {
        | 0 => builder.vertexCount := vertexCount
        | _ => switch (builder.vertexCount^ == vertexCount) {
            | true => ()
            /* TODO: Raise exception */
            | false => print_endline("Vertex count mismatch");
            }
        }
    }
}
