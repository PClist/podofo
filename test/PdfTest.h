
#include "PdfDefines.h"
#include "PdfError.h"

/* Common defines needed in all tests */
#define TEST_SAFE_OP( x ) eCode = x;\
                     if( eCode.IsError() ) {\
                       eCode.PrintErrorMsg();\
                       return eCode.Error();\
                     }
