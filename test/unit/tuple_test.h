// -*- C++ -*- Time-stamp: <2013-02-12 10:01:35 ptr>

/*
 * Copyright (c) 2013
 * Petr Ovtchenkov
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

#ifndef __TEST_TUPLE_TEST_H
#define __TEST_TUPLE_TEST_H

#define __FIT_EXAM

#include <exam/suite.h>

class tuple_test
{
  public:
    int EXAM_DECL(internals);
    int EXAM_DECL(tuple_cv);
    int EXAM_DECL(tuple_ctor);
};

#endif // __TEST_TUPLE_TEST_H
