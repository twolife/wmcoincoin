/* ce fichier est genere autmatiquement � partir du fichier "options"
   par le script "genoptlst.sh" NE PAS EDITER !!! */
#ifndef __OPTIONS_LIST_H

#define __OPTIONS_LIST_H


typedef enum {
OPT_font_encoding,
OPT_tribune_delay,
OPT_tribunenews_max_refresh_delay,
OPT_tribunenews_switch_off_coincoin_delay,
OPT_tribune_max_messages,
OPT_tribune_troll_detector,
OPT_tribune_post_cmd,
OPT_tribune_archive,
OPT_tribune_backend_type,
OPT_tribune_wiki_emulation,
OPT_dock_bg_color,
OPT_dock_bg_pixmap,
OPT_dock_skin_pixmap,
OPT_dock_fg_color,
OPT_dock_draw_border,
OPT_dock_iconwin,
OPT_dock_use_balloons,
OPT_dock_balloons_font_family,
OPT_dock_balloons_font_size,
OPT_dock_pos,
OPT_dock_start_in_boss_mode,
OPT_news_delay,
OPT_news_font_family,
OPT_news_font_size,
OPT_news_bg_color,
OPT_news_fg_color,
OPT_news_location,
OPT_news_dimensions,
OPT_palmipede_username,
OPT_palmipede_userlogin,
OPT_palmipede_useragent,
OPT_palmipede_default_message,
OPT_http_site_url,
OPT_http_path_tribune_backend,
OPT_http_path_news_backend,
OPT_http_path_end_news_url,
OPT_http_path_tribune_add,
OPT_http_path_myposts,
OPT_http_path_messages,
OPT_http_cookie,
OPT_http_force_fortune_retrieval,
OPT_http_proxy,
OPT_http_proxy_auth,
OPT_http_proxy_use_nocache,
OPT_http_disable_if_modified_since,
OPT_http_browser,
OPT_http_browser2,
OPT_http_gogole_search_url,
OPT_pinnipede_font_family,
OPT_pinnipede_font_size,
OPT_pinnipede_bg_color,
OPT_pinnipede_start_in_transparency_mode,
OPT_pinnipede_use_fake_real_transparency,
OPT_pinnipede_transparency,
OPT_pinnipede_fg_color,
OPT_pinnipede_clock_color,
OPT_pinnipede_useragent_color,
OPT_pinnipede_login_color,
OPT_pinnipede_url_color,
OPT_pinnipede_trollscore_color,
OPT_pinnipede_button_color,
OPT_pinnipede_emph_color,
OPT_pinnipede_sel_bgcolor,
OPT_pinnipede_popup_fgcolor,
OPT_pinnipede_popup_bgcolor,
OPT_pinnipede_hilight_my_msg_color,
OPT_pinnipede_hilight_answer_my_msg_color,
OPT_pinnipede_hilight_keyword_color0,
OPT_pinnipede_hilight_keyword_color1,
OPT_pinnipede_hilight_keyword_color2,
OPT_pinnipede_hilight_keyword_color3,
OPT_pinnipede_hilight_keyword_color4,
OPT_pinnipede_plopify_color,
OPT_pinnipede_fortune_bgcolor,
OPT_pinnipede_fortune_fgcolor,
OPT_pinnipede_fortune_font_family,
OPT_pinnipede_fortune_font_size,
OPT_pinnipede_location,
OPT_pinnipede_dimensions,
OPT_pinnipede_buttons,
OPT_pinnipede_show_tags,
OPT_pinnipede_show_seconds,
OPT_pinnipede_nick_mode,
OPT_pinnipede_show_troll_score,
OPT_pinnipede_show_fortune,
OPT_pinnipede_plop_keywords,
OPT_pinnipede_hilight_keywords,
OPT_pinnipede_plop_words,
OPT_pinnipede_use_AM_PM,
OPT_spell_enable,
OPT_spell_cmd,
OPT_spell_dict,
NB_WMCC_OPTIONS
} wmcc_options_id;


#ifdef __PREFS_C

static char *wmcc_options_strings[NB_WMCC_OPTIONS+1] = {
"font.encoding",
"tribune.delay",
"tribunenews.max_refresh_delay",
"tribunenews.switch_off_coincoin_delay",
"tribune.max_messages",
"tribune.troll_detector",
"tribune.post_cmd",
"tribune.archive",
"tribune.backend_type",
"tribune.wiki_emulation",
"dock.bg_color",
"dock.bg_pixmap",
"dock.skin_pixmap",
"dock.fg_color",
"dock.draw_border",
"dock.iconwin",
"dock.use_balloons",
"dock.balloons.font_family",
"dock.balloons.font_size",
"dock.pos",
"dock.start_in_boss_mode",
"news.delay",
"news.font_family",
"news.font_size",
"news.bg_color",
"news.fg_color",
"news.location",
"news.dimensions",
"palmipede.username",
"palmipede.userlogin",
"palmipede.useragent",
"palmipede.default_message",
"http.site_url",
"http.path_tribune_backend",
"http.path_news_backend",
"http.path_end_news_url",
"http.path_tribune_add",
"http.path_myposts",
"http.path_messages",
"http.cookie",
"http.force_fortune_retrieval",
"http.proxy",
"http.proxy_auth",
"http.proxy_use_nocache",
"http.disable_if_modified_since",
"http.browser",
"http.browser2",
"http.gogole_search_url",
"pinnipede.font_family",
"pinnipede.font_size",
"pinnipede.bg_color",
"pinnipede.start_in_transparency_mode",
"pinnipede.use_fake_real_transparency",
"pinnipede.transparency",
"pinnipede.fg_color",
"pinnipede.clock_color",
"pinnipede.useragent_color",
"pinnipede.login_color",
"pinnipede.url_color",
"pinnipede.trollscore_color",
"pinnipede.button_color",
"pinnipede.emph_color",
"pinnipede.sel_bgcolor",
"pinnipede.popup_fgcolor",
"pinnipede.popup_bgcolor",
"pinnipede.hilight.my_msg_color",
"pinnipede.hilight.answer_my_msg_color",
"pinnipede.hilight.keyword_color0",
"pinnipede.hilight.keyword_color1",
"pinnipede.hilight.keyword_color2",
"pinnipede.hilight.keyword_color3",
"pinnipede.hilight.keyword_color4",
"pinnipede.plopify_color",
"pinnipede.fortune_bgcolor",
"pinnipede.fortune_fgcolor",
"pinnipede.fortune_font_family",
"pinnipede.fortune_font_size",
"pinnipede.location",
"pinnipede.dimensions",
"pinnipede.buttons",
"pinnipede.show_tags",
"pinnipede.show_seconds",
"pinnipede.nick_mode",
"pinnipede.show_troll_score",
"pinnipede.show_fortune",
"pinnipede.plop_keywords",
"pinnipede.hilight_keywords",
"pinnipede.plop_words",
"pinnipede.use_AM_PM",
"spell.enable",
"spell.cmd",
"spell.dict",
NULL};
#endif
#endif
