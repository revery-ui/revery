(*
 * zed_utf8.ml
 * -----------
 * Copyright : (c) 2011, Jeremie Dimino <jeremie@dimino.org>
 * Licence   : BSD3
 *
 * This file is a part of Zed, an editor engine.
 *)

type t = string
exception Invalid of string * string
exception Out_of_bounds

let fail str pos msg = raise (Invalid(Printf.sprintf "at position %d: %s" pos msg, str))

let byte str i = Char.code (String.unsafe_get str i)
let set_byte str i n = Bytes.unsafe_set str i (Char.unsafe_chr n)

(* +-----------------------------------------------------------------+
   | Validation                                                      |
   +-----------------------------------------------------------------+ *)

type check_result =
  | Correct of int
  | Message of string

let next_error s i =
  let len = String.length s in
  let rec main i ulen =
    if i = len then
      (i, ulen, "")
    else
      let ch = String.unsafe_get s i in
      match ch with
        | '\x00' .. '\x7f' ->
            main (i + 1) (ulen + 1)
        | '\xc0' .. '\xdf' ->
            if i + 1 >= len then
              (i, ulen, "premature end of UTF8 sequence")
            else begin
              let byte1 = Char.code (String.unsafe_get s (i + 1)) in
              if byte1 land 0xc0 != 0x80 then
                (i, ulen, "malformed UTF8 sequence")
              else if ((Char.code ch land 0x1f) lsl 6) lor (byte1 land 0x3f) < 0x80 then
                (i, ulen, "overlong UTF8 sequence")
              else
                main (i + 2) (ulen + 1)
            end
        | '\xe0' .. '\xef' ->
            if i + 2 >= len then
              (i, ulen, "premature end of UTF8 sequence")
            else begin
              let byte1 = Char.code (String.unsafe_get s (i + 1))
              and byte2 = Char.code (String.unsafe_get s (i + 2)) in
              if byte1 land 0xc0 != 0x80 then
                (i, ulen, "malformed UTF8 sequence")
              else if byte2 land 0xc0 != 0x80 then
                (i, ulen, "malformed UTF8 sequence")
              else if ((Char.code ch land 0x0f) lsl 12) lor ((byte1 land 0x3f) lsl 6) lor (byte2 land 0x3f) < 0x800 then
                (i, ulen, "overlong UTF8 sequence")
              else
                main (i + 3) (ulen + 1)
            end
        | '\xf0' .. '\xf7' ->
            if i + 3 >= len then
              (i, ulen, "premature end of UTF8 sequence")
            else begin
              let byte1 = Char.code (String.unsafe_get s (i + 1))
              and byte2 = Char.code (String.unsafe_get s (i + 2))
              and byte3 = Char.code (String.unsafe_get s (i + 3)) in
              if byte1 land 0xc0 != 0x80 then
                (i, ulen, "malformed UTF8 sequence")
              else if byte2 land 0xc0 != 0x80 then
                (i, ulen, "malformed UTF8 sequence")
              else if byte3 land 0xc0 != 0x80 then
                (i, ulen, "malformed UTF8 sequence")
              else if ((Char.code ch land 0x07) lsl 18) lor ((byte1 land 0x3f) lsl 12) lor ((byte2 land 0x3f) lsl 6) lor (byte3 land 0x3f) < 0x10000 then
                (i, ulen, "overlong UTF8 sequence")
              else
                main (i + 4) (ulen + 1)
            end
        | _ ->
            (i, ulen, "invalid start of UTF8 sequence")
  in
  main i 0

let check str =
  let ofs, len, msg = next_error str 0 in
  if ofs = String.length str then
    Correct len
  else
    Message (Printf.sprintf "at position %d: %s" ofs msg)

let validate str =
  let ofs, len, msg = next_error str 0 in
  if ofs = String.length str then
    len
  else
    fail str ofs msg

(* +-----------------------------------------------------------------+
   | Unsafe UTF-8 manipulation                                       |
   +-----------------------------------------------------------------+ *)

