/* Hooks specific to Revery */

/* open UiReact; */

/* let useEffect = useEffect; */

let useAnimation =
    (v: Animated.animationValue, _opts: Animated.animationOptions) => {
  /* let (currentV, _set) = useState(v); */

  /* useEffect( */
  /*   ~condition=MountUnmount, */
  /*   () => { */
  /*     let anim = Animated.start(v, opts); */

  /*     () => Animated.cancel(anim); */
  /*   }, */
  /* ); */

  v.current

  /* currentV.current; */
};
