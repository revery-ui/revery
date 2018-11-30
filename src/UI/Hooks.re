/* Hooks specific to Revery */

open UiReact;

let useAnimation = (v: Animated.animationValue, opts: Animated.animationOptions) => {
    let (currentV, _set) = useState(v);

    useEffect(~condition=MountUnmount, () => {
        let anim = Animated.start(v, opts);  

        () => Animated.cancel(anim);
    });

    currentV.current
};
