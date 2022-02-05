
# 1 "lookuptable.c"

# 11 "lookuptable.h"
extern unsigned int getLookup(unsigned char value);

# 31 "lookuptable.c"
const unsigned int lookup[] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
16, 18, 19, 20, 21, 23, 24, 25, 26, 28, 29, 31, 32, 33, 35, 36,
38, 40, 41, 43, 44, 46, 48, 49, 51, 53, 55, 57, 59, 60, 62, 64,
66, 68, 71, 73, 75, 77, 79, 82, 84, 86, 89, 91, 94, 96, 99, 101,
104, 107, 109, 112, 115, 118, 121, 124, 127, 130, 133, 136, 140, 143, 146, 150,
153, 157, 160, 164, 168, 172, 175, 179, 183, 187, 192, 196, 200, 204, 209, 213,
218, 223, 227, 232, 237, 242, 247, 252, 258, 263, 269, 274, 280, 286, 291, 297,
303, 310, 316, 322, 329, 335, 342, 349, 356, 363, 370, 377, 385, 392, 400, 408,
416, 424, 432, 441, 449, 458, 467, 476, 485, 494, 503, 513, 523, 533, 543, 553,
564, 574, 585, 596, 608, 619, 631, 642, 655, 667, 679, 692, 705, 718, 731, 745,
759, 773, 787, 801, 816, 831, 847, 862, 878, 894, 911, 927, 944, 961, 979, 997,
1015, 1034, 1052, 1072, 1091, 1111, 1131, 1151, 1172, 1194, 1215, 1237, 1259, 1282, 1305, 1329,
1353, 1377, 1402, 1427, 1453, 1479, 1505, 1532, 1560, 1588, 1616, 1645, 1674, 1704, 1735, 1766,
1797, 1829, 1862, 1895, 1929, 1963, 1998, 2034, 2070, 2107, 2144, 2182, 2221, 2260, 2300, 2341,
2382, 2425, 2468, 2511, 2556, 2601, 2647, 2694, 2741, 2790, 2839, 2889, 2940, 2992, 3044, 3098,
3153, 3208, 3265, 3322, 3381, 3440, 3501, 3562, 3625, 3689, 3754, 3820, 3887, 3955, 4024, 4095
};



unsigned int getLookup(unsigned char value) {

return lookup[value];
}
