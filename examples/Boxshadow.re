open Revery.UI;
open Revery.Core;

  let parentStyles =
    Style.make(
      ~position=LayoutTypes.Relative,
      ~flexGrow=1,
      ~alignItems=LayoutTypes.AlignCenter,
      ~justifyContent=LayoutTypes.JustifySpaceBetween,
      ~flexDirection=LayoutTypes.Column,
      (),
    );

  let shadowOne =
    Style.BoxShadow.make(
      ~yOffset=-10.,
      ~xOffset=10.,
      ~blurRadius=40.,
      ~color=Colors.black,
      ~spreadRadius=20.,
      (),
    );

  let shadowTwo =
    Style.BoxShadow.make(
      ~yOffset=10.,
      ~xOffset=-30.,
      ~blurRadius=50.,
      ~color=Colors.green,
      ~spreadRadius=0.,
      (),
    );

  let firstShadow =
    Style.make(
      ~backgroundColor=Colors.blue,
      ~position=LayoutTypes.Relative,
      ~width=500,
      ~height=480,
      ~boxShadow=shadowOne,
      ~marginTop=20,
      (),
    );

  let secondShadow =
    Style.make(
      ~backgroundColor=Colors.red,
      ~position=LayoutTypes.Relative,
      ~width=500,
      ~height=480,
      ~boxShadow=shadowTwo,
      (),
    );

let render = () => {
    <View style=parentStyles>
      <View style=firstShadow />
      <View style=secondShadow />
    </View>
  };
