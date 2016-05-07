/*
 * Ethumb_Client_Hack.h
 *
 *  Created on: May 6, 2016
 *      Author: avorona
 */

#ifndef ETHUMB_CLIENT_HACK_H_
#define ETHUMB_CLIENT_HACK_H_

#include <Ethumb_Client.h>

EAPI Ethumb_Client_Async *ethumb_client_thumb_async_get(Ethumb_Client *client,
                                                        Ethumb_Client_Async_Done_Cb done,
                                                        Ethumb_Client_Async_Error_Cb error,
                                                        const void *data);
EAPI void ethumb_client_thumb_async_cancel(Ethumb_Client *client, Ethumb_Client_Async *request);


#endif /* ETHUMB_CLIENT_HACK_H_ */
