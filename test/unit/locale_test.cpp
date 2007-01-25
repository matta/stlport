#include <string>
#if !defined (STLPORT) || !defined (_STLP_USE_NO_IOSTREAMS)
#  include <sstream>
#  include <locale>
#  include <stdexcept>
#  include <memory>
//#  include <iostream>

#  include "complete_digits.h"
#  include "cppunit/cppunit_proxy.h"

#  if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#  endif

struct ref_locale {
  const char *name;
  const char *decimal_point;
  const char *thousands_sep;
  const char *money_int_prefix;
  const char *money_int_prefix_old;
  const char *money_prefix;
  const char *money_suffix;
  const char *money_decimal_point;
  const char *money_thousands_sep;
};

// Pls, don't write #ifdef _STLP_REAL_LOCALE_IMPLEMENTED here!
// It undefined in any case!!!!!

static const ref_locale tested_locales[] = {
//{  name,         decimal_point, thousands_sep, money_int_prefix, money_int_prefix_old, money_prefix, money_suffix, money_decimal_point, money_thousands_sep},
  { "fr_FR",       ",",           "\xa0",        "EUR ",           "FRF ",               "",           "",           ",",
#if defined (WIN32) || defined (_WIN32)
                                                                                                                                          "\xa0" },
#else
                                                                                                                                          " " },
#endif
  { "ru_RU.koi8r", ",",           ".",           "RUB ",           "RUR ",               "",           "\xd2\xd5\xc2", ".",               " " },
  { "en_GB",       ".",           ",",           "GBP ",           "",                   "\xa3",       "",           ".",                 "," },
  { "en_US",       ".",           ",",           "USD ",           "",                   "$",          "",           ".",                 "," },
  { "C",           ".",           ",",           "",               "",                   "",           "",           " ",                 " " },
};


//
// TestCase class
//
class LocaleTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(LocaleTest);
#if defined (STLPORT) && !defined (_STLP_USE_EXCEPTIONS)
  CPPUNIT_IGNORE;
#endif
  CPPUNIT_TEST(locale_by_name);
  CPPUNIT_STOP_IGNORE;
  CPPUNIT_TEST(loc_has_facet);
  CPPUNIT_TEST(num_put_get);
  CPPUNIT_TEST(money_put_get);
  CPPUNIT_TEST(money_put_X_bug);
  CPPUNIT_TEST(time_put_get);
  CPPUNIT_TEST(collate_facet);
  CPPUNIT_TEST(ctype_facet);
#if defined (STLPORT) && defined (_STLP_NO_MEMBER_TEMPLATES)
  CPPUNIT_IGNORE;
#endif
  CPPUNIT_TEST(locale_init_problem);
  CPPUNIT_STOP_IGNORE;
  CPPUNIT_TEST(default_locale);
#if !defined (STLPORT)
  CPPUNIT_IGNORE;
#endif
  CPPUNIT_STOP_IGNORE;
#if (defined (STLPORT) && \
   (!defined (_STLP_USE_EXCEPTIONS) || defined (_STLP_NO_MEMBER_TEMPLATES) || defined (_STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS))) || \
   (defined (_MSC_VER) && (_MSC_VER < 1300))
  CPPUNIT_IGNORE;
#endif
  CPPUNIT_TEST(combine);
  CPPUNIT_TEST_SUITE_END();

public:
  void locale_by_name();
  void loc_has_facet();
  void num_put_get();
  void money_put_get();
  void time_put_get();
  void collate_facet();
  void ctype_facet();
  void locale_init_problem();
  void money_put_X_bug();
  void default_locale();
  void combine();
private:
  void _loc_has_facet( const locale&, const ref_locale& );
  void _num_put_get( const locale&, const ref_locale& );
  void _money_put_get( const locale&, const ref_locale& );
  void _money_put_get2( const locale& loc, const locale& streamLoc, const ref_locale& );
  void _time_put_get( const locale&, const ref_locale& );
  void _collate_facet( const locale&, const ref_locale& );
  void _ctype_facet( const locale&, const ref_locale& );
  void _ctype_facet_w( const locale&, const ref_locale& );
  void _locale_init_problem( const locale&, const ref_locale& );
  void _money_put_X_bug( const locale&, const ref_locale& );
};

CPPUNIT_TEST_SUITE_REGISTRATION(LocaleTest);

