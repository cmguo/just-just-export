// ICallback.h

#ifndef _JUST_JUST_DETAIL_I_CALLBACK_H_
#define _JUST_JUST_DETAIL_I_CALLBACK_H_

#define JUST_CALLBACK_0(type, name, fields) JUST_CALLBACK(type, name, 0, fields)
#define JUST_CALLBACK_1(type, name, fields) JUST_CALLBACK(type, name, 1, fields)
#define JUST_CALLBACK_2(type, name, fields) JUST_CALLBACK(type, name, 2, fields)
#define JUST_CALLBACK_3(type, name, fields) JUST_CALLBACK(type, name, 3, fields)
#define JUST_CALLBACK_4(type, name, fields) JUST_CALLBACK(type, name, 4, fields)
#define JUST_CALLBACK_5(type, name, fields) JUST_CALLBACK(type, name, 5, fields)
#define JUST_CALLBACK_6(type, name, fields) JUST_CALLBACK(type, name, 6, fields)
#define JUST_CALLBACK_7(type, name, fields) JUST_CALLBACK(type, name, 7, fields)

#ifndef JUST_CALLBACK
#  define JUST_CALLBACK(type, name, np, params) typedef type (*name)params
#endif

#endif // _JUST_JUST_DETAIL_I_CALLBACK_H_