let unsafe_next str ofs =
  match String.unsafe_get str ofs with
    | '\x00' .. '\x7f' ->
        ofs + 1
    | '\xc0' .. '\xdf' ->
        if ofs + 2 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          ofs + 2
    | '\xe0' .. '\xef' ->
        if ofs + 3 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          ofs + 3
    | '\xf0' .. '\xf7' ->
        if ofs + 4 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          ofs + 4
    | _ ->
        fail str ofs "invalid start of UTF-8 sequence"

let unsafe_prev str ofs =
  match String.unsafe_get str (ofs - 1) with
    | '\x00' .. '\x7f' ->
        ofs - 1
    | '\x80' .. '\xbf' ->
        if ofs >= 2 then
          match String.unsafe_get str (ofs - 2) with
            | '\xc0' .. '\xdf' ->
                ofs - 2
            | '\x80' .. '\xbf' ->
                if ofs >= 3 then
                  match String.unsafe_get str (ofs - 3) with
                    | '\xe0' .. '\xef' ->
                        ofs - 3
                    | '\x80' .. '\xbf' ->
                        if ofs >= 4 then
                          match String.unsafe_get str (ofs - 4) with
                            | '\xf0' .. '\xf7' ->
                                ofs - 4
                            | _ ->
                                fail str (ofs - 4) "invalid start of UTF-8 sequence"
                        else
                          fail str (ofs - 3) "invalid start of UTF-8 string"
                    | _ ->
                        fail str (ofs - 3) "invalid middle of UTF-8 sequence"
                else
                  fail str (ofs - 2) "invaild start of UTF-8 string"
            | _ ->
                fail str (ofs - 2) "invalid middle of UTF-8 sequence"
        else
          fail str (ofs - 1) "invalid start of UTF-8 string"
    | _ ->
        fail str (ofs - 1) "invalid end of UTF-8 sequence"

let unsafe_extract str ofs =
  let ch = String.unsafe_get str ofs in
  match ch with
    | '\x00' .. '\x7f' ->
        Uchar.of_char ch
    | '\xc0' .. '\xdf' ->
        if ofs + 2 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          Uchar.of_int (((Char.code ch land 0x1f) lsl 6) lor (byte str (ofs + 1) land 0x3f))
    | '\xe0' .. '\xef' ->
        if ofs + 3 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          Uchar.of_int (((Char.code ch land 0x0f) lsl 12) lor ((byte str (ofs + 1) land 0x3f) lsl 6) lor (byte str (ofs + 2) land 0x3f))
    | '\xf0' .. '\xf7' ->
        if ofs + 4 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          Uchar.of_int (((Char.code ch land 0x07) lsl 18) lor ((byte str (ofs + 1) land 0x3f) lsl 12) lor ((byte str (ofs + 2) land 0x3f) lsl 6) lor (byte str (ofs + 3) land 0x3f))
    | _ ->
        fail str ofs "invalid start of UTF-8 sequence"

let unsafe_extract_next str ofs =
  let ch = String.unsafe_get str ofs in
  match ch with
    | '\x00' .. '\x7f' ->
        (Uchar.of_char ch, ofs + 1)
    | '\xc0' .. '\xdf' ->
        if ofs + 2 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          (Uchar.of_int (((Char.code ch land 0x1f) lsl 6) lor (byte str (ofs + 1) land 0x3f)), ofs + 2)
    | '\xe0' .. '\xef' ->
        if ofs + 3 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          (Uchar.of_int (((Char.code ch land 0x0f) lsl 12) lor ((byte str (ofs + 1) land 0x3f) lsl 6) lor (byte str (ofs + 2) land 0x3f)), ofs + 3)
    | '\xf0' .. '\xf7' ->
        if ofs + 4 > String.length str then
          fail str ofs "unterminated UTF-8 sequence"
        else
          (Uchar.of_int (((Char.code ch land 0x07) lsl 18) lor ((byte str (ofs + 1) land 0x3f) lsl 12) lor ((byte str (ofs + 2) land 0x3f) lsl 6) lor (byte str (ofs + 3) land 0x3f)), ofs + 4)
    | _ ->
        fail str ofs "invalid start of UTF-8 sequence"

