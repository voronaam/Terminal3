/*
 * E17Hacks.h
 *
 *  Created on: May 6, 2016
 *      Author: avorona
 */

#ifndef E17HACKS_H_
#define E17HACKS_H_

#include <Evas.h>
#include <Ecore_Ipc.h>

/*** FROM THE LEGACY **/

/**
 * Free list of font descriptions returned by evas_font_dir_available_list().
 *
 * @param e The evas instance that returned such list.
 * @param available the list returned by evas_font_dir_available_list().
 *
 * @ingroup Evas_Font_Group
 */
EAPI void                    evas_font_available_list_free(Evas *e, Eina_List *available) EINA_ARG_NONNULL(1);

/**
 *
 * List of available font descriptions known or found by this evas.
 *
 * The list depends on Evas compile time configuration, such as
 * fontconfig support, and the paths provided at runtime as explained
 * in @ref Evas_Font_Path_Group.
 *
 * @return a newly allocated list of strings. Do not change the
 * strings.  Be sure to call evas_font_available_list_free()
 * after you're done.
 *
 * @ingroup Evas_Font_Group
 *
 *
 * @param[in] obj The canvas object
 */
EAPI Eina_List *evas_font_available_list(const Evas_Canvas *obj) EINA_WARN_UNUSED_RESULT;


/*** FROM Ecore_Ipc.h */
EAPI int               ecore_ipc_init(void);
EAPI int               ecore_ipc_shutdown(void);

/* FIXME: need to add protocol type parameter */
EAPI Ecore_Ipc_Server *ecore_ipc_server_add(Ecore_Ipc_Type type, const char *name, int port, const void *data);

/* FIXME: need to add protocol type parameter */
EAPI Ecore_Ipc_Server *ecore_ipc_server_connect(Ecore_Ipc_Type type, char *name, int port, const void *data);
EAPI void             *ecore_ipc_server_del(Ecore_Ipc_Server *svr);
EAPI void             *ecore_ipc_server_data_get(Ecore_Ipc_Server *svr);
EAPI Eina_Bool         ecore_ipc_server_connected_get(Ecore_Ipc_Server *svr);
EAPI Eina_List        *ecore_ipc_server_clients_get(Ecore_Ipc_Server *svr);
/* FIXME: this needs to become an ipc message */
EAPI int               ecore_ipc_server_send(Ecore_Ipc_Server *svr, int major, int minor, int ref, int ref_to, int response, const void *data, int size);
EAPI void              ecore_ipc_server_client_limit_set(Ecore_Ipc_Server *svr, int client_limit, char reject_excess_clients);
EAPI void              ecore_ipc_server_data_size_max_set(Ecore_Ipc_Server *srv, int size);
EAPI int               ecore_ipc_server_data_size_max_get(Ecore_Ipc_Server *srv);
EAPI const char       *ecore_ipc_server_ip_get(Ecore_Ipc_Server *svr);
EAPI void              ecore_ipc_server_flush(Ecore_Ipc_Server *svr);

/* FIXME: this needs to become an ipc message */
EAPI int               ecore_ipc_client_send(Ecore_Ipc_Client *cl, int major, int minor, int ref, int ref_to, int response, const void *data, int size);
EAPI Ecore_Ipc_Server *ecore_ipc_client_server_get(Ecore_Ipc_Client *cl);
EAPI void             *ecore_ipc_client_del(Ecore_Ipc_Client *cl);
EAPI void              ecore_ipc_client_data_set(Ecore_Ipc_Client *cl, const void *data);
EAPI void             *ecore_ipc_client_data_get(Ecore_Ipc_Client *cl);
EAPI void              ecore_ipc_client_data_size_max_set(Ecore_Ipc_Client *cl, int size);
EAPI int               ecore_ipc_client_data_size_max_get(Ecore_Ipc_Client *cl);
EAPI const char       *ecore_ipc_client_ip_get(Ecore_Ipc_Client *cl);
EAPI void              ecore_ipc_client_flush(Ecore_Ipc_Client *cl);





/*** FROM elm_frame_legacy.h */

EAPI Evas_Object                 *elm_frame_add(Evas_Object *parent);


/** FROM Evas_Common.h */

/**
 * Free an #Evas_Smart struct
 *
 * @param s the #Evas_Smart struct to free
 *
 * @warning If this smart handle was created using
 * evas_smart_class_new(), the associated #Evas_Smart_Class will not
 * be freed.
 *
 * @note If you're using the #EVAS_SMART_SUBCLASS_NEW schema to create your
 * smart object, note that an #Evas_Smart handle will be shared amongst all
 * instances of the given smart class, through a static variable.
 * Evas will internally count references on #Evas_Smart handles and free them
 * when they are not referenced anymore. Thus, this function is of no use
 * for Evas users, most probably.
 */
