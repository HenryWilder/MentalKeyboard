#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <thread>
#include <sal.h>

enum KeyboardSide : bool
{
    LEFT = false,
    RIGHT = true,
};

const std::vector<std::string> g_keyboard[2][2] = {
    // left
    {
        // regular
        {
            "`123456",  // row 0
            "\tqwerty", // row 1
            "\1asdfgh", // row 2
            "\1zxcvb",  // row 3
        },

        // shift
        {
            "~!@#$%^",  // row 0
            "\tQWERTY", // row 1
            "\1ASDFGH", // row 2
            "\1ZXCVB",  // row 3
        }
    },

    // right
    {
        // regular
        {
            "7890-=",   // row 0
            "uiop[]\\", // row 1
            "jkl;\'\n", // row 2
            "nm,./",    // row 3
        },

        // shift
        {
            "&*()_+",   // row 0
            "UIOP{}|",  // row 1
            "JKL:\"\n", // row 2
            "NM<>?",    // row 3
        }
    }
};

struct KeyCoords
{
    constexpr KeyCoords(_In_ bool isShifted, _In_range_(0, 3) int row, _In_ KeyboardSide isRightSide, _In_range_(0, 6) int column) :
        cap(isShifted), row(row), hlf(isRightSide), col(column)
    { }

    uint8_t
        cap : 1, // 0-1 - shift or not
        row : 2, // 0-3 - row of the keyboard
        hlf : 1, // 0-1 - left or right hand
        col : 3; // 0-6 - column of the segment

    bool operator==(const KeyCoords &other)
    {
        return
            cap == other.cap &&
            row == other.row &&
            hlf == other.hlf &&
            col == other.col;
    }
};

