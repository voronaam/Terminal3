/*
 * stdlib_Hack.h
 *
 *  Created on: May 6, 2016
 *      Author: avorona
 */

#ifndef STDLIB_HACK_H_
#define STDLIB_HACK_H_

/* Return a master pseudo-terminal handle.  */
extern int posix_openpt (int __oflag);
/* Chown the slave to the calling user.  */
extern int grantpt (int __fd);
/* Release an internal lock so the slave can be opened.
   Call after grantpt().  */
extern int unlockpt (int __fd);
/* Return the pathname of the pseudo terminal slave associated with
   the master FD is open on, or NULL on errors.
   The returned storage is good until the next call to this function.  */
extern char *ptsname (int __fd);



#endif /* STDLIB_HACK_H_ */
