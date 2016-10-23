//
//  ReAttach.cpp
//
//
//  Created by Amir Hosseini on 8/12/16.
//
//

#include "ReAttach.h"


ReAttach::ReAttach()
{
}

ReAttach::~ReAttach()
{
}

ErrVal ReAttach::init(Parameter* pcExtractorParameter)
{
    m_pcExtractorParameter = pcExtractorParameter;
    unsigned int numLayer = m_pcExtractorParameter->getNumEnh();
    
    //Read the each layer bitstream
    ReadFiles = new ReadBitstreamFull* [numLayer + 1];

    RNOKS( ReadBitstreamFull::create( ReadFiles[0] ) );
    RNOKS( ReadFiles[0]->init( m_pcExtractorParameter->getBaseFile() ) );

    for(unsigned int n = 1; n <= numLayer; n++)
    {
        RNOKS( ReadBitstreamFull::create( ReadFiles[n] ) );
        RNOKS( ReadFiles[n]->init( m_pcExtractorParameter->getEnhFile()[n - 1] ) );
    }

    WriteBitstreamToFile*  mWriteFinal;
    RNOKS( WriteBitstreamToFile::create( mWriteFinal ) );
    std::string fileName = "final_output.264";
    RNOKS( mWriteFinal->init( fileName ) );
    WriteFinal = mWriteFinal;
    return (0);
}


ErrVal ReAttach::extract()
{
    unsigned int totalSize = 0;
    finderPosition = 0;
    
    BinData** tempBuffer = new BinData* [m_pcExtractorParameter->getNumEnh() + 1];
    int position[m_pcExtractorParameter->getNumEnh() + 1];
    bool* EoS = new bool[m_pcExtractorParameter->getNumEnh() + 1];
    
    for(unsigned int i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
    {
        position[i] = 0;
        ReadFiles[i]->extractPacket(tempBuffer[i],EoS[i]);
        totalSize += tempBuffer[i]->size();
    }

    //Create the data object to carry the final bytestream to be written to the output file
    finalBuffer = new unsigned char[totalSize];
    unsigned int temporaryNal[m_pcExtractorParameter->getNumEnh() + 1];
    unsigned int tmpNal = 1;
    unsigned int test;
    do
    {
        test = tmpNal;
        int tempPos;
        bool notFound = true;
        for(unsigned int i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
        {
            temporaryNal[i] = 0;
            tempPos = position[i];
            
            reAttach(tempBuffer[i],position[i],tmpNal,temporaryNal[i]);

            if(tempPos != position[i]) //The next nal is found
            {
                notFound = false;
                break;
            }
        }
        if(notFound) // pick the one that is the closest in terms of NAL ID
        {
            //printf("ever entering?\n");
            //printf("here %d\n",tmpNal);
            tmpNal = findMin(temporaryNal);
        }
        
    }while(tmpNal != test);

    
    for(unsigned int i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
        delete tempBuffer[i];
    delete[] EoS;

    //Write the stream into file
    BinData* streamFinder = new BinData;
    streamFinder->set(finalBuffer, totalSize - 2*(tmpNal - 1));
    RNOK( WriteFinal->writePacket( streamFinder ) );
    delete streamFinder;
    return (0);
}

ErrVal ReAttach::reAttach(BinData* file, int& position, unsigned int& Nal, unsigned int& TmpNal)
{
    int tempPosition = position;
    int start = position;
    bool uiCond = false;
    if(position == 0)//first time we enter we have to look for the initial pattern
    {
        if(file->data()[tempPosition] != 0x0)
        {
            printf("ERROR! NAL unit should start with zero!\n");
            printf("It has: %x \n",file->data()[tempPosition + 1]);
        }
        while(file->data()[tempPosition] == 0)
            tempPosition++;
        
        if(file->data()[tempPosition] != 0x01)
            printf("ERROR! Prefix is incorrect!\n");
        tempPosition = start;
    }

    //Look for the delimiter
    int j;
    for(j = tempPosition; j < file->size(); j++)
    {
        uiCond = (file->data()[j] == 0 && file->data()[j + 1] == 0 && file->data()[j + 2] == 0 && file->data()[j + 3] == 1 && j != tempPosition);
        
        
        if( uiCond || (j == file->size()-1))
        {
            tempPosition = j;
            break;
        }
    }

    if(uiCond)
    {
        unsigned int dec = file->data()[tempPosition - 2] << 8;
        dec |= file->data()[tempPosition - 1];
        TmpNal = dec;

        if(dec != Nal)
        {
            tempPosition = start;
        }
        else //remove the last two bytes from stream
        {
            finalBuffer[finderPosition] = 0;
            finalBuffer[finderPosition + 1] = 0;
            finalBuffer[finderPosition + 2] = 0;
            finalBuffer[finderPosition + 3] = 1;

            for (unsigned int c = 4; c < tempPosition - start - 2; c++){
                finalBuffer[finderPosition + c] = file->data()[start + c];
            }
            finderPosition += tempPosition - start - 2;
            Nal++;
        }
        position = tempPosition;
        return (0);
    }
    else if(j == file->size()-1)
    {
        unsigned int dec = file->data()[tempPosition - 1] << 8;
        dec = file->data()[tempPosition];
        
        if(dec != Nal)
            tempPosition = start;
        else //remove the last two bytes from stream
        {
            finalBuffer[finderPosition] = 0;
            finalBuffer[finderPosition + 1] = 0;
            finalBuffer[finderPosition + 2] = 0;
            finalBuffer[finderPosition + 3] = 1;
            
            for (unsigned int c = 4; c < tempPosition - start - 1; c++)
                finalBuffer[finderPosition + c] = file->data()[start + c];
            finderPosition += tempPosition - start - 1;
            Nal++;
        }
        position = tempPosition;
        return (0);
    }
    else
    {
        tempPosition = start;
        position = tempPosition;
        return (0);
    }
}

ErrVal ReAttach::destroy()
{
    for(unsigned int i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
    {
        RNOK( ReadFiles[i]->uninit() );
        RNOK( ReadFiles[i]->destroy() );
    }
    RNOK( WriteFinal->uninit() );
    RNOK( WriteFinal->destroy() );
    delete this;
    return (0);
}

unsigned int ReAttach::findMin(unsigned int array[])
{
    unsigned int min = 1000000;
    for(unsigned int i = 0; i <= m_pcExtractorParameter->getNumEnh(); i++)
        if(array[i] < min)
            min = array[i];
    return min;
}
