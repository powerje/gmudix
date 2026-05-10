/* gMUDix -- portability shim
 *
 * Cross-platform compatibility layer for socket I/O and signal handling.
 * Wraps the differences between POSIX (Linux, macOS, BSD) and Winsock so
 * the rest of the codebase can stay platform-agnostic.
 */

#ifndef _PORTABILITY_H_
#define _PORTABILITY_H_

#include <stddef.h>
#include <errno.h>

#if defined(WIN32) || defined(_WIN32)
  /* winsock2.h must precede windows.h */
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <windows.h>
  #include <io.h>

  static inline int gmx_socket_read(int sock, void *buf, size_t len)
  {
      return recv((SOCKET)sock, (char *)buf, (int)len, 0);
  }

  static inline int gmx_socket_write(int sock, const void *buf, size_t len)
  {
      return send((SOCKET)sock, (const char *)buf, (int)len, 0);
  }

  static inline int gmx_socket_close(int sock)
  {
      return closesocket((SOCKET)sock);
  }

  static inline int gmx_set_nonblocking(int sock)
  {
      unsigned long mode = 1;
      return ioctlsocket((SOCKET)sock, FIONBIO, &mode);
  }

  static inline int gmx_socket_would_block(void)
  {
      int err = WSAGetLastError();
      return err == WSAEWOULDBLOCK || err == WSAEINPROGRESS;
  }

  static inline int gmx_net_init(void)
  {
      WSADATA wsa;
      return WSAStartup(MAKEWORD(2, 2), &wsa);
  }

  static inline void gmx_net_cleanup(void)
  {
      WSACleanup();
  }

#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <fcntl.h>

  static inline int gmx_socket_read(int sock, void *buf, size_t len)
  {
      return (int)read(sock, buf, len);
  }

  static inline int gmx_socket_write(int sock, const void *buf, size_t len)
  {
      return (int)write(sock, buf, len);
  }

  static inline int gmx_socket_close(int sock)
  {
      return close(sock);
  }

  static inline int gmx_set_nonblocking(int sock)
  {
      return fcntl(sock, F_SETFL, O_NONBLOCK);
  }

  static inline int gmx_socket_would_block(void)
  {
      return errno == EWOULDBLOCK || errno == EAGAIN;
  }

  static inline int gmx_net_init(void) { return 0; }
  static inline void gmx_net_cleanup(void) {}
#endif

#endif /* _PORTABILITY_H_ */
