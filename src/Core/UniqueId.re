
module Make = () => {
  let current = ref(0);

  let getUniqueId = () => {
    let ret = current^;
    current := current^ + 1;
    ret;
  };
};
