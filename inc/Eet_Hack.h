/*
 * Eet_Hack.h
 *
 *  Created on: May 6, 2016
 *      Author: avorona
 */

#ifndef EET_HACK_H_
#define EET_HACK_H_

#include <Eet.h>

/**
 * Initialize the EET library.
 *
 * The first time this function is called, it will perform all the internal
 * initialization required for the library to function properly and increment
 * the initialization counter. Any subsequent call only increment this counter
 * and return its new value, so it's safe to call this function more than once.
 *
 * @return The new init count. Will be 0 if initialization failed.
 *
 * @since 1.0.0
 * @ingroup Eet_Group
 */
EAPI int
eet_init(void);

/**
 * Shut down the EET library.
 *
 * If eet_init() was called more than once for the running application,
 * eet_shutdown() will decrement the initialization counter and return its
 * new value, without doing anything else. When the counter reaches 0, all
 * of the internal elements will be shutdown and any memory used freed.
 *
 * @return The new init count.
 *
 * @since 1.0.0
 * @ingroup Eet_Group
 */
EAPI int
eet_shutdown(void);

/**
 * Clear eet cache
 *
 * For a faster access to previously accessed data, Eet keeps an internal
 * cache of files. These files will be freed automatically only when
 * they are unused and the cache gets full, in order based on the last time
 * they were used.
 * On systems with little memory this may present an unnecessary constraint,
 * so eet_clearcache() is available for users to reclaim the memory used by
 * files that are no longer needed. Those that were open using
 * ::EET_FILE_MODE_WRITE or ::EET_FILE_MODE_READ_WRITE and have modifications,
 * will be written down to disk before flushing them from memory.
 *
 * @since 1.0.0
 * @ingroup Eet_Group
 */
EAPI void
eet_clearcache(void);

/**
 * Open an eet file on disk, and returns a handle to it.
 * @param file The file path to the eet file. eg: @c "/tmp/file.eet".
 * @param mode The mode for opening. Either #EET_FILE_MODE_READ,
 *        #EET_FILE_MODE_WRITE or #EET_FILE_MODE_READ_WRITE.
 * @return An opened eet file handle.
 * @ingroup Eet_File_Group
 *
 * This function will open an exiting eet file for reading, and build
 * the directory table in memory and return a handle to the file, if it
 * exists and can be read, and no memory errors occur on the way, otherwise
 * NULL will be returned.
 *
 * It will also open an eet file for writing. This will, if successful,
 * delete the original file and replace it with a new empty file, till
 * the eet file handle is closed or flushed. If it cannot be opened for
 * writing or a memory error occurs, NULL is returned.
 *
 * You can also open the file for read/write. If you then write a key that
 * does not exist it will be created, if the key exists it will be replaced
 * by the new data.
 *
 * If the same file is opened multiple times, then the same file handle will
 * be returned as eet maintains an internal list of all currently open
 * files. Note that it considers files opened for read only and those opened
 * for read/write and write only as 2 separate sets. Those that do not write
 * to the file and those that do. Eet will allow 2 handles to the same file
 * if they are in the 2 separate lists/groups. That means opening a file for
 * read only looks in the read only set, and returns a handle to that file
 * handle and increments its reference count. If you open a file for read/write
 * or write only it looks in the write set and returns a handle after
 * incrementing the reference count. You need to close an eet file handle
 * as many times as it has been opened to maintain correct reference counts.
 * Files whose modified timestamp or size do not match those of the existing
 * referenced file handles will not be returned and a new handle will be
 * returned instead.
 *
 * @since 1.0.0
 */
EAPI Eet_File *
eet_open(const char *file,
         Eet_File_Mode mode);

/**
 * Open an eet file on disk from an Eina_File handle, and returns a handle to it.
 * @param file The Eina_File handle to map to an eet file.
 * @return An opened eet file handle.
 * @ingroup Eet_File_Group
 *
 * This function will open an exiting eet file for reading, and build
 * the directory table in memory and return a handle to the file, if it
 * exists and can be read, and no memory errors occur on the way, otherwise
 * NULL will be returned.
 *
 * This function can't open file for writing only read only mode is supported for now.
 *
 * If the same file is opened multiple times, then the same file handle will
 * be returned as eet maintains an internal list of all currently open
 * files. That means opening a file for read only looks in the read only set,
 * and returns a handle to that file handle and increments its reference count.
 * You need to close an eet file handle as many times as it has been opened to
 * maintain correct reference counts.
 *
 * @since 1.8.0
 */
EAPI Eet_File *
eet_mmap(const Eina_File *file);

/**
 * Open an eet file directly from a memory location. The data is not copied,
 * so you must keep it around as long as the eet file is open. There is
 * currently no cache for this kind of Eet_File, so it's reopened every time
 * you use eet_memopen_read.
 * @param data Address of file in memory.
 * @param size Size of memory to be read.
 * @return A handle to the file.
 *
 * Files opened this way will always be in read-only mode.
 *
 * @since 1.1.0
 * @ingroup Eet_File_Group
 */
EAPI Eet_File *
eet_memopen_read(const void *data,
                 size_t size);

/**
 * Get the mode an Eet_File was opened with.
 * @param ef A valid eet file handle.
 * @return The mode ef was opened with.
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI Eet_File_Mode
eet_mode_get(Eet_File *ef);

/**
 * Close an eet file handle and flush pending writes.
 * @param ef A valid eet file handle.
 * @return An eet error identifier.
 *
 * This function will flush any pending writes to disk if the eet file
 * was opened for write, and free all data associated with the file handle
 * and file, and close the file. If it was opened for read (or read/write),
 * the file handle may still be held open internally for caching purposes.
 * To flush speculatively held eet file handles use eet_clearcache().
 *
 * If the eet file handle is not valid nothing will be done.
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 *
 * @see eet_clearcache()
 */
EAPI Eet_Error
eet_close(Eet_File *ef);

/**
 * Sync content of an eet file handle, flushing pending writes.
 * @param ef A valid eet file handle.
 * @return An eet error identifier.
 *
 * This function will flush any pending writes to disk. The eet file must
 * be opened for write.
 *
 * If the eet file handle is not valid nothing will be done.
 *
 * @since 1.2.4
 * @ingroup Eet_File_Group
 */
EAPI Eet_Error
eet_sync(Eet_File *ef);

/**
 * Return a handle to the shared string dictionary of the Eet file
 * @param ef A valid eet file handle.
 * @return A handle to the dictionary of the file
 *
 * This function returns a handle to the dictionary of an Eet file whose
 * handle is @p ef, if a dictionary exists. NULL is returned otherwise or
 * if the file handle is known to be invalid.
 *
 * @see eet_dictionary_string_check() to know if given string came
 *      from the dictionary or it was dynamically allocated using
 *      the #Eet_Data_Descriptor_Class instructions.
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI Eet_Dictionary *
eet_dictionary_get(Eet_File *ef);

/**
 * Check if a given string comes from a given dictionary
 * @param ed A valid dictionary handle
 * @param string A valid 0 byte terminated C string
 * @return 1 if it is in the dictionary, 0 otherwise
 *
 * This checks the given dictionary to see if the given string is actually
 * inside that dictionary (i.e. comes from it) and returns 1 if it does.
 * If the dictionary handle is invalid, the string is NULL or the string is
 * not in the dictionary, 0 is returned.
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI int
eet_dictionary_string_check(Eet_Dictionary *ed,
                            const char *string);

/**
 * Return the number of strings inside a dictionary
 * @param ed A valid dictionary handle
 * @return the number of strings inside a dictionary
 *
 * @since 1.6.0
 * @ingroup Eet_File_Group
 */
EAPI int
eet_dictionary_count(const Eet_Dictionary *ed);

/**
 * Read a specified entry from an eet file and return data
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param size_ret Number of bytes read from entry and returned.
 * @return The data stored in that entry in the eet file.
 *
 * This function finds an entry in the eet file that is stored under the
 * name specified, and returns that data, decompressed, if successful.
 * NULL is returned if the lookup fails or if memory errors are
 * encountered. It is the job of the calling program to call free() on
 * the returned data. The number of bytes in the returned data chunk are
 * placed in size_ret.
 *
 * If the eet file handle is not valid NULL is returned and size_ret is
 * filled with 0.
 *
 * @see eet_read_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI void *
eet_read(Eet_File *ef,
         const char *name,
         int *size_ret);

/**
 * Read a specified entry from an eet file and return data
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param size_ret Number of bytes read from entry and returned.
 * @return The data stored in that entry in the eet file.
 *
 * This function finds an entry in the eet file that is stored under the
 * name specified, and returns that data if not compressed and successful.
 * NULL is returned if the lookup fails or if memory errors are
 * encountered or if the data is compressed. The calling program must never
 * call free() on the returned data. The number of bytes in the returned
 * data chunk are placed in size_ret.
 *
 * If the eet file handle is not valid NULL is returned and size_ret is
 * filled with 0.
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI const void *
eet_read_direct(Eet_File *ef,
                const char *name,
                int *size_ret);

/**
 * Write a specified entry to an eet file handle
 * @param ef A valid eet file handle opened for writing.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param data Pointer to the data to be stored.
 * @param size Length in bytes in the data to be stored.
 * @param compress Compression flags (1 == compress, 0 = don't compress).
 * @return bytes written on successful write, 0 on failure.
 *
 * This function will write the specified chunk of data to the eet file
 * and return greater than 0 on success. 0 will be returned on failure.
 *
 * The eet file handle must be a valid file handle for an eet file opened
 * for writing. If it is not, 0 will be returned and no action will be
 * performed.
 *
 * Name, and data must not be NULL, and size must be > 0. If these
 * conditions are not met, 0 will be returned.
 *
 * The data will be copied (and optionally compressed) in ram, pending
 * a flush to disk (it will stay in ram till the eet file handle is
 * closed though).
 *
 * @see eet_write_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI int
eet_write(Eet_File *ef,
          const char *name,
          const void *data,
          int size,
          int compress);

/**
 * Delete a specified entry from an Eet file being written or re-written
 * @param ef A valid eet file handle opened for writing.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @return Success or failure of the delete.
 *
 * This function will delete the specified chunk of data from the eet file
 * and return greater than 0 on success. 0 will be returned on failure.
 *
 * The eet file handle must be a valid file handle for an eet file opened
 * for writing. If it is not, 0 will be returned and no action will be
 * performed.
 *
 * Name, must not be NULL, otherwise 0 will be returned.
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI int
eet_delete(Eet_File *ef,
           const char *name);

/**
 * Alias a specific section to another one. Destination may exist or not,
 * no checks are done.
 * @param ef A valid eet file handle opened for writing.
 * @param name Name of the new entry. eg: "/base/file_i_want".
 * @param destination Actual source of the aliased entry eg: "/base/the_real_stuff_i_want".
 * @param compress Compression flags (1 == compress, 0 = don't compress).
 * @return EINA_TRUE on success, EINA_FALSE on failure.
 *
 * Name and Destination must not be NULL, otherwise EINA_FALSE will be returned.
 * The equivalent of this would be calling 'ln -s destination name'
 *
 * @since 1.3.3
 * @ingroup Eet_File_Group
 */
