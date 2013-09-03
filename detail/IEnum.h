// IEnum.h

#ifndef _PPBOX_PPBOX_DETAIL_I_ENUM_H_
#define _PPBOX_PPBOX_DETAIL_I_ENUM_H_

#include "ppbox/ppbox/detail/IPreprocessor.h"

#define ENUM_NAME_VALUE(i, n, v) n = v,
#define ENUMS_NAME_VALUE(ne, enums) LIST_PAIR_FORMAT(ENUM_NAME_VALUE, ne, enums)

#define ENUM_PREFIX_NAME_VALUE(i, p, n, v) BOOST_PP_CAT(BOOST_PP_CAT(p, _), n) = v,
#define ENUMS_PREFIX_NAME_VALUE(p, ne, enums) LIST_DATA_PAIR_FORMAT(ENUM_PREFIX_NAME_VALUE, p, ne, enums)

#define PPBOX_ENUM_0(name, enums) PPBOX_ENUM(name, 0, enums)
#define PPBOX_ENUM_1(name, enums) PPBOX_ENUM(name, 1, enums)
#define PPBOX_ENUM_2(name, enums) PPBOX_ENUM(name, 2, enums)
#define PPBOX_ENUM_3(name, enums) PPBOX_ENUM(name, 3, enums)
#define PPBOX_ENUM_4(name, enums) PPBOX_ENUM(name, 4, enums)
#define PPBOX_ENUM_5(name, enums) PPBOX_ENUM(name, 5, enums)
#define PPBOX_ENUM_6(name, enums) PPBOX_ENUM(name, 6, enums)
#define PPBOX_ENUM_7(name, enums) PPBOX_ENUM(name, 7, enums)
#define PPBOX_ENUM_8(name, enums) PPBOX_ENUM(name, 8, enums)
#define PPBOX_ENUM_9(name, enums) PPBOX_ENUM(name, 9, enums)
#define PPBOX_ENUM_10(name, enums) PPBOX_ENUM(name, 10, enums)
#define PPBOX_ENUM_11(name, enums) PPBOX_ENUM(name, 11, enums)

#ifndef PPBOX_ENUM
#  ifdef __cplusplus
#    define PPBOX_ENUM(name, ne, enums) typedef struct name { enum Enum { ENUMS_NAME_VALUE(ne, enums) }; } name
#  else
#    define PPBOX_ENUM(name, ne, enums) typedef enum name ## _ ## Enum { ENUMS_PREFIX_NAME_VALUE(name, ne, enums) } name ## _ ## Enum
#  endif
#endif

#endif // _PPBOX_PPBOX_DETAIL_I_ENUM_H_
