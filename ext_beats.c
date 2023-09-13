// Assignment 2 21T1 COMP1511: CS bEats
// beats.c
//
// This program was written by YOUR-NAME-HERE (z5286348)
// on 18/04/2021
//
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Add any extra #includes your code needs here.
//
// But note you are not permitted to use functions from string.h
// so do not #include <string.h>

#include "ext_beats.h"

/////////////////////////////////////////////////////////////////////
//      TODO: Replace this with the contents of your beats.c       //
//#include "beats.h"

// Add your own #defines here.
typedef struct note *Note;
//////////////////////////////////////////////////////////////////////

// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.

struct track {
    // TODO: You will have to add extra fields here in Stage 2.
    struct beat *select;
    struct beat *head;
};

// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.

struct beat {
    // You may choose to add or change fields in this struct.
    Note notes;
    struct beat *next;
};

// You don't have to use the provided struct note, you are free to
// make your own struct instead.
// If you use the provided struct note, you add fields
// to it to store other information.

struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    Note next;
};

// Add any other structs you define here.

//////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.
static int input_check(Beat beat, int octave, int key);
static Note create_note(int octave, int key);
static void free_note(Beat beat);
static void print_key(Note notes);
// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;

    // You do not need to change this function, unless you change
    // the implementation of struct beat.

    return new_beat;
}

// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h

//////////////////////////////////////////////////////////////////////
//                        Stage 1 Functions                         //
//////////////////////////////////////////////////////////////////////

// Add a note to the end of a beat.
int add_note_to_beat(Beat beat, int octave, int key) {

    int result = input_check(beat, octave, key);
    if (result != 2) {
        return result;
    }
    Note new_note = create_note(octave, key);
    if (beat->notes == NULL) {
        beat->notes = new_note;
    } else {
        Note curr = beat->notes;
        // loop through the beat to find the last note
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_note;
    }
    return VALID_NOTE;
}

// Print the contents of a beat.
void print_beat(Beat beat) {

    Note new_note = beat->notes;
    while (new_note != NULL) {
        if (beat->notes == new_note) {
            printf("%d ", new_note->octave);
            print_key(new_note);
        } else {
            // if a note lies before, a '|' is needed to seperate them
            printf("| %d ", new_note->octave);
            print_key(new_note);
        }
        new_note = new_note->next;  
    }
    printf("\n");
    return;
}

// Count the number of notes in a beat that are in a given octave.
int count_notes_in_octave(Beat beat, int octave) {
    
    int keep_looping = 1;
    int count = 0;
    Note temp = NULL;
    if (beat->notes != NULL) {
        temp = beat->notes;
    }
    if (beat->notes == NULL) {
        return count;
    }
    // looping should stop when the next note does have required octave
    // or if next notes is NULL, exit function
    while (temp->octave == octave || keep_looping == 1) {
        if (temp->octave == octave) {
            keep_looping = 0;
            count++;
        }
        if (temp->next == NULL) {
            return count;
        }
        temp = temp->next;
    }
    return count;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 2 Functions                         //
//////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    // Note: there is no fprintf in this function, as the
    // Stage 1 autotests call create_track but expect it to return NULL
    // (so this function should do nothing in Stage 1).
    Track track = malloc(sizeof (struct track));
    track->select = NULL;
    track->head = NULL;
    return track;
}

// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {

    if (track->head == NULL) {
        track->head = beat;
        return;
    }
    Beat new_beat = NULL;
    new_beat = beat;
    if (track->select == NULL) {
        // if track is stopped, then add beat to the head
        new_beat->next = track->head;
        track->head = new_beat;
    } else if (track->select != NULL) {
        // if track is playing, then add beat after the selected beat
        Beat curr = track->head;
        while (curr != track->select) {
            curr = curr->next;
        }
        new_beat->next = curr->next;
        track->select->next = new_beat;
    }
    
    return;
}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) {
    
    if (track->head == NULL) {
        track->select = NULL;
        return TRACK_STOPPED;
    } else if (track->select == NULL) {
        track->select = track->head;
        return TRACK_PLAYING;
    }
    struct beat *curr = track->head;
    
    // loop through the track to find the selected beat
    while (curr != track->select) {
        curr = curr->next;
    }
    if (curr->next == NULL) {
        track->select = NULL;
        return TRACK_STOPPED;
    } else {
        track->select = curr->next;
        return TRACK_PLAYING;
    }
}

