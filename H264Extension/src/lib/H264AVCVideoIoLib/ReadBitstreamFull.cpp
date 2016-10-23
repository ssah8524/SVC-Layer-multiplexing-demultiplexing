//
//  ReadBitStreamFull.cpp
//  
//
//  Created by Amir Hosseini on 8/17/16.
//
//

#include "ReadBitstreamFull.h"


ReadBitstreamFull::ReadBitstreamFull()
{
}


ReadBitstreamFull::~ReadBitstreamFull()
{
}

ErrVal ReadBitstreamFull::releasePacket( BinData* pcBinData )
{
    ROFRS( pcBinData, Err::m_nOK );
    pcBinData->deleteData();
    delete pcBinData;
    return Err::m_nOK;
}

ErrVal ReadBitstreamFull::extractPacket( BinData*& rpcBinData, Bool& rbEOS )
{
    UInt   dwBytesRead;
    UInt   dwLength = 0;
    Int64  iPos;
    UInt   n;
    UChar  Buffer[0x400];
    UInt   uiZeros;
    
    ROT( NULL == ( rpcBinData = new BinData ) );
    
    rbEOS     = false;
    // exit if there's no bitstream. File must have been opened before by applying the init function.
    ROFS( m_cFile.is_open());

    // get the current position. It is current because in the function, the offset is set to 0.
    iPos = m_cFile.tell();

    // we max read any number of zeros
    uiZeros = 0;
    do
    {
        // it reads byte by byte! each time we enter, dwBytesRead is initialized as zero
        m_cFile.read( Buffer, 1, dwBytesRead );
        uiZeros++;
    } while ((dwBytesRead==1)&&(Buffer[0]==0)); //all the zeros are written into the first byte of the buffer.
    
    //happens when end is reached? Everything in the file was zero? not sure if this is correct.
    if( 0 == dwBytesRead )
    {
        rbEOS = true;
        return Err::m_nOK;
    }
    
    // next we expect "0x01". This means that this was the condition to exit the do while loop.
    ROTS(Buffer[0]!=0x01);
    
    // there is a min of two zeros in a startcode
    ROTS(uiZeros<2);
    
    
    // read at first 0x400 bytes
    m_cFile.read( Buffer, 0x400, dwBytesRead );
    
    ROFS( dwBytesRead );
    
    do
    {
        if( dwBytesRead == 1 )
        {
            n = 1;
            break;
        }
        
        // found no synch so go on
        dwLength += dwBytesRead-2 ;
        // step 3 bytes back in the stream (why two and not three?)
        RNOK( m_cFile.seek( -2, SEEK_CUR ) );
        
        // read the next chunk or return out of bytes
        m_cFile.read( Buffer, 0x400, dwBytesRead );
        
        if( 2 == dwBytesRead )
        {
            n = 2;
            // end of stream cause we former stepped 4 bytes back
            break;      // this is the last pack
        }
    }
    while( true );
    
    // calc the complete length
    dwLength += n + 4;

    if( 0 == dwLength )
    {
        rbEOS = true;
        return Err::m_nOK;
    }
    
    rpcBinData->set( new UChar[dwLength], dwLength );
    ROT( NULL == rpcBinData->data() );
    
    // seek the bitstream to the prev start position
    RNOK( m_cFile.seek( iPos, SEEK_SET ) );
    
    // read the access unit into the transport buffer
    RNOK( m_cFile.read( rpcBinData->data(), dwLength, dwBytesRead ) );
    
    return Err::m_nOK;
}


ErrVal ReadBitstreamFull::init( const std::string& rcFileName )
{
    // try to open the bitstream binary.
    if ( Err::m_nOK != m_cFile.open( rcFileName, LargeFile::OM_READONLY ) )
    {
        std::cerr << "failed to open input bitstream file " << rcFileName.data() << std::endl;
        return Err::m_nERR;
    }
    // file is opened.
    
    UChar  aucBuffer[0x4];
    UInt uiBytesRead;

    RNOK( m_cFile.read( aucBuffer, 4, uiBytesRead ) );
    
    UInt uiStartPos = ( aucBuffer[0] == 0 && aucBuffer[1] == 0 && aucBuffer[2] == 1) ? 1 : 0;
    
    RNOK( m_cFile.seek( uiStartPos, SEEK_SET ) );
    
    return Err::m_nOK;
}


ErrVal ReadBitstreamFull::create( ReadBitstreamFull*& rpcReadBitstreamFull )
{
    rpcReadBitstreamFull = new ReadBitstreamFull;
    ROT( NULL == rpcReadBitstreamFull );
    return Err::m_nOK;
}


ErrVal ReadBitstreamFull::getPosition( Int& iPos )
{
    ROFS( m_cFile.is_open());
    
    iPos = (Int)m_cFile.tell();
    
    return Err::m_nOK;
}


ErrVal ReadBitstreamFull::setPosition( Int  iPos )
{
    ROFS( m_cFile.is_open());
    
    // seek the bitstream to the prev start position
    RNOK( m_cFile.seek( iPos, SEEK_SET ) );
    
    return Err::m_nOK;
}


ErrVal ReadBitstreamFull::uninit()
{
    if( m_cFile.is_open() )
    {
        RNOK( m_cFile.close() );
    }
    return Err::m_nOK;
}

ErrVal ReadBitstreamFull::destroy()
{
    AOT_DBG( m_cFile.is_open() );
    
    RNOK( uninit() );
    
    delete this;
    return Err::m_nOK;
}
