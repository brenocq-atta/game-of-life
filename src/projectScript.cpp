//--------------------------------------------------
// Game of life
// projectScript.cpp
// Date: 2022-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"
#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>

namespace rsc = atta::resource;

void Project::onLoad() {
    rsc::Image::CreateInfo info;
    info.width = 200;
    info.height = 200;
    info.format = rsc::Image::Format::RGBA8;
    _gofImage = rsc::create<rsc::Image>("gameOfLife", info);

    clear();
}

void Project::onStart() { init(); }

void Project::onStop() { clear(); }

void Project::onUpdateBefore(float dt) {
    static auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 100;

    // Update every second
    if (time > 0) {
        start = end;

        uint32_t w = _gofImage->getWidth();
        uint32_t h = _gofImage->getHeight();
        uint8_t* data = _gofImage->getData();

        uint8_t* backup = new uint8_t[w * h * 4];
        for (int i = 0; i < w * h * 4; i++)
            backup[i] = data[i];

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int idx = (y * w + x) * 4;
                int numNeighbors = 0;
                bool isAlive = backup[idx] == 0;
                // Count neightbors
                for (int yn = y - 1; yn <= y + 1; yn++)
                    for (int xn = x - 1; xn <= x + 1; xn++) {
                        // Ignore center
                        if (yn == y && xn == x)
                            continue;
                        // Wrap coord
                        int X = (xn + w) % w;
                        int Y = (yn + h) % h;
                        if (backup[(Y * w + X) * 4] == 0)
                            numNeighbors++;
                    }

                // Update cell
                if (isAlive && (numNeighbors == 2 || numNeighbors == 3))
                    continue;
                else if (!isAlive && numNeighbors == 3)
                    data[idx] = data[idx + 1] = data[idx + 2] = 0;
                else
                    data[idx] = data[idx + 1] = data[idx + 2] = 255;
            }
        }
        delete[] backup;
        _gofImage->update();
    }
}

struct Pattern {
    std::string pattern;
    atta::vec2i size;
};

void Project::init() {
    unsigned w = _gofImage->getWidth();
    unsigned h = _gofImage->getHeight();

    // Set game of life as white
    uint8_t* data = _gofImage->getData();
    for (int i = 0; i < w * h * 4; i++)
        data[i] = 255;

    // Populate game of life
    std::map<std::string, Pattern> patterns;
    patterns["blinker"] = {
        "***",
        {3, 1},
    };
    patterns["glider"] = {
        "  *"
        "* *"
        " **",
        {3, 3},
    };
    patterns["hammerhead"] = {
        "*****             "
        "*    *       **   "
        "*           ** ***"
        " *         ** ****"
        "   **   ** **  ** "
        "     *    *  *    "
        "      * * * *     "
        "       *          "
        "       *          "
        "      * * * *     "
        "     *    *  *    "
        "   **   ** **  ** "
        " *         ** ****"
        "*           ** ***"
        "*    *       **   "
        "*****             ",
        {18, 16},
    };
    patterns["coverleaf"] = {
        "   * *   "
        " *** *** "
        "*   *   *"
        "* *   * *"
        " ** * ** "
        "         "
        " ** * ** "
        "* *   * *"
        "*   *   *"
        " *** *** "
        "   * *   ",
        {9, 11},
    };
    patterns["zdr"] = {
        " **  ** "
        "   **   "
        "   **   "
        "* *  * *"
        "*      *"
        "        "
        "*      *"
        " **  ** "
        "  ****  "
        "        "
        "   **   "
        "   **   ",
        {8, 12},
    };

    std::map<std::string, std::vector<atta::vec2i>> instances;
    instances["blinker"] = {{10, 10}, {20, 20}, {10, 20}, {20, 10}};
    instances["glider"] = {{30, 30}, {20, 50}, {10, 5}, {0, 0}, {80, 30}, {40, 90}, {90, 10}, {90, 50}};
    instances["coverleaf"] =  {{100, 100}, {130, 100}};
    instances["hammerhead"] =  {{150, 150}, {155, 170}, {160, 190}};
    instances["zdr"] =  {{30, 150}};

    // Write pattern instances
    std::vector<atta::vec2i> liveCells = {};

    for (auto&[name, pattern] : patterns)
        for (atta::vec2i inst : instances[name])
            for (int y = 0; y < pattern.size.y; y++)
                for (int x = 0; x < pattern.size.x; x++)
                    if (pattern.pattern[y * pattern.size.x + x] == '*')
                        liveCells.push_back(inst + atta::vec2i(x, y) - pattern.size/ 2);

    for (atta::vec2i cell : liveCells) {
        unsigned idx = (((cell.y + h) % h) * w + (cell.x + w) % w) * 4;
        data[idx] = data[idx + 1] = data[idx + 2] = 0;
    }

    _gofImage->update();
}

void Project::clear() {
    unsigned w = _gofImage->getWidth();
    unsigned h = _gofImage->getHeight();

    // Set game of life as white
    uint8_t* data = _gofImage->getData();
    for (int i = 0; i < w * h * 4; i++)
        data[i] = 255;
    _gofImage->update();
}
