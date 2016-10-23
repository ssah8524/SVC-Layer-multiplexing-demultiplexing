
#ifndef nalIdAssign_h
#define nalIdAssign_h

#include <stdio.h>

class NalIdAssign
{
public:
    NalIdAssign();
    ~NalIdAssign();
    
    Bool sepLayer;
    ErrVal enterNalId(BinData*& pcBinData);
    UInt getNalId() { return NalId; }
    UInt setNalId() { return NalId++; }
    
protected:
    UInt NalId = 0;

};

#endif /* nalIdAssign_h */