const std::unordered_map<char, KeyCoords> g_keyCoords = {
    {  '`', KeyCoords(false, 0,  LEFT, 0) },
    {  '1', KeyCoords(false, 0,  LEFT, 1) },
    {  '2', KeyCoords(false, 0,  LEFT, 2) },
    {  '3', KeyCoords(false, 0,  LEFT, 3) },
    {  '4', KeyCoords(false, 0,  LEFT, 4) },
    {  '5', KeyCoords(false, 0,  LEFT, 5) },
    {  '6', KeyCoords(false, 0,  LEFT, 6) },
    {  '7', KeyCoords(false, 0, RIGHT, 0) },
    {  '8', KeyCoords(false, 0, RIGHT, 1) },
    {  '9', KeyCoords(false, 0, RIGHT, 2) },
    {  '0', KeyCoords(false, 0, RIGHT, 3) },
    {  '-', KeyCoords(false, 0, RIGHT, 4) },
    {  '=', KeyCoords(false, 0, RIGHT, 5) },
    {  '~', KeyCoords( true, 0,  LEFT, 0) },
    {  '!', KeyCoords( true, 0,  LEFT, 1) },
    {  '@', KeyCoords( true, 0,  LEFT, 2) },
    {  '#', KeyCoords( true, 0,  LEFT, 3) },
    {  '$', KeyCoords( true, 0,  LEFT, 4) },
    {  '%', KeyCoords( true, 0,  LEFT, 5) },
    {  '^', KeyCoords( true, 0,  LEFT, 6) },
    {  '&', KeyCoords( true, 0, RIGHT, 0) },
    {  '*', KeyCoords( true, 0, RIGHT, 1) },
    {  '(', KeyCoords( true, 0, RIGHT, 2) },
    {  ')', KeyCoords( true, 0, RIGHT, 3) },
    {  '_', KeyCoords( true, 0, RIGHT, 4) },
    {  '+', KeyCoords( true, 0, RIGHT, 5) },

    { '\t', KeyCoords(false, 1,  LEFT, 0) },
    {  'q', KeyCoords(false, 1,  LEFT, 1) },
    {  'w', KeyCoords(false, 1,  LEFT, 2) },
    {  'e', KeyCoords(false, 1,  LEFT, 3) },
    {  'r', KeyCoords(false, 1,  LEFT, 4) },
    {  't', KeyCoords(false, 1,  LEFT, 5) },
    {  'y', KeyCoords(false, 1,  LEFT, 6) },
    {  'u', KeyCoords(false, 1, RIGHT, 0) },
    {  'i', KeyCoords(false, 1, RIGHT, 1) },
    {  'o', KeyCoords(false, 1, RIGHT, 2) },
    {  'p', KeyCoords(false, 1, RIGHT, 3) },
    {  '[', KeyCoords(false, 1, RIGHT, 4) },
    {  ']', KeyCoords(false, 1, RIGHT, 5) },
    { '\\', KeyCoords(false, 1, RIGHT, 6) },
    {  'Q', KeyCoords( true, 1,  LEFT, 1) },
    {  'W', KeyCoords( true, 1,  LEFT, 2) },
    {  'E', KeyCoords( true, 1,  LEFT, 3) },
    {  'R', KeyCoords( true, 1,  LEFT, 4) },
    {  'T', KeyCoords( true, 1,  LEFT, 5) },
    {  'Y', KeyCoords( true, 1,  LEFT, 6) },
    {  'U', KeyCoords( true, 1, RIGHT, 0) },
    {  'I', KeyCoords( true, 1, RIGHT, 1) },
    {  'O', KeyCoords( true, 1, RIGHT, 2) },
    {  'P', KeyCoords( true, 1, RIGHT, 3) },
    {  '{', KeyCoords( true, 1, RIGHT, 4) },
    {  '}', KeyCoords( true, 1, RIGHT, 5) },
    {  '|', KeyCoords( true, 1, RIGHT, 6) },

    {  'a', KeyCoords(false, 2,  LEFT, 1) },
    {  's', KeyCoords(false, 2,  LEFT, 2) },
    {  'd', KeyCoords(false, 2,  LEFT, 3) },
    {  'f', KeyCoords(false, 2,  LEFT, 4) },
    {  'g', KeyCoords(false, 2,  LEFT, 5) },
    {  'h', KeyCoords(false, 2,  LEFT, 6) },
    {  'j', KeyCoords(false, 2, RIGHT, 0) },
    {  'k', KeyCoords(false, 2, RIGHT, 1) },
    {  'l', KeyCoords(false, 2, RIGHT, 2) },
    {  ';', KeyCoords(false, 2, RIGHT, 3) },
    { '\'', KeyCoords(false, 2, RIGHT, 4) },
    { '\n', KeyCoords(false, 2, RIGHT, 5) },
    {  'A', KeyCoords( true, 2,  LEFT, 1) },
    {  'S', KeyCoords( true, 2,  LEFT, 2) },
    {  'D', KeyCoords( true, 2,  LEFT, 3) },
    {  'F', KeyCoords( true, 2,  LEFT, 4) },
    {  'G', KeyCoords( true, 2,  LEFT, 5) },
    {  'H', KeyCoords( true, 2,  LEFT, 6) },
    {  'J', KeyCoords( true, 2, RIGHT, 0) },
    {  'K', KeyCoords( true, 2, RIGHT, 1) },
    {  'L', KeyCoords( true, 2, RIGHT, 2) },
    {  ':', KeyCoords( true, 2, RIGHT, 3) },
    { '\"', KeyCoords( true, 2, RIGHT, 4) },

    {  'z', KeyCoords(false, 3,  LEFT, 1) },
    {  'x', KeyCoords(false, 3,  LEFT, 2) },
    {  'c', KeyCoords(false, 3,  LEFT, 3) },
    {  'v', KeyCoords(false, 3,  LEFT, 4) },
    {  'b', KeyCoords(false, 3,  LEFT, 5) },
    {  'n', KeyCoords(false, 3, RIGHT, 0) },
    {  'm', KeyCoords(false, 3, RIGHT, 1) },
    {  ',', KeyCoords(false, 3, RIGHT, 2) },
    {  '.', KeyCoords(false, 3, RIGHT, 3) },
    {  '/', KeyCoords(false, 3, RIGHT, 4) },
    {  'Z', KeyCoords( true, 3,  LEFT, 1) },
    {  'X', KeyCoords( true, 3,  LEFT, 2) },
    {  'C', KeyCoords( true, 3,  LEFT, 3) },
    {  'V', KeyCoords( true, 3,  LEFT, 4) },
    {  'B', KeyCoords( true, 3,  LEFT, 5) },
    {  'N', KeyCoords( true, 3, RIGHT, 0) },
    {  'M', KeyCoords( true, 3, RIGHT, 1) },
    {  '<', KeyCoords( true, 3, RIGHT, 2) },
    {  '>', KeyCoords( true, 3, RIGHT, 3) },
    {  '?', KeyCoords( true, 3, RIGHT, 4) },
};

