var num_pixels_width = 20;

var title = "PixWars";

var cacheBuffer = 1000000;

var pixelUnblockText = "Pixel unblocks in ";
var pixelBlockedText = "Pixel is blocked now";

var colorPickCellSize = 30;
var colorPickerWidth = 270;
var colorPickerHeight = 80;
var colorPickerBorderColor = "lightgrey";

var selectedBorderColor = "grey";
var pixelMargins = 0.5;
var pixelBorderColor = "grey";

var colorsEnum = {
    FIRST: 0,
    SECOND: 1,
    THIRD: 2,
    FOURTH: 3,
    FIFTH: 4,
    SIXTH: 5,
    SEVENTH: 6,
    EIGHTH: 7,
    NINETH: 8,
    TENTH: 9,
    ELEVENTH: 10,
    TWELFTH: 11,
    THIRTEENTH: 12,
    FOURTEENTH: 13,
    FIFTEENTH: 14,
    SIXTEENTH: 15,
    properties: {
        0: {color: "#fff", number: 0},
        1: {color: "#eee", number: 1},
        2: {color: "#ddd", number: 2},
        3: {color: "#ccc", number: 3},
        4: {color: "#bbb", number: 4},
        5: {color: "#aaa", number: 5},
        6: {color: "#999", number: 6},
        7: {color: "#888", number: 7},
        8: {color: "#777", number: 8},
        9: {color: "#666", number: 9},
        10: {color: "#555", number: 10},
        11: {color: "#444", number: 11},
        12: {color: "#333", number: 12},
        13: {color: "#222", number: 13},
        14: {color: "#111", number: 14},
        15: {color: "#000", number: 15}
    }
};

var colors = [colorsEnum.FIRST, colorsEnum.SECOND, colorsEnum.THIRD, colorsEnum.FOURTH,
              colorsEnum.FIFTH, colorsEnum.SIXTH, colorsEnum.SEVENTH, colorsEnum.EIGHTH,
              colorsEnum.NINETH, colorsEnum.TENTH, colorsEnum.ELEVENTH, colorsEnum.TWELFTH,
              colorsEnum.THIRTEENTH, colorsEnum.FOURTEENTH, colorsEnum.FIFTEENTH, colorsEnum.SIXTEENTH];

