#include "actors/actor.h"

#include <utility>

namespace mapf::simulator {

Actor::Actor(ContextPtr context) : context_{std::move(context)} {}

ActorRef Actor::GetActorRef() { return ActorRef(shared_from_this()); }

Actor& Actor::OnStart() { return *this; }

Actor& Actor::OnFinish() { return *this; }

}  // namespace mapf::simulator