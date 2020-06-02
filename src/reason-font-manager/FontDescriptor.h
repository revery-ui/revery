#ifndef FONT_DESCRIPTOR_H
#define FONT_DESCRIPTOR_H

#ifdef __APPLE__
#include <vector>
#else
#include <cstdio>
#include <cstring>
#include <vector>
#endif

enum FontWeight {
  FontWeightUndefined   = 0,
  FontWeightThin        = 100,
  FontWeightUltraLight  = 200,
  FontWeightLight       = 300,
  FontWeightNormal      = 400,
  FontWeightMedium      = 500,
  FontWeightSemiBold    = 600,
  FontWeightBold        = 700,
  FontWeightUltraBold   = 800,
  FontWeightHeavy       = 900
};

enum FontWidth {
  FontWidthUndefined      = 0,
  FontWidthUltraCondensed = 1,
  FontWidthExtraCondensed = 2,
  FontWidthCondensed      = 3,
  FontWidthSemiCondensed  = 4,
  FontWidthNormal         = 5,
  FontWidthSemiExpanded   = 6,
  FontWidthExpanded       = 7,
  FontWidthExtraExpanded  = 8,
  FontWidthUltraExpanded  = 9
};

struct FontDescriptor {
public:
  const char *path;
  const char *postscriptName;
  const char *family;
  const char *style;
  FontWeight weight;
  FontWidth width;
  bool italic;
  bool monospace;

  FontDescriptor() {
    path = NULL;
    postscriptName = NULL;
    family = NULL;
    style = NULL;
    weight = FontWeightUndefined;
    width = FontWidthUndefined;
    italic = false;
    monospace = false;
  }
  
  FontDescriptor(const char *path, const char *postscriptName, const char *family, const char *style, 
                 FontWeight weight, FontWidth width, bool italic, bool monospace) {
    this->path = copyString(path);
    this->postscriptName = copyString(postscriptName);
    this->family = copyString(family);
    this->style = copyString(style);
    this->weight = weight;
    this->width = width;
    this->italic = italic;
    this->monospace = monospace;
  }

  FontDescriptor(FontDescriptor *desc) {
    path = copyString(desc->path);
    postscriptName = copyString(desc->postscriptName);
    family = copyString(desc->family);
    style = copyString(desc->style);
    weight = desc->weight;
    width = desc->width;
    italic = desc->italic;
    monospace = desc->monospace;
  }
  
  ~FontDescriptor() {
    if (path)
      delete [] path;
    
    if (postscriptName)
      delete [] postscriptName;
    
    if (family)
      delete [] family;
    
    if (style)
      delete [] style;
    
    postscriptName = NULL;
    family = NULL;
    style = NULL;
  }
  
private:
  char *copyString(const char *input) {
    if (!input)
      return NULL;

    char *str = new char[strlen(input) + 1];
    strcpy(str, input);
    return str;
  }
};

class ResultSet : public std::vector<FontDescriptor *> {
public:
  ~ResultSet() {
    for (ResultSet::iterator it = this->begin(); it != this->end(); it++) {
      delete *it;
    }
  }
};

#endif