EAPI Eina_Bool
eet_alias(Eet_File *ef,
          const char *name,
          const char *destination,
          int compress);

/**
 * Retrieve the filename of an Eet_File
 * @param ef A valid eet file handle opened for writing.
 * @return The stringshared file string opened with eet_open(), or NULL on error
 *
 * @note This function will return NULL for files opened with eet_memopen_read()
 *
 * @since 1.6
 * @ingroup Eet_File_Group
 */
EAPI const char *
eet_file_get(Eet_File *ef);

/**
 * Retrieve the destination name of an alias
 * @param ef A valid eet file handle opened for writing
 * @param name Name of the entry. eg: "/base/file_i_want"
 * @return Destination of the alias. eg: "/base/the_real_stuff_i_want", NULL on failure
 *
 * Name must not be NULL, otherwise NULL will be returned.
 *
 * @since 1.5
 * @ingroup Eet_File_Group
 */
EAPI const char *
eet_alias_get(Eet_File *ef,
              const char *name);

/**
 * List all entries in eet file matching shell glob.
 * @param ef A valid eet file handle.
 * @param glob A shell glob to match against.
 * @param count_ret Number of entries found to match.
 * @return Pointer to an array of strings.
 *
 * This function will list all entries in the eet file matching the
 * supplied shell glob and return an allocated list of their names, if
 * there are any, and if no memory errors occur.
 *
 * The eet file handle must be valid and glob must not be NULL, or NULL
 * will be returned and count_ret will be filled with 0.
 *
 * The calling program must call free() on the array returned, but NOT
 * on the string pointers in the array. They are taken as read-only
 * internals from the eet file handle. They are only valid as long as
 * the file handle is not closed. When it is closed those pointers in the
 * array are now not valid and should not be used.
 *
 * On success the array returned will have a list of string pointers
 * that are the names of the entries that matched, and count_ret will have
 * the number of entries in this array placed in it.
 *
 * Hint: an easy way to list all entries in an eet file is to use a glob
 * value of "*".
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI char **
eet_list(Eet_File *ef,
         const char *glob,
         int *count_ret);

/**
 * Return an iterator that will describe each entry of an Eet_File.
 * @param ef A valid eet file handle.
 * @return An interator of Eet_Entry.
 *
 * @since 1.8.0
 * @ingroup Eet_File_Group
 */

EAPI Eina_Iterator *eet_list_entries(Eet_File *ef);

/**
 * Return the number of entries in the specified eet file.
 * @param ef A valid eet file handle.
 * @return Number of entries in ef or -1 if the number of entries
 *         cannot be read due to open mode restrictions.
 *
 * @since 1.0.0
 * @ingroup Eet_File_Group
 */
EAPI int
eet_num_entries(Eet_File *ef);

/**
 * @internal
 * @defgroup Eet_File_Cipher_Group Eet File Ciphered Main Functions
 *
 * Most of the @ref Eet_File_Group have alternative versions that
 * accounts for ciphers to protect their content.
 *
 * @see @ref Eet_Cipher_Group
 *
 * @ingroup Eet_File_Group
 */

/**
 * Read a specified entry from an eet file and return data using a cipher.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param size_ret Number of bytes read from entry and returned.
 * @param cipher_key The key to use as cipher.
 * @return The data stored in that entry in the eet file.
 *
 * This function finds an entry in the eet file that is stored under the
 * name specified, and returns that data, decompressed, if successful.
 * NULL is returned if the lookup fails or if memory errors are
 * encountered. It is the job of the calling program to call free() on
 * the returned data. The number of bytes in the returned data chunk are
 * placed in size_ret.
 *
 * If the eet file handle is not valid NULL is returned and size_ret is
 * filled with 0.
 *
 * @see eet_read()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Cipher_Group
 */
EAPI void *
eet_read_cipher(Eet_File *ef,
                const char *name,
                int *size_ret,
                const char *cipher_key);

/**
 * Write a specified entry to an eet file handle using a cipher.
 * @param ef A valid eet file handle opened for writing.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param data Pointer to the data to be stored.
 * @param size Length in bytes in the data to be stored.
 * @param compress Compression flags (1 == compress, 0 = don't compress).
 * @param cipher_key The key to use as cipher.
 * @return bytes written on successful write, 0 on failure.
 *
 * This function will write the specified chunk of data to the eet file
 * and return greater than 0 on success. 0 will be returned on failure.
 *
 * The eet file handle must be a valid file handle for an eet file opened
 * for writing. If it is not, 0 will be returned and no action will be
 * performed.
 *
 * Name, and data must not be NULL, and size must be > 0. If these
 * conditions are not met, 0 will be returned.
 *
 * The data will be copied (and optionally compressed) in ram, pending
 * a flush to disk (it will stay in ram till the eet file handle is
 * closed though).
 *
 * @see eet_write()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Cipher_Group
 */
EAPI int
eet_write_cipher(Eet_File *ef,
                 const char *name,
                 const void *data,
                 int size,
                 int compress,
                 const char *cipher_key);

/**
 * @internal
 * @defgroup Eet_File_Image_Group Image Store and Load
 * @ingroup Eet
 *
 * Eet efficiently stores and loads images, including alpha
 * channels and lossy compressions.
 *
 * Eet can handle both lossy compression with different levels of quality and
 * non-lossy compression with different compression levels. It's also possible,
 * given an image data, to only read its header to get the image information
 * without decoding the entire content for it.
 *
 * The encode family of functions will take an image raw buffer and its
 * parameters and compress it in memory, returning the new buffer.
 * Likewise, the decode functions will read from the given location in memory
 * and return the uncompressed image.
 *
 * The read and write functions will, respectively, encode and decode to or
 * from an Eet file, under the specified key.
 *
 * These functions are fairly low level and the same functionality can be
 * achieved using Evas and Edje, making it much easier to work with images
 * as well as not needing to worry about things like scaling them.
 */

/**
 * Read just the header data for an image and dont decode the pixels.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on successful decode, 0 otherwise
 *
 * Reads and decodes the image header data stored under the given key and
 * Eet file.
 *
 * The information decoded is placed in each of the parameters, which must be
 * provided. The width and height, measured in pixels, will be stored under
 * the variables pointed by @p w and @p h, respectively. If the read or
 * decode of the header fails, this values will be 0. The @p alpha parameter
 * will be 1 or 0, denoting if the alpha channel of the image is used or not.
 * If the image was losslessly compressed, the @p compress parameter will hold
 * the compression amount used, ranging from 0 to 9 and @p lossy will be 0.
 * In the case of lossy compression, @p lossy will be 1, and the compression
 * quality will be placed under @p quality, with a value ranging from 0 to 100.
 *
 * @see eet_data_image_header_decode()
 * @see eet_data_image_header_read_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Group
 */
EAPI int
eet_data_image_header_read(Eet_File *ef,
                           const char *name,
                           unsigned int *w,
                           unsigned int *h,
                           int *alpha,
                           int *compress,
                           int *quality,
                           Eet_Image_Encoding *lossy);

/**
 * Read image data from the named key in the eet file.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return The image pixel data decoded
 *
 * Reads and decodes the image stored in the given Eet file under the named
 * key.
 *
 * The returned pixel data is a linear array of pixels starting from the
 * top-left of the image, scanning row by row from left to right. Each pile
 * is a 32bit value, with the high byte being the alpha channel, the next being
 * red, then green, and the low byte being blue.
 *
 * The rest of the parameters are the same as in eet_data_image_header_read().
 *
 * On success the function returns a pointer to the image data decoded. The
 * calling application is responsible for calling free() on the image data
 * when it is done with it. On failure NULL is returned and the parameter
 * values may not contain any sensible data.
 *
 * @see eet_data_image_header_read()
 * @see eet_data_image_decode()
 * @see eet_data_image_read_cipher()
 * @see eet_data_image_read_to_surface()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Group
 */
EAPI void *
eet_data_image_read(Eet_File *ef,
                    const char *name,
                    unsigned int *w,
                    unsigned int *h,
                    int *alpha,
                    int *compress,
                    int *quality,
                    Eet_Image_Encoding *lossy);

