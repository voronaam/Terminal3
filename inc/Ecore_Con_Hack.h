/*
 * Ecore_Con_Hack.h
 *
 *  Created on: May 6, 2016
 *      Author: avorona
 */

#ifndef ECORE_CON_HACK_H_
#define ECORE_CON_HACK_H_

#include <Ecore_Con.h>

/**
 * Initialises the Ecore_Con library.
 * @return  Number of times the library has been initialised without being
 *          shut down.
 *
 * @note This function already calls ecore_init() internally, so you don't need
 * to call it explicitly.
 */
EAPI int               ecore_con_init(void);

/**
 * Shuts down the Ecore_Con library.
 * @return  Number of times the library has been initialised without being
 *          shut down.
 * @note This function already calls ecore_shutdown() internally, so you don't
 * need to call it explicitly unless you called ecore_init() explicitly too.
 */
EAPI int               ecore_con_shutdown(void);



/*
 * Change the HTTP version used for the request
 * @param url_con Connection object through which the request will be sent.
 * @param version The version to be used
 * @return @c EINA_TRUE on success, @c EINA_FALSE on failure to change version.
 * @since 1.2
 * @see ecore_con_url_pipeline_get()
 */
EAPI Eina_Bool         ecore_con_url_http_version_set(Ecore_Con_Url *url_con, Ecore_Con_Url_Http_Version version);

/**
 * Initialises the Ecore_Con_Url library.
 * @return Number of times the library has been initialised without being
 *          shut down.
 *
 * @note This function doesn't call ecore_con_init(). You still need to call it
 * explicitly before calling this one.
 */
EAPI int               ecore_con_url_init(void);

/**
 * Shuts down the Ecore_Con_Url library.
 * @return  Number of calls that still uses Ecore_Con_Url
 *
 * @note This function doesn't call ecore_con_shutdown(). You still need to call
 * it explicitly after calling this one.
 */
EAPI int               ecore_con_url_shutdown(void);

/**
 * Enable or disable HTTP 1.1 pipelining.
 * @param enable @c EINA_TRUE will turn it on, @c EINA_FALSE will disable it.
 *
 * Pipelining allows to send one request after another one, without having to
 * wait for the reply of the first request. The respective replies are received
 * in the order that the requests were sent.
 *
 * Enabling this feature will be valid for all requests done using @c
 * ecore_con_url.
 *
 * See http://en.wikipedia.org/wiki/HTTP_pipelining for more info.
 *
 * @see ecore_con_url_pipeline_get()
 */
EAPI void              ecore_con_url_pipeline_set(Eina_Bool enable);
/**
 * Is HTTP 1.1 pipelining enable ?
 * @return @c EINA_TRUE if it is enable.
 *
 * @see ecore_con_url_pipeline_set()
 */
EAPI Eina_Bool         ecore_con_url_pipeline_get(void);

/**
 * Creates and initializes a new Ecore_Con_Url connection object.
 *
 * @param url URL that will receive requests. Can be changed using
 *            ecore_con_url_url_set.
 *
 * @return @c NULL on error, a new Ecore_Con_Url on success.
 *
 * Creates and initializes a new Ecore_Con_Url connection object that can be
 * used for sending requests.
 *
 * @see ecore_con_url_custom_new()
 * @see ecore_con_url_url_set()
 */
EAPI Ecore_Con_Url *   ecore_con_url_new(const char *url);
/**
 * Creates a custom connection object.
 *
 * @param url URL that will receive requests
 * @param custom_request Custom request (e.g. GET, POST, HEAD, PUT, etc)
 *
 * @return @c NULL on error, a new Ecore_Con_Url on success.
 *
 * Creates and initializes a new Ecore_Con_Url for a custom request (e.g. HEAD,
 * SUBSCRIBE and other obscure HTTP requests). This object should be used like
 * one created with ecore_con_url_new().
 *
 * @see ecore_con_url_new()
 * @see ecore_con_url_url_set()
 */
EAPI Ecore_Con_Url *   ecore_con_url_custom_new(const char *url,
                                                const char *custom_request);
/**
 * Destroys a Ecore_Con_Url connection object.
 *
 * @param url_con Connection object to free.
 *
 * @see ecore_con_url_new()
 */
EAPI void              ecore_con_url_free(Ecore_Con_Url *url_con);

/**
 * Associates data with a connection object.
 *
 * @param url_con Connection object to associate data.
 * @param data Data to be set.
 *
 * Associates data with a connection object, which can be retrieved later with
 * ecore_con_url_data_get()).
 *
 * @see ecore_con_url_data_get()
 */
