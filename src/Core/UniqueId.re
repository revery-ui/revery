module Make = (()) => {
  let current = ref(0);

  let getUniqueId = () => {
    let ret = current^;
    incr(current);
    ret;
  };
};
