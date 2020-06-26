let persistState:
  (
    ~serialize: 'state => 'serializedState,
    ~deserialize: 'serializedState => 'state,
    'state
  ) =>
  ('state, 'state => unit);
