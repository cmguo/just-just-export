// IStruct.h

#ifndef _PPBOX_PPBOX_DETAIL_I_STRUCT_H_
#define _PPBOX_PPBOX_DETAIL_I_STRUCT_H_

#include "ppbox/ppbox/detail/IPreprocessor.h"

#define STRUCT_FIELD_TYPE_NAME(i, t, n) t n;

#define STRUCT_FIELDS_TYPE_NAME(np, params) LIST_PAIR_FORMAT(STRUCT_FIELD_TYPE_NAME, np, params)

#define PPBOX_STRUCT_0(name, fields) PPBOX_STRUCT(name, 0, fields)
#define PPBOX_STRUCT_1(name, fields) PPBOX_STRUCT(name, 1, fields)
#define PPBOX_STRUCT_2(name, fields) PPBOX_STRUCT(name, 2, fields)
#define PPBOX_STRUCT_3(name, fields) PPBOX_STRUCT(name, 3, fields)
#define PPBOX_STRUCT_4(name, fields) PPBOX_STRUCT(name, 4, fields)
#define PPBOX_STRUCT_5(name, fields) PPBOX_STRUCT(name, 5, fields)
#define PPBOX_STRUCT_6(name, fields) PPBOX_STRUCT(name, 6, fields)
#define PPBOX_STRUCT_7(name, fields) PPBOX_STRUCT(name, 7, fields)
#define PPBOX_STRUCT_8(name, fields) PPBOX_STRUCT(name, 8, fields)
#define PPBOX_STRUCT_9(name, fields) PPBOX_STRUCT(name, 9, fields)
#define PPBOX_STRUCT_10(name, fields) PPBOX_STRUCT(name, 10, fields)
#define PPBOX_STRUCT_11(name, fields) PPBOX_STRUCT(name, 11, fields)
#define PPBOX_STRUCT_12(name, fields) PPBOX_STRUCT(name, 12, fields)

#ifndef PPBOX_STRUCT
#  define PPBOX_STRUCT(name, nf, fields) typedef struct name { STRUCT_FIELDS_TYPE_NAME(nf, fields) } name
#endif

#endif // _PPBOX_PPBOX_DETAIL_I_STRUCT_H_
