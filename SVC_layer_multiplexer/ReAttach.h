//
//  ReAttach.h
//  
//
//  Created by Amir Hosseini on 8/12/16.
//
//

#ifndef ReAttach_h
#define ReAttach_h

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include "ReadWrite.h"

class Parameter
{
public:
    Parameter          ();
    ~Parameter ();

    void setNumEnh (int newNum) {numEnh = newNum;}

    const std::string& getBaseFile()const { return m_cBaseFile;}
    std::string* getEnhFile()const { return m_cEnhFile;}

    unsigned int getNumEnh () const {return numEnh;}

    std::string getBaseFileName() const { return m_cBaseFile;}
    std::string* getEnhFileName() const { return m_cEnhFile;}
    ///Support for layer separation
    ErrVal  init( int argc,char**  argv);

protected:

    ErrVal  xPrintUsage         (char**  argv);
    unsigned int            numEnh;
    std::string           m_cBaseFile;
    std::string*          m_cEnhFile;
    
};


class ReAttach
{
public:
    
    ReAttach();
    ~ReAttach();
     
    ErrVal init(Parameter* pcExtractorParameter);
    ErrVal extract();
    ErrVal searchNal();
    ErrVal reAttach(BinData* file, int& position, unsigned int& Nal, unsigned int& TmpNal);
    ErrVal destroy();
    unsigned int findMin(unsigned int array[]);

protected:
    
    ReadBitstreamFull** ReadFiles;
    WriteBitstreamToFile* WriteFinal;
    unsigned char* finalBuffer;
    unsigned int finderPosition;
    Parameter* m_pcExtractorParameter;
};
#endif /* ReAttach_hpp */

