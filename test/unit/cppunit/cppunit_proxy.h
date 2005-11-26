/*
 * Copyright (c) 2003, 2004
 * Zdenek Nemec
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/* $Id$ */

#ifndef _CPPUNITPROXYINTERFACE_H_
# define _CPPUNITPROXYINTERFACE_H_

 /*
  * Exception-usage settings
  */
#ifdef CPPUNIT_MINI_USE_EXCEPTIONS
#  undef CPPUNIT_MINI_USE_EXCEPTIONS
#endif

 /*
  * STLport specific
  */
#if defined _STLP_USE_EXCEPTIONS
#  define CPPUNIT_MINI_USE_EXCEPTIONS
#endif

#include "cppunit_mini.h"

#endif