let unsafe_extract_prev str ofs =
  let ch1 = String.unsafe_get str (ofs - 1) in
  match ch1 with
    | '\x00' .. '\x7f' ->
        (Uchar.of_char ch1, ofs - 1)
    | '\x80' .. '\xbf' ->
        if ofs >= 2 then
          let ch2 = String.unsafe_get str (ofs - 2) in
          match ch2 with
            | '\xc0' .. '\xdf' ->
                (Uchar.of_int (((Char.code ch2 land 0x1f) lsl 6) lor (Char.code ch1 land 0x3f)), ofs - 2)
            | '\x80' .. '\xbf' ->
                if ofs >= 3 then
                  let ch3 = String.unsafe_get str (ofs - 3) in
                  match ch3 with
                    | '\xe0' .. '\xef' ->
                        (Uchar.of_int (((Char.code ch3 land 0x0f) lsl 12) lor ((Char.code ch2 land 0x3f) lsl 6) lor (Char.code ch1 land 0x3f)), ofs - 3)
                    | '\x80' .. '\xbf' ->
                        if ofs >= 4 then
                          let ch4 = String.unsafe_get str (ofs - 4) in
                          match ch4 with
                            | '\xf0' .. '\xf7' ->
                                (Uchar.of_int (((Char.code ch4 land 0x07) lsl 18) lor ((Char.code ch3 land 0x3f) lsl 12) lor ((Char.code ch2 land 0x3f) lsl 6) lor (Char.code ch1 land 0x3f)), ofs - 4)
                            | _ ->
                                fail str (ofs - 4) "invalid start of UTF-8 sequence"
                        else
                          fail str (ofs - 3) "invalid start of UTF-8 string"
                    | _ ->
                        fail str (ofs - 3) "invalid middle of UTF-8 sequence"
                else
                  fail str (ofs - 2) "invaild start of UTF-8 string"
            | _ ->
                fail str (ofs - 2) "invalid middle of UTF-8 sequence"
        else
          fail str (ofs - 1) "invalid start of UTF-8 string"
    | _ ->
        fail str (ofs - 1) "invalid end of UTF-8 sequence"

let rec move_l str ofs len =
  if len = 0 then
    ofs
  else if ofs = String.length str then
    raise Out_of_bounds
  else
    move_l str (unsafe_next str ofs) (len - 1)

let unsafe_sub str ofs len =
  let res = Bytes.create len in
  String.unsafe_blit str ofs res 0 len;
  Bytes.unsafe_to_string res

(* +-----------------------------------------------------------------+
   | Construction                                                    |
   +-----------------------------------------------------------------+ *)

let singleton char =
  let code = Uchar.to_int char in
  Bytes.unsafe_to_string @@
  if code < 0x80 then begin
    let s = Bytes.create 1 in
    set_byte s 0 code;
    s
  end else if code <= 0x800 then begin
    let s = Bytes.create 2 in
    set_byte s 0 ((code lsr 6) lor 0xc0);
    set_byte s 1 ((code land 0x3f) lor 0x80);
    s
  end else if code <= 0x10000 then begin
    let s = Bytes.create 3 in
    set_byte s 0 ((code lsr 12) lor 0xe0);
    set_byte s 1 (((code lsr 6) land 0x3f) lor 0x80);
    set_byte s 2 ((code land 0x3f) lor 0x80);
    s
  end else if code <= 0x10ffff then begin
    let s = Bytes.create 4 in
    set_byte s 0 ((code lsr 18) lor 0xf0);
    set_byte s 1 (((code lsr 12) land 0x3f) lor 0x80);
    set_byte s 2 (((code lsr 6) land 0x3f) lor 0x80);
    set_byte s 3 ((code land 0x3f) lor 0x80);
    s
  end else
    (* Camomile allow characters with code-point greater than
       0x10ffff *)
    invalid_arg "Zed_utf8.singleton"

let make n code =
  let str = singleton code in
  let len = String.length str in
  let res = Bytes.create (n * len) in
  let ofs = ref 0 in
  for _ = 1 to n do
    String.unsafe_blit str 0 res !ofs len;
    ofs := !ofs + len
  done;
  Bytes.unsafe_to_string res

