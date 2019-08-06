open Revery_UI;
let component = React.nativeComponent("MountUnmount");

let make =
    (
      ~key=?,
      ~onMount=?,
      ~onUnmount=?,
      children,
    ) =>
  component(~key?, hooks =>
    (
      hooks,
      {
        make: () => {
          let node = (new mountUnmountNode)();
          node#setOnMount(onMount);
          node#setOnUnmount(onUnmount);
          Obj.magic(node);
        },
        configureInstance: (~isFirstRender as _, node) => {
          let n: mountUnmountNode = Obj.magic(node);
          n#setOnMount(onMount);
          n#setOnUnmount(onUnmount);
          node;
        },
        children,
      },
    )
  );

let createElement =
    (
      ~key=?,
      ~onMount=?,
      ~onUnmount=?,
      ~children,
      (),
    ) =>
  make(
    ~key?,
    ~onMount?,
    ~onUnmount?,
    React.listToElement(children),
  );
