#include <libintl.h>
#define _(String) gettext (String)
#include "kbcoincoin.h"

static struct KbState KbState_;

struct KbState *kb_state() { return &KbState_; }

KeySym kb_lookup_string(Dock *dock UNUSED, XKeyEvent *event) {
  //EditW *ew = dock->editw;
  KbState_.klen = XLookupString(event, (char*)KbState_.buff, sizeof(KbState_.buff), &KbState_.ksym, &KbState_.compose_status);
  return KbState_.ksym;
}

KeySym kb_lookup_mb_string(Dock *dock UNUSED, XKeyEvent *event) {
  //EditW *ew = dock->editw;
  if (KbState_.input_context) {
    KbState_.klen = XmbLookupString(KbState_.input_context, event, (char*) KbState_.buff, sizeof(KbState_.buff), &KbState_.ksym, 0);
  } else {
    KbState_.klen = XLookupString(event, (char*) KbState_.buff, sizeof(KbState_.buff), &KbState_.ksym, 0);
  }
  return KbState_.ksym;
}

void kb_create_input_context_for(Dock *dock, Window win) {
  /* cette ligne marchait pile poil sur mon qwerty, je la garde en reserve...
     en la reregardant je crois qu'elle etait debile, elle defibnit
     deux fois XNInputSyle avec deux valeurs diffrenetes...
    KbState_.input_context = 
        XCreateIC(dock->input_method, 
	      XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
	      XNInputStyle, XIMStatusNothing,
	      XNClientWindow, ew->win,
	      NULL);
  */

  /* celle-ci est parfaite sur l'azerty...
    KbState_.input_context = 
    XCreateIC(dock->input_method, XNInputStyle, 1032,
	      XNClientWindow, ew->win, XNFocusWindow, ew->win, 0);*/
#ifndef sun
  if (dock->input_method) {
    KbState_.input_context = 
      XCreateIC(dock->input_method, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
		XNClientWindow, win, XNFocusWindow, win, 0);

    if(KbState_.input_context == NULL){
      fprintf(stderr, _("Warning : errot in XCreateIC() !\n"));
    }
  }
#endif
}

void kb_release_input_context() {
#ifndef sun
  if (KbState_.input_context)
    XDestroyIC(KbState_.input_context);
  KbState_.input_context = NULL;
#endif
}

void kb_build() {
  memset(&KbState_, 0, sizeof KbState_);
}
