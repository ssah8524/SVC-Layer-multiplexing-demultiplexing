
#include "ReAttach.h"

//TMM_FIX {
#define equal(a,b)  (!strcasecmp((a),(b)))
//TMM_FIX }

Parameter::Parameter()
: m_cBaseFile      () // name of output file
, m_cEnhFile       ()
{
}

Parameter::~Parameter()
{
}

ErrVal Parameter::init( int argc,char**  argv )
{

#define EXIT(x,m) {if(x){printf("\n%s\n",m);xPrintUsage(argv);}}

    //===== get file names and set parameter =====
    EXIT( argc < 2, "No arguments specified" );
    m_cBaseFile       = argv[1];  // The name of the base layer file

    if(argc > 2)
    {
        argv += 1;
        argc -= 1;

        //===== get the number of enhancement layers to be extracted and their names ====
        if(equal(argv[1],"-n"))
        {
            numEnh = atoi(argv[2]);

            if(numEnh != 0)
            {
                m_cEnhFile = new std::string[numEnh];

                EXIT(argc < numEnh + 2,"Not enough Enhancement layers specfied");
                argv += 2;
                argc -= 2;

                for(int n = 0; n < numEnh; n++)
                m_cEnhFile[n] = argv[n + 1];

                argv += numEnh;
                argc -= numEnh;
            }
            else
            {
                argv += 1;
                argc -= 1;
            }
        }
    }
    else
    {
        numEnh = 0;
    }
    return 0;
#undef EXIT
}



ErrVal Parameter::xPrintUsage(char**  argv)
{
    printf("1) First argument must be the name of the base layer file\n");
    printf("2) It must be followed by '-n x', where x specifies the number of enhancement layers \n");
    printf("3) After that, the x enhancement layers must be listed by their name (Don't need to be in order). \n");
    assert (0);
}




