open Revery;
open Revery.UI;
open Revery.UI.Components;

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
  ];

module Example = {
  let%component make = () => {
    let%hook (url, setURL) =
      Hooks.state("https://github.com/revery-ui/revery");

    <View style=containerStyle>
      <Input
        placeholder="URL to open"
        onChange={(value, _) => setURL(_ => value)}
        value=url
      />
      <Button
        onClick={_ => Native.Shell.openURL(url) |> ignore}
        title="Open!"
      />
    </View>;
  };
};

let render = () => <Example />;
