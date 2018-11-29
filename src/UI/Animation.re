module Event = Reactify.Event;

module Time {
    type t = 
    | Seconds(float)
    | Milliseconds(float);

    let to_float_seconds = (v: t) => {
        switch (v) {
        | Seconds(x) => x
        | Milliseconds(x) => x /. 1000.;
        }
    }

    let show = (v: t) => {
        string_of_float(to_float_seconds(v)) ++ "s";
    };
}


module type AnimationTicker = {
    let time: unit => Time.t;

    let tick: Event.t(Time.t);
};

module Make = (AnimationTickerImpl: AnimationTicker) => {


    type animationValue = {
        mutable current: float,
    };

    type animation = {
        delay: float,
        mutable startTime: float,
        duration: float,
        startValue: float,
        toValue: float,
        value: animationValue,
        repeat: bool,
    };

    let clamp = (f: float, min: float, max: float) => {
        switch (f < min) {
        | true => min
        | false => switch (f > max) {
            | true => max
            | false => f;
        }
        }
    };

    let interpolate = (t: float, fromValue: float, toValue: float) => {
        let t = clamp(t, 0., 1.);

        fromValue +. ((toValue -. fromValue) *. t)
    };

    let activeAnimations: ref(list(animation)) = ref([]);

    type animationOptions = {
        duration: Time.t,
        delay: Time.t,
        toValue: float,
        repeat: bool,
    };

    let floatValue = (v: float) => {
        let ret = {
            current: v,
        };
        ret;
    };

    let getLocalTime = (clock: float, anim: animation) => {
       let adjustedStart = anim.startTime +. anim.delay;
       let endTime = anim.startTime +. anim.duration;
       (clock -. adjustedStart) /. (endTime -. adjustedStart); 
    };

    let hasStarted = (clock: float, anim: animation) => {
        let t = getLocalTime(clock, anim);    
        t > 0.;
    };

    let isComplete = (clock: float, anim: animation) => {
        let t = getLocalTime(clock, anim);
        t > 1. && !anim.repeat;
    };
    let tickAnimation = (clock: float, anim: animation) => {
        let t = getLocalTime(clock, anim);

        /* If the anim is set to repeat and the time has expired, restart */
        if (anim.repeat && t > 1.) {
            /* reset */         

            anim.startTime = anim.startTime +. anim.delay +. anim.duration;
            let newT = getLocalTime(clock, anim);
            anim.value.current = interpolate(newT, anim.startValue, anim.toValue);
        } else {
            anim.value.current = interpolate(t, anim.startValue, anim.toValue);
        }
    };

    let getAnimationCount = () => {
        List.length(activeAnimations^);
    };

    let anyActiveAnimations = () => {
        let t = Time.to_float_seconds(AnimationTickerImpl.time());
        let anims = List.filter(hasStarted(t), activeAnimations^);   
        List.length(anims) > 0
    };

    let start = (animationValue: animationValue, animationOptions: animationOptions) => {
       let animation: animation = {
           delay: Time.to_float_seconds(animationOptions.delay),
           duration: Time.to_float_seconds(animationOptions.duration),
           toValue: animationOptions.toValue,
           repeat: animationOptions.repeat,
           value: animationValue,
           startTime: Time.to_float_seconds(AnimationTickerImpl.time()),
           startValue: animationValue.current,
       };

       activeAnimations := List.append(activeAnimations^, [animation]);
       animation
    }

    let tick = (t: float) => {
        List.iter(tickAnimation(t), activeAnimations^);

        activeAnimations := List.filter((a) => !isComplete(t, a), activeAnimations^);
    };

    Event.subscribe(AnimationTickerImpl.tick, (t) => {
        tick(Time.to_float_seconds(t));
        print_endline("TICK: " ++ Time.show(t));
    })
};
