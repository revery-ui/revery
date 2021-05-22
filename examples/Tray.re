open Revery;
open Revery.UI;
open Revery.UI.Components;

let make = () => {
  Native.Tray.make(
    ~imagePath=Environment.getAssetPath("outrun-logo.png"),
    (),
  )
  |> ignore;

  Native.Tray.make() |> Native.Tray.setTitle(~text="Hello Revery!");

  <Center>
    <Text
      text="You should see two items in your tray-bar. One image and one with
  the text \"Hello Revery!\""
    />
  </Center>;
};

let render = () => make();