//
// tests implementation
//
void LocaleTest::_num_put_get( const locale& loc, const ref_locale& rl ) {
  CPPUNIT_ASSERT( has_facet<numpunct<char> >(loc) );
  numpunct<char> const& npct = use_facet<numpunct<char> >(loc);
  CPPUNIT_ASSERT( npct.decimal_point() == *rl.decimal_point );

  float val = 1234.56f;
  ostringstream fostr;
  fostr.imbue(loc);
  fostr << val;

  string ref = "1";
  if (!npct.grouping().empty()) {
    ref += npct.thousands_sep();
  }
  ref += "234";
  ref += npct.decimal_point();
  ref += "56";
  //cout << "In " << loc.name() << " 1234.56 is written: " << fostr.str() << endl;
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 12345678.9f;
  ref = "1";
  ref += npct.decimal_point();
  ref += "23457e+";
  string digits = "7";
  complete_digits(digits);
  ref += digits;
  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 1000000000.0f;
  fostr.str("");
  fostr << val;
  digits = "9";
  complete_digits(digits);
  CPPUNIT_ASSERT( fostr.str() == string("1e+") + digits );

  val = 1234.0f;
  ref = "1";
  if (!npct.grouping().empty()) {
    ref += npct.thousands_sep();
  }
  ref += "234";
  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 10000001.0f;
  fostr.str("");
  fostr << val;
  digits = "7";
  complete_digits(digits);
  CPPUNIT_ASSERT( fostr.str() == string("1e+") + digits );

#if defined (__BORLANDC__)
  num_put<char> const& nput = use_facet<num_put<char> >(loc);
  typedef numeric_limits<double> limd;
  fostr.setf(ios_base::uppercase | ios_base::showpos);

  if (limd::has_infinity) {
    double infinity = limd::infinity();
    fostr.str("");
    nput.put(fostr, fostr, ' ', infinity);
    CPPUNIT_ASSERT( fostr.str() == string("+Inf") );
  }

  if (limd::has_quiet_NaN) {
    double qnan = limd::quiet_NaN();
    fostr.str("");
    nput.put(fostr, fostr, ' ', qnan);
    CPPUNIT_ASSERT( fostr.str() == string("+NaN") );
  }
#endif
}

void LocaleTest::_money_put_get( const locale& loc, const ref_locale& rl )
{
  _money_put_get2(loc, loc, rl);
}