let init n f =
  let buf = Buffer.create n in
  for i = 0 to n - 1 do
    Buffer.add_string buf (singleton (f i))
  done;
  Buffer.contents buf

let rev_init n f =
  let buf = Buffer.create n in
  for i = n - 1 downto 0 do
    Buffer.add_string buf (singleton (f i))
  done;
  Buffer.contents buf

(* +-----------------------------------------------------------------+
   | Informations                                                    |
   +-----------------------------------------------------------------+ *)

let rec length_rec str ofs len =
  if ofs = String.length str then
    len
  else
    length_rec str (unsafe_next str ofs) (len + 1)

let length str =
  length_rec str 0 0

(* +-----------------------------------------------------------------+
   | Comparison                                                      |
   +-----------------------------------------------------------------+ *)

let rec compare_rec str1 ofs1 str2 ofs2 =
  if ofs1 = String.length str1 then
    if ofs2 = String.length str2 then
      0
    else
      -1
  else if ofs2 = String.length str2 then
    1
  else
    let code1, ofs1 = unsafe_extract_next str1 ofs1
    and code2, ofs2 = unsafe_extract_next str2 ofs2 in
    let d = Uchar.to_int code1 - Uchar.to_int code2 in
    if d <> 0 then
      d
    else
      compare_rec str1 ofs1 str2 ofs2

let compare str1 str2 =
  compare_rec str1 0 str2 0

(* +-----------------------------------------------------------------+
   | Random access                                                   |
   +-----------------------------------------------------------------+ *)

let get str idx =
  if idx < 0 then
    raise Out_of_bounds
  else
    unsafe_extract str (move_l str 0 idx)

(* +-----------------------------------------------------------------+
   | Manipulation                                                    |
   +-----------------------------------------------------------------+ *)

let sub str idx len =
  if idx < 0 || len < 0 then
    raise Out_of_bounds
  else
    let ofs1 = move_l str 0 idx in
    let ofs2 = move_l str ofs1 len in
    unsafe_sub str ofs1 (ofs2 - ofs1)

let break str idx =
  if idx < 0 then
    raise Out_of_bounds
  else
    let ofs = move_l str 0 idx in
    (unsafe_sub str 0 ofs, unsafe_sub str ofs (String.length str - ofs))

let before str idx =
  if idx < 0 then
    raise Out_of_bounds
  else
    let ofs = move_l str 0 idx in
    unsafe_sub str 0 ofs

let after str idx =
  if idx < 0 then
    raise Out_of_bounds
  else
    let ofs = move_l str 0 idx in
    unsafe_sub str ofs (String.length str - ofs)

let concat3 a b c =
  let lena = String.length a
  and lenb = String.length b
  and lenc = String.length c in
  let res = Bytes.create (lena + lenb + lenc) in
  String.unsafe_blit a 0 res 0 lena;
  String.unsafe_blit b 0 res lena lenb;
  String.unsafe_blit c 0 res (lena + lenb) lenc;
  Bytes.unsafe_to_string res

let insert str idx sub =
  let a, b = break str idx in
  concat3 a sub b

let remove str idx len =
  if idx < 0 || len < 0 then
    raise Out_of_bounds
  else
    let ofs1 = move_l str 0 idx in
    let ofs2 = move_l str ofs1 len in
    unsafe_sub str 0 ofs1 ^ unsafe_sub str ofs2 (String.length str - ofs2)

let replace str idx len repl =
  if idx < 0 || len < 0 then
    raise Out_of_bounds
  else
    let ofs1 = move_l str 0 idx in
    let ofs2 = move_l str ofs1 len in
    concat3 (unsafe_sub str 0 ofs1) repl (unsafe_sub str ofs2 (String.length str - ofs2))

(* +-----------------------------------------------------------------+
   | Exploding and imploding                                         |
   +-----------------------------------------------------------------+ *)

