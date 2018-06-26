#ifndef GK_INPUT_PROCESSOR
#define GK_INPUT_PROCESSOR


enum UnikeyEventName {
    // Events change mark, including:
    // a => â (HAT), u => ư (HORN), a => ă (BREVE), d => đ (DASH).
    HAT_ALL, HAT_A, HAT_E, HAT_O,
    HORN_ALL, HORN_UO, HORN_U, HORN_O,
    BREVE, DASH,
    // Events change tone (also called accent), including:
    //      No tone              (Tone 0)
    //      ACCUTE      : a => á (Tone 1)
    //      GRAVE       : a => à (Tone 2)
    //      HOOK ABOVE  : a => ả (Tone 3)
    //      TILDE       : a => ã (Tone 4)
    //      DOT BELOW.  : a => ạ (Tone 5)
    TONE0, TONE1, TONE2, TONE3, TONE4, TONE5,
    TELEX_W,      // Special for Telex? Need more details.
    MAP_CHAR,     // Map [ to ư and map ] to ơ.
    ESCAPE_CHAR,  // Mostly used in VIQR input method.
    NORMAL,       // Do not belong to any of above categories.
    COUNT         // Total events.
};

// GkKeyContext is used to mark event type and mapping input method
// in `mapping.cc` file
// Example: In Telex input method, when we get 'A',
//          it maybe the HAT mark event for character 'A'.
enum GkKeyContext {
    // Events change mark, including:
    // a => â (HAT), u => ư (HORN), a => ă (BREVE), d => đ (DASH).
    HAT_ALL, HAT_A, HAT_E, HAT_O,
    HORN_ALL, HORN_UO, HORN_U, HORN_O,
    BREVE, DASH,
    // Events change tone (also called accent), including:
    //      No tone              (Tone 0)
    //      ACCUTE      : a => á (Tone 1)
    //      GRAVE       : a => à (Tone 2)
    //      HOOK ABOVE  : a => ả (Tone 3)
    //      TILDE       : a => ã (Tone 4)
    //      DOT BELOW.  : a => ạ (Tone 5)
    TONE0, TONE1, TONE2, TONE3, TONE4, TONE5,
    TELEX_W,      // Special for Telex? Need more details.
    MAP_CHAR,     // Map [ to ư and map ] to ơ.
    ESCAPE_CHAR,  // Mostly used in VIQR input method.
    NORMAL,       // Do not belong to any of above categories.
    COUNT         // Total events.
};

#endif/* GK_INPUT_PROCESSOR */