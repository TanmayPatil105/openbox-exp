#ifndef __widget_hh
#define __widget_hh

#include "rect.hh"
#include "point.hh"
#include "texture.hh"
#include "style.hh"
#include "eventdispatcher.hh"
#include "display.hh"

extern "C" {
#include <assert.h>
}

#include <string>
#include <list>

namespace otk {

class OtkWidget : public OtkEventHandler {

public:

  enum Direction { Horizontal, Vertical };

  typedef std::list<OtkWidget *> OtkWidgetList;

  OtkWidget(otk::OtkWidget *parent, Direction = Horizontal);
  OtkWidget(otk::OtkEventDispatcher *event_dispatcher, otk::Style *style,
            Direction direction = Horizontal, Cursor cursor = 0,
            int bevel_width = 1, unsigned long create_mask = 0);

  virtual ~OtkWidget();

  virtual void update(void);

  void exposeHandler(const XExposeEvent &e);
  void configureHandler(const XConfigureEvent &e);

  inline Window window(void) const { return _window; }
  inline const otk::OtkWidget *parent(void) const { return _parent; }
  inline const OtkWidgetList &children(void) const { return _children; }
  inline unsigned int screen(void) const { return _screen; }
  inline const otk::Rect &rect(void) const { return _rect; }

  void move(const otk::Point &to);
  void move(int x, int y);

  virtual void setWidth(int);
  virtual void setHeight(int);

  virtual int width() const { return _rect.width(); }
  virtual int height() const { return _rect.height(); }

  virtual void resize(const otk::Point &to);
  virtual void resize(int x, int y);

  virtual void setGeometry(const otk::Rect &new_geom);
  virtual void setGeometry(const otk::Point &topleft, int width, int height);
  virtual void setGeometry(int x, int y, int width, int height);

  inline bool isVisible(void) const { return _visible; };
  virtual void show(bool recursive = false);
  virtual void hide(bool recursive = false);

  inline bool isFocused(void) const { return _focused; };
  virtual void focus(void);
  virtual void unfocus(void);

  inline bool hasGrabbedMouse(void) const { return _grabbed_mouse; }
  bool grabMouse(void);
  void ungrabMouse(void);

  inline bool hasGrabbedKeyboard(void) const { return _grabbed_keyboard; }
  bool grabKeyboard(void);
  void ungrabKeyboard(void);

  inline otk::BTexture *texture(void) const { return _texture; }
  virtual void setTexture(otk::BTexture *texture)
    { _texture = texture; _dirty = true; }

  inline const otk::BColor *borderColor(void) const { return _bcolor; }
  virtual void setBorderColor(const otk::BColor *color) {
    assert(color); _bcolor = color;
    XSetWindowBorder(OBDisplay::display, _window, color->pixel());
  }

  inline int borderWidth(void) const { return _bwidth; }
  void setBorderWidth(int width) {
    _bwidth = width;
    XSetWindowBorderWidth(otk::OBDisplay::display, _window, width);
  }

  virtual void addChild(OtkWidget *child, bool front = false);
  virtual void removeChild(OtkWidget *child);

  inline bool isStretchableHorz(void) const { return _stretchable_horz; }
  void setStretchableHorz(bool s_horz = true) { _stretchable_horz = s_horz; }

  inline bool isStretchableVert(void) const { return _stretchable_vert; }
  void setStretchableVert(bool s_vert = true)  { _stretchable_vert = s_vert; }

  inline Cursor cursor(void) const { return _cursor; }
  void setCursor(Cursor cursor) {
    _cursor = cursor;
    XDefineCursor(OBDisplay::display, _window, _cursor);
  }

  inline int bevelWidth(void) const { return _bevel_width; }
  void setBevelWidth(int bevel_width)
  { assert(bevel_width > 0); _bevel_width = bevel_width; }

  inline Direction direction(void) const { return _direction; }
  void setDirection(Direction dir) { _direction = dir; }

  inline otk::Style *style(void) const { return _style; }
  virtual void setStyle(otk::Style *style);

  inline otk::OtkEventDispatcher *eventDispatcher(void)
  { return _event_dispatcher; }
  void setEventDispatcher(otk::OtkEventDispatcher *disp);

protected:
  
  bool _dirty;
  bool _focused;

  virtual void adjust(void);
  virtual void create(unsigned long mask = 0);
  virtual void adjustHorz(void);
  virtual void adjustVert(void);
  virtual void internalResize(int width, int height);
  virtual void render(void);

  Window _window;

  OtkWidget *_parent;
  OtkWidgetList _children;

  Style *_style;
  Direction _direction;
  Cursor _cursor;
  int _bevel_width;
  int _ignore_config;

  bool _visible;

  bool _grabbed_mouse;
  bool _grabbed_keyboard;

  bool _stretchable_vert;
  bool _stretchable_horz;

  BTexture *_texture;
  Pixmap _bg_pixmap;
  unsigned int _bg_pixel;

  const BColor *_bcolor;
  unsigned int _bwidth;

  Rect _rect;
  unsigned int _screen;

  bool _fixed_width;
  bool _fixed_height;

  OtkEventDispatcher *_event_dispatcher;
};

}

#endif // __widget_hh
