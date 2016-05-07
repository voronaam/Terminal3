#ifndef _PRIVATE_H__
#define _PRIVATE_H__ 1

#ifdef HAVE_CONFIG_H
#include "terminology_config.h"
#endif

#if HAVE_GETTEXT && ENABLE_NLS
#define _(string) gettext (string)
#else
#define _(string) (string)
#endif
#define gettext_noop(String) String

extern int _log_domain;

#include <dlog.h>

/*
#define CRITICAL(...) EINA_LOG_DOM_CRIT(_log_domain, __VA_ARGS__)
#define ERR(...)      EINA_LOG_DOM_ERR(_log_domain, __VA_ARGS__)
#define WRN(...)      EINA_LOG_DOM_WARN(_log_domain, __VA_ARGS__)
#define INF(...)      EINA_LOG_DOM_INFO(_log_domain, __VA_ARGS__)
#define DBG(...)      EINA_LOG_DOM_DBG(_log_domain, __VA_ARGS__)
*/

#define CRITICAL(...) dlog_print(DLOG_FATAL, "TERM_THREE", __VA_ARGS__)
#define ERR(...)      dlog_print(DLOG_ERROR, "TERM_THREE", __VA_ARGS__)
#define WRN(...)      dlog_print(DLOG_WARN,  "TERM_THREE", __VA_ARGS__)
#define INF(...)      dlog_print(DLOG_INFO,  "TERM_THREE", __VA_ARGS__)
#define DBG(...)      dlog_print(DLOG_DEBUG, "TERM_THREE", __VA_ARGS__)




#endif