void LocaleTest::_money_put_get2( const locale& loc, const locale& streamLoc, const ref_locale& rl )
{
  CPPUNIT_ASSERT( has_facet<money_put<char> >(loc) );
  money_put<char> const& fmp = use_facet<money_put<char> >(loc);
  CPPUNIT_ASSERT( has_facet<money_get<char> >(loc) );
  money_get<char> const& fmg = use_facet<money_get<char> >(loc);

  ostringstream ostr;
  ostr.imbue(streamLoc);
  ostr << showbase;

  //Check a positive value (international format)
  {
    string str_res;
    //money_put
    {
      CPPUNIT_ASSERT( (has_facet<moneypunct<char, true> >(loc)) );
      moneypunct<char, true> const& intl_fmp = use_facet<moneypunct<char, true> >(loc);

      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, true, ostr, ' ', 123456);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      //On a positive value we skip the sign field if exists:
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }
      // international currency abbreviation, if it is before value

      /*
       * int_curr_symbol
       *
       *   The international currency symbol. The operand is a four-character
       *   string, with the first three characters containing the alphabetic
       *   international currency symbol in accordance with those specified
       *   in the ISO 4217 specification. The fourth character is the character used
       *   to separate the international currency symbol from the monetary quantity.
       *
       * (http://www.opengroup.org/onlinepubs/7990989775/xbd/locale.html)
       */
      string::size_type p = strlen( rl.money_int_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( intl_fmp.pos_format().field[fieldIndex] == money_base::symbol );
        string::size_type p_old = strlen( rl.money_int_prefix_old );
        CPPUNIT_ASSERT( (str_res.substr(index, p) == rl.money_int_prefix) ||
                        ((p_old != 0) &&
                         (str_res.substr(index, p_old) == rl.money_int_prefix_old)) );
        if ( str_res.substr(index, p) == rl.money_int_prefix ) {
          index += p;
        } else {
          index += p_old;
        }
        ++fieldIndex;
      }

      // space after currency
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::space ||
          intl_fmp.pos_format().field[fieldIndex] == money_base::none) {
        // iternational currency symobol has four chars, one of these chars
        // is separator, so if format has space on this place, it should
        // be skipped.
        ++fieldIndex;
      }

      // sign
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }

      // value
      CPPUNIT_ASSERT( str_res[index++] == '1' );
      if (!intl_fmp.grouping().empty()) {
        CPPUNIT_ASSERT( str_res[index++] == /* intl_fmp.thousands_sep() */ *rl.money_thousands_sep );
      }
      CPPUNIT_ASSERT( str_res[index++] == '2' );
      CPPUNIT_ASSERT( str_res[index++] == '3' );
      CPPUNIT_ASSERT( str_res[index++] == '4' );
      if (intl_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == /* intl_fmp.decimal_point() */ *rl.money_decimal_point );
      }
      CPPUNIT_ASSERT( str_res[index++] == '5' );
      CPPUNIT_ASSERT( str_res[index++] == '6' );
      ++fieldIndex;

      // sign
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }

      // space
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::space ) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      // sign
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }

      //as space cannot be last the only left format can be none:
      while ( fieldIndex < 3 ) {
        CPPUNIT_ASSERT( intl_fmp.pos_format().field[fieldIndex] == money_base::none );
        ++fieldIndex;
      }
    }

    //money_get
    {
      ios_base::iostate err = ios_base::goodbit;
      string digits;

      istringstream istr(str_res);
      ostr.str( "" );
      ostr.clear();
      fmg.get(istr, istreambuf_iterator<char, char_traits<char> >(), true, ostr, err, digits);
      CPPUNIT_ASSERT( (err & (ios_base::failbit | ios_base::badbit)) == 0 );
      CPPUNIT_ASSERT( digits == "123456" );
    }
  }

  ostr.str("");
  //Check a negative value (national format)
  {
    CPPUNIT_ASSERT( (has_facet<moneypunct<char, false> >(loc)) );
    moneypunct<char, false> const& dom_fmp = use_facet<moneypunct<char, false> >(loc);
    string str_res;
    //Check money_put
    {
      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, false, ostr, ' ', -123456);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      if (dom_fmp.neg_format().field[fieldIndex] == money_base::sign) {
        CPPUNIT_ASSERT( str_res.substr(index, dom_fmp.negative_sign().size()) == dom_fmp.negative_sign() );
        index += dom_fmp.negative_sign().size();
        ++fieldIndex;
      }

      string::size_type p = strlen( rl.money_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_prefix );
        index += p;
        ++fieldIndex;
      }
      if (dom_fmp.neg_format().field[fieldIndex] == money_base::space ||
          dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      CPPUNIT_ASSERT( str_res[index++] == '1' );
      if (!dom_fmp.grouping().empty()) {
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.thousands_sep() );
      }
      CPPUNIT_ASSERT( str_res[index++] == '2' );
      CPPUNIT_ASSERT( str_res[index++] == '3' );
      CPPUNIT_ASSERT( str_res[index++] == '4' );
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.decimal_point() );
      }
      CPPUNIT_ASSERT( str_res[index++] == '5' );
      CPPUNIT_ASSERT( str_res[index++] == '6' );
      ++fieldIndex;

      //space cannot be last:
      if ((fieldIndex < 3) &&
          dom_fmp.neg_format().field[fieldIndex] == money_base::space) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      if (fieldIndex == 3) {
        //If none is last we should not add anything to the resulting string:
        if (dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
          CPPUNIT_ASSERT( index == str_res.size() );
        } else {
          CPPUNIT_ASSERT( dom_fmp.neg_format().field[fieldIndex] == money_base::symbol );
          CPPUNIT_ASSERT( str_res.substr(index, strlen(rl.money_suffix)) == rl.money_suffix );
        }
      }
    }

    //money_get
    {
      ios_base::iostate err = ios_base::goodbit;
#if defined (STLPORT)
      _STLP_LONGEST_FLOAT_TYPE val;
#else
      long double val;
#endif

      istringstream istr(str_res);
      fmg.get(istr, istreambuf_iterator<char, char_traits<char> >(), false, ostr, err, val);
      CPPUNIT_ASSERT( (err & (ios_base::failbit | ios_base::badbit)) == 0 );
      if (dom_fmp.negative_sign().empty()) {
        //Without negative sign there is no way to guess the resulting amount sign ("C" locale):
        CPPUNIT_ASSERT( val == 123456 );
      }
      else {
        CPPUNIT_ASSERT( val == -123456 );
      }
    }
  }
}


// Test for bug in case when number of digits in value less then number
// of digits in fraction. I.e. '9' should be printed as '0.09',
// if x.frac_digits() == 2.

