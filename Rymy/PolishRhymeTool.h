// PolishRhymeTool.h
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <locale>
#include <codecvt> // Required for UTF-8 conversion

// Our new class for Polish-specific linguistic tools
class PolishRhymeTool {
public:
    PolishRhymeTool() {}
    // Helper to check if a wide character is a Polish vowel
    bool isVowel(wchar_t c) {
        c = towlower(c);
        return (c == L'a' || c == L'¹' || c == L'e' || c == L'ê' || c == L'i' ||
            c == L'o' || c == L'u' || c == L'y');
    }
    // Constructor is now empty, as we don't need to load a dictionary for syllable counting

    // Rule-based syllable counter for Polish
    int countSyllables(const std::wstring& word) {
        int count = 0;
        bool inVowelGroup = false;
        for (wchar_t c : word) {
            if (isVowel(c)) {
                // If we enter a new group of vowels, increment count
                if (!inVowelGroup) {
                    count++;
                    inVowelGroup = true;
                }
            }
            else {
                // If it's a consonant, we are no longer in a vowel group
                inVowelGroup = false;
            }
        }
        // A special case for words like "i" which are just one vowel.
        // If the word has characters but the count is 0, it must be 1.
        if (count == 0 && !word.empty()) {
            return 1;
        }
        return count;
    }
    // NEW PUBLIC FUNCTION: Extracts the rhyming part of a word.
    std::wstring getRhymeKey(const std::wstring& word) {
        if (word.empty()) return L"";

        int lastVowelIndex = -1;
        for (int i = word.length() - 1; i >= 0; --i) {
            if (isVowel(word[i])) {
                lastVowelIndex = i;
                break;
            }
        }

        if (lastVowelIndex == -1) {
            return word; // No vowel found, return the whole word as the key
        }

        return word.substr(lastVowelIndex);
    }
};