
#include <list>
#include <string.h>
#include <assert.h>

typedef int ErrVal;

class Err
{
public:

  static const ErrVal m_nOK;
  static const ErrVal m_nERR;
  static const ErrVal m_nEndOfStream;
  static const ErrVal m_nEndOfFile;
  static const ErrVal m_nEndOfBuffer;
  static const ErrVal m_nInvalidParameter;
  static const ErrVal m_nDataNotAvailable;
 };


#if !defined( ASSERT )
#define ASSERT assert
#endif

#define VOID_TYPE void
#define ERR_CLASS Err
#define ERR_VAL   ErrVal


#define RERR( )               \
{                             \
ASSERT( 0 );              \
return (-1); \
}

#define ROF( exp )            \
{                             \
if( !( exp ) )              \
{                           \
RERR();                   \
}                           \
}

#define ROT( exp )            \
{                             \
if( ( exp ) )               \
{                           \
RERR();                    \
}                           \
}

#define RERRS( )               \
{                             \
return (-1);     \
}


#define ROFS( exp )           \
{                             \
if( !( exp ) )              \
{                           \
RERRS();                  \
}                           \
}

#define ROTS( exp )           \
{                             \
if( ( exp ) )               \
{                           \
RERRS();                  \
}                           \
}


#define RVAL( retVal )        \
{                             \
ASSERT( 0 );              \
return retVal;            \
}


#define ROFR( exp, retVal )   \
{                             \
if( !( exp ) )              \
{                           \
RVAL( retVal );           \
}                           \
}


#define ROTR( exp, retVal )   \
{                             \
if( ( exp ) )               \
{                           \
RVAL( retVal );             \
}                           \
}



#define ROFRS( exp, retVal )  \
{                             \
if( !( exp ) )              \
{                           \
return retVal;            \
}                           \
}

#define ROTRS( exp, retVal )  \
{                             \
if( ( exp ) )               \
{                           \
return retVal;            \
}                           \
}

#define ROFV( exp )           \
{                             \
if( !( exp ) )              \
{                           \
ASSERT( 0 );              \
return;                   \
}                           \
}

#define ROTV( exp )           \
{                             \
if( ( exp ) )               \
{                           \
ASSERT( 0 );              \
return;                   \
}                           \
}

#define ROFVS( exp )          \
{                             \
if( !( exp ) )              \
{                           \
return;                   \
}                           \
}

#define ROTVS( exp )          \
{                             \
if( ( exp ) )               \
{                           \
return;                   \
}                           \
}

#define RNOK( exp )               \
{                                 \
const ERR_VAL nMSysRetVal = ( exp );   \
if( (0) != nMSysRetVal )  \
{                               \
ASSERT( 0 );                  \
return nMSysRetVal;                  \
}                               \
}

#define RNOKR( exp, retVal )        \
{                                   \
if( (0) != ( exp ) ) \
{                                 \
ASSERT( 0 );                    \
return retVal;                  \
}                                 \
}

#define RNOKS( exp )                \
{                                   \
const ERR_VAL nMSysRetVal = ( exp );     \
if( (0) != nMSysRetVal )    \
{                                 \
return nMSysRetVal;             \
}                                 \
}

#define RNOKRS( exp, retVal )       \
{                                   \
if( (0) != ( exp ) ) \
{                                 \
return retVal;                  \
}                                 \
}

#define RNOKV( exp )                \
{                                   \
if( (0) != ( exp ) ) \
{                                 \
ASSERT( 0 );                    \
return;                         \
}                                 \
}

#define RNOKVS( exp )               \
{                                   \
if( (0) != ( exp ) ) \
{                                 \
return;                         \
}                                 \
}


#define AF( )                 \
{                             \
ASSERT( 0 );              \
}


#define ANOK( exp )                 \
{                                   \
if( (0) != ( exp ) ) \
{                                 \
ASSERT( 0 );                    \
}                                 \
}

#define AOF( exp )                  \
{                                   \
if( !( exp ) )                    \
{                                 \
ASSERT( 0 );                    \
}                                 \
}

#define AOT( exp )            \
{                             \
if( ( exp ) )               \
{                           \
ASSERT( 0 );              \
}                           \
}





#if defined( _DEBUG ) || defined( DEBUG )
#define CHECK( exp )      ASSERT( exp )
#define AOT_DBG( exp )    AOT( exp )
#define AOF_DBG( exp )    AOF( exp )
#define ANOK_DBG( exp )   ANOK( exp )
#define DO_DBG( exp )     ( exp )
#else  // _DEBUG

