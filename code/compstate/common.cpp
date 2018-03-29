#include "common.h"

vector2d algo::rect_center(const cv::Rect2d &rect)  {
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

vector2d algo::perp_intersect(
    const vector2d &c,
    const vector2d &p0,
    const vector2d &p1
) {
    double dy1 = p1.y() - p0.y();
    double dx1 = p1.x() - p0.x();
    double m_l = dy1 / dx1;
    double m_r = -1 / m_l;
    // y - y0 = (x - x0) * m_l
    // y - yc = (x - xc) * m_r
    // y0 - yc = (x - xc) * m_r + (x0 - x) * m_l
    // y0 - yc + xc * m_r - x0 * m_l = x * (m_r - m_l)
    double xi = (p0.y() - c.y() + c.x() * m_r - p0.x() * m_l) / (m_r - m_l);
    // y - y0 = (x - x0) * m_l
    // yi = (xi - x0) * m_l + y0
    double yi = (xi - p0.x()) * m_l + p0.y();
    return {xi, yi};
}
