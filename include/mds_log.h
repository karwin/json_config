#include <stdio.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <syslog.h>

#ifdef DBG_MSG

static int LOG_ID=0;

static inline void mds_info(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vsyslog(LOG_ID|LOG_DEBUG, format, ap);
	va_end(ap);
}

static inline void mds_err(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vsyslog(LOG_ID|LOG_ERR, format, ap);
	va_end(ap);
}

static inline void mds_dbg(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vsyslog(LOG_ID|LOG_INFO, format, ap);
	va_end(ap);
}

static inline void mds_msg(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vsyslog(LOG_ID|LOG_NOTICE, format, ap);
	va_end(ap);
}

static inline int mds_log_init(void)
{
	int option = LOG_NDELAY | LOG_PID;

	openlog("medusa", option, LOG_ID);
	syslog(LOG_ID|LOG_INFO, "medusa init:");

	return 0;
}

static inline  void mds_log_cleanup(void)
{
	syslog(LOG_ID|LOG_INFO, "Exit!");
	closelog();
}

#define DBG(fmt, arg...)  \
		mds_dbg("[D][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#define MSG(fmt, arg...) \
		mds_msg("[M][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#define ERR(fmt, arg...) \
		mds_err("[E][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#define INFO(fmt, arg...)  \
		mds_info("[I][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#else

#define DBG(fmt, arg...)  \
		printf("[D][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#define MSG(fmt, arg...) \
		printf("[M][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#define ERR(fmt, arg...) \
		printf("[E][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#define INFO(fmt, arg...)  \
		printf("[I][%s:%d] " fmt,__FILE__,__LINE__, ## arg)

#endif
