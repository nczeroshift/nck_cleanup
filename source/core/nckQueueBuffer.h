
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_QUEUE_BUFFER_H
#define NCK_QUEUE_BUFFER_H

#include "nckCoreConfig.h"
#include <stdint.h>
#include <string>

_CORE_BEGIN

/**
* Memory data queue.
*/
class QueueBuffer{
public:
    QueueBuffer();
    QueueBuffer(uint64_t maxCapacity);
    ~QueueBuffer();
    
    void Clear();
    uint64_t Push(const std::string & str);
    uint64_t Push(void * data, uint64_t size);
    uint64_t Size();
    uint64_t Pop(void * targetData, uint64_t targetSize);
    
    /// Convert buffer data to a string.
    uint64_t ToString(std::string * str);
    void * GetData();
    void * CloneData();
private:
    bool m_Resize;
    void * m_Buffer;
    uint64_t m_Capacity;
    uint64_t m_Size;
};

_CORE_END

#endif