EAPI void              ecore_con_url_data_set(Ecore_Con_Url *url_con,
                                              void *data);
/**
 * Retrieves data associated with a Ecore_Con_Url connection object.
 *
 * @param url_con Connection object to retrieve data from.
 *
 * @return Data associated with the given object.
 *
 * Retrieves data associated with a Ecore_Con_Url connection object (previously
 * set with ecore_con_url_data_set()).
 *
 * @see ecore_con_url_data_set()
 */
EAPI void *            ecore_con_url_data_get(Ecore_Con_Url *url_con);
/**
 * Adds an additional header to the request connection object.
 *
 * @param url_con Connection object
 * @param key Header key
 * @param value Header value
 *
 * Adds an additional header (User-Agent, Content-Type, etc.) to the request
 * connection object. This addition will be valid for only one
 * ecore_con_url_get() or ecore_con_url_post() call.
 *
 * Some functions like ecore_con_url_time() also add headers to the request.
 *
 * @see ecore_con_url_get()
 * @see ecore_con_url_post()
 * @see ecore_con_url_additional_headers_clear()
 */
EAPI void              ecore_con_url_additional_header_add(Ecore_Con_Url *url_con,
                                                           const char *key,
                                                           const char *value);
/**
 * Cleans additional headers.
 *
 * @param url_con Connection object to clean additional headers.
 *
 * Cleans additional headers associated with a connection object (previously
 * added with ecore_con_url_additional_header_add()).
 *
 * @see ecore_con_url_additional_header_add()
 * @see ecore_con_url_get()
 * @see ecore_con_url_post()
 */
EAPI void              ecore_con_url_additional_headers_clear(Ecore_Con_Url *url_con);
/**
 * Retrieves headers from last request sent.
 *
 * @param url_con Connection object to retrieve response headers from.
 *
 * Retrieves a list containing the response headers. This function should be
 * used after an ECORE_CON_EVENT_URL_COMPLETE event (headers should normally be
 * ready at that time).
 *
 * @return List of response headers. This list must not be modified by the user.
 */
EAPI const Eina_List * ecore_con_url_response_headers_get(Ecore_Con_Url *url_con);
/**
 * Setup a file for receiving response data.
 *
 * @param url_con Connection object to set file
 * @param fd File descriptor associated with the file. A negative value will
 * unset any previously set fd.
 *
 * Sets up a file to have response data written into. Note that
 * ECORE_CON_EVENT_URL_DATA events will not be emitted if a file has been set to
 * receive the response data.
 *
 * This call can be used to easily setup a file where the downloaded data will
 * be saved.
 */
EAPI void              ecore_con_url_fd_set(Ecore_Con_Url *url_con, int fd);
/**
 * Retrieves the number of bytes received.
 *
 * Retrieves the number of bytes received on the last request of the given
 * connection object.
 *
 * @param url_con Connection object which the request was sent on.
 *
 * @return Number of bytes received on request.
 *
 * @see ecore_con_url_get()
 * @see ecore_con_url_post()
 */
EAPI int               ecore_con_url_received_bytes_get(Ecore_Con_Url *url_con);
/**
 * Sets url_con to use http auth, with given username and password, "safely" or not.
 *
 * @param url_con Connection object to perform a request on, previously created
 *    with ecore_con_url_new() or ecore_con_url_custom_new().
 * @param username Username to use in authentication
 * @param password Password to use in authentication
 * @param safe Whether to use "safer" methods (eg, NOT http basic auth)
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE on error.
 *
 * @attention Requires libcurl >= 7.19.1 to work, otherwise will always return
 * @c 0.
 */
EAPI Eina_Bool         ecore_con_url_httpauth_set(Ecore_Con_Url *url_con,
                                                  const char *username,
                                                  const char *password,
                                                  Eina_Bool safe);
/**
 * Sends a get request.
 *
 * @param url_con Connection object to perform a request on, previously created
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE on error.
 *
 * The request is performed immediately, but you need to setup event handlers
 * for #ECORE_CON_EVENT_URL_DATA, #ECORE_CON_EVENT_URL_COMPLETE or
 * #ECORE_CON_EVENT_URL_PROGRESS to get more information about its result.
 *
 * @see ecore_con_url_custom_new()
 * @see ecore_con_url_additional_headers_clear()
 * @see ecore_con_url_additional_header_add()
 * @see ecore_con_url_data_set()
 * @see ecore_con_url_data_get()
 * @see ecore_con_url_response_headers_get()
 * @see ecore_con_url_time()
 * @see ecore_con_url_post()
 */
