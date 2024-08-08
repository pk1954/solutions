// NNetSound.ixx
//
// NNetCommands

export module NNetCommands:NNetSound;

import std;
import locking_a_door_wav;
import door_unlocking_wav;
import pop_wav;
import tap_wav;
import no_wav;

using std::string;
using std::wstring;
using std::vector;

export
{
    vector<unsigned char> const & SNAP_IN_SOUND      { locking_a_door_wav_data };
    vector<unsigned char> const & DISAPPEAR_SOUND    { pop_wav_data };
    vector<unsigned char> const & UNLOCK_SOUND       { door_unlocking_wav_data };
    vector<unsigned char> const & SELECTION_SOUND    { tap_wav_data };
    vector<unsigned char> const & NOT_POSSIBLE_SOUND { no_wav_data };
}
