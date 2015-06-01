#ifndef _TYPE_DEF_PS_H_
#define _TYPE_DEF_PS_H_

#ifndef int8
    typedef __int8 int8;
#endif

#ifndef int16
    typedef __int16 int16;
#endif

#ifndef int32
    typedef __int32 int32;
#endif

#ifndef int64
    typedef __int64 int64;
#endif

#ifndef uint
    typedef unsigned int uint;
#endif

#ifndef uint8
    typedef unsigned __int8 uint8;
#endif

#ifndef uint16
    typedef unsigned __int16 uint16;
#endif

#ifndef uint32
    typedef unsigned __int32 uint32;
#endif

#ifndef uint64
    typedef unsigned __int64 uint64;
#endif

#ifndef byte
    typedef unsigned char byte;
#endif

#ifndef VarType
	enum VarType
	{
		vtInvalid = 0 ,
		vtBool ,
		vtInt ,
		vtFloat ,
		vtDouble ,
		vtUInt ,
		vtPtrs ,
		vtBuffer
	};
#endif

#endif //_TYPE_DEF_PS_H_