EAPI Eina_Bool         ecore_con_url_get(Ecore_Con_Url *url_con);
/**
 * Sends a post request.
 *
 * @param url_con Connection object to perform a request on, previously created
 *                with ecore_con_url_new() or ecore_con_url_custom_new().
 * @param data    Payload (data sent on the request). Can be @c NULL.
 * @param length  Payload length. If @c -1, rely on automatic length
 *                calculation via @c strlen() on @p data.
 * @param content_type Content type of the payload (e.g. text/xml). Can be @c
 * NULL.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE on error.
 *
 * The request starts immediately, but you need to setup event handlers
 * for #ECORE_CON_EVENT_URL_DATA, #ECORE_CON_EVENT_URL_COMPLETE or
 * #ECORE_CON_EVENT_URL_PROGRESS to get more information about its result.
 *
 * This call won't block your main loop.
 *
 * @see ecore_con_url_custom_new()
 * @see ecore_con_url_additional_headers_clear()
 * @see ecore_con_url_additional_header_add()
 * @see ecore_con_url_data_set()
 * @see ecore_con_url_data_get()
 * @see ecore_con_url_response_headers_get()
 * @see ecore_con_url_time()
 * @see ecore_con_url_get()
 */
EAPI Eina_Bool         ecore_con_url_post(Ecore_Con_Url *url_con,
                                          const void *data, long length,
                                          const char *content_type);
/**
 * Sets whether HTTP requests should be conditional, dependent on
 * modification time.
 *
 * @param url_con   Ecore_Con_Url to act upon.
 * @param time_condition Condition to use for HTTP requests.
 * @param timestamp Time since 1 Jan 1970 to use in the condition.
 *
 * This function may set the header "If-Modified-Since" or
 * "If-Unmodified-Since", depending on the value of @p time_condition, with the
 * value @p timestamp.
 *
 * @sa ecore_con_url_get()
 * @sa ecore_con_url_post()
 */
EAPI void              ecore_con_url_time(Ecore_Con_Url *url_con,
                                          Ecore_Con_Url_Time time_condition,
                                          double timestamp);

/**
 * @brief Uploads a file to an ftp site.
 *
 * @param url_con The Ecore_Con_Url object to send with
 * @param filename The path to the file to send
 * @param user The username to log in with
 * @param pass The password to log in with
 * @param upload_dir The directory to which the file should be uploaded
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 *
 * Upload @p filename to an ftp server set in @p url_con using @p user
 * and @p pass to directory @p upload_dir
 */
EAPI Eina_Bool         ecore_con_url_ftp_upload(Ecore_Con_Url *url_con,
                                                const char *filename,
                                                const char *user,
                                                const char *pass,
                                                const char *upload_dir);
/**
 * Toggle libcurl's verbose output.
 *
 * @param url_con Ecore_Con_Url instance which will be acted upon.
 * @param verbose Whether or not to enable libcurl's verbose output.
 *
 * If @p verbose is @c EINA_TRUE, libcurl will output a lot of verbose
 * information about its operations, which is useful for
 * debugging. The verbose information will be sent to stderr.
 */
EAPI void              ecore_con_url_verbose_set(Ecore_Con_Url *url_con,
                                                 Eina_Bool verbose);
/**
 * Enable or disable EPSV extension
 * @param url_con  The Ecore_Con_Url instance which will be acted upon.
 * @param use_epsv Boolean to enable/disable the EPSV extension.
 */
EAPI void              ecore_con_url_ftp_use_epsv_set(Ecore_Con_Url *url_con,
                                                      Eina_Bool use_epsv);

/**
 * Enables the cookie engine for subsequent HTTP requests.
 *
 * @param url_con Ecore_Con_Url instance which will be acted upon.
 *
 * After this function is called, cookies set by the server in HTTP responses
 * will be parsed and stored, as well as sent back to the server in new HTTP
 * requests.
 *
 * @note Even though this function is called @c ecore_con_url_cookies_init(),
 * there is no symmetrical shutdown operation.
 */
