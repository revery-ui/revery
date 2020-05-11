external _openItem: string => bool = "revery_openItemAtURL";

let openItem = (url: string) =>
  switch (url) {
  | _ => _openItem(url)
  };
