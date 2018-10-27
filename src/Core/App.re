open Reglfw.Glfw;

type reducer('s, 'a) = ('s, 'a) => 's;

type t('s, 'a) = {
    reducer: reducer('s, 'a),
    mutable state: 's,
    mutable windows: list(Window.t)
};

/* If no state is specified, just use unit! */
let defaultState = ();

/* If no reducer is specified, just pass through state */
let defaultReducer: reducer(unit, unit) = (_s, _a) => ()

type appInitFunc('s, 'a) = (t('s, 'a)) => unit;

type startFunc('s, 'a) = ('s, reducer('s, 'a), appInitFunc('s, 'a)) => unit;

let getWindows = (app: t('s, 'a)) => {
    app.windows
};

let getState = (app: t('s, 'a)) => {
    app.state
};

let dispatch = (app: t('s, 'a), action: 'a) => {
    let newState = app.reducer(app.state, action);
    app.state = newState;
};

let createWindow = (~createOptions=Window.defaultCreateOptions, app: t('s, 'a), windowName) => {
    let w = Window.create(windowName, createOptions);
    /* Window.render(w) */
    app.windows = [w, ...app.windows];
    w;
};

let startWithState: startFunc('s, 'a) = (initialState: 's, reducer: reducer('s, 'a), initFunc: appInitFunc('s, 'a)) => {
    let appInstance: t('s, 'a) = {
        reducer: reducer,
        state: initialState,
        windows: [],
    };

    let _ = glfwInit();
    let _ = initFunc(appInstance);

    let appLoop = (_t: float) => {

        List.iter((w) => Window.render(w), getWindows(appInstance));
        glfwPollEvents();
        Unix.sleepf(1. /. 100.);
        false
    };

    glfwRenderLoop(appLoop);
}

let start = (initFunc: appInitFunc(unit, unit)) => startWithState(defaultState, defaultReducer, initFunc);
