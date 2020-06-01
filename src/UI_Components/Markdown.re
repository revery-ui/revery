open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

open Omd;

module Log = (val Log.withNamespace("Revery.Components.Markdown"));

type styles = {
  paragraph: list(Style.textStyleProps),
  bold: list(Style.textStyleProps),
  italic: list(Style.textStyleProps),
  h1: list(Style.textStyleProps),
  h2: list(Style.textStyleProps),
  h3: list(Style.textStyleProps),
  h4: list(Style.textStyleProps),
  h5: list(Style.textStyleProps),
};

module Styles = {
  open Style;
  let inline = [flexDirection(`Row)];
  let container = [justifyContent(`FlexStart), alignItems(`FlexStart)];
};

module InlineAttrs = {
  type t =
    | Italicized
    | Bolded
    | Header(int);

  let attrToStyle = (attr: t, styles: styles) =>
    switch (attr) {
    | Italicized => styles.italic
    | Bolded => styles.bold
    | Header(1) => styles.h1
    | Header(2) => styles.h2
    | Header(3) => styles.h3
    | Header(4) => styles.h4
    | Header(5) => styles.h5
    | Header(_) => styles.bold
    };

  let toStyle = (attrs, styles) =>
    List.fold_right(
      (attr, acc) => {
        let style = attrToStyle(attr, styles);
        Style.merge(style, acc);
      },
      attrs,
      styles.paragraph,
    );

  let compare = compare;
};

let generateText = (text, styles, attrs) => {
  let style = InlineAttrs.toStyle(attrs, styles);

  <Text text style />;
};

let rec _generateInline = (inline, styles, attrs) => {
  InlineAttrs.(
    switch (inline) {
    | Text(t) => generateText(t, styles, attrs)
    | Emph(e) =>
      _generateInline(
        e.content,
        styles,
        switch (e.style) {
        | Star => [Bolded, ...attrs]
        | Underscore => [Italicized, ...attrs]
        },
      )
    | Concat(c) =>
      c
      |> List.map(il => _generateInline(il, styles, attrs))
      |> React.listToElement
    | _ => <View />
    }
  );
};

let generateInline = (inline, styles, attrs) =>
  <Row> {_generateInline(inline, styles, attrs)} </Row>;

let _generateMarkdown = (element, styles) =>
  switch (element) {
  | Paragraph(p) => generateInline(p, styles, [])
  | Heading(h) => generateInline(h.text, styles, [Header(h.level)])
  | _ => <View />
  };

let generateMarkdown = (mdText: string, styles) => {
  let md = Omd.of_string(mdText);
  Log.debugf(m => m("Parsed Markdown as: %s", Omd.to_sexp(md)));
  List.map(elt => _generateMarkdown(elt, styles), md) |> React.listToElement;
};

let make =
    (
      ~markdown as mdText="",
      ~paragraphStyle=Style.emptyTextStyle,
      ~boldStyle=Style.emptyTextStyle,
      ~italicStyle=Style.emptyTextStyle,
      ~h1Style=Style.emptyTextStyle,
      ~h2Style=Style.emptyTextStyle,
      ~h3Style=Style.emptyTextStyle,
      ~h4Style=Style.emptyTextStyle,
      ~h5Style=Style.emptyTextStyle,
      (),
    ) =>
  <View style=Styles.container>
    {generateMarkdown(
       mdText,
       {
         paragraph: paragraphStyle,
         bold: boldStyle,
         italic: italicStyle,
         h1: h1Style,
         h2: h2Style,
         h3: h3Style,
         h4: h4Style,
         h5: h5Style,
       },
     )}
  </View>;
