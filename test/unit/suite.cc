// -*- C++ -*- Time-stamp: <08/12/12 22:31:15 ptr>

/*
 * Copyright (c) 2008
 * Petr Ovtchenkov
 *
 * Licensed under the Academic Free License Version 3.0
 *
 */

#include <exam/suite.h>

#include <misc/opts.h>
#include <string>
#include <sstream>

#include "vector_test.h"
#include "deque_test.h"
#include "list_test.h"
#include "slist_test.h"
#include "string_test.h"
#include "map_test.h"
#include "set_operations_test.h"
#include "numerics.h"
#include "stream_test.h"
#include "alg_test.h"
#include "functor_test.h"
#include "search_test.h"
#include "sort_test.h"
#include "adj_test.h"
#include "copy_test.h"

// ------------
#include "cppunit_proxy.h"
#include "file_reporter.h"
#include "cppunit_timer.h"

#include "stdio.h"

  int TestCase::m_numErrors = 0;
  int TestCase::m_numTests = 0;

  TestCase *TestCase::m_root = 0;
  Reporter *TestCase::m_reporter = 0;

  void TestCase::registerTestCase(TestCase *in_testCase) {
    in_testCase->m_next = m_root;
    m_root = in_testCase;
  }

  int TestCase::run(Reporter *in_reporter, const char *in_testName, bool invert) {
    TestCase::m_reporter = in_reporter;

    m_numErrors = 0;
    m_numTests = 0;

    TestCase *tmp = m_root;
    while (tmp != 0) {
      tmp->myRun(in_testName, invert);
      tmp = tmp->m_next;
    }
    return m_numErrors;
  }

int test_main() {
  CPPUNIT_NS::Reporter* reporter = new FileReporter( stdout, false );

  int num_errors = CPPUNIT_NS::TestCase::run(reporter, "" );

  reporter->printSummary();
  delete reporter;

  return num_errors;
}

// ------------

