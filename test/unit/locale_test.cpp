#include <string>
#if !defined (STLPORT) || !defined (_STLP_USE_NO_IOSTREAMS)
#  include <sstream>
#  include <locale>

#  include <cstdio>

#  if defined (__unix) || defined (__unix__)
#    include <sys/types.h>
#    include <sys/stat.h>
#    include <dirent.h>
#    include <unistd.h>
#    include <cstring>
#    include <iostream>
#  elif defined (WIN32) && !defined (_STLP_WCE)
#    include <windows.h>
#  endif

#  include <set>

#  include "cppunit/cppunit_proxy.h"

#  if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#  endif


class LColl {
public:
  LColl( const char * );

  bool operator[](const string& name) {
    return supported_locales.find(name) != supported_locales.end();
  }

private:
  set<string> supported_locales;
};

#if defined (WIN32) && !defined (_STLP_WCE)
set<string> *psupported_locales = 0;
BOOL CALLBACK EnumLocalesProc(LPTSTR locale_name) {
  LCID lcid;
  {
    istringstream istr(locale_name);
    istr >> hex >> lcid;
    if (!istr)
      return TRUE;
  }

  char lang[128], ctry[128];
  GetLocaleInfoA(lcid, LOCALE_SISO639LANGNAME, lang, sizeof(lang));
  GetLocaleInfoA(lcid, LOCALE_SISO3166CTRYNAME, ctry, sizeof(ctry));

  string localeName = string(lang) + '_' + string(ctry);
  psupported_locales->insert(localeName);
  return TRUE;
}
#endif

LColl::LColl( const char *loc_dir )
{
#  if (defined(__unix) || defined(__unix__)) && !defined(__CYGWIN__)
  /* Iterate through catalog that contain catalogs with locale definitions, installed on system
   * (this is expected /usr/lib/locale for most linuxes and /usr/share/locale for *BSD).
   * The names of catalogs here will give supported locales.
   */
  string base(loc_dir);
  DIR *d = opendir(base.c_str());
  struct dirent *ent;
  while ((ent = readdir( d )) != 0) {
    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
      continue;
    }
    string f = base;
    f += '/';
    f += ent->d_name;
    struct stat s;
    stat( f.c_str(), &s );
    if (S_ISDIR(s.st_mode)) {
      supported_locales.insert(ent->d_name);
      // cout << ent->d_name << endl;
    }
  }
  closedir( d );
#  else
#    ifdef _MSC_VER
  //Avoids warning:
  (void*)loc_dir;
#    endif
#  endif
#  if defined (WIN32) && !defined (_STLP_WCE)
  psupported_locales = &supported_locales;
  EnumSystemLocales(EnumLocalesProc, LCID_INSTALLED);
  psupported_locales = 0;
#  endif
  // std::locale must at least support the C locale
  supported_locales.insert("C");
}

#  if !defined(__GNUC__) || (__GNUC__ > 2)
#    if defined(__FreeBSD__) || defined(__OpenBSD__)
static LColl loc_ent( "/usr/share/locale" );
#    else
static LColl loc_ent( "/usr/lib/locale" );
#    endif
#  endif // !__GNUC__ || __GNUC__ > 2

struct ref_locale {
  const char *name;
  const char *decimal_point;
  const char *thousands_sep;
  const char *money_int_prefix;
  const char *money_prefix;
  const char *money_int_suffix;
  const char *money_int_suffix_old;
  const char *money_suffix;
  const char *money_decimal_point;
  const char *money_thousands_sep;
};


// Pls, don't write #ifdef _STLP_REAL_LOCALE_IMPLEMENTED here!
// It undefined in any case!!!!!

static ref_locale tested_locales[] = {
//{  name,         decimal_point, thousands_sep, money_int_prefix, money_prefix, money_int_suffix, money_int_suffix_old, money_suffix, money_decimal_point,  money_thousands_sep},
  { "fr_FR",       ",",           "\xa0",        "",               "",           " EUR",            " FRF",                "",           ",",                  "\xa0" },
  { "ru_RU.koi8r", ",",           ".",           "",               "",           "RUR ",            "",                   "",           ".",                  " " },
  { "en_GB",       ".",           ",",           "GBP ",            "\xa3",       "",               "",                   "",           ".",                  "," },
  { "en_US",       ".",           ",",           "USD ",            "$",          "",               "",                   "",           ".",                  "," },
  { "C",           ".",           ",",           "",               "",           "",               "",                   "",           " ",                  " " },
};


