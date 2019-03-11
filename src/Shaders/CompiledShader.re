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

let _setUniformIfAvailable = (s, name, f) => {
  let uLoc = uniformNameToLocation(s, name);
  switch (uLoc) {
  | Some(u) => f(u)
  | None => ()
  };
};

let setUniform1f = (uniform: uniformLocation, v) => glUniform1f(uniform, v);

let setUniform2fv = (uniform: uniformLocation, v: Vec2.t) =>
  glUniform2fv(uniform, v);

let setUniform3fv = (uniform: uniformLocation, v: Vec3.t) =>
  glUniform3fv(uniform, v);

let setUniform4f =
    (u: uniformLocation, x: float, y: float, z: float, w: float) =>
  glUniform4f(u, x, y, z, w);

let setUniform4fv = (u: uniformLocation, v: Vec4.t) => glUniform4fv(u, v);

let setUniformMatrix4fv = (s: t, name: string, m: Mat4.t) =>
  _setUniformIfAvailable(s, name, u => glUniformMatrix4fv(u, m));

/*
 * State changes in OpenGL are expensive!
 * We want to minimize those, when possible.
 */
let _cache: ref(option(Glfw.program)) = ref(None);

let use = (s: t) => {
  let (_, _, _, p, _, _, _) = s;

  switch (_cache^) {
  | None =>
    glUseProgram(p);
    _cache := Some(p);
  | Some(v) when v !== p =>
    glUseProgram(p);
    _cache := Some(p);
    ();
  | Some(_) => ()
  };
};
