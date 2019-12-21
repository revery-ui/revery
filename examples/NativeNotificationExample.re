open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module Example = {
  let make = () => {
    let notification =
      Notification.create(
        ~title="Revery Test",
        ~body="This is a test!",
        ~mute=false,
        ~onClick=() => Console.log("Notification clicked!"),
        (),
      );
    <View>
      <Center>
        <Button
          title="Dispatch!"
          onClick={() => Notification.dispatch(notification)}
        />
        <Button
          title="Schedule 5s From Now!"
          onClick={() => Notification.scheduleFromNow(5, notification)}
        />
      </Center>
    </View>;
  };
};

let render = () => <Example />;
