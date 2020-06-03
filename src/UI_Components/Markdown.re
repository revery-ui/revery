open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;
open Revery_Font;

open Omd;

module Log = (val Log.withNamespace("Revery.Components.Markdown"));

type style = {
  paragraph: list(Style.textStyleProps),
  link: list(Style.textStyleProps),
  h1: list(Style.textStyleProps),
  h2: list(Style.textStyleProps),
  h3: list(Style.textStyleProps),
  h4: list(Style.textStyleProps),
  h5: list(Style.textStyleProps),
  h6: list(Style.textStyleProps),
  fontFamily: Family.t,
  baseFontSize: float,
};

module Styles = {
  open Style;
  let inline = [flexDirection(`Row)];
  let container = [justifyContent(`FlexStart), alignItems(`FlexStart)];
};

type inlineAttrs =
  | Italicized
  | Bolded;

type kind = [ | `Paragraph | `Heading(int)];

let selectStyleFromKind = (kind: kind, styles) =>
  switch (kind) {
  | `Paragraph => styles.paragraph
  | `Heading(1) => styles.h1
  | `Heading(2) => styles.h2
  | `Heading(3) => styles.h3
  | `Heading(4) => styles.h4
  | `Heading(5) => styles.h5
  | `Heading(6)
  | `Heading(_) => styles.h6
  };

let fontSizeFromKind = (kind: kind, styles) =>
  switch (kind) {
  | `Heading(1) => styles.baseFontSize *. 2.
  | `Heading(2) => styles.baseFontSize *. 1.5
  | `Heading(3) => styles.baseFontSize *. 1.17
  | `Heading(4) => styles.baseFontSize *. 1.
  | `Heading(5) => styles.baseFontSize *. 0.83
  | `Heading(6)
  | `Heading(_) => styles.baseFontSize *. 0.67
  | _ => styles.baseFontSize
  };

type attrs = {
  inline: list(inlineAttrs),
  kind,
};

let isBold = attrs => List.mem(Bolded, attrs.inline);
let isItalicized = attrs => List.mem(Italicized, attrs.inline);

let generateText = (text, styles, attrs) => {
  let fontSize = fontSizeFromKind(attrs.kind, styles);

  <Text
    text
    fontSize
    fontFamily={styles.fontFamily}
    fontWeight={isBold(attrs) ? Weight.Bold : Weight.Normal}
    italicized={isItalicized(attrs)}
  />;
};

let rec _generateInline = (inline, styles, attrs) => {
  switch (inline) {
  | Text(t) => generateText(t, styles, attrs)
  | Emph(e) =>
    _generateInline(
      e.content,
      styles,
      switch (e.style) {
      | Star => {...attrs, inline: [Bolded, ...attrs.inline]}
      | Underscore => {...attrs, inline: [Italicized, ...attrs.inline]}
      },
    )
  | Hard_break => generateText("\n", styles, attrs)
  | Concat(c) =>
    c
    |> List.map(il => _generateInline(il, styles, attrs))
    |> React.listToElement
  | _ => <View />
  };
};

let generateInline = (inline, styles, attrs) =>
  <Row> {_generateInline(inline, styles, attrs)} </Row>;

let _generateMarkdown = (element, styles) =>
  switch (element) {
  | Paragraph(p) => generateInline(p, styles, {inline: [], kind: `Paragraph})
  | Heading(h) =>
    generateInline(
      h.text,
      styles,
      {inline: [Bolded], kind: `Heading(h.level)},
    )
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
      ~fontFamily=Family.default,
      ~baseFontSize=14.0,
      ~paragraphStyle=Style.emptyTextStyle,
      ~linkStyle=Style.emptyTextStyle,
      ~h1Style=Style.emptyTextStyle,
      ~h2Style=Style.emptyTextStyle,
      ~h3Style=Style.emptyTextStyle,
      ~h4Style=Style.emptyTextStyle,
      ~h5Style=Style.emptyTextStyle,
      ~h6Style=Style.emptyTextStyle,
      (),
    ) =>
  <View style=Styles.container>
    {generateMarkdown(
       mdText,
       {
         paragraph: paragraphStyle,
         link: linkStyle,
         h1: h1Style,
         h2: h2Style,
         h3: h3Style,
         h4: h4Style,
         h5: h5Style,
         h6: h6Style,
         fontFamily,
         baseFontSize,
       },
     )}
  </View>;