void LocaleTest::_money_put_X_bug( const locale& loc, const ref_locale& rl )
{
  CPPUNIT_ASSERT( has_facet<money_put<char> >(loc) );
  money_put<char> const& fmp = use_facet<money_put<char> >(loc);

  ostringstream ostr;
  ostr.imbue(loc);
  ostr << showbase;

  // ostr.str("");
  // Check value with one decimal digit:
  {
    CPPUNIT_ASSERT( (has_facet<moneypunct<char, false> >(loc)) );
    moneypunct<char, false> const& dom_fmp = use_facet<moneypunct<char, false> >(loc);
    string str_res;
    // Check money_put
    {
      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, false, ostr, ' ', 9);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      if (dom_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        CPPUNIT_ASSERT( str_res.substr(index, dom_fmp.positive_sign().size()) == dom_fmp.positive_sign() );
        index += dom_fmp.positive_sign().size();
        ++fieldIndex;
      }

      string::size_type p = strlen( rl.money_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_prefix );
        index += p;
        ++fieldIndex;
      }
      if (dom_fmp.neg_format().field[fieldIndex] == money_base::space ||
          dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == '0' );
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.decimal_point() );
        for ( int fd = 1; fd < dom_fmp.frac_digits(); ++fd ) {
          CPPUNIT_ASSERT( str_res[index++] == '0' );
        }
      }
      CPPUNIT_ASSERT( str_res[index++] == '9' );
      ++fieldIndex;

      //space cannot be last:
      if ((fieldIndex < 3) &&
          dom_fmp.neg_format().field[fieldIndex] == money_base::space) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      if (fieldIndex == 3) {
        //If none is last we should not add anything to the resulting string:
        if (dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
          CPPUNIT_ASSERT( index == str_res.size() );
        } else {
          CPPUNIT_ASSERT( dom_fmp.neg_format().field[fieldIndex] == money_base::symbol );
          CPPUNIT_ASSERT( str_res.substr(index, strlen(rl.money_suffix)) == rl.money_suffix );
        }
      }
    }
  }

  ostr.str("");
  // Check value with two decimal digit:
  {
    CPPUNIT_ASSERT( (has_facet<moneypunct<char, false> >(loc)) );
    moneypunct<char, false> const& dom_fmp = use_facet<moneypunct<char, false> >(loc);
    string str_res;
    // Check money_put
    {
      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, false, ostr, ' ', 90);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      if (dom_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        CPPUNIT_ASSERT( str_res.substr(index, dom_fmp.positive_sign().size()) == dom_fmp.positive_sign() );
        index += dom_fmp.positive_sign().size();
        ++fieldIndex;
      }

      string::size_type p = strlen( rl.money_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_prefix );
        index += p;
        ++fieldIndex;
      }
      if (dom_fmp.neg_format().field[fieldIndex] == money_base::space ||
          dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == '0' );
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.decimal_point() );
        for ( int fd = 1; fd < dom_fmp.frac_digits() - 1; ++fd ) {
          CPPUNIT_ASSERT( str_res[index++] == '0' );
        }
      }
      CPPUNIT_ASSERT( str_res[index++] == '9' );
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == '0' );
      }
      ++fieldIndex;

      //space cannot be last:
      if ((fieldIndex < 3) &&
          dom_fmp.neg_format().field[fieldIndex] == money_base::space) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      if (fieldIndex == 3) {
        //If none is last we should not add anything to the resulting string:
        if (dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
          CPPUNIT_ASSERT( index == str_res.size() );
        } else {
          CPPUNIT_ASSERT( dom_fmp.neg_format().field[fieldIndex] == money_base::symbol );
          CPPUNIT_ASSERT( str_res.substr(index, strlen(rl.money_suffix)) == rl.money_suffix );
        }
      }
    }
  }
}

void LocaleTest::_time_put_get( const locale& loc, const ref_locale&)
{
  CPPUNIT_ASSERT( has_facet<time_put<char> >(loc) );
  const time_put<char>& tmp = use_facet<time_put<char> >(loc);

  struct tm xmas = { 0, 0, 12, 25, 11, 93 };
  ostringstream ostr;
  ostr.imbue(loc);
  string format = "%B %d %Y";

  time_put<char>::iter_type ret = tmp.put(ostr, ostr, ' ', &xmas, format.data(), format.data() + format.size());
  CPPUNIT_ASSERT( !ret.failed() );

  /*
   * In other words, user conformation is required for reliable parsing
   * of user-entered dates and times, but machine-generated formats can be
   * parsed reliably. This allows parsers to be aggressive about interpreting
   * user variations on standard format.
   *
   *                                             ISO/IEC 14882, 22.2.5.1
   */
  CPPUNIT_ASSERT( has_facet<time_get<char> >(loc) );
  const time_get<char>& tmg = use_facet<time_get<char> >(loc);
  basic_ios<char> io(0);
  io.imbue(loc);

  istringstream istr( ostr.str() );
  istreambuf_iterator<char, char_traits<char> > i( istr );
  istreambuf_iterator<char, char_traits<char> > e;
  ios_base::iostate err = ios_base::goodbit;
  struct tm other = { 15, 20, 9, 14, 7, 105 };

  i = tmg.get_monthname( i, e, io, err, &other );
  CPPUNIT_ASSERT( err == ios_base::goodbit );
  CPPUNIT_ASSERT( other.tm_mon == xmas.tm_mon );

  ++i; ++i; ++i; ++i; // skip day of month and spaces around it
  i = tmg.get_year( i, e, io, err, &other );

  CPPUNIT_ASSERT( err == ios_base::eofbit );
  CPPUNIT_ASSERT( other.tm_year == xmas.tm_year );

  ostringstream ostrX;
  ostrX.imbue(loc);
  format = "%x %X";

  ret = tmp.put(ostrX, ostrX, ' ', &xmas, format.data(), format.data() + format.size());
  CPPUNIT_ASSERT( !ret.failed() );

  istringstream istrX( ostrX.str() );
  istreambuf_iterator<char, char_traits<char> > j( istrX );

  err = ios_base::goodbit;

  struct tm yet_more = { 15, 20, 9, 14, 7, 105 };

  j = tmg.get_date( j, e, io, err, &yet_more );

  CPPUNIT_ASSERT( err == ios_base::goodbit );

  CPPUNIT_ASSERT( yet_more.tm_sec != xmas.tm_sec );
  CPPUNIT_ASSERT( yet_more.tm_min != xmas.tm_min );
  CPPUNIT_ASSERT( yet_more.tm_hour != xmas.tm_hour );
  CPPUNIT_ASSERT( yet_more.tm_mday == xmas.tm_mday );
  CPPUNIT_ASSERT( yet_more.tm_mon == xmas.tm_mon );
  CPPUNIT_ASSERT( yet_more.tm_year == xmas.tm_year );

  ++j; // skip space

  j = tmg.get_time( j, e, io, err, &yet_more );

  CPPUNIT_ASSERT( err == ios_base::eofbit || err == ios_base::goodbit );

  CPPUNIT_ASSERT( yet_more.tm_sec == xmas.tm_sec );
  CPPUNIT_ASSERT( yet_more.tm_min == xmas.tm_min );
  CPPUNIT_ASSERT( yet_more.tm_hour == xmas.tm_hour );
  CPPUNIT_ASSERT( yet_more.tm_mday == xmas.tm_mday );
  CPPUNIT_ASSERT( yet_more.tm_mon == xmas.tm_mon );
  CPPUNIT_ASSERT( yet_more.tm_year == xmas.tm_year );
}

