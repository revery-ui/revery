open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

module Make = (Type: {type t;}) => {
  type button = {
    text: string,
    value: Type.t,
  };

  let defaultStyle =
    Style.[
      width(100),
      color(Colors.white),
      fontSize(16),
      marginLeft(10),
      fontFamily("Roboto-Regular.ttf"),
    ];

  let%component make =
                (
                  ~defaultSelected,
                  ~buttons: list(button),
                  ~iconSize,
                  ~style,
                  ~onChange,
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
               style=Style.[
                 fontSize(iconSize),
                 fontFamily("FontAwesome5FreeSolid.otf"),
               ]
             />
             <Text text={button.text} style />
           </Clickable>
         );

    <View style=Style.[justifyContent(`Center), alignItems(`Center)]>
      {buttons |> React.listToElement}
    </View>;
  };
};
