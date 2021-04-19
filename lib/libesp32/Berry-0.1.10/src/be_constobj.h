/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_CONSTOBJ_H
#define BE_CONSTOBJ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "be_object.h"
#include "be_gc.h"
#include "be_map.h"
#include "be_class.h"
#include "be_string.h"
#include "be_module.h"

#ifndef __cplusplus

#define be_const_header(_t)                                     \
    .next = NULL,                                               \
    .type = (_t),                                               \
    .marked = GC_CONST

#define be_const_key(_str, _next) {                             \
    .v.c = &be_const_str_##_str,                                \
    .type = BE_STRING,                                          \
    .next = (uint32_t)(_next) & 0xFFFFFF                        \
}

#define be_const_func(_func) {                                  \
    .v.nf = (_func),                                            \
    .type = BE_FUNCTION                                         \
}

#define be_const_int(_val) {                                    \
    .v.i = (bint)(_val),                                        \
    .type = BE_INT                                              \
}

#define be_const_real(_val) {                                   \
    .v.r = (breal)(_val),                                       \
    .type = BE_REAL                                             \
}

#define be_const_class(_class) {                                \
    .v.c = &(_class),                                           \
    .type = BE_CLASS                                            \
}

#define be_const_module(_module) {                              \
    .v.c = &(_module),                                          \
    .type = BE_MODULE                                           \
}

#define be_define_const_map_slots(_name)                        \
const bmapnode _name##_slots[] =

#define be_define_const_map(_name, _size)                       \
const bmap _name = {                                            \
    be_const_header(BE_MAP),                                    \
    .slots = (bmapnode*)_name##_slots,                          \
    .lastfree = NULL,                                           \
    .size = _size,                                              \
    .count = _size                                              \
}

#define be_define_const_class(_name, _nvar, _super, _name_)     \
const bclass _name = {                                          \
    be_const_header(BE_CLASS),                                  \
    .nvar = _nvar,                                              \
    .super = _super,                                            \
    .members = (bmap*)&_name##_map,                             \
    .name = (bstring*)&be_const_str_##_name_                    \
}

#define be_define_const_module(_name, _name_)                   \
const bmodule _name = {                                         \
    be_const_header(BE_MODULE),                                 \
    .table = (bmap*)&_name##_map,                               \
    .info.name = _name_,                                        \
}

#define be_define_const_vector(_name, _data, _size)             \
const bvector _name = {                                         \
    .capacity = _size,                                          \
    .size = sizeof(bvalue),                                     \
    .count = _size,                                             \
    .data = (void*)_data,                                       \
    .end = (void*)(_data + (_size) - 1)                         \
}

#define be_define_const_native_module(_module, _init)           \
const bntvmodule be_native_module(_module) = {                  \
    .name = #_module,                                           \
    .attrs = NULL,                                              \
    .size = 0,                                                  \
    .module = (bmodule*)&(m_lib##_module),                      \
    .init = _init                                               \
}

#else

#define be_const_key(_str, _next) {                             \
    bvaldata(&be_const_str_##_str),                             \
        BE_STRING,                                              \
        uint32_t((_next)&0xFFFFFF)                              \
}

#define be_const_func(_func) {                                  \
    bvaldata(_func),                                            \
    BE_FUNCTION                                                 \
}

#define be_const_int(_val) {                                    \
    bvaldata(bint(_val)),                                       \
    BE_INT                                                      \
}

#define be_const_real(_val) {                                   \
    bvaldata(breal(_val)),                                      \
    BE_REAL                                                     \
}

#define be_const_class(_class) {                                \
    bvaldata(&(_class)),                                        \
    BE_CLASS                                                    \
}

#define be_const_module(_module) {                              \
    bvaldata(&(_module)),                                       \
    BE_MODULE                                                   \
}

#define be_define_const_map_slots(_name)                        \
const bmapnode _name##_slots[] =

#define be_define_const_map(_name, _size)                       \
const bmap _name(                                               \
    (bmapnode*)_name##_slots, _size                             \
)

#define be_define_const_class(_name, _nvar, _super, _name_)     \
const bclass _name(                                             \
    _nvar, _super, (bmap*)&_name##_map,                         \
    (bstring*)&be_const_str_##_name_                            \
)

#define be_define_const_module(_name, _name_)                   \
const bmodule _name((bmap*)&_name##_map, _name_)

#define be_define_const_vector(_name, _data, _size)             \
const bvector _name = {                                         \
    _size, sizeof(bvalue), _size,                               \
    (void*)_data, (void*)(_data + (_size) - 1)                  \
}

#define be_define_const_native_module(_module, _init)           \
const bntvmodule be_native_module(_module) = {                  \
    #_module,                                                   \
    0, 0,                                                       \
    (bmodule*)&(m_lib##_module),                                \
    _init                                                       \
}

#endif

#ifdef __cplusplus
}
#endif

#endif
