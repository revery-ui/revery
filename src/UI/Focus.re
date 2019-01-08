/* https://dom.spec.whatwg.org/#dispatching-events */
module Make = (T: {type t;}) => {
	open NodeEvents;
  let focused: ref(option(Node.node(T.t))) = ref(None);

	/* Should happen when user clicks anywhere where no focusable node exists */
	let looseFocus = () => switch (focused^) {
		| Some(node) => {
			let _ = node#handleEvent(Blur());
			focused := None;
		}
		| None => ()
	};

	let focus = (node: Node.node(T.t)) => {
		let _ = node#handleEvent(Focus());
		focused := Some(node);
	};
  /* TODO perform checks if a node can be focused ? */
  let dispatch = (node: Node.node(T.t)) => switch (focused^) {
		| Some(focused) => {
			if (node#is(focused)) {
				()
			} else {
				looseFocus();
				focus(node)
			}
		}
		| None => focus(node)
	};
};

include Make({ type t = RenderPass.t });
