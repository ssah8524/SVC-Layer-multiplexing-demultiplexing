
#include "ReAttach.h"
using namespace std;

int main( int argc, char** argv)
{
    Parameter  cParameter;
    cParameter.init( argc, argv );

    ReAttach* cAttach = new ReAttach;
    cAttach->init(&cParameter);

    cAttach->extract();
    cAttach->destroy();
    return 0;
}
