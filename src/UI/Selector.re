open Revery_Core;
open Style;

type selector('a) =
  | Color: selector(Color.t)
  | FontFamily: selector(string)
  | FontSize: selector(int)
  | Width: selector(int)
  | FlexGrow: selector(int)
  | FlexWrap: selector(Layout.LayoutTypes.wrapType)
  | FlexDirection: selector(Layout.LayoutTypes.flexDirection)
  | JustifyContent: selector(Layout.LayoutTypes.justify)
  | AlignItems: selector(Layout.LayoutTypes.align)
  | Position: selector(Layout.LayoutTypes.positionType)
  | BackgroundColor: selector(Color.t)
  | Height: selector(int)
  | Top: selector(int)
  | Right: selector(int)
  | Bottom: selector(int)
  | Left: selector(int)
  | MarginTop: selector(int)
  | MarginLeft: selector(int)
  | MarginRight: selector(int)
  | MarginBottom: selector(int)
  | Margin: selector(int)
  | MarginVertical: selector(int)
  | MarginHorizontal: selector(int)
  | Margin2: selector(xy)
  | Margin4: selector(coords)
  | PaddingTop: selector(int)
  | PaddingLeft: selector(int)
  | PaddingRight: selector(int)
  | PaddingBottom: selector(int)
  | Padding: selector(int)
  | PaddingVertical: selector(int)
  | PaddingHorizontal: selector(int)
  | Padding2: selector(xy)
  | Padding4: selector(coords)
  | Overflow: selector(Layout.LayoutTypes.overflow)
  | BorderTop: selector(Border.t)
  | BorderLeft: selector(Border.t)
  | BorderRight: selector(Border.t)
  | BorderBottom: selector(Border.t)
  | Border: selector(Border.t)
  | BorderHorizontal: selector(Border.t)
  | BorderVertical: selector(Border.t)
  | Transform: selector(list(Transform.t))
  | Opacity: selector(float)
  | BoxShadow: selector(BoxShadow.properties)
  | Cursor: selector(option(MouseCursors.t));

/**
     This function pulls out (selects) a style property based on the matching Variant that is passed in
     it uses a GADT to correctly type the return value of the function by matching the variant passed in
   */
let rec select:
  type selectionType.
    (list(allProps), selector(selectionType), selectionType) => selectionType =
  (styles, selector, default) => {
    switch (styles, selector) {
    | ([], _) => default
    | ([`Color(c), ...rest], Color) => select(rest, selector, c)
    | ([`FontFamily(path), ...rest], FontFamily) =>
      select(rest, selector, path)
    | ([`FontSize(size), ...rest], FontSize) =>
      select(rest, selector, size)
    | ([`Width(size), ...rest], Width) => select(rest, selector, size)
    | ([`FlexGrow(grow), ...rest], FlexGrow) =>
      select(rest, selector, grow)
    | ([`FlexDirection(direction), ...rest], FlexDirection) =>
      select(rest, selector, direction)

    | ([`FlexWrap(wrap), ...rest], FlexWrap) =>
      select(rest, selector, wrap)

    | ([`JustifyContent(justify), ...rest], JustifyContent) =>
      select(rest, selector, justify)
    | ([`AlignItems(align), ...rest], AlignItems) =>
      select(rest, selector, align)
    | ([`Position(pos), ...rest], Position) => select(rest, selector, pos)
    | ([`BackgroundColor(col), ...rest], BackgroundColor) =>
      select(rest, selector, col)
    | ([`Height(h), ...rest], Height) => select(rest, selector, h)
    | ([`Top(t), ...rest], Top) => select(rest, selector, t)
    | ([`Right(r), ...rest], Right) => select(rest, selector, r)
    | ([`Bottom(b), ...rest], Bottom) => select(rest, selector, b)
    | ([`Left(l), ...rest], Left) => select(rest, selector, l)
    | ([`MarginTop(mt), ...rest], MarginTop) => select(rest, selector, mt)
    | ([`MarginLeft(ml), ...rest], MarginLeft) => select(rest, selector, ml)
    | ([`MarginRight(mr), ...rest], MarginRight) =>
      select(rest, selector, mr)
    | ([`MarginBottom(mb), ...rest], MarginBottom) =>
      select(rest, selector, mb)
    | ([`Margin(m), ...rest], Margin) => select(rest, selector, m)
    | ([`MarginVertical(mv), ...rest], MarginVertical) =>
      select(rest, selector, mv)
    | ([`MarginHorizontal(mh), ...rest], MarginHorizontal) =>
      select(rest, selector, mh)
    | ([`Margin2(m2), ...rest], Margin2) => select(rest, selector, m2)
    | ([`Margin4(m4), ...rest], Margin4) => select(rest, selector, m4)
    | ([`PaddingTop(mt), ...rest], PaddingTop) => select(rest, selector, mt)
    | ([`PaddingLeft(ml), ...rest], PaddingLeft) =>
      select(rest, selector, ml)
    | ([`PaddingRight(mr), ...rest], PaddingRight) =>
      select(rest, selector, mr)
    | ([`PaddingBottom(mb), ...rest], PaddingBottom) =>
      select(rest, selector, mb)
    | ([`Padding(m), ...rest], Padding) => select(rest, selector, m)
    | ([`PaddingVertical(mv), ...rest], PaddingVertical) =>
      select(rest, selector, mv)
    | ([`PaddingHorizontal(mh), ...rest], PaddingHorizontal) =>
      select(rest, selector, mh)
    | ([`Padding2(m2), ...rest], Padding2) => select(rest, selector, m2)
    | ([`Padding4(m4), ...rest], Padding4) => select(rest, selector, m4)
    | ([`Overflow(ovfl), ...rest], Overflow) =>
      select(rest, selector, ovfl)
    | ([`BorderTop(bt), ...rest], BorderTop) => select(rest, selector, bt)
    | ([`BorderLeft(bl), ...rest], BorderLeft) => select(rest, selector, bl)
    | ([`BorderRight(br), ...rest], BorderRight) =>
      select(rest, selector, br)
    | ([`BorderBottom(bb), ...rest], BorderBottom) =>
      select(rest, selector, bb)
    | ([`Border(b), ...rest], Border) => select(rest, selector, b)
    | ([`BorderHorizontal(bh), ...rest], BorderHorizontal) =>
      select(rest, selector, bh)
    | ([`BorderVertical(bv), ...rest], BorderVertical) =>
      select(rest, selector, bv)
    | ([`Transform(tr), ...rest], Transform) => select(rest, selector, tr)
    | ([`Opacity(op), ...rest], Opacity) => select(rest, selector, op)
    | ([`BoxShadow(bxsh), ...rest], BoxShadow) =>
      select(rest, selector, bxsh)
    | ([`Cursor(cur), ...rest], Cursor) => select(rest, selector, cur)
    | ([_, ...rest], _) => select(rest, selector, default)
    };
  };
