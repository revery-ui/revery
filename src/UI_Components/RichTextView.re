open Revery_UI;
open Revery_UI_Primitives;
open Revery_Font;

let make =
    (~style=[], ~smoothing=Smoothing.default, ~richtext: RichText.t, ()) => {
  let text =
    RichText.foldRight(
      (acc, textInfo) =>
        [
          <Text
            style=Style.[color(textInfo.color)]
            fontFamily={textInfo.fontFamily}
            fontWeight={textInfo.fontWeight}
            italicized={textInfo.italicized}
            monospaced={textInfo.monospaced}
            fontSize={textInfo.fontSize}
            text={textInfo.text}
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
