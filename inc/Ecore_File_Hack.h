/*
 * Ecore_File_Hack.h
 *
 *  Created on: May 6, 2016
 *      Author: avorona
 */

#ifndef ECORE_FILE_HACK_H_
#define ECORE_FILE_HACK_H_

#include <Ecore_File.h>


EAPI int            ecore_file_init         (void);
EAPI int            ecore_file_shutdown     (void);
EAPI long long      ecore_file_mod_time     (const char *file);
EAPI long long      ecore_file_size         (const char *file);
EAPI Eina_Bool      ecore_file_exists       (const char *file);
EAPI Eina_Bool      ecore_file_is_dir       (const char *file);
EAPI Eina_Bool      ecore_file_mkdir        (const char *dir);
EAPI int            ecore_file_mkdirs       (const char **dirs);
EAPI int            ecore_file_mksubdirs    (const char *base, const char **subdirs);
EAPI Eina_Bool      ecore_file_rmdir        (const char *dir);
EAPI Eina_Bool      ecore_file_recursive_rm (const char *dir);
EAPI Eina_Bool      ecore_file_mkpath       (const char *path);
EAPI int            ecore_file_mkpaths      (const char **paths);
EAPI Eina_Bool      ecore_file_cp           (const char *src, const char *dst);
EAPI Eina_Bool      ecore_file_mv           (const char *src, const char *dst);
EAPI Eina_Bool      ecore_file_symlink      (const char *src, const char *dest);
EAPI char          *ecore_file_realpath     (const char *file);
EAPI Eina_Bool      ecore_file_unlink       (const char *file);
EAPI Eina_Bool      ecore_file_remove       (const char *file);
EAPI const char    *ecore_file_file_get     (const char *path);
EAPI char          *ecore_file_dir_get      (const char *path);
EAPI Eina_Bool      ecore_file_can_read     (const char *file);
EAPI Eina_Bool      ecore_file_can_write    (const char *file);
EAPI Eina_Bool      ecore_file_can_exec     (const char *file);
EAPI char          *ecore_file_readlink     (const char *link);
EAPI Eina_List     *ecore_file_ls           (const char *dir);
EAPI char          *ecore_file_app_exe_get  (const char *app);
EAPI char          *ecore_file_escape_name  (const char *filename);
EAPI char          *ecore_file_strip_ext    (const char *file);
EAPI int            ecore_file_dir_is_empty (const char *dir);

#endif /* ECORE_FILE_HACK_H_ */