void LocaleTest::_collate_facet( const locale& loc, const ref_locale&)
{
  CPPUNIT_ASSERT( has_facet<collate<char> >(loc) );
  collate<char> const& col = use_facet<collate<char> >(loc);

  char const str1[] = "fran�ois";
  char const str2[] = "francois";
  CPPUNIT_ASSERT( col.compare(str1, str1 + sizeof(str1) / sizeof(str1[0]), str2, str2 + sizeof(str2) / sizeof(str2[0])) );
}

void LocaleTest::_ctype_facet( const locale& loc, const ref_locale&)
{
  CPPUNIT_ASSERT( has_facet<ctype<char> >(loc) );
  ctype<char> const& ct = use_facet<ctype<char> >(loc);
# ifndef _STLP_NO_WCHAR_T
  CPPUNIT_ASSERT( has_facet<ctype<wchar_t> >(loc) );
  ctype<wchar_t> const& wct = use_facet<ctype<wchar_t> >(loc);
# endif
  //is
  {
    CPPUNIT_ASSERT( ct.is(ctype_base::digit, '0') );
    CPPUNIT_ASSERT( ct.is(ctype_base::upper, 'A') );
    CPPUNIT_ASSERT( ct.is(ctype_base::lower, 'a') );
    CPPUNIT_ASSERT( ct.is(ctype_base::alpha, 'A') );
    CPPUNIT_ASSERT( ct.is(ctype_base::space, ' ') );
    CPPUNIT_ASSERT( !ct.is(ctype_base::space, '2') );
    CPPUNIT_ASSERT( ct.is(ctype_base::punct, '.') );
    CPPUNIT_ASSERT( ct.is(ctype_base::xdigit, 'a') );
  }

  //is range
  {
    char values[] = "0Aa .";
    ctype_base::mask res[sizeof(values)];
    ct.is(values, values + sizeof(values), res);
    // '0'
    CPPUNIT_ASSERT( (res[0] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[0] & ctype_base::digit) != 0 );
    CPPUNIT_ASSERT( (res[0] & ctype_base::xdigit) != 0 );
    // 'A'
    CPPUNIT_ASSERT( (res[1] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::alpha) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::xdigit) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::upper) != 0 );
    // 'a'
    CPPUNIT_ASSERT( (res[2] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::alpha) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::xdigit) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::lower) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::space) == 0 );
    // ' '
    CPPUNIT_ASSERT( (res[3] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[3] & ctype_base::space) != 0 );
    CPPUNIT_ASSERT( (res[3] & ctype_base::digit) == 0 );
    // '.'
    CPPUNIT_ASSERT( (res[4] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[4] & ctype_base::punct) != 0 );
    CPPUNIT_ASSERT( (res[4] & ctype_base::digit) == 0 );
  }

  //scan_is
  {
    char range[] = "abAc123 .";
    const char *rbeg = range;
    const char *rend = range + sizeof(range);

    const char *res;
    res = ct.scan_is((ctype_base::mask)(ctype_base::alpha | ctype_base::lower), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'a' );

    res = ct.scan_is(ctype_base::upper, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'A' );

    res = ct.scan_is(ctype_base::punct, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == '.' );
  }

  //scan_not
  {
    char range[] = "abAc123 .";
    const char *rbeg = range;
    const char *rend = range + sizeof(range);

    const char *res;
    res = ct.scan_not((ctype_base::mask)(ctype_base::alpha | ctype_base::lower), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == '1' );

    res = ct.scan_not(ctype_base::alpha, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == '1' );

    res = ct.scan_not(ctype_base::punct, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'a' );
  }

  //toupper
  {
    CPPUNIT_ASSERT( ct.toupper('a') == 'A' );
    CPPUNIT_ASSERT( ct.toupper('A') == 'A' );
    CPPUNIT_ASSERT( ct.toupper('1') == '1' );
  }

  //toupper range
  {
    char range[] = "abAc1";
    char expected_range[] = "ABAC1";
    ct.toupper(range, range + sizeof(range));
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), expected_range) );
  }

  //tolower
  {
    CPPUNIT_ASSERT( ct.tolower('A') == 'a' );
    CPPUNIT_ASSERT( ct.tolower('a') == 'a' );
    CPPUNIT_ASSERT( ct.tolower('1') == '1' );
  }

  //tolower range
  {
    char range[] = "ABaC1";
    char expected_range[] = "abac1";
    ct.tolower(range, range + sizeof(range));
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), expected_range) );
  }

  //widen
  {
    CPPUNIT_ASSERT( ct.widen('a') == 'a' );
  }

  //widen range
  {
    char range[] = "ABaC1";
    char res[sizeof(range)];
    ct.widen(range, range + sizeof(range), res);
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), res) );
  }

  //narrow
  {
    CPPUNIT_ASSERT( ct.narrow('a', 'b') == 'a' );
  }

  //narrow range
  {
    char range[] = "ABaC1";
    char res[sizeof(range)];
    ct.narrow(range, range + sizeof(range), 'b', res);
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), res) );
  }
}

