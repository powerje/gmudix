/* gMUDix -- telnet compatibility shim
 *
 * <arpa/telnet.h> ships on POSIX systems but not with the mingw-w64
 * headers used for native Windows builds. On Windows we provide the
 * subset of TELNET protocol constants the codebase depends on, taken
 * verbatim from RFC 854/855/1091/1184/1572 (matching BSD's telnet.h).
 */

#ifndef _TELNET_COMPAT_H_
#define _TELNET_COMPAT_H_

#if !defined(WIN32) && !defined(_WIN32)

  #include <arpa/telnet.h>

#else

  /* IAC commands */
  #define IAC     255  /* interpret as command: */
  #define DONT    254  /* you are not to use option */
  #define DO      253  /* please, you use option */
  #define WONT    252  /* I won't use option */
  #define WILL    251  /* I will use option */
  #define SB      250  /* interpret as subnegotiation */
  #define GA      249  /* you may reverse the line */
  #define EL      248  /* erase the current line */
  #define EC      247  /* erase the current character */
  #define AYT     246  /* are you there */
  #define AO      245  /* abort output--but let prog finish */
  #define IP      244  /* interrupt process--permanently */
  #define BREAK   243  /* break */
  #define DM      242  /* data mark--for connect. cleaning */
  #define NOP     241  /* nop */
  #define SE      240  /* end sub negotiation */
  #define EOR     239  /* end of record (transparent mode) */
  #define ABORT   238  /* abort process */
  #define SUSP    237  /* suspend process */
  #define xEOF    236  /* end of file: EOF is already used... */

  /* TELNET options */
  #define TELOPT_BINARY         0
  #define TELOPT_ECHO           1
  #define TELOPT_RCP            2
  #define TELOPT_SGA            3
  #define TELOPT_NAMS           4
  #define TELOPT_STATUS         5
  #define TELOPT_TM             6
  #define TELOPT_RCTE           7
  #define TELOPT_NAOL           8
  #define TELOPT_NAOP           9
  #define TELOPT_NAOCRD         10
  #define TELOPT_NAOHTS         11
  #define TELOPT_NAOHTD         12
  #define TELOPT_NAOFFD         13
  #define TELOPT_NAOVTS         14
  #define TELOPT_NAOVTD         15
  #define TELOPT_NAOLFD         16
  #define TELOPT_XASCII         17
  #define TELOPT_LOGOUT         18
  #define TELOPT_BM             19
  #define TELOPT_DET            20
  #define TELOPT_SUPDUP         21
  #define TELOPT_SUPDUPOUTPUT   22
  #define TELOPT_SNDLOC         23
  #define TELOPT_TTYPE          24
  #define TELOPT_EOR            25
  #define TELOPT_TUID           26
  #define TELOPT_OUTMRK         27
  #define TELOPT_TTYLOC         28
  #define TELOPT_3270REGIME     29
  #define TELOPT_X3PAD          30
  #define TELOPT_NAWS           31
  #define TELOPT_TSPEED         32
  #define TELOPT_LFLOW          33
  #define TELOPT_LINEMODE       34
  #define TELOPT_XDISPLOC       35
  #define TELOPT_OLD_ENVIRON    36
  #define TELOPT_AUTHENTICATION 37
  #define TELOPT_ENCRYPT        38
  #define TELOPT_NEW_ENVIRON    39
  #define TELOPT_EXOPL          255

  /* sub-option qualifiers */
  #define TELQUAL_IS    0
  #define TELQUAL_SEND  1
  #define TELQUAL_INFO  2
  #define TELQUAL_REPLY 2
  #define TELQUAL_NAME  3

#endif

#endif /* _TELNET_COMPAT_H_ */
