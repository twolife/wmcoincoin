/* ce fichier est genere autmatiquement � partir du fichier "options"
   par le script "genoptlst.sh" NE PAS EDITER !!! */
#ifndef __OPTIONS_LIST_H

#define __OPTIONS_LIST_H


typedef enum {
OPT_site,
OPT_font_encoding,
OPT_tribunenews_max_refresh_delay,
OPT_tribunenews_switch_off_coincoin_delay,
OPT_tribune_troll_detector,
OPT_tribune_post_cmd,
OPT_tribune_archive,
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
OPT_news_font_family,
OPT_news_font_size,
OPT_news_bg_color,
OPT_news_fg_color,
OPT_news_titles_fg_color,
OPT_news_titles_bg_color,
OPT_news_emph_color,
OPT_news_location,
OPT_news_dimensions,
OPT_palmipede_default_message,
OPT_http_timeout,
OPT_http_browser,
OPT_http_browser2,
OPT_http_gogole_search_url,
OPT_pinnipede_font_family,
OPT_pinnipede_font_size,
OPT_pinnipede_start_in_transparency_mode,
OPT_pinnipede_use_fake_real_transparency,
OPT_pinnipede_transparency,
OPT_pinnipede_emph_color,
OPT_pinnipede_sel_bgcolor,
OPT_pinnipede_popup_fgcolor,
OPT_pinnipede_popup_bgcolor,
OPT_pinnipede_buttonbar_bgcolor,
OPT_pinnipede_buttonbar_fgcolor,
OPT_pinnipede_buttonbar_msgcnt_color,
OPT_pinnipede_buttonbar_updlcnt_color,
OPT_pinnipede_buttonbar_progressbar_color,
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
OPT_scrollcoin_bg_color,
OPT_scrollcoin_bg_light_color,
OPT_scrollcoin_bg_dark_color,
OPT_scrollcoin_arrow_normal_color,
OPT_scrollcoin_arrow_emphasized_color,
OPT_scrollcoin_bar_color,
OPT_scrollcoin_bar_light_color,
OPT_scrollcoin_bar_dark_color,
OPT_pinnipede_location,
OPT_pinnipede_dimensions,
OPT_pinnipede_buttons,
OPT_pinnipede_show_tags,
OPT_pinnipede_show_seconds,
OPT_pinnipede_nick_mode,
OPT_pinnipede_show_troll_score,
OPT_pinnipede_show_fortune,
OPT_pinnipede_use_classical_tabs,
OPT_pinnipede_plop_keywords,
OPT_pinnipede_hilight_keywords,
OPT_pinnipede_plop_words,
OPT_pinnipede_url_replace,
OPT_pinnipede_auto_open,
OPT_board_miniua_rule,
OPT_spell_enable,
OPT_spell_cmd,
OPT_spell_dict,
OPTS_http_cookie,
OPTS_check_news,
OPTS_check_board,
OPTS_check_comments,
OPTS_check_messages,
OPTSG_locale,
OPTSG_tribune_delay,
OPTSG_tribune_max_messages,
OPTSG_tribune_backend_type,
OPTSG_tribune_wiki_emulation,
OPTSG_news_delay,
OPTSG_news_max_age,
OPTSG_palmipede_username,
OPTSG_palmipede_userlogin,
OPTSG_palmipede_useragent,
OPTSG_palmipede_msg_max_length,
OPTSG_palmipede_useragent_max_length,
OPTSG_http_site_url,
OPTSG_http_path_tribune_backend,
OPTSG_http_path_news_backend,
OPTSG_http_path_end_news_url,
OPTSG_http_path_tribune_add,
OPTSG_http_board_post,
OPTSG_http_path_myposts,
OPTSG_http_path_messages,
OPTSG_http_force_fortune_retrieval,
OPTSG_http_proxy,
OPTSG_http_proxy_auth,
OPTSG_http_proxy_use_nocache,
OPTSG_http_use_if_modified_since,
OPTSG_pinnipede_bg_color,
OPTSG_pinnipede_fg_color,
OPTSG_pinnipede_clock_color,
OPTSG_pinnipede_useragent_color,
OPTSG_pinnipede_login_color,
OPTSG_pinnipede_url_color,
OPTSG_pinnipede_visited_url_color,
OPTSG_pinnipede_trollscore_color,
OPTSG_pinnipede_strike_color,
OPTSG_pinnipede_mark_id_gaps,
OPTSG_pinnipede_use_AM_PM,
NB_WMCC_OPTIONS
} wmcc_options_id;


