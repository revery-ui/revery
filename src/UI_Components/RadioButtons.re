open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

module Make = (Type: {type t;}) => {
  type button = {
    text: string,
    value: Type.t,
  };

  let defaultStyle =
    Style.[width(100), color(Colors.white), marginLeft(10)];

  let%component make =
                (
                  ~defaultSelected,
                  ~buttons: list(button),
                  ~iconSize=12.,
                  ~style=defaultStyle,
                  ~onChange=_ => (),
                  (),
                ) => {
    let defaultVal = List.nth(buttons, defaultSelected).value;
    let%hook (checkedVal, setCheckedVal) = React.Hooks.state(defaultVal);

    let icon = v => v == checkedVal ? {||} : {||};

    let buttons =
      buttons
      |> List.map(button =>
           <Clickable
             onClick={() => {
               setCheckedVal(_ => button.value);
               onChange(button.value);
             }}
             style=Style.[
               justifyContent(`Center),
               flexDirection(`Row),
               alignItems(`Center),
               height(30),
             ]>
             <Text
               text={icon(button.value)}
               fontSize=iconSize
               fontFamily={Revery_Font.Family.fromFile(
                 "FontAwesome5FreeSolid.otf",
               )}
               style=Style.[textWrap(TextWrapping.NoWrap)]
             />
             <Text text={button.text} style fontSize=16. />
           </Clickable>
         );

    <View style=Style.[justifyContent(`Center), alignItems(`Center)]>
      {buttons |> React.listToElement}
    </View>;
  };
};
