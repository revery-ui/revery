open Revery_Core;
open Revery_Draw;
open Revery_UI;

type t = {
	backgroundColor: Color.t,
	width: int,
	height: int,
	surface: Skia.Surface.t,
	canvasContext: CanvasContext.t,
	rootNode: viewNode,
	container: ref(Container.t),
};

let create = (options: WindowCreateOptions.t) => {

	  let imageInfo = Skia.ImageInfo.make(
	  	options.width |> Int32.of_int, 
		  options.height |> Int32.of_int, 
		  Rgba8888, Premul, None);
	  let surface = Skia.Surface.makeRaster(imageInfo, 0, None);
	  let canvasContext = CanvasContext.createFromSurface(surface);

	let rootNode = (new viewNode)();

	let container = ref(Container.create(rootNode));

	{
	backgroundColor: options.backgroundColor,
	width: options.width,
	height: options.height,
	surface,
	canvasContext,
	rootNode,
	container,
	};
}

let render = (window: t, elem) => {
	let { backgroundColor,width, height, rootNode, container, canvasContext, _} = window;

	container := Container.update(container^, elem);

	CanvasContext.clear(
	~color=backgroundColor |> Color.toSkia, 
	canvasContext);

	rootNode#setStyle(
		Style.make(
			~position=LayoutTypes.Relative,
			~width,
			~height,
			()
		)
	);

	Layout.layout(~force=false, rootNode);
	rootNode#recalculate();

	let skiaRoot = Skia.Matrix.makeScale(
		1.0,
		1.0,
		0.,
		0.
	);
	CanvasContext.setRootTransform(skiaRoot, canvasContext);
	let drawContext = NodeDrawContext.create(
	~canvas=canvasContext,
	~zIndex=0,
	~opacity=1.0,
	()
	);

	rootNode#draw(drawContext);
};

let takeScreenshot = (_, _) => ();
