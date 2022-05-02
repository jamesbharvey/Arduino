#include <avr/pgmspace.h>

const uint16_t hccell[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12641, 27330, 16833, 2080, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 4192, 29442, 50468, 56838, 52646, 37958, 16866, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 12641, 42019, 58853, 65125, 63110, 65319, 65352, 61096, 42182, 21058, 4224, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 25218, 50468, 63013, 65092, 65093, 65126, 65319, 65383, 65383, 65384, 63208, 50566, 23170, 2048, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 12576, 44099, 60901, 65092, 65092, 65061, 65125, 65255, 65352, 65352, 65383, 65383, 65383, 65351, 59015, 35844, 12641, 0, 0, 0, 0,
0, 0, 0, 0, 16801, 50436, 65093, 65092, 65093, 65061, 65158, 65287, 65352, 65352, 65352, 65352, 65352, 65383, 65383, 65383, 63239, 48485, 23170, 2080, 0, 0,
0, 0, 6304, 31522, 56708, 65092, 65092, 65061, 65061, 65190, 65319, 65352, 65352, 65352, 65352, 65352, 65352, 65384, 65384, 65384, 65384, 65384, 63239, 46341, 18977, 4192,
6304, 31523, 52516, 63013, 65092, 65060, 65060, 65093, 65254, 65351, 65352, 65352, 65352, 65352, 65352, 65352, 65352, 65384, 65384, 65384, 65385, 65385, 65353, 65352, 63207, 50566,
25283, 58821, 65061, 65093, 65092, 65092, 65125, 65287, 65352, 65352, 65352, 65352, 65352, 65352, 65320, 65352, 65352, 65384, 65384, 65384, 65385, 65386, 65357, 65356, 65385, 61096,
46245, 63013, 65125, 65125, 65125, 65157, 65255, 65352, 65352, 65352, 65352, 65352, 65352, 65352, 65320, 65352, 65352, 65352, 65352, 65352, 65353, 65390, 65426, 65392, 65387, 54824,
50502, 65158, 65190, 65222, 65222, 65254, 65351, 65352, 65352, 65352, 65352, 65352, 65320, 65320, 65320, 65352, 65352, 65320, 65352, 65352, 65387, 65427, 65428, 65394, 65356, 59016,
52614, 65222, 65254, 65286, 65287, 65351, 65352, 65352, 65352, 65352, 65351, 65351, 65351, 65351, 65319, 65320, 65320, 65352, 65352, 65384, 65388, 65426, 65428, 65394, 65358, 61161,
56839, 65222, 65254, 65287, 65319, 65352, 65384, 65384, 65384, 65383, 65383, 65351, 65351, 65351, 65319, 65319, 65319, 65351, 65384, 65385, 65420, 65425, 65427, 63314, 63278, 61129,
56871, 65222, 65254, 65287, 65319, 65352, 65384, 65384, 65384, 65351, 65351, 65351, 65351, 65351, 65319, 65319, 65351, 65352, 65384, 65385, 65420, 65425, 65395, 63314, 63277, 59048,
58951, 65223, 65255, 65319, 65351, 65384, 65384, 65384, 65352, 65351, 65351, 65351, 65351, 65351, 65319, 65351, 65351, 65384, 65384, 65386, 65421, 65425, 65395, 63314, 65325, 56936,
61031, 65223, 65287, 65319, 65351, 65384, 65384, 65352, 65384, 65383, 65383, 65383, 65383, 65351, 65351, 65351, 65384, 65384, 65385, 65386, 65421, 65426, 65428, 63282, 65324, 56872,
58983, 65255, 65287, 65319, 65352, 65352, 65352, 65352, 65383, 65383, 65383, 65383, 65383, 65351, 65351, 65384, 65384, 65384, 65386, 65420, 65424, 65428, 65397, 63282, 63245, 56872,
56871, 65255, 65319, 65319, 65351, 65352, 65352, 65384, 65383, 65383, 65383, 65383, 65383, 65351, 65384, 65384, 65384, 65386, 65387, 65422, 65461, 65429, 63348, 61234, 61133, 52712,
52646, 65287, 65319, 65319, 65351, 65352, 65384, 65384, 65383, 65383, 65383, 65383, 65383, 65383, 65384, 65385, 65386, 65387, 65421, 65459, 65463, 65429, 63316, 61202, 59021, 40071,
48421, 65320, 65352, 65351, 65351, 65384, 65384, 65384, 65384, 65384, 65384, 65383, 65384, 65384, 65384, 65387, 65388, 65423, 65460, 65463, 65430, 63316, 63283, 61199, 46409, 21092,
23203, 56904, 65320, 65384, 65384, 65384, 65384, 65384, 65384, 65384, 65384, 65384, 65384, 65384, 65386, 65421, 65426, 65463, 65463, 65430, 63349, 63315, 63279, 52683, 19012, 4193,
0, 23172, 46343, 63240, 65352, 65352, 65384, 65384, 65384, 65384, 65384, 65384, 65384, 65386, 65421, 65459, 65463, 65463, 65430, 63349, 63315, 63278, 52714, 18979, 0, 0,
0, 0, 12609, 35813, 59015, 65320, 65320, 65352, 65384, 65384, 65384, 65384, 65385, 65421, 65426, 65463, 65463, 65430, 63317, 63346, 63245, 44265, 14786, 0, 0, 0,
0, 0, 0, 8417, 25283, 52679, 65320, 65320, 65320, 65352, 65353, 65386, 65388, 65425, 65462, 65463, 63349, 63316, 65393, 61132, 35847, 8449, 0, 0, 0, 0,
0, 0, 0, 0, 0, 8449, 33732, 56903, 65320, 65321, 65321, 65387, 65423, 65459, 65462, 63349, 63314, 65391, 56907, 25317, 2112, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 14753, 44261, 61128, 65353, 65387, 65423, 65394, 63348, 65393, 65389, 50633, 14786, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 4160, 27395, 52711, 65322, 65389, 65391, 65390, 63243, 44295, 12641, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 2080, 21058, 46374, 63242, 65356, 54825, 31620, 6304, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12641, 33765, 40038, 12641, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//const uint16_t *hccell16 PROGMEM = (uint16_t*)hccell;

const uint8_t hccell_mask[] PROGMEM = {
  0x00, 0x1e, 0x00, 0x00, 
  0x00, 0x7f, 0x00, 0x00, 
  0x00, 0xff, 0xe0, 0x00, 
  0x01, 0xff, 0xf8, 0x00, 
  0x07, 0xff, 0xfc, 0x00, 
  0x0f, 0xff, 0xff, 0x00, 
  0x3f, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0xff, 0xff, 0xff, 0xc0, 
  0x7f, 0xff, 0xff, 0x00, 
  0x3f, 0xff, 0xfe, 0x00, 
  0x1f, 0xff, 0xfc, 0x00, 
  0x07, 0xff, 0xf8, 0x00, 
  0x01, 0xff, 0xe0, 0x00, 
  0x00, 0xff, 0xc0, 0x00, 
  0x00, 0x7f, 0x80, 0x00, 
  0x00, 0x1e, 0x00, 0x00, 

};