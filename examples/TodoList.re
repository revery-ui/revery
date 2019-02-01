open Revery.Core;
open Revery.UI;
/*open Revery.UI.Components; */

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
    backgroundColor(Colors.white),
  ];
let viewStyle =
  Style.[
    alignItems(`Center),
    backgroundColor(Colors.white),
    position(`Relative),
    width(400),
    height(400),
    boxShadow(
      ~yOffset=-7.,
      ~xOffset=-7.,
      ~blurRadius=20.,
      ~color=Color.hex("#ccc"),
      ~spreadRadius=0.,
    ),
    marginVertical(30),
  ];

let textStyle = Style.[fontSize(20), fontFamily("Roboto-Regular.ttf"), color(Colors.black), marginTop(35)];

module TodoList = {
  let component = React.component("TodoList");
  let make = (_children) => 
    component((_slots: React.Hooks.empty) => {
     /*let (todos, addTodo, _slots: React.Hooks.empty) = 
       React.Hooks.state(["groceries", "dishes", "poop"], slots); */
     <View style=containerStyle>
      <View 
        style=viewStyle
      >
       <Text style=textStyle text="Todo List"/>

      </View>  
     </View> 
    });
  let createElement = (~children as _, ()) => React.element(make());  
};

let render = () => <TodoList />;
