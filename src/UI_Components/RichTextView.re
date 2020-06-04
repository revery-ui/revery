open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

let make = (~style=[], ~richtext: RichText.t, ()) => {
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
            smoothing={textInfo.smoothing}
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
