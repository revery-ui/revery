(*
 * zed_utf8.mli
 * ------------
 * Copyright : (c) 2011, Jeremie Dimino <jeremie@dimino.org>
 * Licence   : BSD3
 *
 * This file is a part of Zed, an editor engine.
 *)

(** UTF-8 enoded strings *)

open CamomileLibrary

type t = string
    (** Type of UTF-8 encoded strings. *)

exception Invalid of string * string
  (** [Invalid(error, text)] Exception raised when an invalid UTF-8
      encoded string is encountered. [text] is the faulty text and
      [error] is a description of the first error in [text]. *)

exception Out_of_bounds
  (** Exception raised when trying to access a character which is
      outside the bounds of a string. *)

(** {5 Validation} *)

(** Result of cheking a string for correct UTF-8. *)
type check_result =
  | Correct of int
      (** The string is correctly UTF-8 encoded, and the paramter is
          the length of the string. *)
  | Message of string
      (** The string is invalid and the parameter is an error
          message. *)

val check : t -> check_result
  (** [check str] checks that [str] is a valid UTF-8 encoded
      string. *)

val validate : t -> int
  (** Same as check but raises an exception in case the argument is
      not a valid text, otherwise returns the length of the string. *)

val next_error : t -> int -> int * int * string
  (** [next_error str ofs] returns [(ofs', count, msg)] where [ofs']
      is the offset of the start of the first invalid sequence after
      [ofs] (inclusive) in [str], [count] is the number of unicode
      character between [ofs] and [ofs'] (exclusive) and [msg] is an
      error message. If there is no error until the end of string then
      [ofs] is [String.length str] and [msg] is the empty string. *)

(** {5 Construction} *)

val singleton : Uchar.t -> t
  (** [singleton ch] creates a string of length 1 containing only the
      given character. *)

val make : int -> Uchar.t -> t
  (** [make n ch] creates a string of length [n] filled with [ch]. *)

val init : int -> (int -> Uchar.t) -> t
  (** [init n f] returns the contenation of [singleton (f 0)],
      [singleton (f 1)], ..., [singleton (f (n - 1))]. *)

val rev_init : int -> (int -> Uchar.t) -> t
  (** [rev_init n f] returns the contenation of [singleton (f (n -
      1))], ..., [singleton (f 1)], [singleton (f 0)]. *)

(** {5 Informations} *)

val length : t -> int
  (** Returns the length of the given string. *)

(** {5 Comparison} *)

val compare : t -> t -> int
  (** Compares two strings (in code point order). *)

(** {5 Random access} *)

val get : t -> int -> Uchar.t
  (** [get str idx] returns the character at index [idx] in
      [str]. *)

(** {5 String manipulation} *)

val sub : t -> int -> int -> t
  (** [sub str ofs len] Returns the sub-string of [str] starting at
      [ofs] and of length [len]. *)

val break : t -> int -> t * t
  (** [break str pos] returns the sub-strings before and after [pos]
      in [str]. It is more efficient than creating two sub-strings
      with {!sub}. *)

val before : t -> int -> t
  (** [before str pos] returns the sub-string before [pos] in [str] *)

val after : t -> int -> t
  (** [after str pos] returns the sub-string after [pos] in [str] *)

val insert : t -> int -> t -> t
  (** [insert str pos sub] inserts [sub] in [str] at position
      [pos]. *)

val remove : t -> int -> int -> t
  (** [remove str pos len] removes the [len] characters at position
      [pos] in [str] *)

val replace : t -> int -> int -> t -> t
  (** [replace str pos len repl] replaces the [len] characters at
      position [pos] in [str] by [repl]. *)

(** {5 Tranformation} *)

val rev : t -> t
  (** [rev str] reverses all characters of [str]. *)

val concat : t -> t list -> t
  (** [concat sep l] returns the concatenation of all strings of [l]
      separated by [sep]. *)

val rev_concat : t -> t list -> t
  (** [concat sep l] returns the concatenation of all strings of [l]
      in reverse order separated by [sep]. *)

val explode : t -> Uchar.t list
  (** [explode str] returns the list of all characters of [str]. *)

val rev_explode : t -> Uchar.t list
  (** [rev_explode str] returns the list of all characters of [str] in
      reverse order. *)

val implode : Uchar.t list -> t
  (** [implode l] returns the concatenation of all characters of [l]. *)

val rev_implode : Uchar.t list -> t
  (** [rev_implode l] is the same as [implode (List.rev l)] but more
      efficient. *)

(** {5 Text traversals} *)

val iter : (Uchar.t -> unit) -> t -> unit
  (** [iter f str] applies [f] an all characters of [str] starting
      from the left. *)

val rev_iter : (Uchar.t -> unit) -> t -> unit
  (** [rev_iter f str] applies [f] an all characters of [str] starting
      from the right. *)

val fold : (Uchar.t -> 'a -> 'a) -> t -> 'a -> 'a
  (** [fold f str acc] applies [f] on all characters of [str]
      starting from the left, accumulating a value. *)

val rev_fold : (Uchar.t -> 'a -> 'a) -> t -> 'a -> 'a
  (** [rev_fold f str acc] applies [f] on all characters of [str]
      starting from the right, accumulating a value. *)

val map : (Uchar.t -> Uchar.t) -> t -> t
  (** [map f str] maps all characters of [str] with [f]. *)

val rev_map : (Uchar.t -> Uchar.t) -> t -> t
  (** [rev_map f str] maps all characters of [str] with [f] in reverse
      order. *)

val map_concat : (Uchar.t -> t) -> t -> t
  (** [map f str] maps all characters of [str] with [f] and
      concatenate the result. *)

val rev_map_concat : (Uchar.t -> t) -> t -> t
  (** [rev_map f str] maps all characters of [str] with [f] in reverse
      order and concatenate the result. *)

val filter : (Uchar.t -> bool) -> t -> t
  (** [filter f str] filters characters of [str] with [f]. *)

val rev_filter : (Uchar.t -> bool) -> t -> t
  (** [rev_filter f str] filters characters of [str] with [f] in
      reverse order. *)

val filter_map : (Uchar.t -> Uchar.t option) -> t -> t
  (** [filter_map f str] filters and maps characters of [str] with
      [f]. *)

val rev_filter_map : (Uchar.t -> Uchar.t option) -> t -> t
  (** [rev_filter_map f str] filters and maps characters of [str] with
      [f] in reverse order. *)

val filter_map_concat : (Uchar.t -> t option) -> t -> t
  (** [filter_map f str] filters and maps characters of [str] with [f]
      and concatenate the result. *)

val rev_filter_map_concat : (Uchar.t -> t option) -> t -> t
  (** [rev_filter_map f str] filters and maps characters of [str] with
      [f] in reverse order and concatenate the result. *)

(** {5 Scanning} *)

val for_all : (Uchar.t -> bool) -> t -> bool
  (** [for_all f text] returns whether all characters of [text] verify
      the predicate [f]. *)

val exists : (Uchar.t -> bool) -> t -> bool
  (** [exists f text] returns whether at least one character of [text]
      verify [f]. *)

val count : (Uchar.t -> bool) -> t -> int
  (** [count f text] returhs the number of characters of [text]
      verifying [f]. *)

(** {5 Tests} *)

val contains : t -> t -> bool
  (** [contains text sub] returns whether [sub] appears in [text] *)

val starts_with : t -> t -> bool
  (** [starts_with text prefix] returns [true] iff [s] starts with
      [prefix]. *)

val ends_with : t -> t -> bool
  (** [ends_with text suffix] returns [true] iff [s] ends with
      [suffix]. *)

(** {5 Stripping} *)

val strip : ?predicate : (Uchar.t -> bool) -> t -> t
  (** [strip ?predicate text] returns [text] without its firsts and
      lasts characters that match [predicate]. [predicate] default to
      testing whether the given character has the [`White_Space]
      unicode property. For example:
      {[
        strip "\n  foo\n  " = "foo"
      ]}
  *)

val lstrip : ?predicate : (Uchar.t -> bool) -> t -> t
  (** [lstrip ?predicate text] is the same as {!strip} but it only
      removes characters at the left of [text]. *)

val rstrip : ?predicate : (Uchar.t -> bool) -> t -> t
  (** [lstrip ?predicate text] is the same as {!strip} but it only
      removes characters at the right of [text]. *)

val lchop : t -> t
  (** [lchop t] returns [t] without is first character. Returns [""]
      if [t = ""] *)

val rchop : t -> t
  (** [rchop t] returns [t] without is last character. Returns [""] if
      [t = ""]. *)

(** {5 Buffers} *)

val add : Buffer.t -> Uchar.t -> unit
  (** [add buf ch] is the same as [Buffer.add_string buf (singleton
      ch)] but is more efficient. *)

(** {5 Escaping} *)

val escaped_char : Uchar.t -> t
  (** [escaped_char ch] returns a string containg [ch] or an escaped
      version of [ch] if:
      - [ch] is a control character (code < 32)
      - [ch] is the character with code 127
      - [ch] is a non-ascii, non-alphabetic character
      It uses the syntax [\xXX], [\uXXXX], [\UXXXXXX] or a specific
      escape sequence [\n, \r, ...]. *)

val add_escaped_char : Buffer.t -> Uchar.t -> unit
  (** [add_escaped_char buf ch] is the same as [Buffer.add_string buf
      (escaped_char ch)] but a bit more efficient. *)

val escaped : t -> t
  (** [escaped text] escape all characters of [text] as with
      [escape_char]. *)

val add_escaped : Buffer.t -> t -> unit
  (** [add_escaped_char buf text] is the same as [Buffer.add_string
      buf (escaped text)] but a bit more efficient. *)

val escaped_string : Uutf.decoder_encoding -> string -> t
  (** [escaped_string enc str] escape the string [str] which is
      encoded with encoding [enc]. If decoding [str] with [enc] fails,
      it escape all non-printable bytes of [str] with the syntax
      [\yAB]. *)

val add_escaped_string : Buffer.t -> Uutf.decoder_encoding -> string -> unit
  (** [add_escaped_char buf enc text] is the same as
      [Buffer.add_string buf (escaped_string enc text)] but a bit more
      efficient. *)

(** {5 Safe offset API} *)

val next : t -> int -> int
  (** [next str ofs] returns the offset of the next character in
      [str]. *)

val prev : t -> int -> int
  (** [prev str ofs] returns the offset of the previous character in
      [str]. *)

val extract : t -> int -> Uchar.t
  (** [extract str ofs] returns the code-point at offset [ofs] in
      [str]. *)

val extract_next : t -> int -> Uchar.t * int
  (** [extract_next str ofs] returns the code-point at offset [ofs] in
      [str] and the offset of the next character. *)

val extract_prev : t -> int -> Uchar.t * int
  (** [extract_prev str ofs] returns the code-point at the previous
      offset in [str] and this offset. *)

(** {5 Unsafe offset API} *)

(** These functions does not check that the given offset is inside the
    bounds of the given string. *)

val unsafe_next : t -> int -> int
  (** [unsafe_next str ofs] returns the offset of the next character
      in [str]. *)

val unsafe_prev : t -> int -> int
  (** [unsafe_prev str ofs] returns the offset of the previous
      character in [str]. *)

val unsafe_extract : t -> int -> Uchar.t
  (** [unsafe_extract str ofs] returns the code-point at offset [ofs]
      in [str]. *)

val unsafe_extract_next : t -> int -> Uchar.t * int
  (** [unsafe_extract_next str ofs] returns the code-point at offset
      [ofs] in [str] and the offset the next character. *)

val unsafe_extract_prev : t -> int -> Uchar.t * int
  (** [unsafe_extract_prev str ofs] returns the code-point at the
      previous offset in [str] and this offset. *)