//
//  ReAttach.h
//  
//
//  Created by Amir Hosseini on 8/12/16.
//
//

#ifndef ReAttach_h
#define ReAttach_h

#include <stdio.h>
#include "ReadBitStreamFull.h"
#include "WriteBitStreamToFile.h"
#include "H264AVCVideoIoLib.h"
#include "ExtractorParameter.h"

class ReAttach
{
public:
    
    ReAttach();
    ~ReAttach();
     
    ErrVal init(ExtractorParameter* pcExtractorParameter);
    ErrVal extract();
    ErrVal searchNal();
    ErrVal reAttach(BinData* file, Int& position, UInt& Nal, UInt& TmpNal);
    ErrVal destroy();
    UInt findMin(UInt array[]);

protected:
    
    ReadBitstreamFull** ReadFiles;
    WriteBitstreamIf* WriteFinal;
    UChar* finalBuffer;
    UInt finderPosition;
    ExtractorParameter* m_pcExtractorParameter;
};
#endif /* ReAttach_hpp */