#define CHECK( exp )      ((VOID_TYPE)( exp ))
#define AOT_DBG( exp )    ((VOID_TYPE)0)
#define AOF_DBG( exp )    ((VOID_TYPE)0)
#define ANOK_DBG( exp )   ((VOID_TYPE)0)
#define DO_DBG( exp )     ((VOID_TYPE)0)
#endif // _DEBUG



enum MemType
{
    MEM_CONT =      0,
    MEM_LIST =      1
};

template< class T >
class MemIf
{
public:
    MemIf() {}
    virtual ~MemIf() {}

public:
    virtual MemType getMemType() const = 0;

    virtual void set( MemIf< T >& rcMemIf ) = 0;
    virtual void set( T* pcT, unsigned int uiSize, T* pcDeleteT=NULL, unsigned int uiUsableSize=0 ) = 0;

    virtual void release( T*& rpcT, unsigned int& ruiSize ) = 0;
    virtual void release( T*& rpcT, unsigned int& ruiSize, T*& rpcDeleteT, unsigned int& ruiUsableSize ) = 0;

    virtual void deleteData() = 0;

    virtual unsigned int size() const = 0;
    virtual unsigned int byteSize() const = 0;
};



template< class T >
class MemAccessor
{
public:
    MemAccessor() :
    m_pcT( NULL ), m_pcOrigT( NULL ), m_uiSize( 0 ), m_uiUsableSize( 0 ) {}

    MemAccessor( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 ) :
    m_pcT( pcT ), m_pcOrigT( pcOrigT ), m_uiSize( uiSize ), m_uiUsableSize( uiUsableSize )
    {
        if( NULL == m_pcOrigT ) m_pcOrigT = m_pcT;
        if( 0 == m_uiUsableSize ) m_uiUsableSize = uiSize;
    }

    MemAccessor( const MemAccessor< T >& rcMemAccessor ) :
    m_pcT( rcMemAccessor.m_pcT ), m_pcOrigT( rcMemAccessor.m_pcOrigT ), m_uiSize( rcMemAccessor.m_uiSize ) , m_uiUsableSize( rcMemAccessor.m_uiUsableSize ) {}

    virtual ~MemAccessor() {}

public:
    MemAccessor< T >& operator=( const MemAccessor< T >& rcMemAccessor )
    {
        m_pcT = rcMemAccessor.m_pcT;
        m_pcOrigT = rcMemAccessor.m_pcOrigT;
        m_uiSize = rcMemAccessor.m_uiSize;
        m_uiUsableSize = rcMemAccessor.m_uiUsableSize;

        return *this;
    }

public:
    void set( const MemAccessor< T >& rcMemAccessor )
    {
        m_pcT = rcMemAccessor.m_pcT;
        m_pcOrigT = rcMemAccessor.m_pcOrigT;
        m_uiSize = rcMemAccessor.m_uiSize;
        m_uiUsableSize = rcMemAccessor.m_uiUsableSize;
    }

    void set( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 )
    {
        m_pcT = pcT;
        m_uiSize = uiSize;
        m_pcOrigT = pcOrigT;
        m_uiUsableSize = uiUsableSize;
        if( NULL == pcOrigT ) { m_pcOrigT = pcT; }
        if( 0 == uiUsableSize ) { m_uiUsableSize = m_uiSize; }
    }

    void clear()
    {
        m_pcT = NULL;
        m_pcOrigT = NULL;
        m_uiSize = 0;
        m_uiUsableSize = 0;
    }

    void deleteData() { if( m_pcOrigT ) { delete[] m_pcOrigT; } m_pcOrigT = NULL; m_pcT = NULL; m_uiSize = 0; m_uiUsableSize = 0; }

    T* data() const { return m_pcT; }
    T* origData() const { return m_pcOrigT; }
    unsigned int size() const { return m_uiSize; }
    unsigned int usableSize() const { return m_uiUsableSize; }
    unsigned int byteSize() const { return sizeof( T ) * m_uiSize; }
    unsigned int usableByteSize() const { return sizeof( T ) * m_uiUsableSize; }

public:
    ErrVal increasePos( unsigned int uiPos )
    {
        ROT( uiPos >= m_uiSize );
        m_pcT += uiPos;
        m_uiSize -= uiPos;
        return (0);
    }