let rec rev_rec (res : Bytes.t) str ofs_src ofs_dst =
  if ofs_src = String.length str then
    Bytes.unsafe_to_string res
  else begin
    let ofs_src' = unsafe_next str ofs_src in
    let len = ofs_src' - ofs_src in
    let ofs_dst = ofs_dst - len in
    String.unsafe_blit str ofs_src res ofs_dst len;
    rev_rec res str ofs_src' ofs_dst
  end

let rev str =
  let len = String.length str in
  rev_rec (Bytes.create len) str 0 len

let concat sep l =
  match l with
    | [] ->
        ""
    | x :: l ->
        let sep_len = String.length sep in
        let len = List.fold_left (fun len str -> len + sep_len + String.length str) (String.length x) l in
        let res = Bytes.create len in
        String.unsafe_blit x 0 res 0 (String.length x);
        ignore
          (List.fold_left
             (fun ofs str ->
                String.unsafe_blit sep 0 res ofs sep_len;
                let ofs = ofs + sep_len in
                let len = String.length str in
                String.unsafe_blit str 0 res ofs len;
                ofs + len)
             (String.length x) l);
        Bytes.unsafe_to_string res

let rev_concat sep l =
  match l with
    | [] ->
        ""
    | x :: l ->
        let sep_len = String.length sep in
        let len = List.fold_left (fun len str -> len + sep_len + String.length str) (String.length x) l in
        let res = Bytes.create len in
        let ofs = len - String.length x in
        String.unsafe_blit x 0 res ofs (String.length x);
        ignore
          (List.fold_left
             (fun ofs str ->
                let ofs = ofs - sep_len in
                String.unsafe_blit sep 0 res ofs sep_len;
                let len = String.length str in
                let ofs = ofs - len in
                String.unsafe_blit str 0 res ofs len;
                ofs)
             ofs l);
        Bytes.unsafe_to_string res

let rec explode_rec str ofs acc =
  if ofs = 0 then
    acc
  else
    let x, ofs = unsafe_extract_prev str ofs in
    explode_rec str ofs (x :: acc)

let explode str =
  explode_rec str (String.length str) []

let rec rev_explode_rec str ofs acc =
  if ofs = String.length str then
    acc
  else
    let x, ofs = unsafe_extract_next str ofs in
    rev_explode_rec str ofs (x :: acc)

let rev_explode str =
  rev_explode_rec str 0 []

let implode l =
  let l = List.map singleton l in
  let len = List.fold_left (fun len str -> len + String.length str) 0 l in
  let res = Bytes.create len in
  ignore
    (List.fold_left
       (fun ofs str ->
          let len = String.length str in
          String.unsafe_blit str 0 res ofs len;
          ofs + len)
       0 l);
  Bytes.unsafe_to_string res

let rev_implode l =
  let l = List.map singleton l in
  let len = List.fold_left (fun len str -> len + String.length str) 0 l in
  let res = Bytes.create len in
  ignore
    (List.fold_left
       (fun ofs str ->
          let len = String.length str in
          let ofs = ofs - len in
          String.unsafe_blit str 0 res ofs len;
          ofs)
       len l);
  Bytes.unsafe_to_string res

(* +-----------------------------------------------------------------+
   | Text transversal                                                |
   +-----------------------------------------------------------------+ *)

let rec iter_rec f str ofs =
  if ofs = String.length str then
    ()
  else begin
    let chr, ofs = unsafe_extract_next str ofs in
    f chr;
    iter_rec f str ofs
  end

let iter f str =
  iter_rec f str 0

let rec rev_iter_rec f str ofs =
  if ofs = 0 then
    ()
  else begin
    let chr, ofs = unsafe_extract_prev str ofs in
    f chr;
    rev_iter_rec f str ofs
  end

let rev_iter f str =
  rev_iter_rec f str (String.length str)

let rec fold_rec f str ofs acc =
  if ofs = String.length str then
    acc
  else begin
    let chr, ofs = unsafe_extract_next str ofs in
    fold_rec f str ofs (f chr acc)
  end

let fold f str acc =
  fold_rec f str 0 acc