// Print the contents of a track.
void print_track(Track track) {
    
    if (track->head == NULL) {
        return;
    }
    Beat curr = track->head;
    int count = 1;
    while (curr != NULL) {
        if (curr == track->select) {
            printf(">[%d] ", count);
        } else {
            printf(" [%d] ", count);
        }
        print_beat(curr);
        curr = curr->next;
        count++;
    }
    return;
}

// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {

    int count = 0;
    if (track->head == NULL) {
        return count;
    }
    Beat curr = NULL;
    if (track->select != NULL) {
        curr = track->select;
        // if track is playing, we need to count beats after it
        // so count needs to subtract one(selected beat)
        count--;
    } else {
        curr = track->head;
    }
    while (curr != NULL) {
        curr = curr->next;
        count++;
    }
    return count;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 3 Functions                         //
//////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_beat but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than print an error).
    if (beat == NULL) {
        // if the beat is NULL then do nothing
        return;
    }
    while (beat->notes != NULL) {
        free_note(beat);
    }
    free(beat);
    return;
}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_track but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than print an error).
    if (track == NULL) {
        // if the track is NULL then do nothing
        return;
    }
    while (track->head  != NULL) {
        Beat curr = track->head;
        track->head = track->head->next;
        free(curr);
    }
    free(track);
    return;
}

// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    //rintf("remove_selected_beat not implemented yet.");
    if (track->select == NULL) {
        // if the Track is stopped than do nothing
        return TRACK_STOPPED;
    }
    Beat curr = track->head;
    Beat prev = NULL;
    while (curr != track->select) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL) {
        // head is selected, we need to remove it
        // and make the next one selected
        track->head = curr->next;
        track->select = curr->next;
    } else if (curr->next == NULL) {
        // the last beat is selected, we need to remove it 
        // and stop the track
        track->select = NULL;
        prev->next = NULL;
    } else {
        // the selected track is in the middle of the track
        // remove it and make the next one selected
        track->select = curr->next;
        prev->next = curr->next;
    }
    free_beat(curr);
    if (track->select == NULL) {
        return TRACK_STOPPED;
    }
    return TRACK_PLAYING;
}

// Helper Functions:

// This function goes through beat to find the highest_key and highest_octave
// Everytime find a higher octave, highest_key will be updated
int input_check(Beat beat, int octave, int key) {
    int highest_octave = 0;
    int highest_key = 0;
    if (octave < 0 || octave >= 10) {
        return INVALID_OCTAVE;
    }
    if (key < 0 || key >= 12) {
        return INVALID_KEY;
    }
    Note temp = NULL;
    if (beat->notes != NULL) {
        temp = beat->notes;
    }
    while (temp != NULL) {
        if (temp->octave >= highest_octave) {
            highest_octave = temp->octave;
            highest_key = temp->key;
        }
        temp = temp->next;
    }
    if (octave < highest_octave) {
        return NOT_HIGHEST_NOTE;
    }
    if (octave == highest_octave) {
        
        if (key <= highest_key) {
            return NOT_HIGHEST_NOTE;
        }
    }
    return 2;
}

static Note create_note(int octave, int key) {
    Note new_note = malloc(sizeof (struct note));
    new_note->octave = octave;
    new_note->key = key;    
    new_note->next = NULL;
    return new_note;
}

// A helper function for free_beat
// everytime this function is called, the head of the beat will be freed
// the next notes will be the new head
static void free_note(Beat beat) {
    if (beat->notes == NULL) {
        //if the beat is empty then do nothing
        return;
    }
    Note rem_note = beat->notes;
    beat->notes = beat->notes->next;
    free(rem_note);
}

// since a '0' needs to be added when key is a single figure
// this function is created to avoid deep nesting
static void print_key(Note notes){
    if (notes->key < 10) {
        printf("0%d ", notes->key);
    } else {
        printf("%d ", notes->key);
    }
}
/////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//                Extension -- Stage 4 Functions                      //
////////////////////////////////////////////////////////////////////////
int count_notes_intrack(Track track, int beats_to_merge);
int count_notes_inbeats(Beat beat);
void insert_beat(Track track);
// Merge `beats_to_merge` beats into `merged_beats`
void merge_beats(Track track, int beats_to_merge, int merged_beats) {
    //printf("merge_beats not implemented yet.\n");
    if (track->select == NULL) {
        // if there is no selected beat than do nothing
        return;
    }
    int num_notes = count_notes_intrack(track, beats_to_merge);
    int num = num_notes / merged_beats;
    int count_note = count_notes_inbeats(track->select);
    int count = 1;
    while (count <= beats_to_merge && track->select != NULL) {
        count_note = count_notes_inbeats(track->select);
        while (count_note < num) {
            insert_beat(track);
            count_note = count_notes_inbeats(track->select);
            Beat temp = track->select->next;
            track->select->next = temp->next;
            //printf("count_note = %d\n", count_note); 
            free(temp);
        }
        track->select = track->select->next;
        beats_to_merge--;
    } 
}

