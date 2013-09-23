// IPreprocessor.h

#ifndef _PPBOX_PPBOX_DETAIL_I_PREPROCESSOR_H_
#define _PPBOX_PPBOX_DETAIL_I_PREPROCESSOR_H_

#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/comma_if.hpp>

#define FOMART(r, f, i, e) f(i, e)
#define LIST_FORMAT(f, n, items) BOOST_PP_SEQ_FOR_EACH_I(FOMART, f, BOOST_PP_TUPLE_TO_SEQ(n, items))

#define PAIR_FOMART(r, f, i, e) f(i, BOOST_PP_TUPLE_ELEM(2, 0, e), BOOST_PP_TUPLE_ELEM(2, 1, e))
#define LIST_PAIR_FORMAT(f, n, items) BOOST_PP_SEQ_FOR_EACH_I(PAIR_FOMART, f, BOOST_PP_TUPLE_TO_SEQ(n, items))

#define PAIR_DATA_FOMART(r, f_d, i, e) \
    BOOST_PP_TUPLE_ELEM(2, 0, f_d)(i, BOOST_PP_TUPLE_ELEM(2, 1, f_d), BOOST_PP_TUPLE_ELEM(2, 0, e), BOOST_PP_TUPLE_ELEM(2, 1, e))
#define LIST_DATA_PAIR_FORMAT(f, d, n, items) BOOST_PP_SEQ_FOR_EACH_I(PAIR_DATA_FOMART, (f, d), BOOST_PP_TUPLE_TO_SEQ(n, items))

#endif // _PPBOX_PPBOX_DETAIL_I_PREPROCESSOR_H_