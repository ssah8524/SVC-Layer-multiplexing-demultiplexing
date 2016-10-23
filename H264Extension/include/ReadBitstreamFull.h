//
//  ReadBitStreamFull.h
//  
//
//  Created by Amir Hosseini on 8/17/16.
//
//

#ifndef ReadBitstreamFull_h
#define ReadBitstreamFull_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "H264AVCVideoIoLib.h"
#include "LargeFile.h"
#include "ReadBitstreamIf.h"

#if defined( WIN32 )
# pragma warning( disable: 4251 )
#endif

class H264AVCVIDEOIOLIB_API ReadBitstreamFull :
public ReadBitstreamIf
{
protected:
    ReadBitstreamFull();
    virtual ~ReadBitstreamFull();
    
public:
    
    virtual ErrVal extractPacket( BinData*& rpcBinData, Bool& rbEOS );
    virtual ErrVal releasePacket( BinData* pcBinData );
    
    static ErrVal create( ReadBitstreamFull*& rpcReadBitstreamFull );
    virtual ErrVal destroy();
    
    ErrVal init( const std::string& rcFileName );
    virtual ErrVal uninit();
    
    virtual ErrVal getPosition( Int& iPos );
    virtual ErrVal setPosition( Int  iPos );
    
    virtual Int64  getFilePos() { return m_cFile.tell(); }
    
protected:
    LargeFile m_cFile;
};

#if defined( WIN32 )
# pragma warning( default: 4251 )
#endif

#endif /* ReadBitstreamFull_h */