/**
 * Read image data from the named key in the eet file and store it in the given buffer.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param src_x The starting x coordinate from where to dump the stream.
 * @param src_y The starting y coordinate from where to dump the stream.
 * @param d A pointer to the pixel surface.
 * @param w The expected width in pixels of the pixel surface to decode.
 * @param h The expected height in pixels of the pixel surface to decode.
 * @param row_stride The length of a pixels line in the destination surface.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 otherwise.
 *
 * Reads and decodes the image stored in the given Eet file, placing the
 * resulting pixel data in the buffer pointed by the user.
 *
 * Like eet_data_image_read(), it takes the image data stored under the
 * @p name key in the @p ef file, but instead of returning a new buffer with
 * the pixel data, it places the result in the buffer pointed by @p d, which
 * must be provided by the user and of sufficient size to hold the requested
 * portion of the image.
 *
 * The @p src_x and @p src_y parameters indicate the top-left corner of the
 * section of the image to decode. These have to be higher or equal than 0 and
 * less than the respective total width and height of the image. The width
 * and height of the section of the image to decode are given in @p w and @p h
 * and also can't be higher than the total width and height of the image.
 *
 * The @p row_stride parameter indicates the length in bytes of each line in
 * the destination buffer and it has to be at least @p w * 4.
 *
 * All the other parameters are the same as in eet_data_image_read().
 *
 * On success the function returns 1, and 0 on failure. On failure the
 * parameter values may not contain any sensible data.
 *
 * @see eet_data_image_read()
 * @see eet_data_image_decode()
 * @see eet_data_image_decode_to_surface()
 * @see eet_data_image_read_to_surface_cipher()
 * @see eet_data_image_decode_to_cspace_surface_cipher()
 *
 * @since 1.0.2
 * @ingroup Eet_File_Image_Group
 */
EAPI int
eet_data_image_read_to_surface(Eet_File *ef,
                               const char *name,
                               unsigned int src_x,
                               unsigned int src_y,
                               unsigned int *d,
                               unsigned int w,
                               unsigned int h,
                               unsigned int row_stride,
                               int *alpha,
                               int *compress,
                               int *quality,
                               Eet_Image_Encoding *lossy);

/**
 * Write image data to the named key in an eet file.
 * @param ef A valid eet file handle opened for writing.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param data A pointer to the image pixel data.
 * @param w The width of the image in pixels.
 * @param h The height of the image in pixels.
 * @param alpha The alpha channel flag.
 * @param compress The compression amount.
 * @param quality The quality encoding amount.
 * @param lossy The lossiness flag.
 * @return Success if the data was encoded and written or not.
 *
 * This function takes image pixel data and encodes it in an eet file
 * stored under the supplied name key, and returns how many bytes were
 * actually written to encode the image data.
 *
 * The data expected is the same format as returned by eet_data_image_read.
 * If this is not the case weird things may happen. Width and height must
 * be between 1 and 8000 pixels. The alpha flags can be 0 or 1 (0 meaning
 * the alpha values are not useful and 1 meaning they are). Compress can
 * be from 0 to 9 (0 meaning no compression, 9 meaning full compression).
 * This is only used if the image is not lossily encoded. Quality is used on
 * lossy compression and should be a value from 0 to 100. The lossy flag
 * can be 0 or 1. 0 means encode losslessly and 1 means to encode with
 * image quality loss (but then have a much smaller encoding).
 *
 * On success this function returns the number of bytes that were required
 * to encode the image data, or on failure it returns 0.
 *
 * @see eet_data_image_read()
 * @see eet_data_image_encode()
 * @see eet_data_image_write_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Group
 */
EAPI int
eet_data_image_write(Eet_File *ef,
                     const char *name,
                     const void *data,
                     unsigned int w,
                     unsigned int h,
                     int alpha,
                     int compress,
                     int quality,
                     Eet_Image_Encoding lossy);

/**
 * Decode Image data header only to get information.
 * @param data The encoded pixel data.
 * @param size The size, in bytes, of the encoded pixel data.
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 on failure.
 *
 * This function works exactly like eet_data_image_header_read(), but instead
 * of reading from an Eet file, it takes the buffer of size @p size pointed
 * by @p data, which must be a valid Eet encoded image.
 *
 * On success the function returns 1 indicating the header was read and
 * decoded properly, or 0 on failure.
 *
 * @see eet_data_image_header_read()
 * @see eet_data_image_header_decode_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Group
 */
EAPI int
eet_data_image_header_decode(const void *data,
                             int size,
                             unsigned int *w,
                             unsigned int *h,
                             int *alpha,
                             int *compress,
                             int *quality,
                             Eet_Image_Encoding *lossy);

/**
 * Decode Image data into pixel data.
 * @param data The encoded pixel data.
 * @param size The size, in bytes, of the encoded pixel data.
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return The image pixel data decoded
 *
 * This function takes encoded pixel data and decodes it into raw RGBA
 * pixels on success.
 *
 * It works exactly like eet_data_image_read(), but it takes the encoded
 * data in the @p data buffer of size @p size, instead of reading from a file.
 * All the others parameters are also the same.
 *
 * On success the function returns a pointer to the image data decoded. The
 * calling application is responsible for calling free() on the image data
 * when it is done with it. On failure NULL is returned and the parameter
 * values may not contain any sensible data.
 *
 * @see eet_data_image_read()
 * @see eet_data_image_decode_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Group
 */
EAPI void *
eet_data_image_decode(const void *data,
                      int size,
                      unsigned int *w,
                      unsigned int *h,
                      int *alpha,
                      int *compress,
                      int *quality,
                      Eet_Image_Encoding *lossy);

/**
 * Decode Image data into pixel data and stores in the given buffer.
 * @param data The encoded pixel data.
 * @param size The size, in bytes, of the encoded pixel data.
 * @param src_x The starting x coordinate from where to dump the stream.
 * @param src_y The starting y coordinate from where to dump the stream.
 * @param d A pointer to the pixel surface.
 * @param w The expected width in pixels of the pixel surface to decode.
 * @param h The expected height in pixels of the pixel surface to decode.
 * @param row_stride The length of a pixels line in the destination surface.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 otherwise.
 *
 * Like eet_data_image_read_to_surface(), but reading the given @p data buffer
 * instead of a file.
 *
 * On success the function returns 1, and 0 on failure. On failure the
 * parameter values may not contain any sensible data.
 *
 * @see eet_data_image_read_to_surface()
 * @see eet_data_image_decode_to_surface_cipher()
 *
 * @since 1.0.2
 * @ingroup Eet_File_Image_Group
 */
EAPI int
eet_data_image_decode_to_surface(const void *data,
                                 int size,
                                 unsigned int src_x,
                                 unsigned int src_y,
                                 unsigned int *d,
                                 unsigned int w,
                                 unsigned int h,
                                 unsigned int row_stride,
                                 int *alpha,
                                 int *compress,
                                 int *quality,
                                 Eet_Image_Encoding *lossy);

/**
 * Encode image data for storage or transmission.
 * @param data A pointer to the image pixel data.
 * @param size_ret A pointer to an int to hold the size of the returned data.
 * @param w The width of the image in pixels.
 * @param h The height of the image in pixels.
 * @param alpha The alpha channel flag.
 * @param compress The compression amount.
 * @param quality The quality encoding amount.
 * @param lossy The lossiness flag.
 * @return The encoded image data.
 *
 * This function stakes image pixel data and encodes it with compression and
 * possible loss of quality (as a trade off for size) for storage or
 * transmission to another system.
 *
 * It works like eet_data_image_write(), but instead of writing the encoded
 * image into an Eet file, it allocates a new buffer of the size required and
 * returns the encoded data in it.
 *
 * On success this function returns a pointer to the encoded data that you
 * can free with free() when no longer needed.
 *
 * @see eet_data_image_write()
 * @see eet_data_image_read()
 * @see eet_data_image_encode_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Group
 */
EAPI void *
eet_data_image_encode(const void *data,
                      int *size_ret,
                      unsigned int w,
                      unsigned int h,
                      int alpha,
                      int compress,
                      int quality,
                      Eet_Image_Encoding lossy);

/**
 * @internal
 * @defgroup Eet_File_Image_Cipher_Group Image Store and Load using a Cipher
 *
 * Most of the @ref Eet_File_Image_Group have alternative versions
 * that accounts for ciphers to protect their content.
 *
 * @see @ref Eet_Cipher_Group
 *
 * @ingroup Eet_File_Image_Group
 */

/**
 * Read just the header data for an image and dont decode the pixels using a cipher.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on successful decode, 0 otherwise
 *
 * This function reads an image from an eet file stored under the named
 * key in the eet file and return a pointer to the decompressed pixel data.
 *
 * The other parameters of the image (width, height etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns 1 indicating the header was read and
 * decoded properly, or 0 on failure.
 *
 * @see eet_data_image_header_read()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI int
eet_data_image_header_read_cipher(Eet_File *ef,
                                  const char *name,
                                  const char *cipher_key,
                                  unsigned int *w,
                                  unsigned int *h,
                                  int *alpha,
                                  int *compress,
                                  int *quality,
                                  Eet_Image_Encoding *lossy);

/**
 * Get the colorspace Eet can decode into of a given eet image ressource
 *
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param cspaces Returned pointer by Eet to a list of possible decoding colorspace finished by @c EET_COLORSPACE_ARGB8888. If @c NULL, only EET_COLORSPACE_ARGB8888 is supported.
 * @return 1 on successful get, 0 otherwise.
 *
 * @since 1.10.0
 * @ingroup Eet_File_Image_Group
 */
EAPI int
eet_data_image_colorspace_get(Eet_File *ef,
                              const char *name,
                              const char *cipher_key,
                              const Eet_Colorspace **cspaces);

