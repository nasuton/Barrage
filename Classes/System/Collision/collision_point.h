#ifndef __COLLISION_POINT_H__
#define __COLLISION_POINT_H__

#ifdef __cplusplus
extern "C" {
#endif

inline int collision_bit_separate32(int n) {
	n = (n | (n << 8)) & 0x00ff00ff;
    n = (n | (n << 4)) & 0x0f0f0f0f;
    n = (n | (n << 2)) & 0x33333333;
    return (n | (n << 1)) & 0x55555555;
}

inline int collision_get_2dmortion_number(int x, int y) {
    return (int)(collision_bit_separate32(x) |
                 collision_bit_separate32(y) << 1);
}

inline int collision_get_point_elem(float pos_x, float pos_y, float width,
                                    float height) {
    return collision_get_2dmortion_number((int)(pos_x / width),
                                          (int)(pos_y / height));
}

#ifdef __cplusplus
}
#endif

#endif