    ErrVal decreasePos( unsigned int uiPos )
    {
        ROT( m_pcT - uiPos < m_pcOrigT );
        m_pcT -= uiPos;
        m_uiSize += uiPos;
        return (0);
    }

    ErrVal resetPos()
    {
        m_uiSize += m_pcT - m_pcOrigT;
        m_pcT = m_pcOrigT;
        return (0);
    }

    ErrVal increaseEndPos( unsigned int uiPos )
    {
        ROT( uiPos > (m_uiUsableSize - m_uiSize) );
        m_uiSize += uiPos;
        return (0);
    }

    ErrVal decreaseEndPos( unsigned int uiPos )
    {
        ROT( uiPos > m_uiSize );
        m_uiSize -= uiPos;
        return (0);
    }

    ErrVal resetEndPos()
    {
        m_uiSize = m_uiUsableSize - (m_pcT - m_pcOrigT);
        return (0);
    }

private:
    T* m_pcT;
    T* m_pcOrigT;
    unsigned int m_uiSize;
    unsigned int m_uiUsableSize;
};



template< class T >
class MemAccessList
{
public:
    MemAccessList() : m_uiSize( 0 ) {}

    MemAccessList( const MemAccessList< T >& rcMemAccessList ) : m_uiSize( 0 )
    {
        m_uiSize = rcMemAccessList.m_uiSize;
        typename MemAccessorList::const_iterator pcPair;
        for( pcPair = rcMemAccessList.m_cMemAccessorList.begin(); pcPair != rcMemAccessList.m_cMemAccessorList.end(); pcPair++  )
        {
            m_cMemAccessorList.push_back( MemAccessor< T >( pcPair->data(), pcPair->size(), pcPair->origData(), pcPair->usableSize() ) );
        }
    }

    MemAccessList( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 ) : m_uiSize( 0 )
    {
        if( NULL == pcOrigT ) { pcOrigT = pcT; }
        if( 0 == uiUsableSize ) { uiUsableSize = uiSize; }
        push_back( pcT, uiSize, pcOrigT, uiUsableSize );
    }

    virtual ~MemAccessList() {}

    MemAccessList< T >& operator=( const MemAccessList< T >& rcMemAccessList )
    {
        // paranoia
        ROTRS( this == &rcMemAccessList, *this );

        // reset class
        reset();

        m_uiSize = rcMemAccessList.m_uiSize;
        typename MemAccessorList::const_iterator pcPair;
        for( pcPair = rcMemAccessList.m_cMemAccessorList.begin(); pcPair != rcMemAccessList.m_cMemAccessorList.end(); pcPair++  )
        {
            m_cMemAccessorList.push_back( MemAccessor< T >( pcPair->data(), pcPair->size(), pcPair->origData(), pcPair->usableSize() ) );
        }

        return *this;
    }

public:
    void set( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 )
    {
        reset();
        if( NULL == pcOrigT ) { pcOrigT = pcT; }
        if( 0 == uiUsableSize ) { uiUsableSize = uiSize; }
        push_back( pcT, uiSize, pcOrigT, uiUsableSize );
    }

    void set( MemAccessor< T >& rcMemAccessor )
    {
        reset();
        push_back( rcMemAccessor );
    }

    void copyPayload( T*& rpcT, unsigned int& ruiSize )
    {
        if( 0 == m_uiSize )
        {
            rpcT = NULL;
            ruiSize = 0;
            return;
        }

        ruiSize = m_uiSize;
        rpcT = new T[ruiSize];
        ROTV( NULL == rpcT );

        unsigned int uiPos = 0;
        MemAccessorListIterator pcPair;
        for( pcPair = m_cMemAccessorList.begin(); pcPair != m_cMemAccessorList.end(); pcPair++ )
        {
            ROTV( uiPos + pcPair->size() > ruiSize );
            memcpy( &rpcT[uiPos], pcPair->data(), sizeof(T) * pcPair->size() );
            uiPos += pcPair->size();
        }

    }

    unsigned int listSize() const { return m_cMemAccessorList.size(); }

    T* entryData( unsigned int uiEntryPos )
    {
        // check if we have more than one entry
        ROTR( uiEntryPos >= m_cMemAccessorList.size(), NULL );
        typename MemAccessorList::const_iterator pcMemAccessor;
        for( pcMemAccessor = m_cMemAccessorList.begin(); uiEntryPos-- != 0; pcMemAccessor++ )
        ;
        return pcMemAccessor->data();
    }