void LocaleTest::_ctype_facet_w( const locale& loc, const ref_locale&)
{
# ifndef _STLP_NO_WCHAR_T
  CPPUNIT_ASSERT( has_facet<ctype<wchar_t> >(loc) );
  ctype<wchar_t> const& wct = use_facet<ctype<wchar_t> >(loc);
  //is
  {
    CPPUNIT_ASSERT( wct.is(ctype_base::digit, L'0') );
    CPPUNIT_ASSERT( wct.is(ctype_base::upper, L'A') );
    CPPUNIT_ASSERT( wct.is(ctype_base::lower, L'a') );
    CPPUNIT_ASSERT( wct.is(ctype_base::alpha, L'A') );
    CPPUNIT_ASSERT( wct.is(ctype_base::space, L' ') );
    CPPUNIT_ASSERT( !wct.is(ctype_base::space, L'2') );
    CPPUNIT_ASSERT( wct.is(ctype_base::punct, L'.') );
    CPPUNIT_ASSERT( wct.is(ctype_base::xdigit, L'a') );
  }

  //is range
  {
    wchar_t values[] = L"0Aa .";
    ctype_base::mask res[sizeof(values)];
    wct.is(values, values + sizeof(values), res);
    // '0'
    CPPUNIT_ASSERT( (res[0] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[0] & ctype_base::digit) != 0 );
    CPPUNIT_ASSERT( (res[0] & ctype_base::xdigit) != 0 );
    // 'A'
    CPPUNIT_ASSERT( (res[1] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::alpha) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::xdigit) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::upper) != 0 );
    // 'a'
    CPPUNIT_ASSERT( (res[2] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::alpha) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::xdigit) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::lower) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::space) == 0 );
    // ' '
    CPPUNIT_ASSERT( (res[3] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[3] & ctype_base::space) != 0 );
    CPPUNIT_ASSERT( (res[3] & ctype_base::digit) == 0 );
    // '.'
    CPPUNIT_ASSERT( (res[4] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[4] & ctype_base::punct) != 0 );
    CPPUNIT_ASSERT( (res[4] & ctype_base::digit) == 0 );
  }

  //scan_is
  {
    wchar_t range[] = L"abAc123 .";
    const wchar_t *rbeg = range;
    const wchar_t *rend = range + sizeof(range);

    const wchar_t *res;
    res = wct.scan_is((ctype_base::mask)(ctype_base::alpha | ctype_base::lower), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == L'a' );

    res = wct.scan_is(ctype_base::upper, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == L'A' );

    res = wct.scan_is(ctype_base::punct, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == L'.' );
  }

  //scan_not
  {
    wchar_t range[] = L"abAc123 .";
    const wchar_t *rbeg = range;
    const wchar_t *rend = range + sizeof(range);

    const wchar_t *res;
    res = wct.scan_not((ctype_base::mask)(ctype_base::alpha | ctype_base::lower), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == L'1' );

    res = wct.scan_not(ctype_base::alpha, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == L'1' );

    res = wct.scan_not(ctype_base::punct, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == L'a' );
  }

  //toupper
  {
    CPPUNIT_ASSERT( wct.toupper(L'a') == L'A' );
    CPPUNIT_ASSERT( wct.toupper(L'A') == L'A' );
    CPPUNIT_ASSERT( wct.toupper(L'1') == L'1' );
  }

  //toupper range
  {
    wchar_t range[] = L"abAc1";
    wchar_t expected_range[] = L"ABAC1";
    wct.toupper(range, range + sizeof(range) / sizeof(wchar_t));
    CPPUNIT_ASSERT( equal(range, range + sizeof(range) / sizeof(wchar_t), expected_range) );
  }

  //tolower
  {
    CPPUNIT_ASSERT( wct.tolower(L'A') == L'a' );
    CPPUNIT_ASSERT( wct.tolower(L'a') == L'a' );
    CPPUNIT_ASSERT( wct.tolower(L'1') == L'1' );
  }

  //tolower range
  {
    wchar_t range[] = L"ABaC1";
    wchar_t expected_range[] = L"abac1";
    wct.tolower(range, range + sizeof(range) / sizeof(wchar_t));
    CPPUNIT_ASSERT( equal(range, range + sizeof(range) / sizeof(wchar_t), expected_range) );
  }

  //widen
  {
    CPPUNIT_ASSERT( wct.widen('a') == L'a' );
  }

  //widen range
  {
    char range[] = "ABaC1";
    wchar_t res[sizeof(range)];
    wchar_t expected_res[] = L"ABaC1";
    wct.widen(range, range + sizeof(range), res);
    CPPUNIT_ASSERT( equal(expected_res, expected_res + sizeof(range), res) );
  }

  //narrow
  {
    CPPUNIT_ASSERT( wct.narrow(L'a', 'b') == L'a' );
  }

  //narrow range
  {
    wchar_t range[] = L"ABaC1";
    char res[sizeof(range)];
    char expected_res[] = "ABaC1";
    wct.narrow(range, range + sizeof(range), 'b', res);
    CPPUNIT_ASSERT( equal(expected_res, expected_res + sizeof(range) / sizeof(wchar_t), res) );
  }