EAPI void                              evas_smart_free(Evas_Smart *s) EINA_ARG_NONNULL(1);

/**
 * Creates a new #Evas_Smart from a given #Evas_Smart_Class struct
 *
 * @param sc the smart class definition
 * @return a new #Evas_Smart pointer
 *
 * #Evas_Smart handles are necessary to create new @b instances of
 * smart objects belonging to the class described by @p sc. That
 * handle will contain, besides the smart class interface definition,
 * all its smart callbacks infrastructure set, too.
 *
 * @note If you are willing to subclass a given smart class to
 * construct yours, consider using the #EVAS_SMART_SUBCLASS_NEW macro,
 * which will make use of this function automatically for you.
 */
EAPI Evas_Smart                       *evas_smart_class_new(const Evas_Smart_Class *sc) EINA_WARN_UNUSED_RESULT EINA_ARG_NONNULL(1) EINA_MALLOC;

/**
 * Get the #Evas_Smart_Class handle of an #Evas_Smart struct
 *
 * @param s a valid #Evas_Smart pointer
 * @return the #Evas_Smart_Class in it
 */
EAPI const Evas_Smart_Class           *evas_smart_class_get(const Evas_Smart *s) EINA_WARN_UNUSED_RESULT EINA_ARG_NONNULL(1);

/**
 * @brief Get the data pointer set on an #Evas_Smart struct
 *
 * @param s a valid #Evas_Smart handle
 *
 * This data pointer is set as the data field in the #Evas_Smart_Class
 * passed in to evas_smart_class_new().
 */
EAPI void                             *evas_smart_data_get(const Evas_Smart *s) EINA_WARN_UNUSED_RESULT EINA_ARG_NONNULL(1);

/**
 * Get the smart callbacks known by this #Evas_Smart handle's smart
 * class hierarchy.
 *
 * @param s A valid #Evas_Smart handle.
 * @param[out] count Returns the number of elements in the returned
 * array.
 * @return The array with callback descriptions known by this smart
 *         class, with its size returned in @a count parameter. It
 *         should not be modified in any way. If no callbacks are
 *         known, @c NULL is returned. The array is sorted by event
 *         names and elements refer to the original values given to
 *         evas_smart_class_new()'s Evas_Smart_Class::callbacks
 *         (pointer to them).
 *
 * This is likely different from
 * evas_object_smart_callbacks_descriptions_get() as it will contain
 * the callbacks of @b all this class hierarchy sorted, while the
 * direct smart class member refers only to that specific class and
 * should not include parent's.
 *
 * If no callbacks are known, this function returns @c NULL.
 *
 * The array elements and thus their contents will be @b references to
 * original values given to evas_smart_class_new() as
 * Evas_Smart_Class::callbacks.
 *
 * The array is sorted by Evas_Smart_Cb_Description::name. The last
 * array element is a @c NULL pointer and is not accounted for in @a
 * count. Loop iterations can check any of these size indicators.
 *
 * @note objects may provide per-instance callbacks, use
 *       evas_object_smart_callbacks_descriptions_get() to get those
 *       as well.
 * @see evas_object_smart_callbacks_descriptions_get()
 */
EAPI const Evas_Smart_Cb_Description **evas_smart_callbacks_descriptions_get(const Evas_Smart *s, unsigned int *count) EINA_ARG_NONNULL(1, 1);

/**
 * Find a callback description for the callback named @a name.
 *
 * @param s The #Evas_Smart where to search for class registered smart
 * event callbacks.
 * @param name Name of the desired callback, which must @b not be @c
 *        NULL. The search has a special case for @a name being the
 *        same pointer as registered with #Evas_Smart_Cb_Description.
 *        One can use it to avoid excessive use of strcmp().
 * @return A reference to the description if found, or @c NULL, otherwise
 *
 * @see evas_smart_callbacks_descriptions_get()
 */
EAPI const Evas_Smart_Cb_Description  *evas_smart_callback_description_find(const Evas_Smart *s, const char *name) EINA_ARG_NONNULL(1, 2);

