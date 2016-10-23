
#include "BStreamExtractor.h"
#include "Extractor.h"
#include "ReAttach.h"

int main( int argc, char** argv)
{
    Bool reBuild = true;
    Extractor*          pcExtractor = NULL;
    ExtractorParameter  cParameter;

    printf( "JSVM %s BitStream Extractor \n\n", _JSVM_VERSION_ );
    
    RNOKRS( cParameter.init       ( argc, argv ),   -2 );
    
    for( Int n = 0; n < 1; n++ )
    {
        RNOKR( Extractor::create    ( pcExtractor ),  -3 );

        RNOKR( pcExtractor->init    ( &cParameter ),  -4 );

        RNOKR( pcExtractor->go      (),               -5 );

        if (reBuild)
        {
            ReAttach*       cAttach = new ReAttach;

            cAttach->init(&cParameter);

            RNOKR( cAttach->extract      (),               -7 );
        
            RNOKR( cAttach->destroy      (),               -8 );
        
        }
        
        RNOKR( pcExtractor->destroy (),               -6 );
        
        
    }
    
    return 0;
}