    unsigned int entrySize( unsigned int uiEntryPos )
    {
        // check if we have more than one entry
        ROTR( uiEntryPos >= m_cMemAccessorList.size(), 0 );
        typename MemAccessorList::const_iterator pcMemAccessor;
        for( pcMemAccessor = m_cMemAccessorList.begin(); uiEntryPos-- != 0; pcMemAccessor++ )
        ;
        return pcMemAccessor->size();
    }

    unsigned int entryByteSize( unsigned int uiEntryPos ) const { return sizeof( T ) * entrySize( uiEntryPos ); }
    unsigned int size() const { return m_uiSize; }
    unsigned int byteSize() const { return m_uiSize * sizeof( T ); }

public:
    void reset() { m_cMemAccessorList.clear(); m_uiSize = 0; }

public:
    void push_back( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 )
    {
        if( NULL == pcOrigT ) { pcOrigT = pcT; }
        if( 0 == uiUsableSize ) { uiUsableSize = uiSize; }
        m_cMemAccessorList.push_back( MemAccessor< T >( pcT, uiSize, pcOrigT, uiUsableSize ) );
        m_uiSize += uiSize;
    }

    void push_back( MemAccessor< T >& rcMemAccessor )
    {
        m_cMemAccessorList.push_back( rcMemAccessor );
        //m_uiSize += uiSize; // leszek: uiSize is not defined
        m_uiSize += rcMemAccessor.m_uiSize; // leszek: this seems to be the intention
    }

    void push_front( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 )
    {
        if( NULL == pcOrigT ) { pcOrigT = pcT; }
        if( 0 == uiUsableSize ) { uiUsableSize = uiSize; }
        m_cMemAccessorList.push_front( MemAccessor< T >( pcT, uiSize, pcOrigT, uiUsableSize ) );
        m_uiSize += uiSize;
    }
    
    void push_front( MemAccessor< T >& rcMemAccessor )
    {
        m_cMemAccessorList.push_front( rcMemAccessor );
        //m_uiSize += uiSize; // leszek: uiSize is not defined
        m_uiSize += rcMemAccessor.m_uiSize; // leszek: this seems to be the intention
    }
    
private:
    typedef std::list< MemAccessor< T > > MemAccessorList;
    typedef typename MemAccessorList::iterator MemAccessorListIterator;
    
private:
    MemAccessorList m_cMemAccessorList;
    unsigned int m_uiSize;
};


template< class T >
class MemCont : public MemIf< T >
{
public:
    class MemContHelper
    {
    public:
        MemContHelper() {}
        MemContHelper( MemIf< T >& rcMemIf )
        {
            rcMemIf.release( m_pcT, m_uiSize, m_pcOrigT, m_uiUsableSize );
        }
        virtual ~MemContHelper() {}

    public:
        MemContHelper& operator=( MemIf< T >& rcMemIf )
        {
            rcMemIf.release( m_pcT, m_uiSize, m_pcOrigT, m_uiUsableSize );
            return *this;
        }

    public:
        T* data() const { return m_pcT; }
        T* origData() const { return m_pcOrigT; }
        unsigned int size() const { return m_uiSize; }
        unsigned int usableSize() const { return m_uiUsableSize; }

    private:
        T* m_pcT;
        T* m_pcOrigT;
        unsigned int m_uiSize;
        unsigned int m_uiUsableSize;
    };

public:
    MemCont() :
    m_pcT( NULL ), m_pcOrigT( NULL ), m_uiSize( 0 ), m_uiUsableSize( 0 ) {}

    MemCont( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 ) :
    m_pcT( pcT ), m_pcOrigT( pcOrigT ), m_uiSize( uiSize ) , m_uiUsableSize( uiUsableSize )
    {
        if( NULL == m_pcOrigT ) m_pcOrigT = m_pcT;
        if( 0 == m_uiUsableSize ) m_uiUsableSize = uiSize;
    }

    MemCont( const MemCont< T >& rcMemCont ) :
    m_pcT( NULL ), m_pcOrigT( NULL ), m_uiSize( 0 ), m_uiUsableSize( 0 )
    {
        if( (0 != rcMemCont.m_uiSize) && (NULL != rcMemCont.m_pcT) )
        {
            m_uiSize = rcMemCont.m_uiSize;
            m_uiUsableSize = rcMemCont.m_uiUsableSize;
            m_pcT = new T[m_uiSize];
            AOT( NULL == m_pcT );
            m_pcOrigT = m_pcT;
            memcpy( m_pcT, rcMemCont.m_pcT, sizeof( T ) * m_uiSize );
        }
    }