/**
 * Sets one class to inherit from the other.
 *
 * Copy all function pointers, set @c parent to @a parent_sc and copy
 * everything after sizeof(Evas_Smart_Class) present in @a parent_sc,
 * using @a parent_sc_size as reference.
 *
 * This is recommended instead of a single memcpy() since it will take
 * care to not modify @a sc name, version, callbacks and possible
 * other members.
 *
 * @param sc child class.
 * @param parent_sc parent class, will provide attributes.
 * @param parent_sc_size size of parent_sc structure, child should be at least
 *        this size. Everything after @c Evas_Smart_Class size is copied
 *        using regular memcpy().
 */
EAPI Eina_Bool                         evas_smart_class_inherit_full(Evas_Smart_Class *sc, const Evas_Smart_Class *parent_sc, unsigned int parent_sc_size) EINA_ARG_NONNULL(1, 2);

/**
 * Get the number of users of the smart instance
 *
 * @param s The Evas_Smart to get the usage count of
 * @return The number of uses of the smart instance
 *
 * This function tells you how many more uses of the smart instance are in
 * existence. This should be used before freeing/clearing any of the
 * Evas_Smart_Class that was used to create the smart instance. The smart
 * instance will refer to data in the Evas_Smart_Class used to create it and
 * thus you cannot remove the original data until all users of it are gone.
 * When the usage count goes to 0, you can evas_smart_free() the smart
 * instance @p s and remove from memory any of the Evas_Smart_Class that
 * was used to create the smart instance, if you desire. Removing it from
 * memory without doing this will cause problems (crashes, undefined
 * behavior etc. etc.), so either never remove the original
 * Evas_Smart_Class data from memory (have it be a constant structure and
 * data), or use this API call and be very careful.
 */
EAPI int                               evas_smart_usage_get(const Evas_Smart *s);


/**
 * Set a given smart class' callbacks so it implements the <b>clipped smart
 * object </b>'s interface.
 *
 * @param sc The smart class handle to operate on
 *
 * This call will assign all the required methods of the @p sc
 * #Evas_Smart_Class instance to the implementations set for clipped
 * smart objects. If one wants to subclass it, call this function
 * and then override desired values. If one wants to call any original
 * method, save it somewhere. Example:
 *
 * @code
 * static Evas_Smart_Class parent_sc = EVAS_SMART_CLASS_INIT_NULL;
 *
 * static void my_class_smart_add(Evas_Object *o)
 * {
 *    parent_sc.add(o);
 *    evas_object_color_set(evas_object_smart_clipped_clipper_get(o),
 *                          255, 0, 0, 255);
 * }
 *
 * Evas_Smart_Class *my_class_new(void)
 * {
 *    static Evas_Smart_Class sc = EVAS_SMART_CLASS_INIT_NAME_VERSION("MyClass");
 *    if (!parent_sc.name)
 *      {
 *         evas_object_smart_clipped_smart_set(&sc);
 *         parent_sc = sc;
 *         sc.add = my_class_smart_add;
 *      }
 *    return &sc;
 * }
 * @endcode
 *
 * Default behavior for each of #Evas_Smart_Class functions on a
 * clipped smart object are:
 * - @c add: creates a hidden clipper with infinite size, to clip
 *    any incoming members;
 * - @c del: delete all children objects;
 * - @c move: move all objects relative relatively;
 * - @c resize: <b>not defined</b>;
 * - @c show: if there are children objects, show clipper;
 * - @c hide: hides clipper;
 * - @c color_set: set the color of clipper;
 * - @c clip_set: set clipper of clipper;
 * - @c clip_unset: unset the clipper of clipper;
 *
 * @note There are other means of assigning parent smart classes to
 * child ones, like the #EVAS_SMART_SUBCLASS_NEW macro or the
 * evas_smart_class_inherit_full() function.
 */
EAPI void                    evas_object_smart_clipped_smart_set(Evas_Smart_Class *sc) EINA_ARG_NONNULL(1);

/**
 * Get a pointer to the <b>clipped smart object's</b> class, to use
 * for proper inheritance
 *
 * @see #Evas_Smart_Object_Clipped for more information on this smart
 * class
 */
EAPI const Evas_Smart_Class *evas_object_smart_clipped_class_get(void) EINA_CONST;






















/** FROM elm_separator.eo.legacy.h*/
#include <elm_separator.eo.legacy.h>

/**
 *
 * @brief Set the horizontal mode of a separator object
 *
 * @ingroup Separator
 *
 * @param[in] obj The elm separator object
 * @param[in] horizontal If true, the separator is horizontal
 */
EAPI void elm_separator_horizontal_set(Elm_Separator *obj, Eina_Bool horizontal);