# endif
}


template <class _Tp>
void test_supported_locale(LocaleTest inst, _Tp __test) {
  size_t n = sizeof(tested_locales) / sizeof(tested_locales[0]);
  for (size_t i = 0; i < n; ++i) {
    auto_ptr<locale> loc;
#if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
    try {
      loc.reset(new locale(tested_locales[i].name));
    }
    catch (runtime_error const&) {
      //This locale is not supported.
      continue;
    }
#else
    //Without exception support we only test C locale.
    if (tested_locales[i].name[0] != 'C' ||
        tested_locales[i].name[1] != 0)
      continue;
    loc.reset(new locale(tested_locales[i].name));
#endif
    CPPUNIT_MESSAGE( loc->name().c_str() );
    (inst.*__test)(*loc, tested_locales[i]);
  }
}

void LocaleTest::locale_by_name() {
#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  /*
   * Check of the 22.1.1.2.7 standard point. Construction of a locale
   * instance from a null pointer or an unknown name should result in
   * a runtime_error exception.
   */
  try {
    locale loc(static_cast<char const*>(0));
    CPPUNIT_ASSERT( false );
  }
  catch (runtime_error const&) {
  }
  catch (...) {
    CPPUNIT_ASSERT( false );
  }

  try {
    locale loc("yasli_language");
    CPPUNIT_ASSERT( false );
  }
  catch (runtime_error const&) {
  }
  catch (...) {
    CPPUNIT_ASSERT( false );
  }
#  endif
}

void LocaleTest::loc_has_facet() {
  locale loc("C");
  typedef numpunct<char> implemented_facet;
  CPPUNIT_ASSERT( has_facet<implemented_facet>(loc) );
  /*
  typedef num_put<char, back_insert_iterator<string> > not_implemented_facet;
  CPPUNIT_ASSERT( !has_facet<not_implemented_facet>(loc) );
  */
}

void LocaleTest::num_put_get()
{ test_supported_locale(*this, &LocaleTest::_num_put_get); }

void LocaleTest::money_put_get()
{ test_supported_locale(*this, &LocaleTest::_money_put_get); }

void LocaleTest::money_put_X_bug()
{ test_supported_locale(*this, &LocaleTest::_money_put_X_bug); }

void LocaleTest::time_put_get()
{ test_supported_locale(*this, &LocaleTest::_time_put_get); }

void LocaleTest::collate_facet()
{ test_supported_locale(*this, &LocaleTest::_collate_facet); }

void LocaleTest::ctype_facet()
{
  test_supported_locale(*this, &LocaleTest::_ctype_facet);
#ifndef _STLP_NO_WCHAR_T
  test_supported_locale(*this, &LocaleTest::_ctype_facet_w);
#endif
}