struct KeyHandCoords
{
    constexpr KeyHandCoords(_In_range_(0, 3) int row, _In_range_(0, 6) int column) :
        row(row), col(column)
    { }

    uint8_t
        row : 2, // 0-3 - row of the keyboard
        col : 3; // 0-6 - column of the segment
};

static char KeyboardChar(_In_ KeyboardSide hand, _In_ bool isShifted, _In_ KeyHandCoords coords)
{
    return g_keyboard[hand][isShifted][coords.row][coords.col];
}

struct DelayedKeyHandCoords
{
    constexpr DelayedKeyHandCoords(_In_ uint16_t preDelay, _In_range_(0, 3) int row, _In_range_(0, 6) int column) :
        preDelay(preDelay), coords(row, column)
    { }

    // how many increments this hand should wait before performing the input
    // 0 = immediately
    uint16_t preDelay;
    KeyHandCoords coords;
};

struct WordInstructions
{
    std::vector<uint16_t> toggleShift; // Number of increments between toggles
    std::vector<DelayedKeyHandCoords> left, right;
};

static void CharsToSequence(_Out_ WordInstructions &sequence, _In_ const std::string &word)
{
    sequence = { };

    bool shiftState = false;
    uint16_t incrSinceShiftToggle = 0;
    uint16_t incrSinceLInput = 0;
    uint16_t incrSinceRInput = 0;
    for (char ch : word)
    {
        KeyCoords coords = g_keyCoords.find(ch)->second;
        if (coords.cap != shiftState)
        {
            sequence.toggleShift.push_back(incrSinceShiftToggle);
            shiftState = !shiftState;
            incrSinceShiftToggle = 0;
        }
        else
        {
            ++incrSinceShiftToggle;
        }

        if (coords.hlf == LEFT)
        {
            sequence.left.push_back(DelayedKeyHandCoords(incrSinceLInput, coords.row, coords.col));
            incrSinceLInput = 0;

            ++incrSinceRInput;
        }
        else
        {
            sequence.right.push_back(DelayedKeyHandCoords(incrSinceRInput, coords.row, coords.col));
            incrSinceRInput = 0;

            ++incrSinceLInput;
        }
    }
}

std::unordered_map<std::string, WordInstructions> g_wordCoords = {};

static void MemorizeWord(_In_ const std::string &word)
{
    //std::cout << "i am learning \"" << word << "\"\n";

    auto it = g_wordCoords.find(word);
    if (it == g_wordCoords.end( ))
    {
        g_wordCoords.insert({ word, { } });
        it = g_wordCoords.find(word);
    }

    CharsToSequence(it->second, word);
}

