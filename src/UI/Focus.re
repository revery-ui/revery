/* https://dom.spec.whatwg.org/#dispatching-events */

module Make = (T: {type t;}) => {
  let focused: ref(option(Node.node(T.t))) = ref(None);

	let focus = (node: Node.node(T.t)) => {
		let _ = node#focus();
		focused := Some(node);
	};

  let dispatch = (node: Node.node(T.t)) => {
		switch (focused^) {
			| None => focus(node)
			| Some(focused) => {
				if (node#getInternalId() === focused#getInternalId()) {
					()
				} else {
					focus(node)
				}
			}	
		};
  };
};