void LocaleTest::locale_init_problem() {
#if !defined (STLPORT) || !defined (_STLP_NO_MEMBER_TEMPLATES)
  test_supported_locale(*this, &LocaleTest::_locale_init_problem);
#endif
}


/*
 * Creation of a locale instance imply initialization of some STLport internal
 * static objects first. We use a static instance of locale to check that this
 * initialization is done correctly.
 */
static locale global_loc;
static locale other_loc("");

#if !defined (STLPORT) || !defined (_STLP_NO_MEMBER_TEMPLATES)
void LocaleTest::_locale_init_problem( const locale& loc, const ref_locale&)
{
#  if !defined (__APPLE__) && !defined (__FreeBSD__) || \
      !defined(__GNUC__) || ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__> 3)))
  typedef codecvt<char,char,mbstate_t> my_facet;
#  else
// std::mbstate_t required for gcc 3.3.2 on FreeBSD...
// I am not sure what key here---FreeBSD or 3.3.2...
//      - ptr 2005-04-04
  typedef codecvt<char,char,std::mbstate_t> my_facet;
#  endif

  locale loc_ref(global_loc);
  {
    locale gloc( loc_ref, new my_facet() );
    CPPUNIT_ASSERT( has_facet<my_facet>( gloc ) );
    //The following code is just here to try to confuse the reference counting underlying mecanism:
    locale::global( locale::classic() );
    locale::global( gloc );
  }

#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  try {
#  endif
    ostringstream os("test") ;
    locale loc2( loc, new my_facet() );
    CPPUNIT_ASSERT( has_facet<my_facet>( loc2 ) );
    os.imbue( loc2 );
#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  }
  catch ( runtime_error& ) {
    CPPUNIT_ASSERT( false );
  }
  catch ( ... ) {
   CPPUNIT_ASSERT( false );
  }
#  endif

#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  try {
#  endif
    ostringstream os2("test2");
#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  }
  catch ( runtime_error& ) {
    CPPUNIT_ASSERT( false );
  }
  catch ( ... ) {
    CPPUNIT_ASSERT( false );
  }
#  endif
}
#endif

void LocaleTest::default_locale()
{
  locale loc( "" );
}

void LocaleTest::combine()
{
#if !defined (STLPORT) || \
   (defined (_STLP_USE_EXCEPTIONS) && !defined (_STLP_NO_MEMBER_TEMPLATES) && !defined (_STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS)) && \
   (!defined (_MSC_VER) || (_MSC_VER >= 1300))
  auto_ptr<locale> loc1, loc2;
  size_t loc1_index = 0;
  size_t n = sizeof(tested_locales) / sizeof(tested_locales[0]);
  for (size_t i = 0; i < n; ++i) {
    try {
      locale *ploc = new locale(tested_locales[i].name);
      if (loc1.get() == 0)
      {
        loc1.reset(ploc);
        loc1_index = i;
        continue;
      }
      else
      {
        loc2.reset(ploc);
      }

      //We can start the test
      ostringstream ostr;
      ostr << "combining '" << loc2->name() << "' money facets with '" << loc1->name() << "'";
      CPPUNIT_MESSAGE( ostr.str().c_str() );

      //We are going to combine money facets as all formats are different.
      {
        //We check that resulting locale has correctly acquire loc2 facets.
        locale loc = loc1->combine<moneypunct<char, true> >(*loc2);
        loc = loc.combine<moneypunct<char, false> >(*loc2);
        loc = loc.combine<money_put<char> >(*loc2);
        loc = loc.combine<money_get<char> >(*loc2);

        //Check loc has the correct facets:
        _money_put_get2(*loc2, loc, tested_locales[i]);

        //Check loc1 has not been impacted:
        _money_put_get2(*loc1, *loc1, tested_locales[loc1_index]);

        //Check loc2 has not been impacted:
        _money_put_get2(*loc2, *loc2, tested_locales[i]);
      }
      {
        //We check that resulting locale has not wrongly acquire loc1 facets that hasn't been combine:
        locale loc = loc2->combine<numpunct<char> >(*loc1);
        loc = loc.combine<time_put<char> >(*loc1);
        loc = loc.combine<time_get<char> >(*loc1);

        //Check loc has the correct facets:
        _money_put_get2(*loc2, loc, tested_locales[i]);

        //Check loc1 has not been impacted:
        _money_put_get2(*loc1, *loc1, tested_locales[loc1_index]);

        //Check loc2 has not been impacted:
        _money_put_get2(*loc2, *loc2, tested_locales[i]);
      }

      {
        // Check auto combination do not result in weird reference counting behavior 
        // (might generate a crash).
        loc1->combine<numpunct<char> >(*loc1);
      }

      loc1.reset(loc2.release());
      loc1_index = i;
    }
    catch (runtime_error const&) {
      //This locale is not supported.
      continue;
    }
  }
#endif
}

#endif
