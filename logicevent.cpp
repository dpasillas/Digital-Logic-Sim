#include "logicevent.h"

QQueue<LogicEvent*> LogicEvent::queue;
Pin* LogicEvent::toBeDeleted;
