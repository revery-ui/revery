open Revery.UI;

module ImageSample = {
  let make = (~quality, ()) => {
    <View style=Style.[padding(8)]>
      <Image height=50 width=50 src={`File("reason-logo.png")} quality />
    </View>;
  };
};

module Images = {
  let make = () => {
    <View
      style=Style.[
        flexDirection(`Column),
        flexGrow(1),
        justifyContent(`Center),
        alignItems(`Center),
      ]>
      <ImageSample quality=`none />
      <ImageSample quality=`low />
      <ImageSample quality=`medium />
      <ImageSample quality=`high />
    </View>;
  };
};

let render = () => <Images />;
