// -*- C++ -*- Time-stamp: <2013-05-04 10:27:31 ptr>

/*
 * Copyright (c) 2004-2009
 * Petr Ovtchenkov
 *
 * Copyright (c) 2004-2008
 * Francois Dumont
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

#ifndef __TEST_BIND_TEST_H
#define __TEST_BIND_TEST_H

#define __FIT_EXAM

#include <exam/suite.h>

class bind_test
{
  public:
    int EXAM_DECL(bind1st1);
    int EXAM_DECL(bind2nd1);
    int EXAM_DECL(bind2nd2);
    int EXAM_DECL(bind2nd3);
    int EXAM_DECL(bind_memfn);
    int EXAM_DECL(bind_core);
    int EXAM_DECL(bind_ret_convert);
};

#endif // __TEST_BIND_TEST_H
