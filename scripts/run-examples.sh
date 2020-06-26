native="
(executables
 (names Examples)
 (modes exe)
 (preprocess
  (pps brisk-reconciler.ppx lwt_ppx))
 (package ReveryExamples)
 (public_names Examples)
 (libraries ExampleStubs skia str Revery Revery_Lwt flex timber))
"

byte="
(executables
 (names Examples)
 (modes byte)
 (preprocess
  (pps brisk-reconciler.ppx lwt_ppx))
 (package ReveryExamples)
 (public_names Examples)
 (libraries ExampleStubs skia str Revery Revery_Lwt flex timber))
"

if [[ "$1" = "byte" ]]
then
	echo $byte > examples/examples.sexp
else
	echo $native > examples/examples.sexp
fi

shift

esy @examples x Examples "$@"
