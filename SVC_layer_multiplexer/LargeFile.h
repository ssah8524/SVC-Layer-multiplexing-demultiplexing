#include <string>
#include <sstream>
#include <iostream>
#include "CommonHeader.h"

class LargeFile
{
public:
  enum OpenMode
  {
    OM_READONLY,
    OM_WRITEONLY,
    OM_APPEND,
    OM_READWRITE
  };

public:
  LargeFile();
  ~LargeFile();

  ErrVal open( const std::string& rcFilename, enum OpenMode eOpenMode, int iPermMode=0777 );

  bool is_open() const { return -1 != m_iFileHandle; }

  ErrVal close();
  ErrVal seek( long long int iOffset, int iOrigin );
  long long int tell();

  ErrVal read( void *pvBuffer, unsigned int uiCount, unsigned int& ruiBytesRead );
  ErrVal write( const void *pvBuffer, unsigned int uiCount );

  int getFileHandle() { return m_iFileHandle; }

private:
  int m_iFileHandle;
};

