
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#if defined( MSYS_WIN32 )
#pragma warning( disable: 4275 )
#endif

#if defined( MSYS_WIN32 )
#pragma warning( disable: 4250 )
#endif



#include "LargeFile.h"
#include "WriteBitstreamIf.h"

class H264AVCVIDEOIOLIB_API WriteFakeBaseLayer :
public WriteBitstreamIf
{
protected:
    WriteFakeBaseLayer() : m_uiNumber(0), m_bNewFileOnNewAu(false) {}
    virtual ~WriteFakeBaseLayer() {}
    
public:
    static ErrVal create( WriteFakeBaseLayer*& rpcWriteFakeBaseLayer );
    virtual ErrVal destroy();
    ErrVal init( const std::string& rcFileName, Bool bNewFileOnNewAu = false );
    virtual ErrVal uninit();
    
    virtual ErrVal writePacket( BinDataAccessor* pcBinDataAccessor, Bool bNewAU = false );
    virtual ErrVal writePacket( BinData* pcBinData, Bool bNewAU = false );
    virtual ErrVal writePacket( Void* pBuffer, UInt uiLength );
    
private:
    UInt m_uiNumber;
    LargeFile m_cFile;
    std::string m_cFileName;
    Bool m_bNewFileOnNewAu;
};

#if defined( MSYS_WIN32 )
#pragma warning( default: 4275 )
#endif

#if defined( MSYS_WIN32 )
#pragma warning( default: 4250 )
#endif