//
// TestCase class
//
class LocaleTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(LocaleTest);
  CPPUNIT_TEST(locale_by_name);
  CPPUNIT_TEST(loc_has_facet);
  CPPUNIT_TEST(num_put_get);
  CPPUNIT_TEST(money_put_get);
  CPPUNIT_TEST(time_put_get);
  CPPUNIT_TEST(collate_facet);
  CPPUNIT_TEST(ctype_facet);
  CPPUNIT_TEST(locale_init_problem);
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
private:
  void _loc_has_facet( const locale&, const ref_locale& );
  void _num_put_get( const locale&, const ref_locale& );
  void _money_put_get( const locale&, const ref_locale& );
  void _time_put_get( const locale&, const ref_locale& );
  void _collate_facet( const locale&, const ref_locale& );
  void _ctype_facet( const locale&, const ref_locale& );
  void _locale_init_problem( const locale&, const ref_locale& );
};

CPPUNIT_TEST_SUITE_REGISTRATION(LocaleTest);

//
// tests implementation
//
void LocaleTest::_num_put_get( const locale& loc, const ref_locale& rl ) {
  float val = 1234.56f;
  ostringstream ostr;
  ostr << val;
  CPPUNIT_ASSERT( ostr );
  //CPPUNIT_TEST( ostr.str() == "1234.56" );
  CPPUNIT_ASSERT( ostr.str() == "1234.56" );

  numpunct<char> const& npct = use_facet<numpunct<char> >(loc);

  CPPUNIT_ASSERT( npct.decimal_point() == *rl.decimal_point );
  CPPUNIT_ASSERT( npct.thousands_sep() == *rl.thousands_sep );

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
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 12345678.9f;
  ostr.str("");
  ostr << val;
  CPPUNIT_ASSERT( ostr.str() == "1.23457e+07" );

  ref = "1";
  ref += npct.decimal_point();
  ref += "23457e+07";
  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 1000000000.0f;
  ostr.str("");
  ostr << val;
  CPPUNIT_ASSERT( ostr.str() == "1e+09" );

  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == "1e+09" );
}

