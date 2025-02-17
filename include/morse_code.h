#pragma once

#include <Arduino.h>

// Structure to hold character and its morse code
struct MorseMapping {
    char character;
    const char* code;
};

// Morse code alphabet stored in flash memory
const MorseMapping MORSE_TABLE[] = {
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'E', "."},
    {'F', "..-."},
    {'G', "--."},
    {'H', "...."},
    {'I', ".."},
    {'J', ".---"},
    {'K', "-.-"},
    {'L', ".-.."},
    {'M', "--"},
    {'N', "-."},
    {'O', "---"},
    {'P', ".--."},
    {'Q', "--.-"},
    {'R', ".-."},
    {'S', "..."},
    {'T', "-"},
    {'U', "..-"},
    {'V', "...-"},
    {'W', ".--"},
    {'X', "-..-"},
    {'Y', "-.--"},
    {'Z', "--.."},
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
    {' ', " "},
    {'.', ".-.-.-"},
    {',', "--..--"},
    {'?', "..--.."},
    {'!', "-.-.--"},
    {'/', "-..-."},
    {'@', ".--.-."},
    {'&', ".-..."}
};

const int MORSE_TABLE_SIZE = sizeof(MORSE_TABLE) / sizeof(MorseMapping); 