    MemCont( const MemAccessor< T >& rcMemAccessor ) :
    m_pcT( NULL ), m_pcOrigT( NULL ), m_uiSize( 0 ), m_uiUsableSize( 0 )
    {
        if( (0 != rcMemAccessor.size()) && (NULL != rcMemAccessor.data()) )
        {
            m_uiSize = rcMemAccessor.size();
            m_uiUsableSize = rcMemAccessor.usableSize() ;
            m_pcT = rcMemAccessor.data();
            m_pcOrigT = rcMemAccessor.origData();
        }
    }

    MemCont( const MemContHelper& rcMemContHelper ) :
    m_pcT( NULL ), m_pcOrigT( NULL ), m_uiSize( 0 ), m_uiUsableSize( 0 )
    {
        if( (0 != rcMemContHelper.size()) && (NULL != rcMemContHelper.data()) )
        {
            m_uiSize = rcMemContHelper.size();
            m_uiUsableSize = rcMemContHelper.usableSize() ;
            m_pcT = rcMemContHelper.data();
            m_pcOrigT = rcMemContHelper.origData();
        }
    }

    virtual ~MemCont() { if( m_pcOrigT ) { delete[] m_pcOrigT; } }

public:
    MemCont< T >& operator=( const MemCont< T >& rcMemCont )
    {
        if( this == &rcMemCont ) { return *this; }

        // delete memory if allocated
        if( m_pcOrigT ) { delete[] m_pcOrigT; }

        if( (0 != rcMemCont.m_uiSize) && (NULL != rcMemCont.m_pcT) )
        {
            m_uiSize = rcMemCont.m_uiSize;
            m_uiUsableSize = rcMemCont.m_uiUsableSize;
            m_pcT = new T[m_uiSize];
            AOT( NULL == m_pcT );
            m_pcOrigT = m_pcT;
            memcpy( m_pcT, rcMemCont.m_pcT, sizeof( T ) * m_uiSize );
        }
        else
        {
            m_pcT = NULL;
            m_pcOrigT = NULL;
            m_uiSize = 0;
            m_uiUsableSize = 0;
        }

        return *this;
    }

    MemCont< T >& operator=( const MemAccessor< T >& rcMemAccessor )
    {
        if( m_pcOrigT == rcMemAccessor.origData() )
        {
            m_uiUsableSize = rcMemAccessor.usableSize() ;
            m_pcT = rcMemAccessor.data();
            m_pcOrigT = rcMemAccessor.origData();
        }
        else
        {
            if( m_pcOrigT ) { delete[] m_pcOrigT; }
            if( (0 != rcMemAccessor.size()) && (NULL != rcMemAccessor.data()) )
            {
                m_uiSize = rcMemAccessor.size();
                m_uiUsableSize = rcMemAccessor.usableSize() ;
                m_pcT = rcMemAccessor.data();
                m_pcOrigT = rcMemAccessor.origData();
            }
            else
            {
                m_pcT = NULL;
                m_pcOrigT = NULL;
                m_uiSize = 0;
                m_uiUsableSize = 0;
            }
        }

        return *this;
    }

    MemCont< T >& operator=( const MemContHelper& rcMemContHelper )
    {
        if( m_pcOrigT == rcMemContHelper.origData() )
        {
            m_uiUsableSize = rcMemContHelper.usableSize() ;
            m_pcT = rcMemContHelper.data();
            m_pcOrigT = rcMemContHelper.origData();
        }
        else
        {
            if( m_pcOrigT ) { delete[] m_pcOrigT; }
            if( (0 != rcMemContHelper.size()) && (NULL != rcMemContHelper.data()) )
            {
                m_uiSize = rcMemContHelper.size();
                m_uiUsableSize = rcMemContHelper.usableSize() ;
                m_pcT = rcMemContHelper.data();
                m_pcOrigT = rcMemContHelper.origData();
            }
            else
            {
                m_pcT = NULL;
                m_pcOrigT = NULL;
                m_uiSize = 0;
                m_uiUsableSize = 0;
            }
        }

        return *this;
    }

public:
    MemType getMemType() const { return MEM_CONT; }

    void set( MemIf< T >& rcMemIf )
    {
        ROTV( this == &rcMemIf );
        if( m_pcOrigT ) { delete[] m_pcOrigT; }
        rcMemIf.release( m_pcT, m_uiSize, m_pcOrigT, m_uiUsableSize );
    }