/**
 *
 * @brief Get the horizontal mode of a separator object
 *
 * @return If true, the separator is horizontal
 *
 * @see elm_separator_horizontal_set()
 *
 * @ingroup Separator
 *
 * @param[in] obj The elm separator object
 */
EAPI Eina_Bool elm_separator_horizontal_get(const Elm_Separator *obj);

EAPI Evas_Object *elm_separator_add(Evas_Object *parent);



/** FROM Ecore_Common.h */
EAPI void ecore_app_args_set(int argc, const char **argv);

/** FROM efreet_base */

/**
 * @return Returns the XDG Data Home directory
 * @brief Retrieves the XDG Data Home directory
 *
 * This is where user-specific data files should be written
 * ($XDG_DATA_HOME or $HOME/.local/share).
 */
EAPI const char *efreet_data_home_get(void);

/**
 * @return Returns the Eina_List of preference ordered extra data directories
 * @brief Returns the Eina_List of preference ordered extra data directories
 *
 * Ordered base directories relative to which data files should be
 * searched ($XDG_DATA_DIRS or /usr/local/share/:/usr/share/)
 *
 * @note The returned list is static inside Efreet. If you add/remove from the
 * list then the next call to efreet_data_dirs_get() will return your
 * modified values. DO NOT free this list.
 */
EAPI Eina_List *efreet_data_dirs_get(void);


/**
 * @return Returns the XDG Config Home directory
 * @brief Retrieves the XDG Config Home directory
 *
 * This is where user-specific configuration files should be
 * written ($XDG_CONFIG_HOME or $HOME/.config).
 */
EAPI const char *efreet_config_home_get(void);

/**
 * @return Returns the XDG Desktop directory
 * @brief Retrieves the XDG Desktop directory
 *
 * This is where user-specific desktop files should be located. It's
 * localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * @since 1.3
 */
EAPI const char *efreet_desktop_dir_get(void);

/**
 * @return Returns the XDG Download directory
 * @brief Retrieves the XDG Download directory
 *
 * This is where user-specific download files should be located. It's
 * localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * It's meant for large download or in-progress downloads, it's
 * private for the user but may be shared among different
 * machines. It's not automatically cleaned up.
 *
 * @see efreet_cache_home_get()
 * @see efreet_runtime_dir_get()
 * @see http:0pointer.de/blog/projects/tmp.html
 *
 * @since 1.8
 */
EAPI const char *efreet_download_dir_get(void);

/**
 * @return Returns the XDG Templates directory
 * @brief Retrieves the XDG Templates directory
 *
 * This is where user-specific template files should be located. It's
 * localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * @see efreet_documents_dir_get()
 * @see efreet_music_dir_get()
 * @see efreet_pictures_dir_get()
 * @see efreet_videos_dir_get()
 *
 * @since 1.8
 */
EAPI const char *efreet_templates_dir_get(void);

/**
 * @return Returns the XDG Public Share directory
 * @brief Retrieves the XDG Public Share directory
 *
 * This is where user-specific public shareable files should be
 * located. It's localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * Usually local file servers should look here (Apache, Samba, FTP).
 *
 * @since 1.8
 */
EAPI const char *efreet_public_share_dir_get(void);

/**
 * @return Returns the XDG Documents directory
 * @brief Retrieves the XDG Documents directory
 *
 * This is where user-specific documents files should be located. It's
 * localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * @see efreet_templates_dir_get()
 * @see efreet_music_dir_get()
 * @see efreet_pictures_dir_get()
 * @see efreet_videos_dir_get()
 *
 * @since 1.8
 */
EAPI const char *efreet_documents_dir_get(void);

/**
 * @return Returns the XDG Music directory
 * @brief Retrieves the XDG Music directory
 *
 * This is where user-specific music files should be located. It's
 * localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * @see efreet_templates_dir_get()
 * @see efreet_download_dir_get()
 * @see efreet_pictures_dir_get()
 * @see efreet_videos_dir_get()
 *
 * @since 1.8
 */
EAPI const char *efreet_music_dir_get(void);

/**
 * @return Returns the XDG Pictures directory
 * @brief Retrieves the XDG Pictures directory
 *
 * This is where user-specific pictures files should be located. It's
 * localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * @see efreet_templates_dir_get()
 * @see efreet_documents_dir_get()
 * @see efreet_music_dir_get()
 * @see efreet_videos_dir_get()
 *
 * @since 1.8
 */
EAPI const char *efreet_pictures_dir_get(void);

