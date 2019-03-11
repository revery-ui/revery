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
 * Cache to stash global state, so we can
 * minimize calls to OpenGL when possible
 */
module ShaderCache = {
  type t = {mutable program: option(Glfw.program)};

  let create = () => {program: None};

  let useProgramIfNew = (cache: t, program: Glfw.program, f) => {
    switch (cache.program) {
    | None =>
      cache.program = Some(program);
      f();
    | Some(v) when v !== program =>
      cache.program = Some(program);
      f();
    | Some(_) => ()
    };
  };
};

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
  ShaderCache.useProgramIfNew(_cache^, p, () => glUseProgram(p));
};