////////////////////////////////////////////////////////////////////////
//                Extension -- Stage 5 Functions                      //
////////////////////////////////////////////////////////////////////////

void save_track(Track track, char *name) {
    printf("save_track not implemented yet.\n");

}

Track load_track(char *name) {
    printf("load_track not implemented yet.\n");

    return NULL;
}

// Helper Function:

int count_notes_intrack(Track track, int beats_to_merge) {
    Beat curr1 = track->select;
    Note curr2 = curr1->notes;
    int count_notes = 0;
    int count_beats = 0;
    while (curr1 != NULL && count_beats < beats_to_merge) {
        curr2 = curr1->notes;
        while (curr2 != NULL) {
            count_notes++;
            curr2 = curr2->next;
        }
        curr1 = curr1->next;
        count_beats++;
    }
    return count_notes;
}

int count_notes_inbeats(Beat beat) {
    Note curr = beat->notes;
    int count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

void insert_beat(Track track) {
    Note prev_select = NULL;
    Note curr_select = track->select->notes;
    //Beat add_beat = track->select->next;
    if (curr_select == NULL) {
        Note temp = track->select->next->notes;
        temp->next = NULL;
        curr_select = temp;
        track->select->notes = curr_select;
        track->select->next->notes = track->select->next->notes->next;
    }
    int added = 0;
    while (track->select->next->notes != NULL) {
        added = 0;
        while (curr_select != NULL && added == 0) {
            if (curr_select->octave == track->select->next->notes->octave &&
            curr_select->key == track->select->next->notes->key) {
                // remove the notes from add_beat
                Note temp = track->select->next->notes;
                track->select->next->notes = temp->next;
                temp->next = NULL;
                free(temp);
            }
            if (prev_select == NULL) {
                if (curr_select->octave > track->select->next->notes->octave) {
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select;
                    prev_select = temp;
                    track->select->notes = prev_select;
                    added = 1;
                } else if (curr_select->octave == track->select->next->notes->octave &&
                curr_select->key > track->select->next->notes->key) {
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select;
                    prev_select = temp; 
                    track->select->notes = prev_select;
                    added = 1;
                } else if (curr_select->octave == track->select->next->notes->octave &&
                curr_select->key < track->select->next->notes->key &&
                curr_select->next == NULL) {
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select->next;
                    curr_select->next = temp;
                    added = 1;
                } else if (curr_select->octave < track->select->next->notes->octave &&
                curr_select->next == NULL) {
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select->next;
                    curr_select->next = temp;
                    added = 1;
                }
            } else if (prev_select != NULL){
                if (curr_select->octave > track->select->next->notes->octave &&
                track->select->next->notes->octave > prev_select->octave) {
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select;
                    prev_select->next = temp;
                    added = 1;
                } else if (curr_select->octave == track->select->next->notes->octave &&
                track->select->next->notes->octave == prev_select->octave) {
                    if (curr_select->key > track->select->next->notes->key &&
                    track->select->next->notes->key > prev_select->key) {
                        Note temp = track->select->next->notes;
                        track->select->next->notes = track->select->next->notes->next;
                        temp->next = curr_select;
                        prev_select->next = temp;
                        added = 1;
                    }
                } else if (track->select->next->notes->octave == prev_select->octave &&
                track->select->next->notes->key > prev_select->key) {
                    if (curr_select->octave > track->select->next->notes->octave) {
                        Note temp = track->select->next->notes;
                        track->select->next->notes = track->select->next->notes->next;
                        temp->next = curr_select;
                        prev_select->next = temp;
                        added = 1;
                    }
                } else if (curr_select->octave == track->select->next->notes->octave &&
                track->select->next->notes->key < curr_select->key) {
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select->next;
                    curr_select->next = temp;
                    added = 1;
                } else if (curr_select->octave < track->select->next->notes->octave){
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select->next;
                    curr_select->next = temp;
                    added = 1;
                } else if (curr_select->octave == track->select->next->notes->octave &&
                track->select->next->notes->key > curr_select->key) {
                    Note temp = track->select->next->notes;
                    track->select->next->notes = track->select->next->notes->next;
                    temp->next = curr_select->next;
                    curr_select->next = temp;
                    added = 1;
                }
                      
            }
            prev_select = curr_select;
            curr_select = curr_select->next;
        }
        curr_select = track->select->notes;
        prev_select = NULL;
    }
}











