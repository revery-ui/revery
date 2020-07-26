open Revery_UI;
open Revery_UI_Primitives;
open Revery_Font;

let make =
    (
      ~style=[],
      ~smoothing=Smoothing.default,
      ~richtext: RichText.t,
      ~textWrap as textWrapping: Revery_Core.TextWrapping.wrapType=Wrap,
      (),
    ) => {
  let text =
    RichText.foldRight(
      (acc, {italic, fontFamily, fontSize, fontWeight, text, color}) =>
        [
          <Text
            style=[Style.color(color), Style.textWrap(textWrapping)]
            fontFamily
            fontWeight
            italic
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
