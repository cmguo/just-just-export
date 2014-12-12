// IStruct.h

#ifndef _JUST_EXPORT_DETAIL_I_STRUCT_H_
#define _JUST_EXPORT_DETAIL_I_STRUCT_H_

#include "just/export/detail/IPreprocessor.h"

#define STRUCT_FIELD_TYPE_NAME(i, t, n) t n;

#define STRUCT_FIELDS_TYPE_NAME(np, params) LIST_PAIR_FORMAT(STRUCT_FIELD_TYPE_NAME, np, params)

#define JUST_STRUCT_0(name, fields) JUST_STRUCT(name, 0, fields)
#define JUST_STRUCT_1(name, fields) JUST_STRUCT(name, 1, fields)
#define JUST_STRUCT_2(name, fields) JUST_STRUCT(name, 2, fields)
#define JUST_STRUCT_3(name, fields) JUST_STRUCT(name, 3, fields)
#define JUST_STRUCT_4(name, fields) JUST_STRUCT(name, 4, fields)
#define JUST_STRUCT_5(name, fields) JUST_STRUCT(name, 5, fields)
#define JUST_STRUCT_6(name, fields) JUST_STRUCT(name, 6, fields)
#define JUST_STRUCT_7(name, fields) JUST_STRUCT(name, 7, fields)
#define JUST_STRUCT_8(name, fields) JUST_STRUCT(name, 8, fields)
#define JUST_STRUCT_9(name, fields) JUST_STRUCT(name, 9, fields)
#define JUST_STRUCT_10(name, fields) JUST_STRUCT(name, 10, fields)
#define JUST_STRUCT_11(name, fields) JUST_STRUCT(name, 11, fields)
#define JUST_STRUCT_12(name, fields) JUST_STRUCT(name, 12, fields)

#ifndef JUST_STRUCT
#  define JUST_STRUCT(name, nf, fields) typedef struct name { STRUCT_FIELDS_TYPE_NAME(nf, fields) } name
#endif

#endif // _JUST_EXPORT_DETAIL_I_STRUCT_H_
