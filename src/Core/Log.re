let info = (category, str) => {
  Timber.info(category ++ ": " ++ str);
};

let error = (category, str) => {
  Timber.error(category ++ ": " ++ str);
};

let perf = str => {
  Timber.debug(() => str);
};
