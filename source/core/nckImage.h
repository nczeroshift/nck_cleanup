
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_IMAGE_H
#define NCK_IMAGE_H

#include "nckCoreConfig.h"
#include "nckDataIO.h"
#include <string>

_CORE_BEGIN

enum PixelFormat{
    PIXEL_FORMAT_ALPHA_8B,
    PIXEL_FORMAT_RGB_8B,
    PIXEL_FORMAT_RGBA_8B,
};

enum ImageType{
    IMAGE_TYPE_TGA,
    IMAGE_TYPE_PNG,
    IMAGE_TYPE_JPEG,
    IMAGE_TYPE_BMP,
    IMAGE_TYPE_TBIN,
    IMAGE_TYPE_RAW,
};

/**
 * Image class with methods to create, loading and saving.
 */
class Image
{
public:
    Image();
    ~Image();

    /**
    * Create blank image with format.
    */
    static Image * Create(int width, int height, PixelFormat format);

    /**
     * Load image from path.
     * @throws Exception on image loading failure. 
     */
    static Image * Load(const std::string & filename);

    /**
    * Load image from data stream reader.
    */
    static Image * Load(ImageType type, DataReader * reader);

    /**
    * Generate circle image.
    */
    static Image * GenerateCircle(int size, bool expand, PixelFormat format);

    /**
    * Generate check (for checkbox) image.
    */
    static Image * GenerateCheck(int size, PixelFormat format);

    /**
     * Get image type from filename extension.
     * @return Image file type
     * @throws Exception if extension is unsupported.
     */
    static ImageType GetImageType(const std::string & filename);

    /// Get image width.
    int GetWidth();

    /// Get image height.
    int GetHeight();

    PixelFormat GetFormat();
    ImageType GetType();

    /// Get raw data image pointer.
    unsigned char * GetData();

    /**
     * Save image data to file.
     */
    void Save(const std::string & filename, ImageType type, float quality = 100.0);

    /**
     * Save image to memory.
     * @param type Output image type, BMP and JPEG supported.
     * @param quality Compression quality, JPEG only.
     * @param output Reference to pointer where the image file will be written.
     * @return Image file size.
     */
    uint64_t Save(ImageType type, float quality, unsigned char ** output);

private:
    int width;
    int height;
    PixelFormat format;
    ImageType type;
    unsigned char * data;
};

/// Copy raw image data between different formats.
bool ImageCopy(int srcWidth, int srcHeight ,PixelFormat srcFormat, unsigned char * src,
     int dstWidth, int dstHeight, PixelFormat dstFormat, unsigned char * dst);

_CORE_END

#endif