int main( int argc, const char** argv )
{
  Opts opts;

  opts.description( "test suite for 'sockios' framework" );
  opts.usage( "[options]" );

  opts << option<bool>( "print this help message", 'h', "help" )
       << option<bool>( "list all test cases", 'l', "list" )
       << option<std::string>( "run tests by number", 'r', "run" )["0"]
       << option<bool>( "print status of tests within test suite", 'v', "verbose" )
       << option<bool>(  "trace checks", 't', "trace" );

  try {
    opts.parse( argc, argv );
  }
  catch (...) {
    opts.help( std::cerr );
    return 1;
  }

  if ( opts.is_set( 'h' ) ) {
    opts.help( std::cerr );
    return 0;
  }

  exam::test_suite t( "STLport test" );
  vector_test vec_test;

  exam::test_suite::test_case_type vec_tc[16];

  t.add( &vector_test::vec_test_1, vec_test, "vec_test_1" );
  t.add( &vector_test::vec_test_2, vec_test, "vec_test_2" );
  t.add( &vector_test::vec_test_3, vec_test, "vec_test_3" );
  t.add( &vector_test::vec_test_4, vec_test, "vec_test_4" );
  t.add( &vector_test::vec_test_5, vec_test, "vec_test_5" );
  t.add( &vector_test::vec_test_6, vec_test, "vec_test_6" );
  t.add( &vector_test::vec_test_7, vec_test, "vec_test_7" );
  t.add( &vector_test::capacity, vec_test, "vector capacity" );
  t.add( &vector_test::at, vec_test, "vector at" );
  t.add( &vector_test::pointer, vec_test, "vector pointer" );
  t.add( &vector_test::auto_ref, vec_test, "vector auto_ref" );
  t.add( &vector_test::allocator_with_state, vec_test, "vector allocator_with_state" );
  vec_tc[12] = t.add( &vector_test::iterators, vec_test, "vector iterators" );
  t.add( &vector_test::optimizations_check, vec_test, "vector optimizations_check" );
  t.add( &vector_test::assign_check, vec_test, "vector assign_check" );
  t.add( &vector_test::ebo, vec_test, "vector ebo" );

  deque_test deq_test;

  t.add( &deque_test::deque1, deq_test, "deque1" );
  t.add( &deque_test::at, deq_test, "deque at" );
  t.add( &deque_test::insert, deq_test, "deque insert" );
  t.add( &deque_test::erase, deq_test, "deque erase" );
  t.add( &deque_test::auto_ref, deq_test, "deque auto_ref" );
  t.add( &deque_test::allocator_with_state, deq_test, "deque allocator_with_state" );
  t.add( &deque_test::optimizations_check, deq_test, "deque optimizations_check" );

  queue_test que_test;

  t.add( &queue_test::pqueue1, que_test, "queue test 1" );
  t.add( &queue_test::queue1, que_test, "queue test 2" );

  stack_test st_test;

  t.add( &stack_test::stack1, st_test, "stack test 1" );
  t.add( &stack_test::stack2, st_test, "stack test 2" );

  list_test lst_test;

  t.add( &list_test::list1, lst_test, "list1" );
  t.add( &list_test::list2, lst_test, "list2" );
  t.add( &list_test::list3, lst_test, "list3" );
  t.add( &list_test::list4, lst_test, "list4" );
  t.add( &list_test::erase, lst_test, "list erase" );
  t.add( &list_test::resize, lst_test, "list resize" );
  t.add( &list_test::push_back, lst_test, "list push_back" );
  t.add( &list_test::push_front, lst_test, "list push_front" );
  t.add( &list_test::allocator_with_state, lst_test, "list allocator_with_state" );
  t.add( &list_test::swap, lst_test, "list swap" );
  t.add( &list_test::adl, lst_test, "list adl" );
  // t.add( &list_test::const_list, lst_test, "const_list" );

  slist_test slst_test;

  t.add( &slist_test::slist1, slst_test, "slist1" );
  t.add( &slist_test::erase, slst_test, "slist erase" );
  t.add( &slist_test::insert, slst_test, "slist insert" );
  t.add( &slist_test::splice, slst_test, "slist splice" );
  t.add( &slist_test::allocator_with_state, slst_test, "slist allocator_with_state" );

  string_test str_test;

  t.add( &string_test::constructor, str_test, "constructor" );
  t.add( &string_test::trivial_char_compare, str_test, "trivial_char_compare" );
  t.add( &string_test::reserve, str_test, "reserve" );
  t.add( &string_test::assign, str_test, "assign" );
  t.add( &string_test::erase, str_test, "erase" );
  t.add( &string_test::data, str_test, "data" );
  t.add( &string_test::c_str, str_test, "c_str" );
  t.add( &string_test::null_char, str_test, "null_char" );
  t.add( &string_test::insert, str_test, "insert" );
  t.add( &string_test::replace, str_test, "replace" );
  t.add( &string_test::resize, str_test, "resize" );
  t.add( &string_test::short_string, str_test, "short_string" );
  t.add( &string_test::find, str_test, "find" );
  t.add( &string_test::bogus_edge_find, str_test, "bogus_edge_find" );
  t.add( &string_test::rfind, str_test, "rfind" );
  t.add( &string_test::find_last_of, str_test, "find_last_of" );
  t.add( &string_test::find_last_not_of, str_test, "find_last_not_of" );
  t.add( &string_test::copy, str_test, "copy" );
  t.add( &string_test::mt, str_test, "mt" );
  t.add( &string_test::short_string_optim_bug, str_test, "short_string_optim_bug" );
  t.add( &string_test::compare, str_test, "compare" );
  t.add( &string_test::template_expression, str_test, "template_expression" );
  t.add( &string_test::te_tmp, str_test, "te_tmp" );
  t.add( &string_test::oper_tmp, str_test, "oper_tmp" );
  t.add( &string_test::template_wexpression, str_test, "template_wexpression" );
  t.add( &string_test::io, str_test, "io" );
  t.add( &string_test::allocator_with_state, str_test, "allocator_with_state" );
  t.add( &string_test::capacity, str_test, "capacity" );
  t.add( &string_test::concat24, str_test, "concat24" );

  alg_test algtest;

  t.add( &alg_test::remcopy1, algtest, "algorithm remcopy1" );
  t.add( &alg_test::remcpif1, algtest, "algorithm remcpif1" );
  t.add( &alg_test::remif1, algtest, "algorithm remif1" );
  t.add( &alg_test::remove1, algtest, "algorithm remove1" );
  t.add( &alg_test::repcpif1, algtest, "algorithm repcpif1" );
  t.add( &alg_test::replace0, algtest, "algorithm replace0" );
  t.add( &alg_test::replace1, algtest, "algorithm replace1" );
  t.add( &alg_test::replcpy1, algtest, "algorithm replcpy1" );
  t.add( &alg_test::replif1, algtest, "algorithm replif1" );
  t.add( &alg_test::revcopy1, algtest, "algorithm revcopy1" );
  t.add( &alg_test::reverse1, algtest, "algorithm reverse1" );

  t.add( &alg_test::min_max, algtest, "algorithm min_max" );
  t.add( &alg_test::count_test, algtest, "algorithm count_test" );
  t.add( &alg_test::sort_test, algtest, "algorithm sort_test" );
  t.add( &alg_test::search_n_test, algtest, "algorithm search_n_test" );
  t.add( &alg_test::find_first_of_test, algtest, "algorithm find_first_of_test" );
  t.add( &alg_test::find_first_of_nsc_test, algtest, "algorithm find_first_of_nsc_test" );

  t.add( &alg_test::max1, algtest, "algorithm max1" );
  t.add( &alg_test::max2, algtest, "algorithm max2" );
  t.add( &alg_test::maxelem1, algtest, "algorithm maxelem1" );
  t.add( &alg_test::maxelem2, algtest, "algorithm maxelem2" );

  t.add( &alg_test::min1, algtest, "algorithm min1" );
  t.add( &alg_test::min2, algtest, "algorithm min2" );
  t.add( &alg_test::minelem1, algtest, "algorithm minelem1" );
  t.add( &alg_test::minelem2, algtest, "algorithm minelem2" );

  t.add( &alg_test::lwrbnd1, algtest, "algorithm lwrbnd1" );
  t.add( &alg_test::lwrbnd2, algtest, "algorithm lwrbnd2" );
  t.add( &alg_test::uprbnd1, algtest, "algorithm uprbnd1" );
  t.add( &alg_test::uprbnd2, algtest, "algorithm uprbnd2" );

  t.add( &alg_test::equal_range0, algtest, "algorithm equal_range0" );
  t.add( &alg_test::equal_range1, algtest, "algorithm equal_range1" );
  t.add( &alg_test::equal_range2, algtest, "algorithm equal_range2" );

  t.add( &alg_test::foreach0, algtest, "algorithm foreach0" );
  t.add( &alg_test::foreach1, algtest, "algorithm foreach1" );

  t.add( &alg_test::gener1, algtest, "algorithm gener1" );
  t.add( &alg_test::gener2, algtest, "algorithm gener2" );
  t.add( &alg_test::genern1, algtest, "algorithm genern1" );
  t.add( &alg_test::genern2, algtest, "algorithm genern2" );

  functor_test fntest;

  t.add( &functor_test::equal0, fntest, "functor equal0" );
  t.add( &functor_test::equal1, fntest, "functor equal1" );
  t.add( &functor_test::equal2, fntest, "functor equal2" );
  t.add( &functor_test::equalto, fntest, "functor equalto" );

  t.add( &functor_test::bnegate1, fntest, "functor bnegate1" );
  t.add( &functor_test::bnegate2, fntest, "functor bnegate2" );

  t.add( &functor_test::logicand, fntest, "functor logic and" );
  t.add( &functor_test::logicnot, fntest, "functor logic not" );
  t.add( &functor_test::logicor, fntest, "functor logic or" );

  t.add( &functor_test::lesst, fntest, "functor less" );
  t.add( &functor_test::lesseqt, fntest, "functor less-or-equal" );

  t.add( &functor_test::greatert, fntest, "functor greater" );
  t.add( &functor_test::greatereq, fntest, "functor greater-or-equal" );

  t.add( &functor_test::func1, fntest, "functor func1" );
  t.add( &functor_test::func2, fntest, "functor func2" );
  t.add( &functor_test::func3, fntest, "functor func3" );

  t.add( &functor_test::plus0, fntest, "functor plus0" );
  t.add( &functor_test::minus0, fntest, "functor minus0" );

  iostream_test strm_test;

  t.add( &iostream_test::manipulators, strm_test, "manipulators" );
  t.add( &iostream_test::in_avail, strm_test, "in_avail in std streams" );

  sstream_test sstrm_test;

  t.add( &sstream_test::output, sstrm_test, "sstream output" );
  t.add( &sstream_test::input, sstrm_test, "sstream input" );
  t.add( &sstream_test::input_char, sstrm_test, "sstream input_char" );
  t.add( &sstream_test::io, sstrm_test, "sstream io" );
  t.add( &sstream_test::err, sstrm_test, "sstream err" );
  t.add( &sstream_test::err_long, sstrm_test, "sstream err_long" );
  t.add( &sstream_test::maxint, sstrm_test, "sstream maxint" );
  t.add( &sstream_test::init_in, sstrm_test, "sstream init_in" );
  t.add( &sstream_test::init_out, sstrm_test, "sstream init_out" );
  t.add( &sstream_test::buf, sstrm_test, "sstream buf" );
  t.add( &sstream_test::rdbuf, sstrm_test, "sstream rdbuf" );
  t.add( &sstream_test::streambuf_output, sstrm_test, "sstream streambuf_output" );
  t.add( &sstream_test::seek, sstrm_test, "sstream seek" );
  t.add( &sstream_test::seekp, sstrm_test, "sstream seekp" );
  t.add( &sstream_test::seek_gp, sstrm_test, "sstream seek_gp" );
  t.add( &sstream_test::tellp, sstrm_test, "sstream tellp" );
  t.add( &sstream_test::negative, sstrm_test, "sstream negative" );

  numerics num_test;

  exam::test_suite::test_case_type num_tc[6];

  t.add( &numerics::accum2, num_test, "accumulate, non-default functor",
    num_tc[0] = t.add( &numerics::accum1, num_test, "accumulate" ) );

  num_tc[1] = vec_tc[12];
  t.add( &numerics::times, num_test, "accumulate, multiplies<int> functor", num_tc, num_tc + 2 );

  num_tc[1] = t.add( &numerics::partsum0, num_test, "partial summ, raw array" );
  num_tc[2] = vec_tc[12];
  num_tc[2] = t.add( &numerics::partsum1, num_test, "partial summ, vector", num_tc + 1, num_tc + 3 );
  num_tc[3] = vec_tc[12];
  t.add( &numerics::partsum2, num_test, "partial summ, multiplies<int> functor", num_tc + 2, num_tc + 4 );

  num_tc[3] = t.add( &numerics::innprod0, num_test, "inner product, raw array" );
  num_tc[4] = vec_tc[12];
  num_tc[4] = t.add( &numerics::innprod1, num_test, "inner product, vector", num_tc + 3, num_tc + 5 );
  num_tc[5] = vec_tc[12];
  t.add( &numerics::innprod2, num_test, "inner product, userdefined functors", num_tc + 4, num_tc + 6 );

  map_test m_test;

  t.add( &map_test::map1, m_test, "map1" );
  t.add( &map_test::mmap1, m_test, "mmap1" );
  t.add( &map_test::mmap2, m_test, "mmap2" );
  t.add( &map_test::iterators, m_test, "map iterators" );
  t.add( &map_test::equal_range, m_test, "map equal_range" );
  t.add( &map_test::allocator_with_state, m_test, "map allocator_with_state" );
  t.add( &map_test::template_methods, m_test, "map template_methods" );

  set_test s_test;

  t.add( &set_test::set1, s_test, "set1" );
  t.add( &set_test::set2, s_test, "set2" );
  t.add( &set_test::erase, s_test, "set erase" );
  t.add( &set_test::insert, s_test, "set insert" );
  t.add( &set_test::find, s_test, "set find" );
  t.add( &set_test::bounds, s_test, "set bounds" );
  t.add( &set_test::specialized_less, s_test, "set specialized_less" );
  t.add( &set_test::implementation_check, s_test, "set implementation_check" );
  t.add( &set_test::allocator_with_state, s_test, "set allocator_with_state" );
  t.add( &set_test::reverse_iterator_test, s_test, "set reverse_iterator_test" );
  t.add( &set_test::template_methods, s_test, "set template_methods" );

  set_union_test su_test;

  t.add( &set_union_test::setunon0, su_test, "setunon0" );
  t.add( &set_union_test::setunon1, su_test, "setunon1" );
  t.add( &set_union_test::setunon2, su_test, "setunon2" );

  set_intersection_test si_test;

  t.add( &set_intersection_test::setintr0, si_test, "setintr0" );
  t.add( &set_intersection_test::setintr1, si_test, "setintr1" );
  t.add( &set_intersection_test::setintr2, si_test, "setintr2" );

  set_difference_test sd_test;

  t.add( &set_difference_test::setdiff0, sd_test, "setdiff0" );
  t.add( &set_difference_test::setdiff1, sd_test, "setdiff1" );
  t.add( &set_difference_test::setdiff2, sd_test, "setdiff2" );
  t.add( &set_difference_test::setsymd0, sd_test, "setsymd0" );
  t.add( &set_difference_test::setsymd1, sd_test, "setsymd1" );
  t.add( &set_difference_test::setsymd2, sd_test, "setsymd2" );

  multiset_test ms_test;

  t.add( &multiset_test::mset1, ms_test, "mset1" );
  t.add( &multiset_test::mset3, ms_test, "mset3" );
  t.add( &multiset_test::mset5, ms_test, "mset5" );

  hash_test h_test;

  t.add( &hash_test::hmap1, h_test, "hmap1" );
  t.add( &hash_test::hmmap1, h_test, "hmmap1" );
  t.add( &hash_test::hmmap2, h_test, "hmmap2" );
  t.add( &hash_test::hmset1, h_test, "hmset1" );
  t.add( &hash_test::hmset1, h_test, "hmset1" );
  t.add( &hash_test::hset2, h_test, "hset2" );
  t.add( &hash_test::insert_erase, h_test, "hash insert_erase" );
  t.add( &hash_test::allocator_with_state, h_test, "hash allocator_with_state" );

  unordered_test uo_test;

  t.add( &unordered_test::uset, uo_test, "unordered uset" );
  t.add( &unordered_test::umultiset, uo_test, "unordered umultiset" );
  t.add( &unordered_test::umap, uo_test, "unordered umap" );
  t.add( &unordered_test::umultimap, uo_test, "unordered umultimap" );
  t.add( &unordered_test::user_case, uo_test, "unordered user_case" );
  t.add( &unordered_test::hash_policy, uo_test, "unordered hash_policy" );
  t.add( &unordered_test::buckets, uo_test, "unordered buckets" );
  t.add( &unordered_test::equal_range, uo_test, "unordered equal_range" );
  t.add( &unordered_test::template_methods, uo_test, "unordered template_methods" );

  fstream_test fstrm_test;

  t.add( &fstream_test::output, fstrm_test, "fstream output" );
  t.add( &fstream_test::input, fstrm_test, "fstream input" );
  t.add( &fstream_test::input_char, fstrm_test, "fstream input_char" );
  t.add( &fstream_test::io, fstrm_test, "fstream io" );
  t.add( &fstream_test::err, fstrm_test, "fstream err" );
  t.add( &fstream_test::tellg, fstrm_test, "fstream tellg" );
  t.add( &fstream_test::tellp, fstrm_test, "fstream tellp" );
  t.add( &fstream_test::seek, fstrm_test, "fstream seek" );
  t.add( &fstream_test::buf, fstrm_test, "fstream buf" );
  t.add( &fstream_test::rdbuf, fstrm_test, "fstream rdbuf" );
  t.add( &fstream_test::streambuf_output, fstrm_test, "fstream streambuf_output" );
  t.add( &fstream_test::win32_file_format, fstrm_test, "fstream win32_file_format" );
  t.add( &fstream_test::null_stream, fstrm_test, "fstream null_stream" );
  t.add( &fstream_test::null_buf, fstrm_test, "fstream null_buf" );
  t.add( &fstream_test::offset, fstrm_test, "fstream offset" );
  t.add( &fstream_test::big_file, fstrm_test, "fstream big_file" );
  t.add( &fstream_test::custom_facet, fstrm_test, "fstream custom_facet" );
  
  search_test sch_test;
  
  t.add( &search_test::search0, sch_test, "search0" );
  t.add( &search_test::search1, sch_test, "search1" );
  t.add( &search_test::search2, sch_test, "search2" );

  sort_test srt_test;
  
  t.add( &sort_test::sort1, srt_test, "sort1" );
  t.add( &sort_test::sort2, srt_test, "sort2" );
  t.add( &sort_test::sort3, srt_test, "sort3" );
  t.add( &sort_test::sort4, srt_test, "sort4" );
  t.add( &sort_test::stblsrt1, srt_test, "stblsrt1" );
  t.add( &sort_test::stblsrt2, srt_test, "stblsrt2" );
  t.add( &sort_test::bad_predicate_detected, srt_test, "bad_predicate_detected" );

  adj_test a_test;

  t.add( &adj_test::adjfind0, a_test, "adjfind0" );
  t.add( &adj_test::adjfind1, a_test, "adjfind1" );
  t.add( &adj_test::adjfind2, a_test, "adjfind2" );
  t.add( &adj_test::adjdiff0, a_test, "adjdiff0" );
  t.add( &adj_test::adjdiff1, a_test, "adjdiff1" );
  t.add( &adj_test::adjdiff2, a_test, "adjdiff2" );
  
  copy_test cp_test;

  t.add( &copy_test::copy_array, cp_test, "copy_array" );
  t.add( &copy_test::copy_volatile, cp_test, "copy_volatile" );
  t.add( &copy_test::copy_vector, cp_test, "copy_vector" );
  t.add( &copy_test::copy_insert, cp_test, "copy_insert" );
  t.add( &copy_test::copy_back, cp_test, "copy_back" );
  t.add( &copy_test::copy_back_array, cp_test, "copy_back_array" );

  if ( opts.is_set( 'l' ) ) {
    t.print_graph( std::cerr );
    return 0;
  }

  if ( opts.is_set( 'v' ) ) {
    t.flags( t.flags() | exam::base_logger::verbose );
  }

  if ( opts.is_set( 't' ) ) {
    t.flags( t.flags() | exam::base_logger::trace );
  }

  if ( opts.is_set( 'r' ) ) {
    std::stringstream ss( opts.get<std::string>( 'r' ) );
    int n;
    while ( ss >> n ) {
      t.single( n );
    }

    return 0;
  }

  return t.girdle() + test_main();
}
