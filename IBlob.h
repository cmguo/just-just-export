// ICallback.h

#ifndef _PPBOX_PPBOX_I_CALLBACK_H_
#define _PPBOX_PPBOX_I_CALLBACK_H_

#include "IPpboxTypes.h"
#include "IFormat.h"

PPBOX_FUNC_3(PP_err, PPBOX_InsertBlob, (
    (PP_str, key), 
    (PPBOX_ConstBuffer  const *, blob), 
    (PP_bool, remove_on_get)));

PPBOX_FUNC_2(PP_err, PPBOX_GetBlob, (
    (PP_str, key), 
    (PPBOX_ConstBuffer  *, blob)));

PPBOX_FUNC_1(PP_err, PPBOX_RemoveBlob, 
    (PP_str, key)));

#endif // _PPBOX_PPBOX_I_CALLBACK_H_
