
#include "ReadWrite.h"


ErrVal WriteBitstreamToFile::create( WriteBitstreamToFile*& rpcWriteBitstreamToFile )
{
  rpcWriteBitstreamToFile = new WriteBitstreamToFile;
  ROT( NULL == rpcWriteBitstreamToFile );
  return (0);
}



ErrVal WriteBitstreamToFile::init( const std::string& rcFileName, bool bNewFileOnNewAu )
{

  m_bNewFileOnNewAu = bNewFileOnNewAu;
  m_cFileName = rcFileName;
  if( (0) != m_cFile.open( rcFileName, LargeFile::OM_WRITEONLY ) )
  {
    std::cerr << "Failed to create output bitstream " << rcFileName.data() << std::endl;
    return (-1);
  }
  m_uiNumber = 0;

  return (0);
}

ErrVal WriteBitstreamToFile::uninit()
{
  if( m_cFile.is_open() )
  {
    RNOK( m_cFile.close() );
  }
  return (0);
}

ErrVal WriteBitstreamToFile::destroy()
{
  ROT( m_cFile.is_open() );
  RNOK( uninit() );
  delete this;
  return (0);
}





ErrVal WriteBitstreamToFile::writePacket( BinData* pcBinData, bool bNewAU )
{
  BinDataAccessor cBinDataAccessor;
  pcBinData->setMemAccessor( cBinDataAccessor );
  RNOK( writePacket( &cBinDataAccessor, bNewAU ) );
  return (0);
}

ErrVal WriteBitstreamToFile::writePacket( BinDataAccessor* pcBinDataAccessor, bool bNewAU )
{
  ROTRS( NULL == pcBinDataAccessor, (0) );

  if( bNewAU && m_bNewFileOnNewAu )
  {
#if defined MSYS_WIN32
    if( m_cFile.is_open() )
    {
      RNOK( m_cFile.close() );
    }

    std::string cFileName = m_cFileName;
    int iPos = (int)cFileName.find_last_of(".");

    Char acBuffer[20];
    itoa( ++m_uiNumber, acBuffer, 10 );
    cFileName.insert( iPos, acBuffer );
    if( (0) != m_cFile.open( cFileName, LargeFile::OM_WRITEONLY ) )
    {
      std::cerr << "Failed to create output bitstream " << cFileName.data() << std::endl;
      return (-1);
    }
#else
   std::cerr << "multiple output bitstreams only supported in Win32";
   AF();
#endif
  }
  
  if( 0 != pcBinDataAccessor->size())
  {
    RNOK( m_cFile.write( pcBinDataAccessor->data(), pcBinDataAccessor->size() ) );
  }

  return (0);
}




ErrVal
WriteBitstreamToFile::writePacket( void* pBuffer, unsigned int uiLength )
{

  if( uiLength )
  {
    RNOK( m_cFile.write( pBuffer, uiLength ) );
  }
  return (0);
}
