#ifndef NEWSWIN_H
#define NEWSWIN_H

#include "coincoin.h"

void newswin_show(Dock *dock, id_type id);
void newswin_unmap(Dock *dock);
void newswin_build(Dock *dock);
void newswin_destroy(Dock *dock);
int  newswin_is_used(const Dock *dock);
Window newswin_get_window(const Dock *dock);
int  newswin_get_xpos(const Dock *dock);
int  newswin_get_ypos(const Dock *dock);
void newswin_save_state(Dock *dock, FILE *f);
void newswin_restore_state(Dock *dock, FILE *f);
void newswin_dispatch_event(Dock *dock, XEvent *event);
void newswin_update_content(Dock *dock, int reset_decal);
void newswin_update_info(Dock *dock, int mx, int my);
void newswin_draw(Dock *dock);
#endif
