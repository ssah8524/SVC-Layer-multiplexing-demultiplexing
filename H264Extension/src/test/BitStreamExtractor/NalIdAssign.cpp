
#include "BStreamExtractor.h"
#include "NalIdAssign.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

NalIdAssign::NalIdAssign()
{
}

NalIdAssign::~NalIdAssign()
{
}

ErrVal NalIdAssign::enterNalId(BinData*& pcBinData)
{
    
    if(sepLayer == true)
    {
        NalId++;
        // maximum number of bytes assigned to nal id is set to 2
        UInt size = pcBinData->size();
        UChar newBin[size + 2];
        
        for (UInt i = 0; i < size;i++)
            newBin[i] = pcBinData->data()[i];
        
        if(NalId < 256)
            newBin[size] = 0x00;
        else
        {
            newBin[size] = (NalId & 0xff00) >> 8;
            printf("number of NAL units is large. Beware possible errors.\n");
        }
        newBin[size + 1] = (NalId & 0x00ff);
        pcBinData->set(new UChar[size + 2], size + 2);
        for(int u = 0; u < size + 2; u++ )
            pcBinData->data()[u] = newBin[u];
        //memcpy(pcBinData->data(),newBin,size + 2);
    }
    else
        printf("No layer separation requested!");

    return Err::m_nOK;
}