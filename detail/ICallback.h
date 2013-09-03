// ICallback.h

#ifndef _PPBOX_PPBOX_DETAIL_I_CALLBACK_H_
#define _PPBOX_PPBOX_DETAIL_I_CALLBACK_H_

#define PPBOX_CALLBACK_0(type, name, fields) PPBOX_CALLBACK(type, name, 0, fields)
#define PPBOX_CALLBACK_1(type, name, fields) PPBOX_CALLBACK(type, name, 1, fields)
#define PPBOX_CALLBACK_2(type, name, fields) PPBOX_CALLBACK(type, name, 2, fields)
#define PPBOX_CALLBACK_3(type, name, fields) PPBOX_CALLBACK(type, name, 3, fields)
#define PPBOX_CALLBACK_4(type, name, fields) PPBOX_CALLBACK(type, name, 4, fields)
#define PPBOX_CALLBACK_5(type, name, fields) PPBOX_CALLBACK(type, name, 5, fields)
#define PPBOX_CALLBACK_6(type, name, fields) PPBOX_CALLBACK(type, name, 6, fields)
#define PPBOX_CALLBACK_7(type, name, fields) PPBOX_CALLBACK(type, name, 7, fields)

#ifndef PPBOX_CALLBACK
#  define PPBOX_CALLBACK(type, name, np, params) typedef type (*name)params
#endif

#endif // _PPBOX_PPBOX_DETAIL_I_CALLBACK_H_
