open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let sliderContainerStyle =
  Style.[
    margin(10),
    borderBottom(~width=1, ~color=Colors.darkGray),
    flexDirection(`Row),
    justifyContent(`Center),
    alignItems(`Center),
  ];
let controlsStyle =
  Style.[
    margin(10),
    flexDirection(`Row),
    justifyContent(`Center),
    alignItems(`Center),
  ];
let containerStyle =
  Style.[
    flexGrow(1),
    justifyContent(`Center),
    alignItems(`Center),
    flexDirection(`Column),
  ];

let textStyle =
  Style.[
    color(Colors.white),
    width(100),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(16),
    margin(14),
  ];

let logo = {
  let component = React.component("logo");

  (~children as _: list(React.syntheticElement), ()) =>
    component(hooks => {
      let (damping, setDamping, hooks) = Hooks.state(10., hooks);
      let (stiffness, setStiffness, hooks) = Hooks.state(160., hooks);
      let (springPosition, setSpringPosition, hooks) =
        Hooks.state(0.5, hooks);

      let (curr, hooks) =
        Hooks.spring(
          0.5,
          Hooks.Spring.Options.create(
            ~damping,
            ~springConstant=stiffness,
            springPosition,
          ),
          hooks,
        );

      let onMouseDown = _ => setSpringPosition(1.0);

      let onMouseUp = _ => setSpringPosition(0.5);

      let imageWidth = 512 |> float_of_int |> (s => s *. curr |> int_of_float);

      let float_to_string = v => v |> int_of_float |> string_of_int;

      (
        hooks,
        <Center>
          <View onMouseDown onMouseUp>
            <Image
              src="outrun-logo.png"
              style=Style.[width(imageWidth), height(256)]
            />
          </View>
          <View style=containerStyle>
            <View style=sliderContainerStyle>
              <Text style=textStyle text="Damping:" />
              <Slider
                onValueChanged=setDamping
                value=damping
                minimumValue=1.
                maximumValue=100.
              />
              <Text
                style=textStyle
                text={"Value: " ++ float_to_string(damping)}
              />
            </View>
            <View style=sliderContainerStyle>
              <Text style=textStyle text="Stiffness: " />
              <Slider
                onValueChanged=setStiffness
                value=stiffness
                minimumValue=1.
                maximumValue=500.
              />
              <Text
                style=textStyle
                text={"Stiffness: " ++ float_to_string(stiffness)}
              />
            </View>
          </View>
        </Center>,
      );
    });
};

let render = () => <Center> <logo /> </Center>;