    void set( T* pcT, unsigned int uiSize, T* pcOrigT=NULL, unsigned int uiUsableSize=0 )
    {
        if( NULL == pcOrigT ) { pcOrigT = pcT; }
        if( 0 == uiUsableSize ) { uiUsableSize = uiSize; }
        if( m_pcOrigT != pcOrigT ) { if( m_pcOrigT ) { delete[] m_pcOrigT; } }
        m_pcT = pcT;
        m_pcOrigT = pcOrigT;
        m_uiSize = uiSize;
        m_uiUsableSize = uiUsableSize;
    }

    void release( T*& rpcT, unsigned int& ruiSize )
    {
        if( m_pcT == m_pcOrigT )
        {
            rpcT = m_pcT;
            ruiSize = m_uiSize;
        }
        else
        {
            if( 0 != m_uiSize )
            {
                ruiSize = m_uiSize;
                rpcT = new T[ruiSize];
                AOT( NULL == rpcT );
                memcpy( rpcT, m_pcT, sizeof( T ) * ruiSize );
            }
            else
            {
                rpcT = NULL;
                ruiSize = 0;
            }
            if( m_pcOrigT ) { delete[] m_pcOrigT; }
        }

        m_pcT = NULL;
        m_pcOrigT = NULL;
        m_uiSize = 0;
    }

    void release( T*& rpcT, unsigned int& ruiSize, T*& rpcOrigT, unsigned int& ruiUsableSize )
    {
        rpcT = m_pcT;
        ruiSize = m_uiSize;
        rpcOrigT = m_pcOrigT;
        ruiUsableSize = m_uiUsableSize;

        m_pcT = NULL;
        m_pcOrigT = NULL;
        m_uiSize = 0;
        m_uiUsableSize = 0;
    }

    void release( MemAccessor< T >& rcMemAccessor )
    {
        rcMemAccessor.set( m_pcT, m_uiSize, m_pcOrigT, m_uiUsableSize );

        m_pcT = NULL;
        m_pcOrigT = NULL;
        m_uiSize = 0;
        m_uiUsableSize = 0;
    }

    void deleteData() { if( m_pcOrigT ) { delete[] m_pcOrigT; } m_pcOrigT = NULL; m_pcT = NULL; m_uiSize = 0; m_uiUsableSize = 0; }
    void reset() { m_pcOrigT = NULL; m_pcT = NULL; m_uiSize = 0; m_uiUsableSize = 0; }

    T* data() const { return m_pcT; }
    T* origData() const { return m_pcOrigT; }
    unsigned int size() const { return m_uiSize; }
    unsigned int usableSize() const { return m_uiUsableSize; }
    unsigned int byteSize() const { return sizeof( T ) * m_uiSize; }
    unsigned int usableByteSize() const { return sizeof( T ) * m_uiUsableSize; }
    
    void setMemAccessor( MemAccessor< T >& rcMemAccessor ) { rcMemAccessor.set( m_pcT, m_uiSize, m_pcOrigT, m_uiUsableSize ); }
    
public:
    ErrVal increasePos( unsigned int uiPos )
    {
        ROT( uiPos >= m_uiSize );
        m_pcT += uiPos;
        m_uiSize -= uiPos;
        return (0);
    }
    ErrVal decreasePos( unsigned int uiPos )
    {
        ROT( m_pcT - uiPos < m_pcOrigT );
        m_pcT -= uiPos;
        m_uiSize += uiPos;
        return (0);
    }
    
    ErrVal resetPos()
    {
        m_uiSize += m_pcT - m_pcOrigT;
        m_pcT = m_pcOrigT;
        return (0);
    }
    
    ErrVal increaseEndPos( unsigned int uiPos )
    {
        ROT( uiPos > (m_uiUsableSize - m_uiSize) );
        m_uiSize += uiPos;
        return (0);
    }
    
    ErrVal decreaseEndPos( unsigned int uiPos )
    {
        ROT( uiPos > m_uiSize );
        m_uiSize -= uiPos;
        return (0);
    }
    
    ErrVal resetEndPos()
    {
        m_uiSize = m_uiUsableSize - (m_pcT - m_pcOrigT);
        return (0);
    }
    
private:
    T* m_pcT;
    T* m_pcOrigT;
    unsigned int m_uiSize;
    unsigned int m_uiUsableSize;
};


typedef MemCont< unsigned char > BinData;
typedef MemAccessor< unsigned char > BinDataAccessor;

