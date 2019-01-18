open Revery.UI;
open Revery.Core;

let parentStyles =
  Style.make(
    ~position=LayoutTypes.Relative,
    ~flexGrow=1,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyCenter,
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
    ~width=100,
    ~height=100,
    ~boxShadow=shadowOne,
    ~marginBottom=30,
    (),
  );

let secondShadow =
  Style.make(
    ~backgroundColor=Colors.red,
    ~position=LayoutTypes.Relative,
    ~width=100,
    ~height=100,
    ~boxShadow=shadowTwo,
    (),
  );

let render = () => {
  <View style=parentStyles>
    <View style=firstShadow />
    <View style=secondShadow />
  </View>;
};
