
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckUtils.h"
#include "nckDataIO.h"
#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32) || defined (_WIN64)
#include <Windows.h>
#endif

_CORE_BEGIN

bool IsBaseOfTwo(int number){
    bool ret = false;
    for(int i = 0;i<32;i++){
        if(number>>i & 0x1)
        {
            if(!ret){ret = true;
            }
        else
        {
            ret = false;break;}
        }
    }
    return ret;
}

_CORE_END
