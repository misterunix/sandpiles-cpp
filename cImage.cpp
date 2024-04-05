#include "cImage.hh"

// class to hold an image

/**
 * @brief Construct a new cImage::cImage object
 *
 */
cImage::cImage()
{
    m_img = NULL; // no image
    m_width = 0;  // no width
    m_height = 0; // no height
}

/**
 * @brief Construct a new cImage::cImage object with width and height
 *
 * @param t_w width of the image
 * @param t_h height of the image
 */
cImage::cImage(int t_w, int t_h)
{
    m_img = gdImageCreate(t_w, t_h); // create the image

    // check for no image
    if (m_img == NULL)
    {
        std::cout << "Cannot create image" << std::endl;
        return;
    }

    initColors();                                                        // initialize the colors
    m_width = t_w;                                                       // set the width
    m_height = t_h;                                                      // set the height
    gdImageFilledRectangle(m_img, 0, 0, m_width, m_height, m_colors[0]); // fill the image with black
}

/**
 * @brief Destroy the cImage::cImage object
 *
 */
cImage::~cImage()
{
    gdImageDestroy(m_img); // destroy the image
}

/**
 * @brief set the width and height of the image
 *
 * @param t_w width of the image
 * @param t_h height of the image
 */
void cImage::setWidthHeight(int t_w, int t_h)
{
    cImage(t_w, t_h);
}

/**
 * @brief save the current image as a jpeg to a file
 *
 * @param t_i index of the image
 * @param t_grains number of grains
 * @return true if successful
 * @return false if not successful
 */
bool cImage::saveImage(int t_i, int t_grains)
{
    std::string fn = "img/sandpile-" + std::to_string(m_width) + "-" + std::to_string(m_height) + "-" +
                     std::to_string(t_grains) + "-" + std::to_string(t_i) + ".jpg";
    m_imgFileName = fn;

    FILE *fp;
    fp = fopen(fn.c_str(), "wb");
    if (fp == NULL)
    {
        std::cout << "Cannot open file " << fn << std::endl;
        return false;
    }
    gdImageJpeg(m_img, fp, 100);
    fclose(fp);
    return true;
}

/**
 * @brief helper function to get the image file name
 *
 * @return std::string
 */
std::string cImage::getImgFileName()
{
    return m_imgFileName;
}

/**
 * @brief helper function to initialize the colors
 *
 */
void cImage::initColors()
{
    if (m_img == NULL)
    {
        std::cout << "initColors, img==NULL" << std::endl;
        return;
    }
    m_colors[0] = gdImageColorAllocate(m_img, 0x00, 0x00, 0x00); // black
    m_colors[1] = gdImageColorAllocate(m_img, 0x00, 0xff, 0x00); // green
    m_colors[2] = gdImageColorAllocate(m_img, 0xff, 0x00, 0xff); // purple
    m_colors[3] = gdImageColorAllocate(m_img, 0xff, 0xd7, 0x00); // gold
    m_colors[4] = gdImageColorAllocate(m_img, 0xff, 0xff, 0xff); // white
}
