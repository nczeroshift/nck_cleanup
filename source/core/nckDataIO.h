
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_DATAIO_H_
#define NCK_DATAIO_H_

#include "nckCoreConfig.h"
#include <stdint.h>
#include <string>

_CORE_BEGIN

/**
 * Get absolute path for toolkit data folder.
 */
std::string GetDataFolder();

/**
 * Retreive file extension part from filename.
 * @return String with file extension, empty otherwise.
 */
std::string FindExtension(const std::string & filename);

/**
 * Resolve file system path from URL scheme. 
 */
std::string ResolveFilename(const std::string & filename);

/**
* Resolve sub path to a file.
*/
std::string ResolveSubPath(const std::string & fileName);

/** 
 * Get file last modified date
 */
uint64_t GetFileLastModified(const std::string & filename);

/**
 * File seek mode.
 */
enum SeekOffsetMode{
    SEEK_OFFSET_BEGIN,
    SEEK_OFFSET_END,
    SEEK_OFFSET_CURRENT,
};

/**
 * Interface data reader class.
 */
class DataReader{
public:
    virtual ~DataReader(){};

    /**
    * Open a file reader from a file or mounted tar
    * @return NULL if file not found or open failure.
    */
    static DataReader * Open(const std::string & filename);

    /**
     * Read data from stream.
     * @return Number of bytes read, 0 on failure.
     */
    virtual size_t Read(void * data, size_t size) = 0;

    /**
     * Read string from binary stream.
     * @param str Reference to output string.
     * @return True if the string was correctly read.
     */
    virtual bool Read(std::string * str) = 0;

    /**
     * Tell current position in the stream.
     * @return Returns -1 if the end of the stream is reached.
     */
    virtual uint64_t Tell() = 0;

    /**
     * Seek to position in the stream.
     * @return True if seek was successfull.
     */
    virtual bool Seek(int64_t pos, SeekOffsetMode mode) = 0;

    /**
    * Return stream total length.
    */
    virtual uint64_t Length() = 0;
};

/**
 * Data reader using a file stream.
 */
class FileReader: public virtual DataReader{
public:
    FileReader();
    virtual ~FileReader();

    /**
     * Open a file reader from path.
     * @return NULL if file not found or open failure.
     */
    static FileReader * Open(const std::string & filename);
 
    /**
     * Get file size on disk from path.
     * @return Returns file size, 
     */
    static uint64_t Size(const std::string & filename);

    /**
     * Check if file exists on path.
     */
    static bool Exists(const std::string & filename);

    /**
    * Read string from binary.
    */
    bool Read(std::string * str);
    
    /// Return the current read position in bytes.
    uint64_t Tell();

    /// Seek within the file.
    bool Seek(int64_t pos, SeekOffsetMode mode);

    /// Returns the number of bytes read, 0 if an error happen.
    size_t Read(void * data, size_t size);

    /// File length in bytes.
    uint64_t Length();
private:
    std::string fPath;
    FILE * fHandle;
};

/**
 * Class for data reading from a byte array.
 */
class MemoryReader: public virtual DataReader{
public:
    /**
     * Constructor
     * @param data Byte array pointer
     * @param size Byte array size
     * @para copy If true, copy all data and released it on destructor.
     */
    MemoryReader(uint8_t *data, uint64_t size, bool copy);

    virtual ~MemoryReader();
    uint64_t GetSize();
    size_t Read(void * data, size_t size);
    bool Read(std::string * str);
    uint64_t Tell();
    bool Seek(int64_t pos, SeekOffsetMode mode);
    uint64_t Length();
protected:
    bool mCopy;
    uint64_t mStart;
    uint64_t mSize;
    uint8_t * mData;
};

/**
 * TAR files class reader.
 */
class TarReader{
public:
    class Entry : public MemoryReader{
    public:
        virtual ~Entry();

        /// Get entry filename title.
        std::string GetName();

    protected:
        friend class TarReader;

        Entry(const std::string & name,uint8_t *data,size_t size);

        std::string mName;
    };

    TarReader(DataReader * reader);
    ~TarReader();

    /**
    * Mounts this tar reader as a virtual file system. 
    */
    void Mount();

    /**
     * Returns the next archive file inside the Tar file.
     */
    Entry * Next();
    
    /**
    * Returns the entry with the filename.
    */
    Entry * Find(const std::string & filename);

private:
    DataReader * fReader;
};

/**
 * Interface class for data writer.
 */
class DataWriter{
public:
    virtual ~DataWriter(){}

    /**
     * Write data to stream.
     * @eturn Number of bytes writen to stream, 0 on failure.
     */
    virtual size_t Write(uint8_t * data, size_t size) = 0;

    /// Return current write position.
    virtual uint64_t Tell() = 0;

    /// Change writing position.
    virtual bool Seek(int64_t pos, SeekOffsetMode mode) = 0;
};

/**
 * Implementation for a file writer.
 */
class FileWriter : public DataWriter{
public:
    FileWriter();
    virtual ~FileWriter();

    static FileWriter * Open(const std::string & filename, bool append = false);

    /**
    * Write data to stream.
    * @return Number of bytes writen to stream, 0 on failure.
    */
    virtual size_t Write(uint8_t * data, size_t size);

    /// Return current write position.
    virtual uint64_t Tell();

    /// Return file length.
    virtual uint64_t Length();

    /// Change writing position.
    virtual bool Seek(int64_t pos, SeekOffsetMode mode);
private:
    uint64_t m_Length;
    FILE * fHandle;
};

_CORE_END

#endif
