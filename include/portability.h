/* gMUDix -- portability shim
 *
 * Cross-platform compatibility layer for socket I/O and signal handling.
 * Wraps the differences between POSIX (Linux, macOS, BSD) and Winsock so
 * the rest of the codebase can stay platform-agnostic.
 */

#ifndef _PORTABILITY_H_
#define _PORTABILITY_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(WIN32) || defined(_WIN32)
  /* winsock2.h must precede windows.h */
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <windows.h>
  #include <io.h>
  #include <dirent.h>  /* mingw provides opendir/readdir but not scandir */

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

  /* scandir + alphasort polyfills for mingw, sufficient for the
   * codebase's "list files in a directory" use. Not 100% BSD-compatible
   * (no errno on partial OOM, etc.), but the call site treats any
   * non-positive return as "directory unreadable". */
  static int gmx_alphasort(const struct dirent **a, const struct dirent **b)
  {
      return strcmp((*a)->d_name, (*b)->d_name);
  }

  static int gmx_scandir(const char *dirpath, struct dirent ***namelist,
                         int (*filter)(const struct dirent *),
                         int (*compar)(const struct dirent **,
                                       const struct dirent **))
  {
      DIR *dir = opendir(dirpath);
      if (!dir) return -1;

      struct dirent **list = NULL;
      size_t cap = 0, count = 0;
      struct dirent *ent;

      while ((ent = readdir(dir)) != NULL) {
          if (filter && !filter(ent)) continue;
          if (count == cap) {
              size_t new_cap = cap ? cap * 2 : 16;
              struct dirent **new_list =
                  (struct dirent **)realloc(list, new_cap * sizeof(*new_list));
              if (!new_list) goto oom;
              list = new_list;
              cap = new_cap;
          }
          struct dirent *copy = (struct dirent *)malloc(sizeof(*copy));
          if (!copy) goto oom;
          *copy = *ent;
          list[count++] = copy;
      }
      closedir(dir);

      if (compar) {
          qsort(list, count, sizeof(*list),
                (int (*)(const void *, const void *))compar);
      }
      *namelist = list;
      return (int)count;

oom:
      closedir(dir);
      while (count) free(list[--count]);
      free(list);
      return -1;
  }

  #define scandir   gmx_scandir
  #define alphasort gmx_alphasort

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