let rec rev_fold_rec f str ofs acc =
  if ofs = 0 then
    acc
  else begin
    let chr, ofs = unsafe_extract_prev str ofs in
    rev_fold_rec f str ofs (f chr acc)
  end

let rev_fold f str acc =
  rev_fold_rec f str (String.length str) acc

let rec map_rec buf f str ofs =
  if ofs = String.length str then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_next str ofs in
    Buffer.add_string buf (singleton (f chr));
    map_rec buf f str ofs
  end

let map f str =
  map_rec (Buffer.create (String.length str)) f str 0

let rec map_concat_rec buf f str ofs =
  if ofs = String.length str then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_next str ofs in
    Buffer.add_string buf (f chr);
    map_concat_rec buf f str ofs
  end

let map_concat f str =
  map_concat_rec (Buffer.create (String.length str)) f str 0

let rec rev_map_rec buf f str ofs =
  if ofs = 0 then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_prev str ofs in
    Buffer.add_string buf (singleton (f chr));
    rev_map_rec buf f str ofs
  end

let rev_map f str =
  rev_map_rec (Buffer.create (String.length str)) f str (String.length str)

let rec rev_map_concat_rec buf f str ofs =
  if ofs = 0 then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_prev str ofs in
    Buffer.add_string buf (f chr);
    rev_map_concat_rec buf f str ofs
  end

let rev_map_concat f str =
  rev_map_concat_rec (Buffer.create (String.length str)) f str (String.length str)

let rec filter_rec buf f str ofs =
  if ofs = String.length str then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_next str ofs in
    if f chr then
      Buffer.add_string buf (singleton chr);
    filter_rec buf f str ofs
  end

let filter f str =
  filter_rec (Buffer.create (String.length str)) f str 0

let rec rev_filter_rec buf f str ofs =
  if ofs = 0 then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_prev str ofs in
    if f chr then
      Buffer.add_string buf (singleton chr);
    rev_filter_rec buf f str ofs
  end

let rev_filter f str =
  rev_filter_rec (Buffer.create (String.length str)) f str (String.length str)

let rec filter_map_rec buf f str ofs =
  if ofs = String.length str then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_next str ofs in
    (match f chr with
       | Some chr ->
           Buffer.add_string buf (singleton chr)
       | None ->
           ());
    filter_map_rec buf f str ofs
  end

let filter_map f str =
  filter_map_rec (Buffer.create (String.length str)) f str 0

let rec filter_map_concat_rec buf f str ofs =
  if ofs = String.length str then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_next str ofs in
    (match f chr with
       | Some txt ->
           Buffer.add_string buf txt
       | None ->
           ());
    filter_map_concat_rec buf f str ofs
  end

let filter_map_concat f str =
  filter_map_concat_rec (Buffer.create (String.length str)) f str 0

let rec rev_filter_map_rec buf f str ofs =
  if ofs = 0 then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_prev str ofs in
    (match f chr with
       | Some chr ->
           Buffer.add_string buf (singleton chr)
       | None ->
           ());
    rev_filter_map_rec buf f str ofs
  end

let rev_filter_map f str =
  rev_filter_map_rec (Buffer.create (String.length str)) f str (String.length str)

let rec rev_filter_map_concat_rec buf f str ofs =
  if ofs = 0 then
    Buffer.contents buf
  else begin
    let chr, ofs = unsafe_extract_prev str ofs in
    (match f chr with
       | Some txt ->
           Buffer.add_string buf txt
       | None ->
           ());
    rev_filter_map_concat_rec buf f str ofs
  end

let rev_filter_map_concat f str =
  rev_filter_map_concat_rec (Buffer.create (String.length str)) f str (String.length str)

(* +-----------------------------------------------------------------+
   | Scanning                                                        |
   +-----------------------------------------------------------------+ *)

let rec for_all_rec f str ofs =
  if ofs = String.length str then
    true
  else
    let chr, ofs = unsafe_extract_next str ofs in
    f chr && for_all_rec f str ofs

let for_all f str =
  for_all_rec f str 0

