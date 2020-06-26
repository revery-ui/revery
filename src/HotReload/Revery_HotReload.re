open Revery_Core;

let persistState =
    (
      ~serialize: 'state => 'serializedState,
      ~deserialize: 'serializedState => 'state,
      getState: unit => 'state,
      initialState: 'state,
    ) => {
  let currentState = ref(initialState);

  let onUpdate = () => currentState := getState();

  onUpdate;
};
