#ifndef MINOTAUR_CPP_CONVERTER_H
#define MINOTAUR_CPP_CONVERTER_H

#include <QObject>

// Forward declarations
class ImageViewer;
namespace cv {
    class UMat;
}

/**
 * This object is responsible for taking processed video frames
 * from the image pipeline and convert them to QImage for display
 * on the ImageViewer.
 */
class Converter : public QObject {
Q_OBJECT

public:
    explicit Converter(ImageViewer *image_viewer);

    /**
     * Signal emitted when a QImage has been produced.
     *
     * @param img converted QImage
     */
    Q_SIGNAL void image_ready(const QImage &img);

    /**
     * Slot to receive a processed frame to convert.
     *
     * @param frame processed frame to convert
     */
    Q_SLOT void process_frame(const cv::UMat &frame);

    /**
     * Grab the number of frames processed since the last time
     * this function was called, then reset the frame count to zero
     *
     * @return number of frames since lasst call
     */
    int get_and_reset_frames();

    /**
     * Converter will scale the cv::Mat up to fit the ImageViewer dimensions.
     * This scale value is saved so that coordinates can be scaled properly.
     *
     * @return the previous image scale value
     */
    double get_previous_scale() const;

private:
    /**
     * Number of frames processed since last call to get_and_reset_frames().
     */
    int m_frames;
    /**
     * Previous scale on Mat to QImage.
     */
    double m_scale;
    /**
     * Reference to ImageViewer used to poll width and height for scaling.
     */
    ImageViewer *m_image_viewer;
};


#endif //MINOTAUR_CPP_CONVERTER_H