/**
 * Read image data from the named key in the eet file using a cipher.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return The image pixel data decoded
 *
 * This function reads an image from an eet file stored under the named
 * key in the eet file and return a pointer to the decompressed pixel data.
 *
 * The other parameters of the image (width, height etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns a pointer to the image data decoded. The
 * calling application is responsible for calling free() on the image data
 * when it is done with it. On failure NULL is returned and the parameter
 * values may not contain any sensible data.
 *
 * @see eet_data_image_read()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI void *
eet_data_image_read_cipher(Eet_File *ef,
                           const char *name,
                           const char *cipher_key,
                           unsigned int *w,
                           unsigned int *h,
                           int *alpha,
                           int *compress,
                           int *quality,
                           Eet_Image_Encoding *lossy);

/**
 * Read image data from the named key in the eet file using a cipher.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param src_x The starting x coordinate from where to dump the stream.
 * @param src_y The starting y coordinate from where to dump the stream.
 * @param d A pointer to the pixel surface.
 * @param w The expected width in pixels of the pixel surface to decode.
 * @param h The expected height in pixels of the pixel surface to decode.
 * @param row_stride The length of a pixels line in the destination surface.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 otherwise.
 *
 * This function reads an image from an eet file stored under the named
 * key in the eet file and store the decompressed pixel data in the provided
 * surface with an @c EET_COLORSPACE_ARGB8888 colorspace.
 *
 * The other parameters of the image (width, height etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns 1, and 0 on failure. On failure the
 * parameter values may not contain any sensible data.
 *
 * @see eet_data_image_read_to_surface()
 * @see eet_data_image_decode_to_cspace_surface_cipher()
 *
 * @since 1.0.2
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI int
eet_data_image_read_to_surface_cipher(Eet_File *ef,
                                      const char *name,
                                      const char *cipher_key,
                                      unsigned int src_x,
                                      unsigned int src_y,
                                      unsigned int *d,
                                      unsigned int w,
                                      unsigned int h,
                                      unsigned int row_stride,
                                      int *alpha,
                                      int *compress,
                                      int *quality,
                                      Eet_Image_Encoding *lossy);


/**
 * Read image data from the named key in the eet file using a cipher.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param src_x The starting x coordinate from where to dump the stream.
 * @param src_y The starting y coordinate from where to dump the stream.
 * @param d A pointer to the pixel surface.
 * @param w The expected width in pixels of the pixel surface to decode.
 * @param h The expected height in pixels of the pixel surface to decode.
 * @param row_stride The length of a pixels line in the destination surface.
 * @param cspace The color space of the pixels bsurface.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param comp A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 otherwise.
 *
 * This function reads an image from an eet file stored under the named
 * key in the eet file and store the decompressed pixel data in the provided
 * surface colorspace.
 *
 * The other parameters of the image (width, height etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns 1, and 0 on failure. On failure the
 * parameter values may not contain any sensible data.
 *
 * @see eet_data_image_read_to_surface()
 * @see eet_data_image_decode_to_cspace_surface_cipher()
 * @see eet_data_image_read_to_surface_cipher()
 *
 * @since 1.10.0
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI int
eet_data_image_read_to_cspace_surface_cipher(Eet_File     *ef,
                                             const char   *name,
                                             const char   *cipher_key,
                                             unsigned int  src_x,
                                             unsigned int  src_y,
                                             unsigned int *d,
                                             unsigned int  w,
                                             unsigned int  h,
                                             unsigned int  row_stride,
                                             Eet_Colorspace cspace,
                                             int          *alpha,
                                             int          *comp,
                                             int          *quality,
                                             Eet_Image_Encoding *lossy);


/**
 * Read image data from the named key in the eet file using a cipher.
 * @param ef A valid eet file handle opened for reading.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param src_x The starting x coordinate from where to dump the stream.
 * @param src_y The starting y coordinate from where to dump the stream.
 * @param d A pointer to the pixel surface.
 * @param w The expected width in pixels of the pixel surface to decode.
 * @param h The expected height in pixels of the pixel surface to decode.
 * @param row_stride The length of a pixels line in the destination surface.
 * @param cspace The color space of the pixel surface
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param comp A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 otherwise.
 *
 * This function reads an image from an eet file stored under the named
 * key in the eet file and store the decompressed pixels in the specified
 * color space inside the given surface.
 *
 * The other parameters of the image (width, height etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns 1, and 0 on failure. On failure the
 * parameter values may not contain any sensible data.
 *
 * @see eet_data_image_read_to_surface()
 * @see eet_data_image_read_to_surface_cipher()
 *
 * @since 1.10.0
 * @ingroup Eet_File_Image_Cipher_Group
 */

EAPI int
eet_data_image_decode_to_cspace_surface_cipher(const void   *data,
                                               const char   *cipher_key,
                                               int           size,
                                               unsigned int  src_x,
                                               unsigned int  src_y,
                                               unsigned int *d,
                                               unsigned int  w,
                                               unsigned int  h,
                                               unsigned int  row_stride,
                                               Eet_Colorspace cspace,
                                               int          *alpha,
                                               int          *comp,
                                               int          *quality,
                                               Eet_Image_Encoding *lossy);

/**
 * Write image data to the named key in an eet file using a cipher.
 * @param ef A valid eet file handle opened for writing.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param data A pointer to the image pixel data.
 * @param w The width of the image in pixels.
 * @param h The height of the image in pixels.
 * @param alpha The alpha channel flag.
 * @param compress The compression amount.
 * @param quality The quality encoding amount.
 * @param lossy The lossiness flag.
 * @return Success if the data was encoded and written or not.
 *
 * This function takes image pixel data and encodes it in an eet file
 * stored under the supplied name key, and returns how many bytes were
 * actually written to encode the image data.
 *
 * The data expected is the same format as returned by eet_data_image_read.
 * If this is not the case weird things may happen. Width and height must
 * be between 1 and 8000 pixels. The alpha flags can be 0 or 1 (0 meaning
 * the alpha values are not useful and 1 meaning they are). Compress can
 * be from 0 to 9 (0 meaning no compression, 9 meaning full compression).
 * This is only used if the image is not lossily encoded. Quality is used on
 * lossy compression and should be a value from 0 to 100. The lossy flag
 * can be 0 or 1. 0 means encode losslessly and 1 means to encode with
 * image quality loss (but then have a much smaller encoding).
 *
 * On success this function returns the number of bytes that were required
 * to encode the image data, or on failure it returns 0.
 *
 * @see eet_data_image_write()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI int
eet_data_image_write_cipher(Eet_File *ef,
                            const char *name,
                            const char *cipher_key,
                            const void *data,
                            unsigned int w,
                            unsigned int h,
                            int alpha,
                            int compress,
                            int quality,
                            Eet_Image_Encoding lossy);

/**
 * Decode Image data header only to get information using a cipher.
 * @param data The encoded pixel data.
 * @param cipher_key The key to use as cipher.
 * @param size The size, in bytes, of the encoded pixel data.
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 on failure.
 *
 * This function takes encoded pixel data and decodes it into raw RGBA
 * pixels on success.
 *
 * The other parameters of the image (width, height etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns 1 indicating the header was read and
 * decoded properly, or 0 on failure.
 *
 * @see eet_data_image_header_decode()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI int
eet_data_image_header_decode_cipher(const void *data,
                                    const char *cipher_key,
                                    int size,
                                    unsigned int *w,
                                    unsigned int *h,
                                    int *alpha,
                                    int *compress,
                                    int *quality,
                                    Eet_Image_Encoding *lossy);

/**
 * Decode Image data into pixel data using a cipher.
 * @param data The encoded pixel data.
 * @param cipher_key The key to use as cipher.
 * @param size The size, in bytes, of the encoded pixel data.
 * @param w A pointer to the unsigned int to hold the width in pixels.
 * @param h A pointer to the unsigned int to hold the height in pixels.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return The image pixel data decoded
 *
 * This function takes encoded pixel data and decodes it into raw RGBA
 * pixels on success.
 *
 * The other parameters of the image (width, height etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns a pointer to the image data decoded. The
 * calling application is responsible for calling free() on the image data
 * when it is done with it. On failure NULL is returned and the parameter
 * values may not contain any sensible data.
 *
 * @see eet_data_image_decode()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI void *
eet_data_image_decode_cipher(const void *data,
                             const char *cipher_key,
                             int size,
                             unsigned int *w,
                             unsigned int *h,
                             int *alpha,
                             int *compress,
                             int *quality,
                             Eet_Image_Encoding *lossy);

/**
 * Decode Image data into pixel data using a cipher.
 * @param data The encoded pixel data.
 * @param cipher_key The key to use as cipher.
 * @param size The size, in bytes, of the encoded pixel data.
 * @param src_x The starting x coordinate from where to dump the stream.
 * @param src_y The starting y coordinate from where to dump the stream.
 * @param d A pointer to the pixel surface.
 * @param w The expected width in pixels of the pixel surface to decode.
 * @param h The expected height in pixels of the pixel surface to decode.
 * @param row_stride The length of a pixels line in the destination surface.
 * @param alpha A pointer to the int to hold the alpha flag.
 * @param compress A pointer to the int to hold the compression amount.
 * @param quality A pointer to the int to hold the quality amount.
 * @param lossy A pointer to the int to hold the lossiness flag.
 * @return 1 on success, 0 otherwise.
 *
 * This function takes encoded pixel data and decodes it into raw RGBA
 * pixels on success.
 *
 * The other parameters of the image (alpha, compress etc.) are placed into
 * the values pointed to (they must be supplied). The pixel data is a linear
 * array of pixels starting from the top-left of the image scanning row by
 * row from left to right. Each pixel is a 32bit value, with the high byte
 * being the alpha channel, the next being red, then green, and the low byte
 * being blue. The width and height are measured in pixels and will be
 * greater than 0 when returned. The alpha flag is either 0 or 1. 0 denotes
 * that the alpha channel is not used. 1 denotes that it is significant.
 * Compress is filled with the compression value/amount the image was
 * stored with. The quality value is filled with the quality encoding of
 * the image file (0 - 100). The lossy flags is either 0 or 1 as to if
 * the image was encoded lossily or not.
 *
 * On success the function returns 1, and 0 on failure. On failure the
 * parameter values may not contain any sensible data.
 *
 * @see eet_data_image_decode_to_surface()
 *
 * @since 1.0.2
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI int
eet_data_image_decode_to_surface_cipher(const void *data,
                                        const char *cipher_key,
                                        int size,
                                        unsigned int src_x,
                                        unsigned int src_y,
                                        unsigned int *d,
                                        unsigned int w,
                                        unsigned int h,
                                        unsigned int row_stride,
                                        int *alpha,
                                        int *compress,
                                        int *quality,
                                        Eet_Image_Encoding *lossy);

/**
 * Encode image data for storage or transmission using a cipher.
 * @param data A pointer to the image pixel data.
 * @param cipher_key The key to use as cipher.
 * @param size_ret A pointer to an int to hold the size of the returned data.
 * @param w The width of the image in pixels.
 * @param h The height of the image in pixels.
 * @param alpha The alpha channel flag.
 * @param compress The compression amount.
 * @param quality The quality encoding amount.
 * @param lossy The lossiness flag.
 * @return The encoded image data.
 *
 * This function stakes image pixel data and encodes it with compression and
 * possible loss of quality (as a trade off for size) for storage or
 * transmission to another system.
 *
 * The data expected is the same format as returned by eet_data_image_read.
 * If this is not the case weird things may happen. Width and height must
 * be between 1 and 8000 pixels. The alpha flags can be 0 or 1 (0 meaning
 * the alpha values are not useful and 1 meaning they are). Compress can
 * be from 0 to 9 (0 meaning no compression, 9 meaning full compression).
 * This is only used if the image is not lossily encoded. Quality is used on
 * lossy compression and should be a value from 0 to 100. The lossy flag
 * can be 0 or 1. 0 means encode losslessly and 1 means to encode with
 * image quality loss (but then have a much smaller encoding).
 *
 * On success this function returns a pointer to the encoded data that you
 * can free with free() when no longer needed.
 *
 * @see eet_data_image_encode()
 *
 * @since 1.0.0
 * @ingroup Eet_File_Image_Cipher_Group
 */
