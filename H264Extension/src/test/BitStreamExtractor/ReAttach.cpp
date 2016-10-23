//
//  ReAttach.cpp
//
//
//  Created by Amir Hosseini on 8/12/16.
//
//

#include "ReAttach.h"
#include "BStreamExtractor.h"
#include "ReadBitStreamFull.h"
#include "WriteBitStreamToFile.h"
#include "WriteFakeBaseLayer.h"
#include "ExtractorParameter.h"


ReAttach::ReAttach()
{
}

ReAttach::~ReAttach()
{
}

ErrVal ReAttach::init(ExtractorParameter* pcExtractorParameter)
{
    m_pcExtractorParameter = pcExtractorParameter;
    UInt numLayer = m_pcExtractorParameter->getNumEnh();
    
    //Read the each layer bitstream
    ReadFiles = new ReadBitstreamFull* [numLayer + 1];

    RNOKS( ReadBitstreamFull::create( ReadFiles[0] ) );
    RNOKS( ReadFiles[0]->init( m_pcExtractorParameter->getBaseFile() ) );

    for(UInt n = 1; n <= numLayer; n++)
    {
        RNOKS( ReadBitstreamFull::create( ReadFiles[n] ) );
        RNOKS( ReadFiles[n]->init( m_pcExtractorParameter->getEnhFile()[n - 1] ) );
    }

    WriteBitstreamToFile*  mWriteFinal;
    RNOKS( WriteBitstreamToFile::create( mWriteFinal ) );
    std::string fileName = "final_output.264";
    RNOKS( mWriteFinal->init( fileName ) );
    WriteFinal = (WriteBitstreamIf*)mWriteFinal;
    return Err::m_nOK;
}


ErrVal ReAttach::extract()
{
    UInt totalSize = 0;
    finderPosition = 0;
    
    BinData** tempBuffer = new BinData* [m_pcExtractorParameter->getNumEnh() + 1];
    Int position[m_pcExtractorParameter->getNumEnh() + 1];
    Bool* EoS = new Bool[m_pcExtractorParameter->getNumEnh() + 1];
    
    for(UInt i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
    {
        position[i] = 0;
        RNOK(ReadFiles[i]->extractPacket(tempBuffer[i],EoS[i]));
        totalSize += tempBuffer[i]->size();
    }
    
    //Create the data object to carry the final bytestream to be written to the output file
    finalBuffer = new UChar[totalSize];
    UInt temporaryNal[m_pcExtractorParameter->getNumEnh() + 1];
    UInt tmpNal = 1;
    UInt test;
    do
    {
        test = tmpNal;
        Int tempPos;
        Bool notFound = true;
        for(UInt i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
        {
            temporaryNal[i] = 0;
            tempPos = position[i];
            RNOK(reAttach(tempBuffer[i],position[i],tmpNal,temporaryNal[i]));
            if(tempPos != position[i]) //The next nal is found
            {
                notFound = false;
                break;
            }
        }
        if(notFound) // pick the one that is the closest in terms of NAL ID
        {
            //printf("here %d\n",tmpNal);
            tmpNal = findMin(temporaryNal);
        }
        
    }while(tmpNal != test);

    
    for(UInt i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
        delete tempBuffer[i];
    delete[] EoS;

    //Write the stream into file
    BinData* streamFinder = new BinData;
    streamFinder->set(finalBuffer, totalSize - 2*(tmpNal - 1));
    RNOK( WriteFinal->writePacket( streamFinder ) );
    delete streamFinder;
    return Err::m_nOK;
}

ErrVal ReAttach::reAttach(BinData* file, Int& position, UInt& Nal, UInt& TmpNal)
{
    Int start = position;
    Bool uiCond = false;
    
    if(position == 0)//first time we enter we have to look for the initial pattern
    {
        if(file->data()[position] != 0x0)
        {
            printf("ERROR! NAL unit should start with zero!\n");
            printf("It has: %x \n",file->data()[position + 1]);
        }
        
        while(file->data()[position] == 0)
            position++;
        
        if(file->data()[position] != 0x01)
            printf("ERROR! Prefix is incorrect!\n");
        position = start;
    }
    
    //Look for the delimiter
    Int j;
    for(j = position; j < file->size(); j++)
    {
        uiCond = (file->data()[j] == 0 && file->data()[j + 1] == 0 && file->data()[j + 2] == 0 && file->data()[j + 3] == 1 && j != position);
        
        
        if( uiCond || (j == file->size()-1))
        {
            position = j;
            break;
        }
    }
    //printf("position: %d\n",position);
    if(uiCond)
    {
        UInt dec = file->data()[position - 2] << 8;
        dec |= file->data()[position - 1];
        TmpNal = dec;
        
        if(dec != Nal)
            position = start;
        else //remove the last two bytes from stream
        {
            finalBuffer[finderPosition] = 0;
            finalBuffer[finderPosition + 1] = 0;
            finalBuffer[finderPosition + 2] = 0;
            finalBuffer[finderPosition + 3] = 1;
        
            for (UInt c = 4; c < position - start - 2; c++)
                finalBuffer[finderPosition + c] = file->data()[start + c];
            
            finderPosition += position - start - 2;
            Nal++;
        }
        //printf("position: %d\n",position);
        return Err::m_nOK;
    }
    else if(j == file->size()-1)
    {
        UInt dec = file->data()[position - 1] << 8;
        dec = file->data()[position];
        
        if(dec != Nal)
            position = start;
        else //remove the last two bytes from stream
        {
            finalBuffer[finderPosition] = 0;
            finalBuffer[finderPosition + 1] = 0;
            finalBuffer[finderPosition + 2] = 0;
            finalBuffer[finderPosition + 3] = 1;
            
            for (UInt c = 4; c < position - start - 1; c++)
                finalBuffer[finderPosition + c] = file->data()[start + c];
            finderPosition += position - start - 1;
            Nal++;
        }
        return Err::m_nOK;
    }
    else
    {
        position = start;
        return Err::m_nOK;
    }
}

ErrVal ReAttach::destroy()
{
    for(UInt i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
    {
        RNOK( ReadFiles[i]->uninit() );
        RNOK( ReadFiles[i]->destroy() );
    }
    RNOK( WriteFinal->uninit() );
    RNOK( WriteFinal->destroy() );
    delete this;
    return Err::m_nOK;
}

UInt ReAttach::findMin(UInt array[])
{
    UInt min = 1000000;
    for(UInt i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
        if(array[i] < min)
            min = array[i];
    return min;
}
