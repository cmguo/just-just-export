// IBlob.h

#ifndef _JUST_EXPORT_I_BLOB_H_
#define _JUST_EXPORT_I_BLOB_H_

#include "IJustTypes.h"
#include "IFormat.h"

JUST_FUNC_3(PP_err, JUST_InsertBlob, (
    (PP_str, key), 
    (JUST_ConstBuffer  const *, blob), 
    (PP_bool, remove_on_get)));

JUST_FUNC_2(PP_err, JUST_GetBlob, (
    (PP_str, key), 
    (JUST_ConstBuffer  *, blob)));

JUST_FUNC_1(PP_err, JUST_RemoveBlob, (
    (PP_str, key)));

#endif // _JUST_EXPORT_I_BLOB_H_