#if defined(__PREFS_C) || defined(__WMCCC_C)

static char *wmcc_options_strings[NB_WMCC_OPTIONS+1] = {
"site",
"font.encoding",
"tribunenews.max_refresh_delay",
"tribunenews.switch_off_coincoin_delay",
"tribune.troll_detector",
"tribune.post_cmd",
"tribune.archive",
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
"news.font_family",
"news.font_size",
"news.bg_color",
"news.fg_color",
"news.titles.fg_color",
"news.titles.bg_color",
"news.emph_color",
"news.location",
"news.dimensions",
"palmipede.default_message",
"http.timeout",
"http.browser",
"http.browser2",
"http.gogole_search_url",
"pinnipede.font_family",
"pinnipede.font_size",
"pinnipede.start_in_transparency_mode",
"pinnipede.use_fake_real_transparency",
"pinnipede.transparency",
"pinnipede.emph_color",
"pinnipede.sel_bgcolor",
"pinnipede.popup_fgcolor",
"pinnipede.popup_bgcolor",
"pinnipede.buttonbar.bgcolor",
"pinnipede.buttonbar_fgcolor",
"pinnipede.buttonbar_msgcnt_color",
"pinnipede.buttonbar_updlcnt_color",
"pinnipede.buttonbar_progressbar_color",
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
"scrollcoin.bg_color",
"scrollcoin.bg_light_color",
"scrollcoin.bg_dark_color",
"scrollcoin.arrow_normal_color",
"scrollcoin.arrow_emphasized_color",
"scrollcoin.bar_color",
"scrollcoin.bar_light_color",
"scrollcoin.bar_dark_color",
"pinnipede.location",
"pinnipede.dimensions",
"pinnipede.buttons",
"pinnipede.show_tags",
"pinnipede.show_seconds",
"pinnipede.nick_mode",
"pinnipede.show_troll_score",
"pinnipede.show_fortune",
"pinnipede.use_classical_tabs",
"pinnipede.plop_keywords",
"pinnipede.hilight_keywords",
"pinnipede.plop_words",
"pinnipede.url_replace",
"pinnipede.auto_open",
"board.miniua_rule",
"spell.enable",
"spell.cmd",
"spell.dict",
".http.cookie",
".check_news",
".check_board",
".check_comments",
".check_messages",
"!locale",
"!tribune.delay",
"!tribune.max_messages",
"!tribune.backend_type",
"!tribune.wiki_emulation",
"!news.delay",
"!news.max_age",
"!palmipede.username",
"!palmipede.userlogin",
"!palmipede.useragent",
"!palmipede.msg_max_length",
"!palmipede.useragent_max_length",
"!http.site_url",
"!http.path_tribune_backend",
"!http.path_news_backend",
"!http.path_end_news_url",
"!http.path_tribune_add",
"!http.board_post",
"!http.path_myposts",
"!http.path_messages",
"!http.force_fortune_retrieval",
"!http.proxy",
"!http.proxy_auth",
"!http.proxy_use_nocache",
"!http.use_if_modified_since",
"!pinnipede.bg_color",
"!pinnipede.fg_color",
"!pinnipede.clock_color",
"!pinnipede.useragent_color",
"!pinnipede.login_color",
"!pinnipede.url_color",
"!pinnipede.visited_url_color",
"!pinnipede.trollscore_color",
"!pinnipede.strike_color",
"!pinnipede.mark_id_gaps",
"!pinnipede.use_AM_PM",
NULL};
#endif
#endif
