open Revery;
open Revery.UI;

let parentWidth = 400;
let childWidth = 300;

let parentStyles =
    (~alignment as a=`Auto, ~direction as d=`Row, ~justify as j=`Center, ()) =>
  Style.[
    backgroundColor(Colors.green),
    position(`Relative),
    width(parentWidth),
    height(100),
    alignItems(a),
    justifyContent(j),
    flexDirection(d),
  ];

let childStyles =
  Style.[
    backgroundColor(Colors.blue),
    position(`Relative),
    width(childWidth),
    height(40),
  ];

let defaultTextStyles =
  Style.[color(Colors.white), backgroundColor(Colors.blue)];

let parentColumnStyle = parentStyles(~direction=`Column);
let headerStyle =
  Style.[marginTop(25), marginBottom(25), color(Colors.white)];

let horizontalStyles =
  <View>
    <Text style=headerStyle fontSize=30. text="Flex Direction Row" />
    <View style={parentColumnStyle(~alignment=`Auto, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Default Flex" />
      </View>
    </View>
    <View style={parentColumnStyle(~alignment=`Center, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Center" />
      </View>
    </View>
    <View style={parentColumnStyle(~alignment=`FlexStart, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Flex Start" />
      </View>
    </View>
    <View style={parentColumnStyle(~alignment=`FlexEnd, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Flex End" />
      </View>
    </View>
    <View style={parentColumnStyle(~alignment=`Stretch, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Flex Stretch" />
      </View>
    </View>
  </View>;

let verticalStyles =
  <View>
    <Text style=headerStyle fontSize=30. text="Flex Direction Column" />
    <View style={parentStyles(~direction=`Column, ~justify=`FlexStart, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Align Flex Start" />
      </View>
    </View>
    <View style={parentStyles(~direction=`Column, ~justify=`Center, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Align Flex Center" />
      </View>
    </View>
    <View style={parentStyles(~direction=`Column, ~justify=`FlexEnd, ())}>
      <View style=childStyles>
        <Text style=defaultTextStyles fontSize=20. text="Align Flex End" />
      </View>
    </View>
  </View>;

let render = () => <View> horizontalStyles verticalStyles </View>;
