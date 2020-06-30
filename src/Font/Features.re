type t =
  | AccessAllAlternates
  | AboveBaseForms
  | AboveBaseMarkPositioning
  | AboveBaseSubstitutions
  | AlternativeFractions
  | Akhands
  | BelowBaseForms
  | BelowBaseMarkPositioning
  | BelowBaseSubstitutions
  | ContextualAlternates
  | CaseSensitiveForms
  | GlyphComposition
  | ConjunctFormAfterRo
  | ConjunctForms
  | ContextualLigatures
  | CenteredCjkPunctuation
  | CapitalSpacing
  | ContextualSwash
  | CursivePositioning
  | CharacterVariants1
  | CharacterVariants2
  | CharacterVariants3
  | CharacterVariants4
  | CharacterVariants5
  | CharacterVariants6
  | CharacterVariants7
  | CharacterVariants8
  | CharacterVariants9
  | CharacterVariants10
  | CharacterVariants11
  | CharacterVariants12
  | CharacterVariants13
  | CharacterVariants14
  | CharacterVariants15
  | CharacterVariants16
  | CharacterVariants17
  | CharacterVariants18
  | CharacterVariants19
  | CharacterVariants20
  | CharacterVariants21
  | CharacterVariants22
  | CharacterVariants23
  | CharacterVariants24
  | CharacterVariants25
  | CharacterVariants26
  | CharacterVariants27
  | CharacterVariants28
  | CharacterVariants29
  | CharacterVariants30
  | CharacterVariants31
  | CharacterVariants32
  | CharacterVariants33
  | CharacterVariants34
  | CharacterVariants35
  | CharacterVariants36
  | CharacterVariants37
  | CharacterVariants38
  | CharacterVariants39
  | CharacterVariants40
  | CharacterVariants41
  | CharacterVariants42
  | CharacterVariants43
  | CharacterVariants44
  | CharacterVariants45
  | CharacterVariants46
  | CharacterVariants47
  | CharacterVariants48
  | CharacterVariants49
  | CharacterVariants50
  | CharacterVariants51
  | CharacterVariants52
  | CharacterVariants53
  | CharacterVariants54
  | CharacterVariants55
  | CharacterVariants56
  | CharacterVariants57
  | CharacterVariants58
  | CharacterVariants59
  | CharacterVariants60
  | CharacterVariants61
  | CharacterVariants62
  | CharacterVariants63
  | CharacterVariants64
  | CharacterVariants65
  | CharacterVariants66
  | CharacterVariants67
  | CharacterVariants68
  | CharacterVariants69
  | CharacterVariants70
  | CharacterVariants71
  | CharacterVariants72
  | CharacterVariants73
  | CharacterVariants74
  | CharacterVariants75
  | CharacterVariants76
  | CharacterVariants77
  | CharacterVariants78
  | CharacterVariants79
  | CharacterVariants80
  | CharacterVariants81
  | CharacterVariants82
  | CharacterVariants83
  | CharacterVariants84
  | CharacterVariants85
  | CharacterVariants86
  | CharacterVariants87
  | CharacterVariants88
  | CharacterVariants89
  | CharacterVariants90
  | CharacterVariants91
  | CharacterVariants92
  | CharacterVariants93
  | CharacterVariants94
  | CharacterVariants95
  | CharacterVariants96
  | CharacterVariants97
  | CharacterVariants98
  | CharacterVariants99
  | PetiteCapitalsFromCapitals
  | SmallCapitalsFromCapitals
  | Distances
  | DiscretionaryLigatures
  | Denominators
  | DotlessForms
  | ExpertForms
  | FinalGlyphOnLineAlternates
  | TerminalForms2
  | TerminalForms3
  | TerminalForms
  | FlattenedAccentForms
  | Fractions
  | FullWidths
  | HalfForms
  | HalantForms
  | AlternateHalfWidths
  | HistoricalForms
  | HorizontalKanaAlternates
  | HistoricalLigatures
  | Hangul
  | HojoKanjiForms
  | HalfWidths
  | InitialForms
  | IsolatedForms
  | Italics
  | JustificationAlternates
  | Jis78Forms
  | Jis83Forms
  | Jis90Forms
  | Jis2004Forms
  | Kerning
  | LeftBounds
  | StandardLigatures
  | LeadingJamoForms
  | LiningFigures
  | LocalizedForms
  | LeftToRightAlternates
  | LeftToRightMirroredForms
  | MarkPositioning
  | MedialForms2
  | MedialForms
  | MathematicalGreek
  | MarkToMarkPositioning
  | MarkPositioningViaSubstitution
  | AlternateAnnotationForms
  | NlcKanjiForms
  | NuktaForms
  | Numerators
  | OldstyleFigures
  | OpticalBounds
  | Ordinals
  | Ornaments
  | ProportionalAlternateWidths
  | PetiteCapitals
  | ProportionalKana
  | ProportionalFigures
  | PreBaseForms
  | PreBaseSubstitutions
  | PostBaseForms
  | PostBaseSubstitutions
  | ProportionalWidths
  | QuarterWidths
  | Randomize
  | RequiredContextualAlternates
  | RakarForms
  | RequiredLigatures
  | RephForms
  | RightBounds
  | RightToLeftAlternates
  | RightToLeftMirroredForms
  | RubyNotationForms
  | RequiredVariationAlternates
  | StylisticAlternates
  | ScientificInferiors
  | OpticalSize
  | SmallCapitals
  | SimplifiedForms
  | StylisticSet1
  | StylisticSet2
  | StylisticSet3
  | StylisticSet4
  | StylisticSet5
  | StylisticSet6
  | StylisticSet7
  | StylisticSet8
  | StylisticSet9
  | StylisticSet10
  | StylisticSet11
  | StylisticSet12
  | StylisticSet13
  | StylisticSet14
  | StylisticSet15
  | StylisticSet16
  | StylisticSet17
  | StylisticSet18
  | StylisticSet19
  | StylisticSet20
  | MathScriptStyleAlternates
  | StretchingGlyphDecomposition
  | Subscript
  | Superscript
  | Swash
  | Titling
  | TrailingJamoForms
  | TraditionalNameForms
  | TabularFigures
  | TraditionalForms
  | ThirdWidths
  | Unicase
  | AlternateVerticalMetrics
  | VattuVariants
  | VerticalWriting
  | AlternateVerticalHalfMetrics
  | VowelJamoForms
  | VerticalKanaAlternates
  | VerticalKerning
  | ProportionalAlternateVerticalMetrics
  | VerticalAlternatesAndRotation
  | VerticalAlternatesForRotation
  | SlashedZero;

