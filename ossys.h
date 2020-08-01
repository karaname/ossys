extern struct system_info {
  char uptime_buf[32];
  char ctime_buf[16];
  char tmpf_buf[128];
  char loadavg_buf[64];
  char osname[16];
  char hostname[32];
  char domain[32];
  char arch[16];
  char ramtot[8];
  char ramfree[8];
  char cprocs[8];
  char tty_buf[12];
} sys;

extern struct env_info {
  char *dskt_session;
  char *shell;
} genv;
