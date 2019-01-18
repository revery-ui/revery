open Revery.UI;
open Revery.Core;

let parentWidth = 400;
let childWidth = 300;

  let parentStyles =
      (
        ~alignment=LayoutTypes.AlignAuto,
        ~direction=LayoutTypes.Row,
        ~justify=LayoutTypes.JustifyCenter,
        (),
      ) =>
    Style.make(
      ~backgroundColor=Colors.green,
      ~position=LayoutTypes.Relative,
      ~width=parentWidth,
      ~height=100,
      ~alignItems=alignment,
      ~justifyContent=justify,
      ~flexDirection=direction,
      (),
    );

  let childStyles =
    Style.make(
      ~backgroundColor=Colors.blue,
      ~position=LayoutTypes.Relative,
      ~width=childWidth,
      ~height=40,
      (),
    );

  let defaultTextStyles =
    Style.make(
      ~fontSize=20,
      ~fontFamily="Roboto-Regular.ttf",
      ~color=Colors.white,
      ~backgroundColor=Colors.blue,
      (),
    );

  let parentColumnStyle = parentStyles(~direction=LayoutTypes.Column);
  let headerStyle =
    Style.make(
      ~marginTop=25,
      ~marginBottom=25,
      ~fontSize=30,
      ~fontFamily="Roboto-Regular.ttf",
      ~color=Colors.white,
      (),
    );

  let horizontalStyles =
    <View>
      <Text style=headerStyle text="Flex Direction Row" />
      <View style={parentColumnStyle(~alignment=LayoutTypes.AlignAuto, ())}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Default Flex" />
        </View>
      </View>
      <View style={parentColumnStyle(~alignment=LayoutTypes.AlignCenter, ())}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Center" />
        </View>
      </View>
      <View
        style={parentColumnStyle(~alignment=LayoutTypes.AlignFlexStart, ())}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Flex Start" />
        </View>
      </View>
      <View
        style={parentColumnStyle(~alignment=LayoutTypes.AlignFlexEnd, ())}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Flex End" />
        </View>
      </View>
      <View
        style={parentColumnStyle(~alignment=LayoutTypes.AlignStretch, ())}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Flex Stretch" />
        </View>
      </View>
    </View>;

  let verticalStyles =
    <View>
      <Text style=headerStyle text="Flex Direction Column" />
      <View
        style={parentStyles(
          ~direction=LayoutTypes.Column,
          ~justify=LayoutTypes.JustifyFlexStart,
          (),
        )}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Align Flex Start" />
        </View>
      </View>
      <View
        style={parentStyles(
          ~direction=LayoutTypes.Column,
          ~justify=LayoutTypes.JustifyCenter,
          (),
        )}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Align Flex Center" />
        </View>
      </View>
      <View
        style={parentStyles(
          ~direction=LayoutTypes.Column,
          ~justify=LayoutTypes.JustifyFlexEnd,
          (),
        )}>
        <View style=childStyles>
          <Text style=defaultTextStyles text="Align Flex End" />
        </View>
      </View>
    </View>;

  let render = () => <View> horizontalStyles verticalStyles </View>;