EAPI void              ecore_con_url_cookies_init(Ecore_Con_Url *url_con);
/**
 * Controls whether session cookies from previous sessions shall be loaded.
 *
 * @param url_con Ecore_Con_Url instance which will be acted upon.
 * @param ignore  If @c EINA_TRUE, ignore session cookies when loading cookies
 *                from files. If @c EINA_FALSE, all cookies will be loaded.
 *
 * Session cookies are cookies with no expire date set, which usually means
 * they are removed after the current session is closed.
 *
 * By default, when Ecore_Con_Url loads cookies from a file, all cookies are
 * loaded, including session cookies, which, most of the time, were supposed
 * to be loaded and valid only for that session.
 *
 * If @p ignore is set to @c EINA_TRUE, when Ecore_Con_Url loads cookies from
 * the files passed to @c ecore_con_url_cookies_file_add(), session cookies
 * will not be loaded.
 *
 * @see ecore_con_url_cookies_file_add()
 */
EAPI void              ecore_con_url_cookies_ignore_old_session_set(Ecore_Con_Url *url_con,
                                                                    Eina_Bool ignore);
/**
 * Clears currently loaded cookies.
 * @param url_con      Ecore_Con_Url instance which will be acted upon.
 *
 * The cleared cookies are removed and will not be sent in subsequent HTTP
 * requests, nor will they be written to the cookiejar file set via
 * @c ecore_con_url_cookies_jar_file_set().
 *
 * @note This function will initialize the cookie engine if it has not been
 *       initialized yet.
 * @note The cookie files set by ecore_con_url_cookies_file_add() aren't loaded
 *       immediately, just when the request is started. Thus, if you ask to
 *       clear the cookies, but has a file already set by that function, the
 *       cookies will then be loaded and you will have old cookies set. In order
 *       to don't have any old cookie set, you need to don't call
 *       ecore_con_url_cookies_file_add() ever on the @p url_con handler, and
 *       call this function to clear any cookie set by a previous request on
 *       this handler.
 *
 * @see ecore_con_url_cookies_session_clear()
 * @see ecore_con_url_cookies_ignore_old_session_set()
 */
EAPI void              ecore_con_url_cookies_clear(Ecore_Con_Url *url_con);
/**
 * Clears currently loaded session cookies.
 *
 * @param url_con      Ecore_Con_Url instance which will be acted upon.
 *
 * Session cookies are cookies with no expire date set, which usually means
 * they are removed after the current session is closed.
 *
 * The cleared cookies are removed and will not be sent in subsequent HTTP
 * requests, nor will they be written to the cookiejar file set via
 * @c ecore_con_url_cookies_jar_file_set().
 *
 * @note This function will initialize the cookie engine if it has not been
 *       initialized yet.
 * @note The cookie files set by ecore_con_url_cookies_file_add() aren't loaded
 *       immediately, just when the request is started. Thus, if you ask to
 *       clear the session cookies, but has a file already set by that function,
 *       the session cookies will then be loaded and you will have old cookies
 *       set.  In order to don't have any old session cookie set, you need to
 *       don't call ecore_con_url_cookies_file_add() ever on the @p url_con
 *       handler, and call this function to clear any session cookie set by a
 *       previous request on this handler. An easier way to don't use old
 *       session cookies is by using the function
 *       ecore_con_url_cookies_ignore_old_session_set().
 *
 * @see ecore_con_url_cookies_clear()
 * @see ecore_con_url_cookies_ignore_old_session_set()
 */
EAPI void              ecore_con_url_cookies_session_clear(Ecore_Con_Url *url_con);
/**
 * Adds a file to the list of files from which to load cookies.
 *
 * @param url_con   Ecore_Con_Url instance which will be acted upon.
 * @param file_name Name of the file that will be added to the list.
 *
 * Files must contain cookies defined according to two possible formats:
 *
 * @li HTTP-style header ("Set-Cookie: ...").
 * @li <a href="http://www.cookiecentral.com/faq/#3.5">Netscape/Mozilla cookie data format.</a>
 *
 * Cookies will only be @b read from this file. If you want to save cookies to a
 * file, use ecore_con_url_cookies_jar_file_set(). Also notice that this
 * function supports the both types of cookie file cited above, while
 * ecore_con_url_cookies_jar_file_set() will save only in the Netscape/Mozilla's
 * format.
 *
 * Please notice that the file will not be read immediately, but rather added
 * to a list of files that will be loaded and parsed at a later time.
 *
 * @note This function will initialize the cookie engine if it has not been
 *       initialized yet.
 *
 * @see ecore_con_url_cookies_ignore_old_session_set()
 * @see ecore_con_url_cookies_jar_file_set()
 */