let rec exists_rec f str ofs =
  if ofs = String.length str then
    false
  else
    let chr, ofs = unsafe_extract_next str ofs in
    f chr || exists_rec f str ofs

let exists f str =
  exists_rec f str 0

let rec count_rec f str ofs n =
  if ofs = String.length str then
    n
  else
    let chr, ofs = unsafe_extract_next str ofs in
    count_rec f str ofs (if f chr then n + 1 else n)

let count f str =
  count_rec f str 0 0

(* +-----------------------------------------------------------------+
   | Tests                                                           |
   +-----------------------------------------------------------------+ *)

let rec unsafe_sub_equal str ofs sub ofs_sub =
  if ofs_sub = String.length sub then
    true
  else
    (String.unsafe_get str ofs = String.unsafe_get sub ofs_sub)
    && unsafe_sub_equal str (ofs + 1) sub (ofs_sub + 1)

let rec contains_rec str sub ofs =
  if ofs + String.length sub > String.length str then
    false
  else
    unsafe_sub_equal str ofs sub 0 || contains_rec str sub (unsafe_next str ofs)

let contains str sub =
  contains_rec str sub 0

let starts_with str prefix =
  if String.length prefix > String.length str then
    false
  else
    unsafe_sub_equal str 0 prefix 0

let ends_with str suffix =
  let ofs = String.length str - String.length suffix in
  if ofs < 0 then
    false
  else
    unsafe_sub_equal str ofs suffix 0

(* +-----------------------------------------------------------------+
   | Stripping                                                       |
   +-----------------------------------------------------------------+ *)

let rec lfind predicate str ofs =
  if ofs = String.length str then
    ofs
  else
    let chr, ofs' = unsafe_extract_next str ofs in
    if predicate chr then
      lfind predicate str ofs'
    else
      ofs

let rec rfind predicate str ofs =
  if ofs = 0 then
    0
  else
    let chr, ofs' = unsafe_extract_prev str ofs in
    if predicate chr then
      rfind predicate str ofs'
    else
      ofs

let strip ?(predicate=Uucp.White.is_white_space) str =
  let lofs = lfind predicate str 0 and rofs = rfind predicate str (String.length str) in
  if lofs < rofs then
    unsafe_sub str lofs (rofs - lofs)
  else
    ""

let lstrip ?(predicate=Uucp.White.is_white_space) str =
  let lofs = lfind predicate str 0 in
  unsafe_sub str lofs (String.length str - lofs)

let rstrip ?(predicate=Uucp.White.is_white_space) str =
  let rofs = rfind predicate str (String.length str) in
  unsafe_sub str 0 rofs

let lchop = function
  | "" ->
      ""
  | str ->
      let ofs = unsafe_next str 0 in
      unsafe_sub str ofs (String.length str - ofs)

let rchop = function
  | "" ->
      ""
  | str ->
      let ofs = unsafe_prev str (String.length str) in
      unsafe_sub str 0 ofs

(* +-----------------------------------------------------------------+
   | Buffers                                                         |
   +-----------------------------------------------------------------+ *)

let add buf char =
  let code = Uchar.to_int char in
  if code < 0x80 then
    Buffer.add_char buf (Char.unsafe_chr code)
  else if code <= 0x800 then begin
    Buffer.add_char buf (Char.unsafe_chr ((code lsr 6) lor 0xc0));
    Buffer.add_char buf (Char.unsafe_chr ((code land 0x3f) lor 0x80))
  end else if code <= 0x10000 then begin
    Buffer.add_char buf (Char.unsafe_chr ((code lsr 12) lor 0xe0));
    Buffer.add_char buf (Char.unsafe_chr (((code lsr 6) land 0x3f) lor 0x80));
    Buffer.add_char buf (Char.unsafe_chr ((code land 0x3f) lor 0x80))
  end else if code <= 0x10ffff then begin
    Buffer.add_char buf (Char.unsafe_chr ((code lsr 18) lor 0xf0));
    Buffer.add_char buf (Char.unsafe_chr (((code lsr 12) land 0x3f) lor 0x80));
    Buffer.add_char buf (Char.unsafe_chr (((code lsr 6) land 0x3f) lor 0x80));
    Buffer.add_char buf (Char.unsafe_chr ((code land 0x3f) lor 0x80))
  end else
    invalid_arg "Zed_utf8.add"

