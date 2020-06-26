let persistState:
  (
    ~serialize: 'state => 'serializedState,
    ~deserialize: 'serializedState => 'state,
    unit => 'state,
    'state,
    unit
  ) =>
  unit;
