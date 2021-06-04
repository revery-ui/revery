module Animation = Animation;
module Spring = Spring;
module Easing = Easing;

module ImageResizeMode = ImageResizeMode;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Style = Style;
module Transform = Transform;
module Selector = Selector;
module RichText = RichText;

class node = class Node.node;
class layerNode = class LayerNode.layerNode;
class viewNode = class ViewNode.viewNode;
class textNode = class TextNode.textNode;
class imageNode = class ImageNode.imageNode;
class canvasNode = class CanvasNode.canvasNode;
class nativeButtonNode = class NativeButtonNode.nativeButtonNode;
module NodeDrawContext = NodeDrawContext;

module Keyboard = Keyboard;
module Mouse = Mouse;
module NodeEvents = NodeEvents;
module UiEvents = UiEvents;

module Container = Container;
module React = React;
module Focus = Focus;
module Dimensions = Dimensions;
module Offset = Offset;

module RenderCondition = RenderCondition;

type element = React.element(node);

let measureText =
    (
      ~smoothing=Revery_Font.Smoothing.default,
      ~width,
      ~style,
      ~fontFamily,
      ~fontWeight,
      ~fontSize,
      text,
    ) => {
  let node: TextNode.textNode = (new TextNode.textNode)(text);
  let styles = Style.create(~style, ());
  node#setStyle(styles);
  node#setFontFamily(fontFamily);
  node#setFontWeight(fontWeight);
  node#setFontSize(fontSize);
  node#setSmoothing(smoothing);

  let measureResult = node#measure(width, 0);
  measureResult.height;
};

include Ui;