(* +-----------------------------------------------------------------+
   | Offset API                                                      |
   +-----------------------------------------------------------------+ *)

let extract str ofs =
  if ofs < 0 || ofs >= String.length str then
    raise Out_of_bounds
  else
    unsafe_extract str ofs

let next str ofs =
  if ofs < 0 || ofs >= String.length str then
    raise Out_of_bounds
  else
    unsafe_next str ofs

let extract_next str ofs =
  if ofs < 0 || ofs >= String.length str then
    raise Out_of_bounds
  else
    unsafe_extract_next str ofs

let prev str ofs =
  if ofs <= 0 || ofs > String.length str then
    raise Out_of_bounds
  else
    unsafe_prev str ofs

let extract_prev str ofs =
  if ofs <= 0 || ofs > String.length str then
    raise Out_of_bounds
  else
    unsafe_extract_prev str ofs

(* +-----------------------------------------------------------------+
   | Escaping                                                        |
   +-----------------------------------------------------------------+ *)

let escaped_char ch =
  match Uchar.to_int ch with
    | 7 ->
        "\\a"
    | 8 ->
        "\\b"
    | 9 ->
        "\\t"
    | 10 ->
        "\\n"
    | 11 ->
        "\\v"
    | 12 ->
        "\\f"
    | 13 ->
        "\\r"
    | 27 ->
        "\\e"
    | 92 ->
        "\\\\"
    | code when code >= 32 && code <= 126 ->
        String.make 1 (Char.chr code)
    | _ when Uucp.Alpha.is_alphabetic ch ->
        singleton ch
    | code when code <= 127 ->
        Printf.sprintf "\\x%02x" code
    | code when code <= 0xffff ->
        Printf.sprintf "\\u%04x" code
    | code ->
        Printf.sprintf "\\U%06x" code

let add_escaped_char buf ch =
  match Uchar.to_int ch with
    | 7 ->
        Buffer.add_string buf "\\a"
    | 8 ->
        Buffer.add_string buf "\\b"
    | 9 ->
        Buffer.add_string buf "\\t"
    | 10 ->
        Buffer.add_string buf "\\n"
    | 11 ->
        Buffer.add_string buf "\\v"
    | 12 ->
        Buffer.add_string buf "\\f"
    | 13 ->
        Buffer.add_string buf "\\r"
    | 27 ->
        Buffer.add_string buf "\\e"
    | 92 ->
        Buffer.add_string buf "\\\\"
    | code when code >= 32 && code <= 126 ->
        Buffer.add_char buf (Char.chr code)
    | _ when Uucp.Alpha.is_alphabetic ch ->
        add buf ch
    | code when code <= 127 ->
        Printf.bprintf buf "\\x%02x" code
    | code when code <= 0xffff ->
        Printf.bprintf buf "\\u%04x" code
    | code ->
        Printf.bprintf buf "\\U%06x" code

let escaped str =
  let buf = Buffer.create (String.length str) in
  iter (add_escaped_char buf) str;
  Buffer.contents buf

let add_escaped buf str =
  iter (add_escaped_char buf) str

let add_escaped_string buf encoding str =
  let b = Buffer.create (String.length str) in
  let d = Uutf.decoder ~encoding (`String str) in
  let rec loop () =
    match Uutf.decode d with
    | `Uchar u -> ignore (Uutf.Buffer.add_utf_8 b u); loop ()
    | `End -> add_escaped buf (Buffer.contents b)
    | `Malformed _ ->
        String.iter
          (function
             | '\x20' .. '\x7e' as ch ->
                 Buffer.add_char buf ch
             | ch ->
                 Printf.bprintf buf "\\y%02x" (Char.code ch))
          str
    | `Await -> assert false
  in
  loop ()

let escaped_string enc str =
  let buf = Buffer.create (String.length str) in
  add_escaped_string buf enc str;
  Buffer.contents buf
