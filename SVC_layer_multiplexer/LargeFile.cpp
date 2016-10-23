
#include "LargeFile.h"
#include <errno.h>

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
// heiko.schwarz@hhi.fhg.de: support for BSD systems as proposed by Steffen Kamp [kamp@ient.rwth-aachen.de]
#define _FILE_OFFSET_BITS 64
#endif
# include <sys/ioctl.h>
# include <sys/types.h>
# include <fcntl.h>
# include <dlfcn.h>
# include <cerrno>
# include <unistd.h>


LargeFile::LargeFile() :
  m_iFileHandle( -1 )
{
}


LargeFile::~LargeFile()
{
  if( -1 != m_iFileHandle )
  {
    ::close( m_iFileHandle );
  }
}


ErrVal LargeFile::open( const std::string& rcFilename, enum OpenMode eOpenMode, int iPermMode )
{
    
  ROT( rcFilename.empty() );
  ROF( -1 == m_iFileHandle );

  int iOpenMode;

  if( eOpenMode == OM_READONLY )
  {
    iOpenMode = O_RDONLY;
  }
  else if( eOpenMode == OM_WRITEONLY )
  {
    iOpenMode = O_CREAT | O_TRUNC | O_WRONLY;
  }
  else if( eOpenMode == OM_APPEND )
  {
    iOpenMode = O_APPEND | O_CREAT | O_WRONLY;
  }
  else if( eOpenMode == OM_READWRITE )
  {
    iOpenMode = O_CREAT | O_RDWR;
  }
  else
  {
    AOT( 1 );
    return (-1);
  }

  // heiko.schwarz@hhi.fhg.de: support for BSD systems as proposed by Steffen Kamp [kamp@ient.rwth-aachen.de]
  //m_iFileHandle = open64( rcFilename.c_str(), iOpenMode, iPermMode );
    //I guess this open function is fstream::open
  m_iFileHandle = ::open( rcFilename.c_str(), iOpenMode, iPermMode );

  // check if file is really open
  ROTS( -1 == m_iFileHandle );
  // and return
  return (0);
}

ErrVal LargeFile::close()
{
  int iRetv;

  ROTS( -1 == m_iFileHandle );

  iRetv = ::close( m_iFileHandle );

  m_iFileHandle = -1;

  return ( iRetv == 0 ) ? (0) : (-1);
}


ErrVal LargeFile::seek( long long int iOffset, int iOrigin )
{
  long long int iNewOffset;
  ROT( -1 == m_iFileHandle );

  // heiko.schwarz@hhi.fhg.de: support for BSD systems as proposed by Steffen Kamp [kamp@ient.rwth-aachen.de]
  //iNewOffset = lseek64( m_iFileHandle, iOffset, iOrigin );
  iNewOffset = ::lseek( m_iFileHandle, iOffset, iOrigin );

  return ( iNewOffset == -1 ) ? (-1) : (0);
}


long long int LargeFile::tell()
{
  ROTR( -1 == m_iFileHandle, -1 );
  long long int iOffset;
  iOffset = ::lseek( m_iFileHandle, 0, SEEK_CUR ); //find the
  ROT( iOffset == -1 )

  // and return
  return iOffset;
}


ErrVal LargeFile::read( void *pvBuffer, unsigned int uiCount, unsigned int& ruiBytesRead )
{
  int iRetv;

  ROT( -1 == m_iFileHandle );
  ROT( 0 == uiCount );

  ruiBytesRead = 0;

  // returns the number of bytes read from file descriptor m_iFileHandle
  iRetv = ::read( m_iFileHandle, pvBuffer, uiCount );
  if( iRetv != (int)uiCount )
  {
    //need to handle partial reads before hitting EOF
    //If the function tries to read at end of file, it returns 0.
    //If the handle is invalid, or the file is not open for reading,
    //or the file is locked, the function returns -1 and sets errno to EBADF.
    if( iRetv > 0 )
    {
      //partial reads are acceptable and return the standard success code. Anything
      //else must be implemented by the caller.
      ruiBytesRead = iRetv;
      return (0);
    }
    else if( iRetv == -1 )
    {
      return errno;
    }
    else if( iRetv == 0)
    {
      return (-3);
    }
    else
    {
      AOF( ! "fix me, unexpected return code" );
      return (-1);
    }
  }
  else // have read everything we wanted.
  {
    ruiBytesRead = uiCount;
  }

  ROF( iRetv == (int)uiCount );

  return (0);
}


ErrVal LargeFile::write( const void *pvBuffer, unsigned int uiCount )
{
  int iRetv;

  ROT( -1 == m_iFileHandle );
  ROT( 0 == uiCount );

  iRetv = ::write( m_iFileHandle, pvBuffer, uiCount );
  ROF( iRetv == (int)uiCount );

  return (0);
}


