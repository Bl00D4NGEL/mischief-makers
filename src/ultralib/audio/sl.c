#include <PR/libaudio.h>

ALGlobals* alGlobals = 0;

void alInit(ALGlobals* globals, ALSynConfig* config) {
    if (!alGlobals) {
        alGlobals = globals;
        alSynNew(&alGlobals->drvr, config);
    }
}

void alClose(ALGlobals* globals) {
    if (alGlobals) {
        alSynDelete(&globals->drvr);
        alGlobals = 0;
    }
}

void alLink(ALLink* link, ALLink* to) {
    link->next = to->next;
    link->prev = to;

    if (to->next) {
        to->next->prev = link;
    }

    to->next = link;
}

void alUnlink(ALLink* link) {
    if (link->next) {
        link->next->prev = link->prev;
    }

    if (link->prev) {
        link->prev->next = link->next;
    }
}