void LocaleTest::_money_put_get( const locale& loc, const ref_locale& rl )
{
  money_put<char> const& fmp = use_facet<money_put<char> >(loc);
  money_get<char> const& fmg = use_facet<money_get<char> >(loc);
  moneypunct<char, true> const& intl_fmp = use_facet<moneypunct<char, true> >(loc);

  ostringstream ostr;
  ostr.imbue(loc);
  ostr << showbase;

  //Check a positive value
  {
    string str_res;
    //money_put
    {
      ostreambuf_iterator<char> res = fmp.put(ostr, true, ostr, ' ', 123456);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      //On a positive value we skip the sign field if exists:
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }
      // iternational currency abbreviation, if it before value

      /*
       * int_curr_symbol
       *
       *   The international currency symbol. The operand is a four-character
       *   string, with the first three characters containing the alphabetic
       *   international currency symbol in accordance with those specified
       *   in the . The fourth character is the character used to separate
       *   the international currency symbol from the monetary quantity.
       *
       * (http://www.opengroup.org/onlinepubs/7990989775/xbd/locale.html)
       */
      string::size_type p = strlen( rl.money_int_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( intl_fmp.pos_format().field[fieldIndex] == money_base::symbol );
        CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_int_prefix );
        index += p;
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

      // iternational currency abbreviation, if it sacceeds value

      // Oh, RUR typed wrong here, but test pass this:
      // it print '1 234.56 RUR ' (see space after RUR)
      // This is due to intl_fmp.curr_symbol() == "RUR ".
      // Sources I see isn't clear say about the format...

      p = strlen( rl.money_int_suffix );
      string::size_type p_old = strlen( rl.money_int_suffix_old );
      if ( p != 0 || p_old != 0 ) {
        CPPUNIT_ASSERT( intl_fmp.pos_format().field[fieldIndex] == money_base::symbol );
        if ( p != 0 && p_old != 0 ) {
          CPPUNIT_ASSERT( (str_res.substr(index, p) == rl.money_int_suffix) || (str_res.substr(index, p_old) ==  rl.money_int_suffix_old) );
          if ( str_res.substr(index, p) == rl.money_int_suffix ) {
            index += p;
          } else {
            index += p_old;
          }
        } else if ( p != 0 ) {
          CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_int_suffix );
          index += p;
        } else {
          CPPUNIT_ASSERT( str_res.substr(index, p_old) ==  rl.money_int_suffix_old );
          index += p_old;
        }
        ++fieldIndex;
      }

      // sign
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }

      //space cannot be last:
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
      fmg.get(istr, istreambuf_iterator<char>(), true, ostr, err, digits);
      CPPUNIT_ASSERT( (err & (ios_base::failbit | ios_base::badbit)) == 0 );
      CPPUNIT_ASSERT( digits == "123456" );
    }
  }

  ostr.str("");
  //Check a negative value:
  {
    moneypunct<char, false> const& dom_fmp = use_facet<moneypunct<char, false> >(loc);
    string str_res;
    //Check money_put
    {
      ostreambuf_iterator<char> res = fmp.put(ostr, false, ostr, ' ', -123456);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      if (intl_fmp.neg_format().field[fieldIndex] == money_base::sign) {
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
      if (intl_fmp.neg_format().field[fieldIndex] == money_base::space ||
          intl_fmp.neg_format().field[fieldIndex] == money_base::none) {
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
          intl_fmp.neg_format().field[fieldIndex] == money_base::space) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }
      if (fieldIndex == 3) {
        //If none is last we should not add anything to the resulting string:
        if (intl_fmp.neg_format().field[fieldIndex] == money_base::none) {
          CPPUNIT_ASSERT( index == str_res.size() );
        }
        else {
          CPPUNIT_ASSERT( str_res.substr(index, strlen(rl.money_suffix)) == rl.money_suffix );
        }
      }
    }

    //money_get
    {
      ios_base::iostate err = ios_base::goodbit;
#if defined (STLPORT)
      _STLP_LONG_DOUBLE val;
#else
      long double val;
#endif

      istringstream istr(str_res);
      fmg.get(istr, istreambuf_iterator<char>(), false, ostr, err, val);
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

void LocaleTest::_time_put_get( const locale& loc, const ref_locale&)
{
  time_put<char> const&tmp = use_facet<time_put<char> >(loc);
  //time_get<char> const&tmg = use_facet<time_get<char> >(loc);

  struct tm xmas = { 0, 0, 12, 25, 11, 93 };
  ostringstream ostr;
  ostr.imbue(loc);
  string format = "%a %d %b %y";

  time_put<char>::iter_type ret = tmp.put(ostr, ostr, ' ', &xmas, format.data(), format.data() + format.size());
  CPPUNIT_ASSERT( !ret.failed() );

  string str_ret = ostr.str();
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

  //is
  {
    CPPUNIT_ASSERT( ct.is(ctype_base::digit, '0') );
    CPPUNIT_ASSERT( ct.is(ctype_base::upper, 'A') );
    CPPUNIT_ASSERT( ct.is(ctype_base::lower, 'a') );
    CPPUNIT_ASSERT( ct.is(ctype_base::alpha, 'A') );
    CPPUNIT_ASSERT( ct.is(ctype_base::space, ' ') );
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

    res = ct.scan_is((ctype_base::mask)(ctype_base::alpha | ctype_base::upper), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'A' );

    res = ct.scan_is(ctype_base::punct, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == '.' );

    res = ct.scan_is((ctype_base::mask)(ctype_base::punct | ctype_base::digit), rbeg, rend);
    CPPUNIT_ASSERT( res == rend );
  }

  //scan_not
  {
    char range[] = "abAc123 .";
    const char *rbeg = range;
    const char *rend = range + sizeof(range);

    const char *res;
    res = ct.scan_not((ctype_base::mask)(ctype_base::alpha | ctype_base::lower), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'A' );

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

template <class _Tp>
void test_supported_locale(LocaleTest inst, _Tp __test) {
#  if defined(__GNUC__) && (__GNUC__ < 3) // workaround for gcc 2.95.x
#    if defined(__FreeBSD__) || defined(__OpenBSD__)
   LColl loc_ent( "/usr/share/locale" );
#    else
   LColl loc_ent( "/usr/lib/locale" );
#    endif
#  endif // __GNUC__ || __GNUC__ < 3

  int n = sizeof(tested_locales) / sizeof(tested_locales[0]);
  for ( int i = 0; i < n; ++i ) {
    if ( loc_ent[string( tested_locales[i].name )] ) {
      // cout << '\t' << tested_locales[i].name << endl;
      locale loc( tested_locales[i].name );
      CPPUNIT_MESSAGE( loc.name().c_str() );
      (inst.*__test)(loc, tested_locales[i] );
    }
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

void LocaleTest::num_put_get() {
  test_supported_locale(*this, &LocaleTest::_num_put_get);
}

void LocaleTest::money_put_get() {
  test_supported_locale(*this, &LocaleTest::_money_put_get);
}

void LocaleTest::time_put_get() {
  test_supported_locale(*this, &LocaleTest::_time_put_get);
}

void LocaleTest::collate_facet() {
  test_supported_locale(*this, &LocaleTest::_collate_facet);
}

void LocaleTest::ctype_facet() {
  test_supported_locale(*this, &LocaleTest::_ctype_facet);
}

void LocaleTest::locale_init_problem()
{
  test_supported_locale(*this, &LocaleTest::_locale_init_problem);
}

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

  locale loc_ref;
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
    // cerr << "-- " << err.what() << endl;
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