/**
 * @return Returns the XDG Videos directory
 * @brief Retrieves the XDG Videos directory
 *
 * This is where user-specific video files should be located. It's
 * localized (translated) to current user locale
 * (~/.config/user-dirs.locale and ~/.config/user-dirs.dirs).
 *
 * @see efreet_templates_dir_get()
 * @see efreet_documents_dir_get()
 * @see efreet_music_dir_get()
 * @see efreet_pictures_dir_get()
 *
 * @since 1.8
 */
EAPI const char *efreet_videos_dir_get(void);

/**
 * @return Returns the Eina_List of preference ordered extra config directories
 * @brief Returns the Eina_List of preference ordered extra config
 * directories
 *
 * Ordered base directories relative to which configuration files
 * should be searched ($XDG_CONFIG_DIRS or /etc/xdg)
 *
 * @note The returned list is static inside Efreet. If you add/remove from the
 * list then the next call to efreet_config_dirs_get() will return your
 * modified values. DO NOT free this list.
 */
EAPI Eina_List *efreet_config_dirs_get(void);


/**
 * @return Returns the XDG Cache Home directory
 * @brief Retrieves the XDG Cache Home directory
 *
 * This is the base directory relative to which user specific
 * non-essential data files should be stored ($XDG_CACHE_HOME or
 * $HOME/.cache).
 *
 * @see efreet_runtime_dir_get()
 * @see efreet_download_dir_get()
 * @see efreet_config_home_get()
 */
EAPI const char *efreet_cache_home_get(void);

/**
 * @return Returns the XDG User Runtime directory.
 * @brief Retrieves the XDG User Runtime directory.
 *
 * This is the base directory relative to which user-specific
 * non-essential runtime files and other file objects (such as
 * sockets, named pipes, ...) should be stored. The directory @b must
 * be owned by the user, and he @b must be the only one having read
 * and write access to it. Its Unix access mode @b must be 0700.
 *
 * The lifetime of this directory @b must be bound to the user being
 * logged in. It @b must be created when the user first logs in and if
 * the user fully logs out the directory @b must be removed. If the
 * user logs in more than once he should get pointed to the same
 * directory, and it is mandatory that the directory continues to
 * exist from his first login to his last logout on the system, and
 * not removed in between. Files in the directory @b must not survive
 * reboot or a full logout/login cycle.
 *
 * The directory @b must be on a local file system and not shared with
 * any other system. The directory @b must by fully-featured by the
 * standards of the operating system. More specifically, on Unix-like
 * operating systems AF_UNIX sockets, symbolic links, hard links,
 * proper permissions, file locking, sparse files, memory mapping,
 * file change notifications, a reliable hard link count must be
 * supported, and no restrictions on the file name character set
 * should be imposed. Files in this directory @b may be subjected to
 * periodic clean-up. To ensure that your files are not removed, they
 * should have their access time timestamp modified at least once
 * every 6 hours of monotonic time or the 'sticky' bit should be set
 * on the file.
 *
 * If @c NULL applications should fall back to a replacement directory
 * with similar capabilities and print a warning message. Applications
 * should use this directory for communication and synchronization
 * purposes and should not place larger files in it, since it might
 * reside in runtime memory and cannot necessarily be swapped out to
 * disk.
 *
 * @note this directory is similar to @c /run and is often created in
 * tmpfs (memory-only/RAM filesystem). It is created, managed and
 * cleaned automatically by systemd.
 *
 * @since 1.8
 */
EAPI const char *efreet_runtime_dir_get(void);

/**
 * @return Returns the current hostname
 * @brief Returns the current hostname or empty string if not found
 */
EAPI const char *efreet_hostname_get(void);

EAPI void             evas_object_geometry_set(Evas_Object *obj, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h) EINA_ARG_NONNULL(1);
EAPI void evas_object_size_hint_request_set(Evas_Object *obj, Evas_Coord w, Evas_Coord h);
EAPI void ecore_app_args_get(int *argc, char ***argv);
EAPI Evas_Object *evas_object_textgrid_add(Evas *e);
EAPI void evas_object_textgrid_font_set(Eo *obj, const char *font_name, Evas_Font_Size font_size);
#include <Ecore_Getopt.h>
EAPI int        ecore_getopt_parse(const Ecore_Getopt *parser, Ecore_Getopt_Value *values, int argc, char **argv);
EAPI void evas_event_feed_mouse_in(Evas_Canvas *obj, unsigned int timestamp, const void *data);


#endif /* E17HACKS_H_ */