let getCode = feature =>
  switch (feature) {
  | AccessAllAlternates => "aalt"
  | AboveBaseForms => "abvf"
  | AboveBaseMarkPositioning => "abvm"
  | AboveBaseSubstitutions => "abvs"
  | AlternativeFractions => "afrc"
  | Akhands => "akhn"
  | BelowBaseForms => "blwf"
  | BelowBaseMarkPositioning => "blwm"
  | BelowBaseSubstitutions => "blws"
  | ContextualAlternates => "calt"
  | CaseSensitiveForms => "case"
  | GlyphComposition => "ccmp"
  | ConjunctFormAfterRo => "cfar"
  | ConjunctForms => "cjct"
  | ContextualLigatures => "clig"
  | CenteredCjkPunctuation => "cpct"
  | CapitalSpacing => "cpsp"
  | ContextualSwash => "cswh"
  | CursivePositioning => "curs"
  | CharacterVariants1 => "cv1"
  | CharacterVariants2 => "cv2"
  | CharacterVariants3 => "cv3"
  | CharacterVariants4 => "cv4"
  | CharacterVariants5 => "cv5"
  | CharacterVariants6 => "cv6"
  | CharacterVariants7 => "cv7"
  | CharacterVariants8 => "cv8"
  | CharacterVariants9 => "cv9"
  | CharacterVariants10 => "cv10"
  | CharacterVariants11 => "cv11"
  | CharacterVariants12 => "cv12"
  | CharacterVariants13 => "cv13"
  | CharacterVariants14 => "cv14"
  | CharacterVariants15 => "cv15"
  | CharacterVariants16 => "cv16"
  | CharacterVariants17 => "cv17"
  | CharacterVariants18 => "cv18"
  | CharacterVariants19 => "cv19"
  | CharacterVariants20 => "cv20"
  | CharacterVariants21 => "cv21"
  | CharacterVariants22 => "cv22"
  | CharacterVariants23 => "cv23"
  | CharacterVariants24 => "cv24"
  | CharacterVariants25 => "cv25"
  | CharacterVariants26 => "cv26"
  | CharacterVariants27 => "cv27"
  | CharacterVariants28 => "cv28"
  | CharacterVariants29 => "cv29"
  | CharacterVariants30 => "cv30"
  | CharacterVariants31 => "cv31"
  | CharacterVariants32 => "cv32"
  | CharacterVariants33 => "cv33"
  | CharacterVariants34 => "cv34"
  | CharacterVariants35 => "cv35"
  | CharacterVariants36 => "cv36"
  | CharacterVariants37 => "cv37"
  | CharacterVariants38 => "cv38"
  | CharacterVariants39 => "cv39"
  | CharacterVariants40 => "cv40"
  | CharacterVariants41 => "cv41"
  | CharacterVariants42 => "cv42"
  | CharacterVariants43 => "cv43"
  | CharacterVariants44 => "cv44"
  | CharacterVariants45 => "cv45"
  | CharacterVariants46 => "cv46"
  | CharacterVariants47 => "cv47"
  | CharacterVariants48 => "cv48"
  | CharacterVariants49 => "cv49"
  | CharacterVariants50 => "cv50"
  | CharacterVariants51 => "cv51"
  | CharacterVariants52 => "cv52"
  | CharacterVariants53 => "cv53"
  | CharacterVariants54 => "cv54"
  | CharacterVariants55 => "cv55"
  | CharacterVariants56 => "cv56"
  | CharacterVariants57 => "cv57"
  | CharacterVariants58 => "cv58"
  | CharacterVariants59 => "cv59"
  | CharacterVariants60 => "cv60"
  | CharacterVariants61 => "cv61"
  | CharacterVariants62 => "cv62"
  | CharacterVariants63 => "cv63"
  | CharacterVariants64 => "cv64"
  | CharacterVariants65 => "cv65"
  | CharacterVariants66 => "cv66"
  | CharacterVariants67 => "cv67"
  | CharacterVariants68 => "cv68"
  | CharacterVariants69 => "cv69"
  | CharacterVariants70 => "cv70"
  | CharacterVariants71 => "cv71"
  | CharacterVariants72 => "cv72"
  | CharacterVariants73 => "cv73"
  | CharacterVariants74 => "cv74"
  | CharacterVariants75 => "cv75"
  | CharacterVariants76 => "cv76"
  | CharacterVariants77 => "cv77"
  | CharacterVariants78 => "cv78"
  | CharacterVariants79 => "cv79"
  | CharacterVariants80 => "cv80"
  | CharacterVariants81 => "cv81"
  | CharacterVariants82 => "cv82"
  | CharacterVariants83 => "cv83"
  | CharacterVariants84 => "cv84"
  | CharacterVariants85 => "cv85"
  | CharacterVariants86 => "cv86"
  | CharacterVariants87 => "cv87"
  | CharacterVariants88 => "cv88"
  | CharacterVariants89 => "cv89"
  | CharacterVariants90 => "cv90"
  | CharacterVariants91 => "cv91"
  | CharacterVariants92 => "cv92"
  | CharacterVariants93 => "cv93"
  | CharacterVariants94 => "cv94"
  | CharacterVariants95 => "cv95"
  | CharacterVariants96 => "cv96"
  | CharacterVariants97 => "cv97"
  | CharacterVariants98 => "cv98"
  | CharacterVariants99 => "cv99"
  | PetiteCapitalsFromCapitals => "c2pc"
  | SmallCapitalsFromCapitals => "c2sc"
  | Distances => "dist"
  | DiscretionaryLigatures => "dlig"
  | Denominators => "dnom"
  | DotlessForms => "dtls"
  | ExpertForms => "expt"
  | FinalGlyphOnLineAlternates => "falt"
  | TerminalForms2 => "fin2"
  | TerminalForms3 => "fin3"
  | TerminalForms => "fina"
  | FlattenedAccentForms => "flac"
  | Fractions => "frac"
  | FullWidths => "fwid"
  | HalfForms => "half"
  | HalantForms => "haln"
  | AlternateHalfWidths => "halt"
  | HistoricalForms => "hist"
  | HorizontalKanaAlternates => "hkna"
  | HistoricalLigatures => "hlig"
  | Hangul => "hngl"
  | HojoKanjiForms => "hojo"
  | HalfWidths => "hwid"
  | InitialForms => "init"
  | IsolatedForms => "isol"
  | Italics => "ital"
  | JustificationAlternates => "jalt"
  | Jis78Forms => "jp78"
  | Jis83Forms => "jp83"
  | Jis90Forms => "jp90"
  | Jis2004Forms => "jp04"
  | Kerning => "kern"
  | LeftBounds => "lfbd"
  | StandardLigatures => "liga"
  | LeadingJamoForms => "ljmo"
  | LiningFigures => "lnum"
  | LocalizedForms => "locl"
  | LeftToRightAlternates => "ltra"
  | LeftToRightMirroredForms => "ltrm"
  | MarkPositioning => "mark"
  | MedialForms2 => "med2"
  | MedialForms => "medi"
  | MathematicalGreek => "mgrk"
  | MarkToMarkPositioning => "mkmk"
  | MarkPositioningViaSubstitution => "mset"
  | AlternateAnnotationForms => "nalt"
  | NlcKanjiForms => "nlck"
  | NuktaForms => "nukt"
  | Numerators => "numr"
  | OldstyleFigures => "onum"
  | OpticalBounds => "opbd"
  | Ordinals => "ordn"
  | Ornaments => "ornm"
  | ProportionalAlternateWidths => "palt"
  | PetiteCapitals => "pcap"
  | ProportionalKana => "pkna"
  | ProportionalFigures => "pnum"
  | PreBaseForms => "pref"
  | PreBaseSubstitutions => "pres"
  | PostBaseForms => "pstf"
  | PostBaseSubstitutions => "psts"
  | ProportionalWidths => "pwid"
  | QuarterWidths => "qwid"
  | Randomize => "rand"
  | RequiredContextualAlternates => "rclt"
  | RakarForms => "rkrf"
  | RequiredLigatures => "rlig"
  | RephForms => "rphf"
  | RightBounds => "rtbd"
  | RightToLeftAlternates => "rtla"
  | RightToLeftMirroredForms => "rtlm"
  | RubyNotationForms => "ruby"
  | RequiredVariationAlternates => "rvrn"
  | StylisticAlternates => "salt"
  | ScientificInferiors => "sinf"
  | OpticalSize => "size"
  | SmallCapitals => "smcp"
  | SimplifiedForms => "smpl"
  | StylisticSet1 => "ss01"
  | StylisticSet2 => "ss02"
  | StylisticSet3 => "ss03"
  | StylisticSet4 => "ss04"
  | StylisticSet5 => "ss05"
  | StylisticSet6 => "ss06"
  | StylisticSet7 => "ss07"
  | StylisticSet8 => "ss08"
  | StylisticSet9 => "ss09"
  | StylisticSet10 => "ss10"
  | StylisticSet11 => "ss11"
  | StylisticSet12 => "ss12"
  | StylisticSet13 => "ss13"
  | StylisticSet14 => "ss14"
  | StylisticSet15 => "ss15"
  | StylisticSet16 => "ss16"
  | StylisticSet17 => "ss17"
  | StylisticSet18 => "ss18"
  | StylisticSet19 => "ss19"
  | StylisticSet20 => "ss20"
  | MathScriptStyleAlternates => "ssty"
  | StretchingGlyphDecomposition => "stch"
  | Subscript => "subs"
  | Superscript => "sups"
  | Swash => "swsh"
  | Titling => "titl"
  | TrailingJamoForms => "tjmo"
  | TraditionalNameForms => "tnam"
  | TabularFigures => "tnum"
  | TraditionalForms => "trad"
  | ThirdWidths => "twid"
  | Unicase => "unic"
  | AlternateVerticalMetrics => "valt"
  | VattuVariants => "vatu"
  | VerticalWriting => "vert"
  | AlternateVerticalHalfMetrics => "vhal"
  | VowelJamoForms => "vjmo"
  | VerticalKanaAlternates => "vkna"
  | VerticalKerning => "vkrn"
  | ProportionalAlternateVerticalMetrics => "vpal"
  | VerticalAlternatesAndRotation => "vrt2"
  | VerticalAlternatesForRotation => "vrtr"
  | SlashedZero => "zero"
  };
