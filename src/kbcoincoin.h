#ifndef KB_COINCOIN_H
#define KB_COINCOIN_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xmd.h>
#include <X11/keysym.h>
#include "coincoin.h"

struct KbState {
  KeySym ksym;
  int klen;
  unsigned char buff[4];  
  XComposeStatus compose_status;
  XIC input_context;
};
struct KbState *kb_state();
KeySym kb_lookup_string(Dock *dock, XKeyEvent *event);
KeySym kb_lookup_mb_string(Dock *dock, XKeyEvent *event);
void kb_create_input_context_for(Dock *dock, Window win);
void kb_release_input_context();
void kb_build();
#endif