EAPI void *
eet_data_image_encode_cipher(const void *data,
                             const char *cipher_key,
                             unsigned int w,
                             unsigned int h,
                             int alpha,
                             int compress,
                             int quality,
                             Eet_Image_Encoding lossy,
                             int *size_ret);

/**
 * @internal
 * @defgroup Eet_Cipher_Group Cipher, Identity and Protection Mechanisms
 * @ingroup Eet
 *
 * Eet allows one to protect entries of an #Eet_File
 * individually. This may be used to ensure data was not tampered or
 * that third party does not read your data.
 *
 * @see @ref Eet_File_Cipher_Group
 * @see @ref Eet_File_Image_Cipher_Group
 *
 * @{
 */

/**
 * @typedef Eet_Key
 * Opaque handle that defines an identity (also known as key)
 * in Eet's cipher system.
 */
typedef struct _Eet_Key Eet_Key;

/**
 * @}
 */

/**
 * Callback used to request if needed the password of a private key.
 *
 * @param buffer the buffer where to store the password.
 * @param size the maximum password size (size of buffer, including '@\0').
 * @param rwflag if the buffer is also readable or just writable.
 * @param data currently unused, may contain some context in future.
 * @return 1 on success and password was set to @p buffer, 0 on failure.
 *
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
typedef int (*Eet_Key_Password_Callback)(char *buffer, int size, int rwflag, void *data);

/**
 * Create an Eet_Key needed for signing an eet file.
 *
 * The certificate should provide the public that match the private key.
 * No verification is done to ensure that.
 *
 * @param certificate_file The file where to find the certificate.
 * @param private_key_file The file that contains the private key.
 * @param cb Function to callback if password is required to unlock
 *        private key.
 * @return A key handle to use, or @c NULL on failure.
 *
 * @see eet_identity_close()
 *
 * @warning You need to compile signature support in EET.
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
EAPI Eet_Key *
eet_identity_open(const char *certificate_file,
                  const char *private_key_file,
                  Eet_Key_Password_Callback cb);

/**
 * Close and release all resource used by an Eet_Key.  An
 * reference counter prevent it from being freed until all file
 * using it are also closed.
 *
 * @param key the key handle to close and free resources.
 *
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
EAPI void
eet_identity_close(Eet_Key *key);

/**
 * Set a key to sign a file
 *
 * @param ef the file to set the identity.
 * @param key the key handle to set as identity.
 * @return #EET_ERROR_BAD_OBJECT if @p ef is invalid or
 *         #EET_ERROR_NONE on success.
 *
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
EAPI Eet_Error
eet_identity_set(Eet_File *ef,
                 Eet_Key *key);

/**
 * Display both private and public key of an Eet_Key.
 *
 * @param key the handle to print.
 * @param out where to print.
 *
 * @warning You need to compile signature support in EET.
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
EAPI void
eet_identity_print(Eet_Key *key,
                   FILE *out);

/**
 * Compare the identify certificate of an eet file against a stored one
 *
 * @param ef The file handle to check the identify of
 * @param certificate_file The path to the certificate file
 * @return EINA_TRUE if the certificates match, otherwise EINA_FALSE;
 *
 * The @p ef file handle mus be valid, and a signed file, otherwise
 * checking will fail. The path to the certificate file must be a valid
 * file path to a 'pem' format file (the same used for siging with
 * eet_identity_open() as a certificate file).
 *
 * @note This function can not be used to provide any security mecanism. You
 * need to check your x509 certificate against a chain of trust to have a proper
 * security. This is just a convenience test function.
 * @warning You need to compile signature support in EET.
 * @since 1.13
 * @ingroup Eet_Cipher_Group
 */
EAPI Eina_Bool
eet_identity_verify(Eet_File *ef,
                    const char *certificate_file);

/**
 * Get the x509 der certificate associated with an Eet_File. Will return NULL
 * if the file is not signed.
 *
 * @param ef The file handle to query.
 * @param der_length The length of returned data, may be @c NULL.
 * @return the x509 certificate or @c NULL on error.
 *
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
EAPI const void *
eet_identity_x509(Eet_File *ef,
                  int *der_length);

/**
 * Get the raw signature associated with an Eet_File. Will return NULL
 * if the file is not signed.
 *
 * @param ef The file handle to query.
 * @param signature_length The length of returned data, may be @c NULL.
 * @return the raw signature or @c NULL on error.
 *
 * @ingroup Eet_Cipher_Group
 */
EAPI const void *
eet_identity_signature(Eet_File *ef,
                       int *signature_length);

/**
 * Get the SHA1 associated with a file. Could be the one used to
 * sign the data or if the data where not signed, it will be the
 * SHA1 of the file.
 *
 * @param ef The file handle to query.
 * @param sha1_length The length of returned data, may be @c NULL.
 * @return the associated SHA1 or @c NULL on error.
 *
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
EAPI const void *
eet_identity_sha1(Eet_File *ef,
                  int *sha1_length);

/**
 * Display the x509 der certificate to out.
 *
 * @param certificate the x509 certificate to print
 * @param der_length The length the certificate.
 * @param out where to print.
 *
 * @warning You need to compile signature support in EET.
 * @since 1.2.0
 * @ingroup Eet_Cipher_Group
 */
EAPI void
eet_identity_certificate_print(const unsigned char *certificate,
                               int der_length,
                               FILE *out);

/**
 * This function creates a new data descriptor and returns a handle to the
 * new data descriptor. On creation it will be empty, containing no contents
 * describing anything other than the shell of the data structure.
 * @param eddc The class from where to create the data descriptor.
 * @return A handle to the new data descriptor.
 *
 * You add structure members to the data descriptor using the macros
 * EET_DATA_DESCRIPTOR_ADD_BASIC(), EET_DATA_DESCRIPTOR_ADD_SUB() and
 * EET_DATA_DESCRIPTOR_ADD_LIST(), depending on what type of member you are
 * adding to the description.
 *
 * Once you have described all the members of a struct you want loaded or
 * saved, eet can load and save those members for you, encode them into
 * endian-independent serialised data chunks for transmission across a
 * network or more.
 *
 * This function specially ignores str_direct_alloc and str_direct_free. It
 * is useful when the eet_data you are reading doesn't have a dictionary,
 * like network stream or IPC. It also mean that all string will be allocated
 * and duplicated in memory.
 *
 * @since 1.2.3
 * @ingroup Eet_Data_Group
 */
EAPI Eet_Data_Descriptor *
eet_data_descriptor_stream_new(const Eet_Data_Descriptor_Class *eddc);

/**
 * This function creates a new data descriptor and returns a handle to the
 * new data descriptor. On creation it will be empty, containing no contents
 * describing anything other than the shell of the data structure.
 * @param eddc The class from where to create the data descriptor.
 * @return A handle to the new data descriptor.
 *
 * You add structure members to the data descriptor using the macros
 * EET_DATA_DESCRIPTOR_ADD_BASIC(), EET_DATA_DESCRIPTOR_ADD_SUB() and
 * EET_DATA_DESCRIPTOR_ADD_LIST(), depending on what type of member you are
 * adding to the description.
 *
 * Once you have described all the members of a struct you want loaded or
 * saved, eet can load and save those members for you, encode them into
 * endian-independent serialised data chunks for transmission across a
 * a network or more.
 *
 * This function uses str_direct_alloc and str_direct_free. It is
 * useful when the eet_data you are reading come from a file and
 * have a dictionary. This will reduce memory use and improve the
 * possibility for the OS to page this string out.
 * However, the load speed and memory saving comes with some drawbacks to keep
 * in mind. If you never modify the contents of the structures loaded from
 * the file, all you need to remember is that closing the eet file will make
 * the strings go away. On the other hand, should you need to free a string,
 * before doing so you have to verify that it's not part of the eet dictionary.
 * You can do this in the following way, assuming @p ef is a valid Eet_File
 * and @p str is a string loaded from said file.
 *
 * @code
 * void eet_string_free(Eet_File *ef, const char *str)
 * {
 *    Eet_Dictionary *dict = eet_dictionary_get(ef);
 *    if (dict && eet_dictionary_string_check(dict, str))
 *      {
 *          The file contains a dictionary and the given string is a part of
 *          of it, so we can't free it, just return.
 *         return;
 *      }
 *     We assume eina_stringshare was used on the descriptor
 *    eina_stringshare_del(str);
 * }
 * @endcode
 *
 * @since 1.2.3
 * @ingroup Eet_Data_Group
 */
EAPI Eet_Data_Descriptor *
eet_data_descriptor_file_new(const Eet_Data_Descriptor_Class *eddc);

/**
 * This function is an helper that set all the parameters of an
 * Eet_Data_Descriptor_Class correctly when you use Eina data type
 * with a stream.
 * @param eddc The Eet_Data_Descriptor_Class you want to set.
 * @param eddc_size The size of the Eet_Data_Descriptor_Class at the compilation time.
 * @param name The name of the structure described by this class.
 * @param size The size of the structure described by this class.
 * @return EINA_TRUE if the structure was correctly set (The only
 *         reason that could make it fail is if you did give wrong
 *         parameter).
 *
 * @note Unless there's a very specific reason to use this function directly,
 * the EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET macro is recommended.
 *
 * @since 1.2.3
 * @ingroup Eet_Data_Group
 */
