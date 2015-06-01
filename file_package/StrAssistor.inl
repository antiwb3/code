#include "stdafx.h"

#include <stdlib.h>
#include <string.h>

namespace StrAssistor
{

inline int		ToInt(const char* value, int defValue = 0);
inline float	ToFloat(const char* value, float defValue = 0.0f);
inline double	ToDouble(const char* value, double defValue = 0.0);
inline int		ToBool(const char* value, int defValue = false);

inline int ToInt(const char* value, int defValue /*= 0*/)
{
	if (!value)
		return defValue;

	return (int)strtol(value, 0, 10);
}

inline float ToFloat(const char* value, float defValue /*= 0.0f*/)
{
	if (!value)
		return defValue;

	return (float)strtod(value, 0);
}

inline double ToDouble(const char* value, double defValue /*= 0.0*/)
{
	if (!value)
		return defValue;
	return (double)strtod(value, 0);
}

inline int ToBool(const char* value, int defValue /*= false*/)
{
	if (!value)
		return defValue;

	if (_stricmp(value, "true") == 0)
		return true;
	else if (_stricmp(value, "false") == 0)
		return false;

	return defValue;
}

};