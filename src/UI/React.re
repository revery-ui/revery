let onStale = Reconciler.onStale;

module React = Brisk_reconciler.Make(Reconciler);

let empty = React.listToElement([]);

include React;
