
#ifndef __vsttools_h__included__
#	define __vsttools_h__included__

#include <pluginterfaces/base/fplatform.h>
#include <pluginterfaces/base/ftypes.h>

// we save little endian
#if BYTEORDER == kLittleEndian
#	define MYSWAP_16(w)
#	define MYSWAP_32(w)
#	define MYSWAP_64(w)
#else
#	define MYSWAP_16(w) SWAP_16(w)
#	define MYSWAP_32(w) SWAP_32(w)
#	define MYSWAP_64(w) SWAP_64(w)
#endif

#endif