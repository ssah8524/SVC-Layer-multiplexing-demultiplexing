//
//  ReadBitStreamFull.h
//  
//
//  Created by Amir Hosseini on 8/17/16.
//
//


#include "LargeFile.h"

class ReadBitstreamFull
{
protected:
    ReadBitstreamFull();
    ~ReadBitstreamFull();
    
public:
    
    ErrVal extractPacket( BinData*& rpcBinData, bool& rbEOS );
    ErrVal releasePacket( BinData* pcBinData );
    
    static ErrVal create( ReadBitstreamFull*& rpcReadBitstreamFull );
    ErrVal destroy();
    
    ErrVal init( const std::string& rcFileName );
    ErrVal uninit();
    
    ErrVal getPosition( int& iPos );
    ErrVal setPosition( int  iPos );
    
   long long int  getFilePos() { return m_cFile.tell(); }
    
protected:
    LargeFile m_cFile;
};

class WriteBitstreamToFile
{
protected:
    WriteBitstreamToFile() : m_uiNumber(0), m_bNewFileOnNewAu(false) {}
    ~WriteBitstreamToFile() {}

public:
    static ErrVal create( WriteBitstreamToFile*& rpcWriteBitstreamToFile );
    ErrVal destroy();

    ErrVal init( const std::string& rcFileName, bool bNewFileOnNewAu = false );
    ErrVal uninit();

    ErrVal writePacket( BinDataAccessor* pcBinDataAccessor, bool bNewAU = false );
    ErrVal writePacket( BinData* pcBinData, bool bNewAU = false );

    ErrVal writePacket( void* pBuffer, unsigned int uiLength );

private:
    unsigned int m_uiNumber;

    LargeFile m_cFile;
    LargeFile m_cFileEnh;

    std::string m_cFileName;
    std::string m_cFileEnhancement;
    
    bool m_bNewFileOnNewAu;
};