EAPI void              ecore_con_url_cookies_file_add(Ecore_Con_Url *url_con,
                                                      const char * const file_name);
/**
 * Sets the name of the file to which all current cookies will be written when
 * either cookies are flushed or Ecore_Con is shut down.
 *
 * @param url_con        Ecore_Con_Url instance which will be acted upon.
 * @param cookiejar_file File to which the cookies will be written.
 *
 * @return @c EINA_TRUE is the file name has been set successfully,
 *         @c EINA_FALSE otherwise.
 *
 * Cookies are written following Netscape/Mozilla's data format, also known as
 * cookie-jar.
 *
 * Cookies will only be @b saved to this file. If you need to read cookies from
 * a file, use ecore_con_url_cookies_file_add() instead.
 *
 * @note This function will initialize the cookie engine if it has not been
 *       initialized yet.
 *
 * @see ecore_con_url_cookies_jar_write()
 */
EAPI Eina_Bool         ecore_con_url_cookies_jar_file_set(Ecore_Con_Url *url_con,
                                                          const char * const cookiejar_file);
/**
 * Writes all current cookies to the cookie jar immediately.
 *
 * @param url_con Ecore_Con_Url instance which will be acted upon.
 *
 * A cookie-jar file must have been previously set by
 * @c ecore_con_url_jar_file_set, otherwise nothing will be done.
 *
 * @note This function will initialize the cookie engine if it has not been
 *       initialized yet.
 *
 * @see ecore_con_url_cookies_jar_file_set()
 */
EAPI void              ecore_con_url_cookies_jar_write(Ecore_Con_Url *url_con);

EAPI void              ecore_con_url_ssl_verify_peer_set(Ecore_Con_Url *url_con,
                                                         Eina_Bool verify);
EAPI int               ecore_con_url_ssl_ca_set(Ecore_Con_Url *url_con,
                                                const char *ca_path);

/**
 * Set HTTP proxy to use.
 *
 * The parameter should be a char * to a zero terminated string holding
 * the host name or dotted IP address. To specify port number in this string,
 * append :[port] to the end of the host name.
 * The proxy string may be prefixed with [protocol]:// since any such prefix
 * will be ignored.
 * The proxy's port number may optionally be specified with the separate option.
 * If not specified, libcurl will default to using port 1080 for proxies.
 *
 * @param url_con Connection object that will use the proxy.
 * @param proxy Porxy string or @c NULL to disable
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE on error.
 * @since 1.2
 */
EAPI Eina_Bool ecore_con_url_proxy_set(Ecore_Con_Url *url_con, const char *proxy);

/**
 * Set zero terminated username to use for proxy.
 *
 * if socks protocol is used for proxy, protocol should be socks5 and above.
 *
 * @param url_con Connection object that will use the proxy.
 * @param username Username string.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE on error.
 *
 * @see ecore_con_url_proxy_set()
 *
 * @since 1.2
 */
EAPI Eina_Bool ecore_con_url_proxy_username_set(Ecore_Con_Url *url_con, const char *username);

/**
 * Set zero terminated password to use for proxy.
 *
 * if socks protocol is used for proxy, protocol should be socks5 and above.
 *
 * @param url_con Connection object that will use the proxy.
 * @param password Password string.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE on error.
 *
 * @see ecore_con_url_proxy_set()
 *
 * @since 1.2
 */
EAPI Eina_Bool ecore_con_url_proxy_password_set(Ecore_Con_Url *url_con, const char *password);

/**
 * Set timeout in seconds.
 *
 * the maximum time in seconds that you allow the ecore con url transfer
 * operation to take. Normally, name lookups can take a considerable time
 * and limiting operations to less than a few minutes risk aborting perfectly
 * normal operations.
 *
 * @param url_con Connection object that will use the timeout.
 * @param timeout time in seconds.
 *
 * @see ecore_con_url_cookies_jar_file_set()
 *
 * @since 1.2
 */
EAPI void ecore_con_url_timeout_set(Ecore_Con_Url *url_con, double timeout);

/**
 * Get the returned HTTP STATUS code
 *
 * This is used to, at any time, try to return the status code for a transmission.
 * @param url_con Connection object
 * @return A valid HTTP STATUS code, or 0 on failure
 *
 * @since 1.2
 */
EAPI int ecore_con_url_status_code_get(Ecore_Con_Url *url_con);



#endif /* ECORE_CON_HACK_H_ */
