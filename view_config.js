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
        1: {color: "#d7d7d7", number: 1},
        2: {color: "#e6e6fa", number: 2},
        3: {color: "#8b00ff", number: 3},
        4: {color: "#0000ff", number: 4},
        5: {color: "#7fc7ff", number: 5},
        6: {color: "#afeeee", number: 6},
        7: {color: "#009b76", number: 7},
        8: {color: "#99ff99", number: 8},
        9: {color: "#ffff99", number: 9},
        10: {color: "#ffbd88", number: 10},
        11: {color: "#fc892c", number: 11},
        12: {color: "#ff00cc", number: 12},
        13: {color: "#f80000", number: 13},
        14: {color: "#c10020", number: 14},
        15: {color: "#000", number: 15}
    }
};

var colors = [colorsEnum.FIRST, colorsEnum.SECOND, colorsEnum.THIRD, colorsEnum.FOURTH,
              colorsEnum.FIFTH, colorsEnum.SIXTH, colorsEnum.SEVENTH, colorsEnum.EIGHTH,
              colorsEnum.NINETH, colorsEnum.TENTH, colorsEnum.ELEVENTH, colorsEnum.TWELFTH,
              colorsEnum.THIRTEENTH, colorsEnum.FOURTEENTH, colorsEnum.FIFTEENTH, colorsEnum.SIXTEENTH];

var serverAdress = "http://138.68.185.254:80";
var updateInterval = 5000; //msec
var failText = "Вас опередили :( Повторите попытку через ";
