/*
 * Ecore_Evas_Hack.h
 *
 *  Created on: May 6, 2016
 *      Author: avorona
 */

#ifndef ECORE_EVAS_HACK_H_
#define ECORE_EVAS_HACK_H_

#include <Ecore_Evas.h>
EAPI Ecore_Window ecore_evas_window_get(const Ecore_Evas *ee);
EAPI Ecore_Evas *ecore_evas_ecore_evas_get(const Evas *e);



#endif /* ECORE_EVAS_HACK_H_ */
