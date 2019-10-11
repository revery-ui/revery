open Sdl2;
open Sdl2.Gl;

type t = {
  buffer: Gl.buffer,
  length: ref(int),
};

let create = () => {
  let buffer = Gl.glCreateBuffer();

  let length = ref(0);
  let result: t = {buffer, length};
  result;
};

let setData = (ib: t, arr: array(int)) => {
  let {buffer, length} = ib;
  let fArray = Uint16Array.of_array(arr);
  length := Array.length(arr);
  Gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
  Gl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, fArray, GL_STATIC_DRAW);
};

let draw = (ib: t) => {
  let {buffer, length} = ib;
  Gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
  Gl.glDrawElements(GL_TRIANGLES, length^, GL_UNSIGNED_SHORT, 0);
};
