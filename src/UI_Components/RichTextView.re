open Revery_UI;
open Revery_UI_Primitives;
open Revery_Font;

let make =
    (~style=[], ~smoothing=Smoothing.default, ~richtext: RichText.t, ()) => {
  let text =
    RichText.foldRight(
      (
        acc,
        {italic, monospaced, fontFamily, fontSize, fontWeight, text, color},
      ) =>
        [
          <Text
            style=[Style.color(color)]
            fontFamily
            fontWeight
            italic
            monospaced
            fontSize
            text
            smoothing
          />,
          ...acc,
        ],
      [],
      richtext,
    );

  // TODO: Add alignItems(`Baseline) if that exists
  <View style=Style.[flexDirection(`Row), ...style]>
    {text |> React.listToElement}
  </View>;
};
