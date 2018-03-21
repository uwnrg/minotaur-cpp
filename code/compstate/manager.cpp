#include "manager.h"

CompetitionState &Manager::state() {
    static CompetitionState state;
    return state;
}
