#ifndef TYPES_HPP_
#define TYPES_HPP_

// Adapted from Google testing framework.

template<size_t size>
class TypeWithSize {
public:
    typedef void UInt;
};

// The specialization for size 1.
template<>
class TypeWithSize<1> {
public:
    typedef char Int;
    typedef unsigned char UInt;
};

// The specialization for size 2.
template<>
class TypeWithSize<2> {
public:
    typedef short Int;
    typedef unsigned short UInt;
};

// The specialization for size 4.
template<>
class TypeWithSize<4> {
public:
    // unsigned int has size 4 in both gcc and MSVC.
    //
    // As base/basictypes.h doesn't compile on Windows,
    // we cannot use uint32, uint64, and etc here.
    typedef int Int;
    typedef unsigned int UInt;
};

// The specialization for size 8.
template<>
class TypeWithSize<8> {
public:
#if _WIN32
    typedef __int64 Int;
    typedef unsigned __int64 UInt;
#else
    typedef long long Int;
    typedef unsigned long long UInt;
#endif
};

typedef TypeWithSize<1>::Int    int8;
typedef TypeWithSize<1>::UInt   uint8;
typedef TypeWithSize<2>::Int    int16;
typedef TypeWithSize<2>::UInt   uint16;
typedef TypeWithSize<4>::Int    int32;
typedef TypeWithSize<4>::UInt   uint32;
typedef TypeWithSize<8>::Int    int64;
typedef TypeWithSize<8>::UInt   uint64;

#endif /* TYPES_HPP_ */
