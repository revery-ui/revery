open Reglfw;
open Reglfw.Glfw;
open Reglm;

open Types;

type attributeNameToLocation = Hashtbl.t(string, attribLocation);
type attributeChannelToLocation = Hashtbl.t(VertexChannel.t, attribLocation);
type uniformNameToLocation = Hashtbl.t(string, uniformLocation);

type t = (
  list(ShaderUniform.t),
  list(ShaderAttribute.t),
  list(ShaderVarying.t),
  Glfw.program,
  attributeNameToLocation,
  attributeChannelToLocation,
  uniformNameToLocation,
);

let attributeNameToLocation = (s: t, a: string) => {
  let (_, _, _, _, dict, _, _) = s;
  Hashtbl.find_opt(dict, a);
};

let attributeChannelToLocation = (s: t, a: VertexChannel.t) => {
  let (_, _, _, _, _, dict, _) = s;
  Hashtbl.find_opt(dict, a);
};

exception UniformNotFoundException(string);
let _getOrThrowUniform = (name, opt) => {
  switch (opt) {
  | Some(v) => v
  | None => raise(UniformNotFoundException(name))
  };
};

let uniformNameToLocation = (s: t, name: string) => {
  let (_, _, _, _, _, _, u) = s;
  Hashtbl.find_opt(u, name);
};

let getUniformLocation: (t, string) => uniformLocation =
  (s, a) => {
    uniformNameToLocation(s, a) |> _getOrThrowUniform(a);
  };

/*
 * State changes in OpenGL are expensive!
 * We want to minimize those, when possible.
 */
module ShaderCache {
    let identityMatrix = Mat4.create();


    let cache_size = 32;

    type t = {
       mutable program: option(Glfw.program),
       cachedMat4: array(Mat4.t),
    };

    let create = () => {
        program: None,
        cachedMat4: Array.make(cache_size, identityMatrix),
    };

    let setMat4IfNew = (cache: t, u: uniformLocation, v: Mat4.t,  f) => {
        let uInt: int = Obj.magic(u);
        /* print_endline ("uniform location: " ++ string_of_int(uInt)); */
        if (uInt >= cache_size) {
            print_endline ("Setting over cache size");
           f(); 
        }

        let currentVal = cache.cachedMat4[uInt];
        if (currentVal !== v) {
            /* print_endline ("Setting cached"); */
            cache.cachedMat4[uInt] = v;
            f();
        } else {
            ();
            /* print_endline ("not setting CACHED"); */
        }
    };

    let useProgramIfNew = (cache: t, program: Glfw.program, f) => {
        switch (cache.program) {
        | None =>
            
            cache.program = Some(program);
            f();
        | Some(v) when v !== program =>
            cache.program = Some(program);
            f();
        | Some(_) => {
            ();
            /* print_endline (" NOT SWITCHING CACHED" ); */
        }
        }
    }

}

let _cache: ref(ShaderCache.t) = ref(ShaderCache.create());

let setUniform1f = (uniform: uniformLocation, v) => glUniform1f(uniform, v);

let setUniform2fv = (uniform: uniformLocation, v: Vec2.t) =>
  glUniform2fv(uniform, v);

let setUniform3fv = (uniform: uniformLocation, v: Vec3.t) =>
  glUniform3fv(uniform, v);

let setUniform4f =
    (u: uniformLocation, x: float, y: float, z: float, w: float) =>
  glUniform4f(u, x, y, z, w);

let setUniform4fv = (u: uniformLocation, v: Vec4.t) => glUniform4fv(u, v);

let setUniformMatrix4fv = (u: uniformLocation, m: Mat4.t) =>
  glUniformMatrix4fv(u, m);

let use = (s: t) => {
    let (_, _, _, p, _, _, _) = s;
    ShaderCache.useProgramIfNew(_cache^, p, () => glUseProgram(p))
};