static void TypeWord(const WordInstructions &instr)
{
    size_t iShift = 0;
    size_t iLeft  = 0;
    size_t iRight = 0;

    bool isShiftHeld = false;
    bool isRemainingShiftInstrs = iShift < instr.toggleShift.size( );
    bool isRemainingLInstrs     = iLeft  < instr.left.size( );
    bool isRemainingRInstrs     = iRight < instr.right.size( );

    uint16_t toggleShiftIn = isRemainingShiftInstrs ? instr.toggleShift[iShift++]  : 0;
    uint16_t inputLeftIn   = isRemainingLInstrs     ? instr.left [iLeft ].preDelay : 0;
    uint16_t inputRightIn  = isRemainingRInstrs     ? instr.right[iRight].preDelay : 0;

    while (isRemainingShiftInstrs || isRemainingLInstrs || isRemainingRInstrs)
    {
        if (isRemainingShiftInstrs)
        {
            if (toggleShiftIn == 0)
            {
                isShiftHeld = !isShiftHeld;
                std::cout << (isShiftHeld ? "press" : "release") << " shift\n";

                if (isRemainingShiftInstrs = iShift < instr.toggleShift.size())
                    toggleShiftIn = instr.toggleShift[iShift++];
            }
            else
            {
                --toggleShiftIn;
            }
        }

        if (isRemainingLInstrs)
        {
            if (inputLeftIn == 0)
            {
                KeyHandCoords coords = instr.left[iLeft++].coords;
                std::cout << "L: " << (int)coords.row << ' ' << (int)coords.col << " (" << KeyboardChar(LEFT, isShiftHeld, coords) << ")\n";

                if (isRemainingLInstrs = iLeft < instr.left.size( ))
                    inputLeftIn = instr.left[iLeft].preDelay;
            }
            else
            {
                --inputLeftIn;
            }
        }

        if (isRemainingRInstrs)
        {
            if (inputRightIn == 0)
            {
                KeyHandCoords coords = instr.right[iRight++].coords;
                std::cout << "R: " << (int)coords.row << ' ' << (int)coords.col << " (" << KeyboardChar(RIGHT, isShiftHeld, coords) << ")\n";

                if (isRemainingRInstrs = iRight < instr.right.size( ))
                    inputRightIn = instr.right[iRight].preDelay;
            }
            else
            {
                --inputRightIn;
            }
        }
    }

    if (isShiftHeld) std::cout << "release shift\n";
}

// Number of times the word or subword has been encountered
// Word will get memorized after MEMORIZE_REPS
std::unordered_map<std::string, size_t> g_unmemorizedButEncountered;
constexpr size_t MEMORIZE_REPS = 1000;

// returns the length of the word that was memorized; 1 being the minimum
static size_t GetWordInstructions(_Out_ WordInstructions &sequence, _In_ const std::string &word)
{
    std::string subword;
    for (size_t length = word.size( ); length > 1; --length)
    {
        subword = word.substr(0, length);
        auto it = g_wordCoords.find(subword);
        bool isKnown = it != g_wordCoords.end( );

        std::cout << "i ";
        if (!isKnown) std::cout << "dont ";
        std::cout << "know \"" << subword << "\"\n";

        if (isKnown)
        {
            sequence = it->second;
            return length;
        }
        else
        {
            auto it1 = g_unmemorizedButEncountered.find(subword);
            if (it1 != g_unmemorizedButEncountered.end( ))
            {
                ++it1->second;
            }
            else
            {
                g_unmemorizedButEncountered.emplace(subword, 1);
                it1 = g_unmemorizedButEncountered.find(subword);
            }

            if (it1->second == MEMORIZE_REPS)
            {
                MemorizeWord(subword);
            }
        }
    }
    CharsToSequence(sequence, word.substr(0, 1));
    return 1;
}

static void TypeString(_In_ const std::string &str)
{
    std::istringstream stream(str);
    std::string word;
    bool isFirst = true;
    while (std::getline(stream, word, ' '))
    {
        if (!isFirst) std::cout << "spacebar\n";

        if (!isFirst) std::cout << '\n';
        while (!word.empty())
        {
            WordInstructions instr;
            word = word.substr(GetWordInstructions(instr, word));
            TypeWord(instr);

            std::cout << '\n';
        }

        isFirst = false;
    }
}

int main()
{
    TypeString("Hello world!");
}
