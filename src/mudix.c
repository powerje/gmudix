/* gMUDix -- MUDix for X windows
 * Copyright (c) 2002 Marko Boomstra (m.boomstra@chello.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include "portability.h"
#include "mudix.h"

#if !defined(WIN32) && !defined(_WIN32)
  #include <signal.h>
  #include <sys/wait.h>

void signal_handler(int signal) {
    /* just call wait with no hang so that we get no zombies */
    waitpid(-1, NULL, WNOHANG);
}
#endif

int main(int argc, char *argv[]) {
    gmx_net_init();

#if !defined(WIN32) && !defined(_WIN32)
    /* install a signal for SIGCHLD for when a child dies */
    signal(SIGCHLD, signal_handler);
#endif

    init_gui(argc, argv);

    gmx_net_cleanup();
    return 0;
}