EAPI Eina_Bool
eet_eina_stream_data_descriptor_class_set(Eet_Data_Descriptor_Class *eddc,
                                          unsigned int eddc_size,
                                          const char *name,
                                          int size);

/**
 * This macro is an helper that set all the parameter of an
 * Eet_Data_Descriptor_Class correctly when you use Eina data type
 * with stream.
 * @param clas The Eet_Data_Descriptor_Class you want to set.
 * @param type The type of the structure described by this class.
 * @return EINA_TRUE if the structure was correctly set (The only
 *         reason that could make it fail is if you did give wrong
 *         parameter).
 *
 * @see eet_data_descriptor_stream_new
 * @since 1.2.3
 * @ingroup Eet_Data_Group
 */
#define EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(clas, type) \
  (eet_eina_stream_data_descriptor_class_set(clas, sizeof (*(clas)), # type, sizeof(type)))

/**
 * This function is an helper that set all the parameter of an
 * Eet_Data_Descriptor_Class correctly when you use Eina data type
 * with a file.
 * @param eddc The Eet_Data_Descriptor_Class you want to set.
 * @param eddc_size The size of the Eet_Data_Descriptor_Class at the compilation time.
 * @param name The name of the structure described by this class.
 * @param size The size of the structure described by this class.
 * @return EINA_TRUE if the structure was correctly set (The only
 *         reason that could make it fail is if you did give wrong
 *         parameter).
 *
 * @note Unless there's a very specific reason to use this function directly,
 * the EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET macro is recommended.
 *
 * @since 1.2.3
 * @ingroup Eet_Data_Group
 */
EAPI Eina_Bool
eet_eina_file_data_descriptor_class_set(Eet_Data_Descriptor_Class *eddc,
                                        unsigned int eddc_size,
                                        const char *name,
                                        int size);

/**
 * This function frees a data descriptor when it is not needed anymore.
 * @param edd The data descriptor to free.
 *
 * This function takes a data descriptor handle as a parameter and frees all
 * data allocated for the data descriptor and the handle itself. After this
 * call the descriptor is no longer valid.
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI void
eet_data_descriptor_free(Eet_Data_Descriptor *edd);

/**
 * This function returns the name of a data descriptor.
 *
 * @since 1.8.0
 * @ingroup Eet_Data_Group
 */
EAPI const char *eet_data_descriptor_name_get(const Eet_Data_Descriptor *edd);

/**
 * This function is an internal used by macros.
 *
 * This function is used by macros EET_DATA_DESCRIPTOR_ADD_BASIC(),
 * EET_DATA_DESCRIPTOR_ADD_SUB() and EET_DATA_DESCRIPTOR_ADD_LIST(). It is
 * complex to use by hand and should be left to be used by the macros, and
 * thus is not documented.
 *
 * @param edd The data descriptor handle to add element (member).
 * @param name The name of element to be serialized.
 * @param type The type of element to be serialized, like
 *        #EET_T_INT. If #EET_T_UNKNOW, then it is considered to be a
 *        group, list or hash.
 * @param group_type If element type is #EET_T_UNKNOW, then the @p
 *        group_type will specify if it is a list (#EET_G_LIST),
 *        array (#EET_G_ARRAY) and so on. If #EET_G_UNKNOWN, then
 *        the member is a subtype (pointer to another type defined by
 *        another #Eet_Data_Descriptor).
 * @param offset byte offset inside the source memory to be serialized.
 * @param count number of elements (if #EET_G_ARRAY or #EET_G_VAR_ARRAY).
 * @param counter_name variable that defines the name of number of elements.
 * @param subtype If contains a subtype, then its data descriptor.
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI void
eet_data_descriptor_element_add(Eet_Data_Descriptor *edd,
                                const char *name,
                                int type,
                                int group_type,
                                int offset,
     /* int                  count_offset, */
                                int count,
                                const char *counter_name,
                                Eet_Data_Descriptor *subtype);

/**
 * Read a data structure from an eet file and decodes it.
 * @param ef The eet file handle to read from.
 * @param edd The data descriptor handle to use when decoding.
 * @param name The key the data is stored under in the eet file.
 * @return A pointer to the decoded data structure.
 *
 * This function decodes a data structure stored in an eet file, returning
 * a pointer to it if it decoded successfully, or NULL on failure. This
 * can save a programmer dozens of hours of work in writing configuration
 * file parsing and writing code, as eet does all that work for the program
 * and presents a program-friendly data structure, just as the programmer
 * likes. Eet can handle members being added or deleted from the data in
 * storage and safely zero-fills unfilled members if they were not found
 * in the data. It checks sizes and headers whenever it reads data, allowing
 * the programmer to not worry about corrupt data.
 *
 * Once a data structure has been described by the programmer with the
 * fields they wish to save or load, storing or retrieving a data structure
 * from an eet file, or from a chunk of memory is as simple as a single
 * function call.
 *
 * @see eet_data_read_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI void *
eet_data_read(Eet_File *ef,
              Eet_Data_Descriptor *edd,
              const char *name);

/**
 * Write a data structure from memory and store in an eet file.
 * @param ef The eet file handle to write to.
 * @param edd The data descriptor to use when encoding.
 * @param name The key to store the data under in the eet file.
 * @param data A pointer to the data structure to save and encode.
 * @param compress Compression flags for storage.
 * @return bytes written on successful write, 0 on failure.
 *
 * This function is the reverse of eet_data_read(), saving a data structure
 * to an eet file. The file must have been opening in write mode and the data
 * will be kept in memory until the file is either closed or eet_sync() is
 * called to flush any unwritten changes.
 *
 * @see eet_data_write_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI int
eet_data_write(Eet_File *ef,
               Eet_Data_Descriptor *edd,
               const char *name,
               const void *data,
               int compress);

/**
 * Dump an eet encoded data structure into ascii text
 * @param data_in The pointer to the data to decode into a struct.
 * @param size_in The size of the data pointed to in bytes.
 * @param dumpfunc The function to call passed a string when new
 *        data is converted to text
 * @param dumpdata The data to pass to the @p dumpfunc callback.
 * @return 1 on success, 0 on failure
 *
 * This function will take a chunk of data encoded by
 * eet_data_descriptor_encode() and convert it into human readable
 * ascii text.  It does this by calling the @p dumpfunc callback
 * for all new text that is generated. This callback should append
 * to any existing text buffer and will be passed the pointer @p
 * dumpdata as a parameter as well as a string with new text to be
 * appended.
 *
 * Example:
 *
 * @code
 * void output(void *data, const char *string)
 * {
 *   printf("%s", string);
 * }
 *
 * void dump(const char *file)
 * {
 *   FILE *f;
 *   int len;
 *   void *data;
 *
 *   f = fopen(file, "r");
 *   fseek(f, 0, SEEK_END);
 *   len = ftell(f);
 *   rewind(f);
 *   data = malloc(len);
 *   fread(data, len, 1, f);
 *   fclose(f);
 *   eet_data_text_dump(data, len, output, NULL);
 * }
 * @endcode
 *
 * @see eet_data_text_dump_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI int
eet_data_text_dump(const void *data_in,
                   int size_in,
                   Eet_Dump_Callback dumpfunc,
                   void *dumpdata);

/**
 * Take an ascii encoding from eet_data_text_dump() and re-encode in binary.
 * @param text The pointer to the string data to parse and encode.
 * @param textlen The size of the string in bytes (not including 0
 *        byte terminator).
 * @param size_ret This gets filled in with the encoded data blob
 *        size in bytes.
 * @return The encoded data on success, NULL on failure.
 *
 * This function will parse the string pointed to by @p text and return
 * an encoded data lump the same way eet_data_descriptor_encode() takes an
 * in-memory data struct and encodes into a binary blob. @p text is a normal
 * C string.
 *
 * @see eet_data_text_undump_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI void *
eet_data_text_undump(const char *text,
                     int textlen,
                     int *size_ret);

/**
 * Dump an eet encoded data structure from an eet file into ascii text
 * @param ef A valid eet file handle.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param dumpfunc The function to call passed a string when new
 *        data is converted to text
 * @param dumpdata The data to pass to the @p dumpfunc callback.
 * @return 1 on success, 0 on failure
 *
 * This function will take an open and valid eet file from
 * eet_open() request the data encoded by
 * eet_data_descriptor_encode() corresponding to the key @p name
 * and convert it into human readable ascii text. It does this by
 * calling the @p dumpfunc callback for all new text that is
 * generated. This callback should append to any existing text
 * buffer and will be passed the pointer @p dumpdata as a parameter
 * as well as a string with new text to be appended.
 *
 * @see eet_data_dump_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI int
eet_data_dump(Eet_File *ef,
              const char *name,
              Eet_Dump_Callback dumpfunc,
              void *dumpdata);

/**
 * Take an ascii encoding from eet_data_dump() and re-encode in binary.
 * @param ef A valid eet file handle.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param text The pointer to the string data to parse and encode.
 * @param textlen The size of the string in bytes (not including 0
 *        byte terminator).
 * @param compress Compression flags (1 == compress, 0 = don't compress).
 * @return 1 on success, 0 on failure
 *
 * This function will parse the string pointed to by @p text,
 * encode it the same way eet_data_descriptor_encode() takes an
 * in-memory data struct and encodes into a binary blob.
 *
 * The data (optionally compressed) will be in ram, pending a flush to
 * disk (it will stay in ram till the eet file handle is closed though).
 *
 * @see eet_data_undump_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI int
eet_data_undump(Eet_File *ef,
                const char *name,
                const char *text,
                int textlen,
                int compress);

/**
 * Decode a data structure from an arbitrary location in memory.
 * @param edd The data  descriptor to use when decoding.
 * @param data_in The pointer to the data to decode into a struct.
 * @param size_in The size of the data pointed to in bytes.
 * @return NULL on failure, or a valid decoded struct pointer on success.
 *
 * This function will decode a data structure that has been encoded using
 * eet_data_descriptor_encode(), and return a data structure with all its
 * elements filled out, if successful, or NULL on failure.
 *
 * The data to be decoded is stored at the memory pointed to by @p data_in,
 * and is described by the descriptor pointed to by @p edd. The data size is
 * passed in as the value to @p size_in, ande must be greater than 0 to
 * succeed.
 *
 * This function is useful for decoding data structures delivered to the
 * application by means other than an eet file, such as an IPC or socket
 * connection, raw files, shared memory etc.
 *
 * Please see eet_data_read() for more information.
 *
 * @see eet_data_descriptor_decode_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI void *
eet_data_descriptor_decode(Eet_Data_Descriptor *edd,
                           const void *data_in,
                           int size_in);

/**
 * Encode a dsata struct to memory and return that encoded data.
 * @param edd The data  descriptor to use when encoding.
 * @param data_in The pointer to the struct to encode into data.
 * @param size_ret pointer to the an int to be filled with the decoded size.
 * @return NULL on failure, or a valid encoded data chunk on success.
 *
 * This function takes a data structutre in memory and encodes it into a
 * serialised chunk of data that can be decoded again by
 * eet_data_descriptor_decode(). This is useful for being able to transmit
 * data structures across sockets, pipes, IPC or shared file mechanisms,
 * without having to worry about memory space, machine type, endianness etc.
 *
 * The parameter @p edd must point to a valid data descriptor, and
 * @p data_in must point to the right data structure to encode. If not, the
 * encoding may fail.
 *
 * On success a non NULL valid pointer is returned and what @p size_ret
 * points to is set to the size of this decoded data, in bytes. When the
 * encoded data is no longer needed, call free() on it. On failure NULL is
 * returned and what @p size_ret points to is set to 0.
 *
 * Please see eet_data_write() for more information.
 *
 * @see eet_data_descriptor_encode_cipher()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Group
 */
EAPI void *
eet_data_descriptor_encode(Eet_Data_Descriptor *edd,
                           const void *data_in,
                           int *size_ret);


/**
 * Read a data structure from an eet file and decodes it using a cipher.
 * @param ef The eet file handle to read from.
 * @param edd The data descriptor handle to use when decoding.
 * @param name The key the data is stored under in the eet file.
 * @param cipher_key The key to use as cipher.
 * @return A pointer to the decoded data structure.
 *
 * This function decodes a data structure stored in an eet file, returning
 * a pointer to it if it decoded successfully, or NULL on failure. This
 * can save a programmer dozens of hours of work in writing configuration
 * file parsing and writing code, as eet does all that work for the program
 * and presents a program-friendly data structure, just as the programmer
 * likes. Eet can handle members being added or deleted from the data in
 * storage and safely zero-fills unfilled members if they were not found
 * in the data. It checks sizes and headers whenever it reads data, allowing
 * the programmer to not worry about corrupt data.
 *
 * Once a data structure has been described by the programmer with the
 * fields they wish to save or load, storing or retrieving a data structure
 * from an eet file, or from a chunk of memory is as simple as a single
 * function call.
 *
 * @see eet_data_read()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI void *
eet_data_read_cipher(Eet_File *ef,
                     Eet_Data_Descriptor *edd,
                     const char *name,
                     const char *cipher_key);

/**
 * Read a data structure from an eet file and decodes it into a buffer using a cipher,
 * @param ef The eet file handle to read from.
 * @param edd The data descriptor handle to use when decoding.
 * @param name The key the data is stored under in the eet file.
 * @param cipher_key The key to use as cipher.
 * @param buffer Buffer.
 * @param buffer_size The buffer size.
 * @return A pointer to buffer if successful and NULL on error.
 *
 * This function decodes a data structure stored in an eet file, returning
 * a pointer to it if it decoded successfully, or NULL on failure. This
 * can save a programmer dozens of hours of work in writing configuration
 * file parsing and writing code, as eet does all that work for the program
 * and presents a program-friendly data structure, just as the programmer
 * likes. Eet can handle members being added or deleted from the data in
 * storage and safely zero-fills unfilled members if they were not found
 * in the data. It checks sizes and headers whenever it reads data, allowing
 * the programmer to not worry about corrupt data.
 *
 * Once a data structure has been described by the programmer with the
 * fields they wish to save or load, storing or retrieving a data structure
 * from an eet file, or from a chunk of memory is as simple as a single
 * function call.
 *
 * @see eet_data_read_cipher()
 *
 * @since 1.10.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI void *
eet_data_read_cipher_buffer(Eet_File            *ef,
                            Eet_Data_Descriptor *edd,
                            const char          *name,
                            const char          *cipher_key,
                            char                *buffer,
                            int                 buffer_size);

/**
 * Read a data structure from an eet extended attribute and decodes it using a cipher.
 * @param filename The file to extract the extended attribute from.
 * @param attribute The attribute to get the data from.
 * @param edd The data descriptor handle to use when decoding.
 * @param cipher_key The key to use as cipher.
 * @return A pointer to the decoded data structure.
 *
 * This function decodes a data structure stored in an eet extended attribute,
 * returning a pointer to it if it decoded successfully, or NULL on failure.
 * Eet can handle members being added or deleted from the data in
 * storage and safely zero-fills unfilled members if they were not found
 * in the data. It checks sizes and headers whenever it reads data, allowing
 * the programmer to not worry about corrupt data.
 *
 * Once a data structure has been described by the programmer with the
 * fields they wish to save or load, storing or retrieving a data structure
 * from an eet file, from a chunk of memory or from an extended attribute
 * is as simple as a single function call.
 *
 * @since 1.5.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI void *
eet_data_xattr_cipher_get(const char *filename,
                          const char *attribute,
                          Eet_Data_Descriptor *edd,
                          const char *cipher_key);

/**
 * Write a data structure from memory and store in an eet file
 * using a cipher.
 * @param ef The eet file handle to write to.
 * @param edd The data descriptor to use when encoding.
 * @param name The key to store the data under in the eet file.
 * @param cipher_key The key to use as cipher.
 * @param data A pointer to the data structure to save and encode.
 * @param compress Compression flags for storage.
 * @return bytes written on successful write, 0 on failure.
 *
 * This function is the reverse of eet_data_read_cipher(), saving a data structure
 * to an eet file.
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI int
eet_data_write_cipher(Eet_File *ef,
                      Eet_Data_Descriptor *edd,
                      const char *name,
                      const char *cipher_key,
                      const void *data,
                      int compress);

/**
 * Write a data structure from memory and store in an eet extended attribute
 * using a cipher.
 * @param filename The file to write the extended attribute to.
 * @param attribute The attribute to store the data to.
 * @param edd The data descriptor to use when encoding.
 * @param cipher_key The key to use as cipher.
 * @param data A pointer to the data structure to save and encode.
 * @param flags The policy to use when setting the data.
 * @return EINA_TRUE on success, EINA_FALSE on failure.
 *
 * This function is the reverse of eet_data_xattr_cipher_get(), saving a data structure
 * to an eet extended attribute.
 *
 * @since 1.5.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI Eina_Bool
eet_data_xattr_cipher_set(const char *filename,
                          const char *attribute,
                          Eet_Data_Descriptor *edd,
                          const char *cipher_key,
                          const void *data,
                          Eina_Xattr_Flags flags);

/**
 * Dump an eet encoded data structure into ascii text using a cipher.
 * @param data_in The pointer to the data to decode into a struct.
 * @param cipher_key The key to use as cipher.
 * @param size_in The size of the data pointed to in bytes.
 * @param dumpfunc The function to call passed a string when new
 *        data is converted to text
 * @param dumpdata The data to pass to the @p dumpfunc callback.
 * @return 1 on success, 0 on failure
 *
 * This function will take a chunk of data encoded by
 * eet_data_descriptor_encode() and convert it into human readable
 * ascii text.  It does this by calling the @p dumpfunc callback
 * for all new text that is generated. This callback should append
 * to any existing text buffer and will be passed the pointer @p
 * dumpdata as a parameter as well as a string with new text to be
 * appended.
 *
 * Example:
 *
 * @code
 * void output(void *data, const char *string)
 * {
 *   printf("%s", string);
 * }
 *
 * void dump(const char *file)
 * {
 *   FILE *f;
 *   int len;
 *   void *data;
 *
 *   f = fopen(file, "r");
 *   fseek(f, 0, SEEK_END);
 *   len = ftell(f);
 *   rewind(f);
 *   data = malloc(len);
 *   fread(data, len, 1, f);
 *   fclose(f);
 *   eet_data_text_dump_cipher(data, cipher_key, len, output, NULL);
 * }
 * @endcode
 *
 * @see eet_data_text_dump()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI int
eet_data_text_dump_cipher(const void *data_in,
                          const char *cipher_key,
                          int size_in,
                          Eet_Dump_Callback dumpfunc,
                          void *dumpdata);

/**
 * Take an ascii encoding from eet_data_text_dump() and re-encode
 * in binary using a cipher.
 * @param text The pointer to the string data to parse and encode.
 * @param cipher_key The key to use as cipher.
 * @param textlen The size of the string in bytes (not including 0
 *        byte terminator).
 * @param size_ret This gets filled in with the encoded data blob
 *        size in bytes.
 * @return The encoded data on success, NULL on failure.
 *
 * This function will parse the string pointed to by @p text and return
 * an encoded data lump the same way eet_data_descriptor_encode() takes an
 * in-memory data struct and encodes into a binary blob. @p text is a normal
 * C string.
 *
 * @see eet_data_text_undump()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI void *
eet_data_text_undump_cipher(const char *text,
                            const char *cipher_key,
                            int textlen,
                            int *size_ret);

/**
 * Dump an eet encoded data structure from an eet file into ascii
 * text using a cipher.
 * @param ef A valid eet file handle.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param dumpfunc The function to call passed a string when new
 *        data is converted to text
 * @param dumpdata The data to pass to the @p dumpfunc callback.
 * @return 1 on success, 0 on failure
 *
 * This function will take an open and valid eet file from
 * eet_open() request the data encoded by
 * eet_data_descriptor_encode() corresponding to the key @p name
 * and convert it into human readable ascii text. It does this by
 * calling the @p dumpfunc callback for all new text that is
 * generated. This callback should append to any existing text
 * buffer and will be passed the pointer @p dumpdata as a parameter
 * as well as a string with new text to be appended.
 *
 * @see eet_data_dump()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI int
eet_data_dump_cipher(Eet_File *ef,
                     const char *name,
                     const char *cipher_key,
                     Eet_Dump_Callback dumpfunc,
                     void *dumpdata);

/**
 * Take an ascii encoding from eet_data_dump() and re-encode in
 * binary using a cipher.
 * @param ef A valid eet file handle.
 * @param name Name of the entry. eg: "/base/file_i_want".
 * @param cipher_key The key to use as cipher.
 * @param text The pointer to the string data to parse and encode.
 * @param textlen The size of the string in bytes (not including 0
 *        byte terminator).
 * @param compress Compression flags (1 == compress, 0 = don't compress).
 * @return 1 on success, 0 on failure
 *
 * This function will parse the string pointed to by @p text,
 * encode it the same way eet_data_descriptor_encode() takes an
 * in-memory data struct and encodes into a binary blob.
 *
 * The data (optionally compressed) will be in ram, pending a flush to
 * disk (it will stay in ram till the eet file handle is closed though).
 *
 * @see eet_data_undump()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI int
eet_data_undump_cipher(Eet_File *ef,
                       const char *name,
                       const char *cipher_key,
                       const char *text,
                       int textlen,
                       int compress);

/**
 * Decode a data structure from an arbitrary location in memory
 * using a cipher.
 * @param edd The data  descriptor to use when decoding.
 * @param data_in The pointer to the data to decode into a struct.
 * @param cipher_key The key to use as cipher.
 * @param size_in The size of the data pointed to in bytes.
 * @return NULL on failure, or a valid decoded struct pointer on success.
 *
 * This function will decode a data structure that has been encoded using
 * eet_data_descriptor_encode(), and return a data structure with all its
 * elements filled out, if successful, or NULL on failure.
 *
 * The data to be decoded is stored at the memory pointed to by @p data_in,
 * and is described by the descriptor pointed to by @p edd. The data size is
 * passed in as the value to @p size_in, ande must be greater than 0 to
 * succeed.
 *
 * This function is useful for decoding data structures delivered to the
 * application by means other than an eet file, such as an IPC or socket
 * connection, raw files, shared memory etc.
 *
 * Please see eet_data_read() for more information.
 *
 * @see eet_data_descriptor_decode()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI void *
eet_data_descriptor_decode_cipher(Eet_Data_Descriptor *edd,
                                  const void *data_in,
                                  const char *cipher_key,
                                  int size_in);

/**
 * Encode a data struct to memory and return that encoded data
 * using a cipher.
 * @param edd The data  descriptor to use when encoding.
 * @param data_in The pointer to the struct to encode into data.
 * @param cipher_key The key to use as cipher.
 * @param size_ret pointer to the an int to be filled with the decoded size.
 * @return NULL on failure, or a valid encoded data chunk on success.
 *
 * This function takes a data structutre in memory and encodes it into a
 * serialised chunk of data that can be decoded again by
 * eet_data_descriptor_decode(). This is useful for being able to transmit
 * data structures across sockets, pipes, IPC or shared file mechanisms,
 * without having to worry about memory space, machine type, endianess etc.
 *
 * The parameter @p edd must point to a valid data descriptor, and
 * @p data_in must point to the right data structure to encode. If not, the
 * encoding may fail.
 *
 * On success a non NULL valid pointer is returned and what @p size_ret
 * points to is set to the size of this decoded data, in bytes. When the
 * encoded data is no longer needed, call free() on it. On failure NULL is
 * returned and what @p size_ret points to is set to 0.
 *
 * Please see eet_data_write() for more information.
 *
 * @see eet_data_descriptor_encode()
 *
 * @since 1.0.0
 * @ingroup Eet_Data_Cipher_Group
 */
EAPI void *
eet_data_descriptor_encode_cipher(Eet_Data_Descriptor *edd,
                                  const void *data_in,
                                  const char *cipher_key,
                                  int *size_ret);

/**
 * Create a new character node.
 * @param name Name of the node.
 * @param c Character value.
 * @return A new character node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_char_new(const char *name,
                  char c);

/**
 * Create a new short node.
 * @param name Name of the node.
 * @param s short value.
 * @return A new short node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_short_new(const char *name,
                   short s);

/**
 * Create a new integer node.
 * @param name Name of the node.
 * @param i integer value.
 * @return A new integer node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_int_new(const char *name,
                 int i);

/**
 * Create a new long long integer node.
 * @param name Name of the node.
 * @param l long long integer value.
 * @return A new long long integer node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_long_long_new(const char *name,
                       long long l);

/**
 * Create a new float node.
 * @param name Name of the node.
 * @param f float value.
 * @return A new float node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_float_new(const char *name,
                   float f);

/**
 * Create a new double node.
 * @param name Name of the node.
 * @param d double value.
 * @return A new double node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_double_new(const char *name,
                    double d);

/**
 * Create a new unsigned character node.
 * @param name Name of the node.
 * @param uc unsigned char value.
 * @return A new unsigned char node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_unsigned_char_new(const char *name,
                           unsigned char uc);

/**
 * Create a new unsigned short node.
 * @param name Name of the node.
 * @param us unsigned short value.
 * @return A new unsigned short node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_unsigned_short_new(const char *name,
                            unsigned short us);

/**
 * Create a new unsigned integer node.
 * @param name Name of the node.
 * @param ui unsigned integer value.
 * @return A new unsigned integer node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_unsigned_int_new(const char *name,
                          unsigned int ui);

/**
 * Create a new unsigned long long integer node.
 * @param name Name of the node.
 * @param l unsigned long long integer value.
 * @return A new unsigned long long integer node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_unsigned_long_long_new(const char *name,
                                unsigned long long l);

/**
 * Create a new string node.
 * @param name Name of the node.
 * @param str string value.
 * @return A new string node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_string_new(const char *name,
                    const char *str);

/**
 * Create a new inlined string node.
 * @param name Name of the node.
 * @param str string value.
 * @return A new inlined string node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_inlined_string_new(const char *name,
                            const char *str);

/**
 * Create a new empty node.
 * @param name Name of the node.
 * @return A new empty node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_null_new(const char *name);

/**
 * Create a new list node.
 * @param name Name of the node.
 * @param nodes list of nodes.
 * @return A new list node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_list_new(const char *name,
                  Eina_List *nodes);

/**
 * Create a new array node.
 * @param name Name of the node.
 * @param count number of nodes
 * @param nodes list of nodes.
 * @return A new array node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_array_new(const char *name,
                   int count,
                   Eina_List *nodes);

/**
 * Create a new variable array node.
 * @param name Name of the node.
 * @param nodes list of nodes.
 * @return A new variable array node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_var_array_new(const char *name,
                       Eina_List *nodes);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
/**
 * Create a new short node.
 * @param name Name of the node.
 * @param s short value.
 * @return A new short node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_hash_new(const char *name,
                  const char *key,
                  Eet_Node *node);

/**
 * Create a new struct node.
 * @param name Name of the node.
 * @param nodes list of nodes.
 * @return A new struct node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_struct_new(const char *name,
                    Eina_List *nodes);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
/**
 * Create a new short node.
 * @param name Name of the node.
 * @param s short value.
 * @return A new short node.
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_node_struct_child_new(const char *parent,
                          Eet_Node *child);

/**
 * @brief Get a node's child nodes
 * @param node The node
 * @return The first child node which contains a pointer to the
 * next child node and the parent.
 * @since 1.5
 */
EAPI Eet_Node *
eet_node_children_get(Eet_Node *node);

/**
 * @brief Get the next node in a list of nodes
 * @param node The node
 * @return A node which contains a pointer to the
 * next child node and the parent.
 * @since 1.5
 */
EAPI Eet_Node *
eet_node_next_get(Eet_Node *node);

/**
 * @brief Get the parent node of a node
 * @param node The node
 * @return The parent node of @p node
 * @since 1.5
 */
EAPI Eet_Node *
eet_node_parent_get(Eet_Node *node);

/**
 * @brief Append a "list" node TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI void
eet_node_list_append(Eet_Node *parent,
                     const char *name,
                     Eet_Node *child);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI void
eet_node_struct_append(Eet_Node *parent,
                       const char *name,
                       Eet_Node *child);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI void
eet_node_hash_add(Eet_Node *parent,
                  const char *name,
                  const char *key,
                  Eet_Node *child);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI void
eet_node_dump(Eet_Node *n,
              int dumplevel,
              Eet_Dump_Callback dumpfunc,
              void *dumpdata);

/**
 * @brief Return the type of a node
 * @param node The node
 * @return The node's type (EET_T_$TYPE)
 * @since 1.5
 */
EAPI int
eet_node_type_get(Eet_Node *node);

/**
 * @brief Return the node's data
 * @param node The node
 * @return The data contained in the node
 * @since 1.5
 */
EAPI Eet_Node_Data *
eet_node_value_get(Eet_Node *node);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI void
eet_node_del(Eet_Node *n);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI void *
eet_data_node_encode_cipher(Eet_Node *node,
                            const char *cipher_key,
                            int *size_ret);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_data_node_decode_cipher(const void *data_in,
                            const char *cipher_key,
                            int size_in);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI Eet_Node *
eet_data_node_read_cipher(Eet_File *ef,
                          const char *name,
                          const char *cipher_key);

/**
 * TODO FIX ME
 * @ingroup Eet_Node_Group
 */
EAPI int
eet_data_node_write_cipher(Eet_File *ef,
                           const char *name,
                           const char *cipher_key,
                           Eet_Node *node,
                           int compress);

#endif /* EET_HACK_H_ */
