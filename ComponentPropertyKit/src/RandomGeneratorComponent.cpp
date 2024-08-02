#include "RandomGeneratorComponent.h"

template<>
comp_prop_kit_lib::RandomGeneratorComponent& comp_prop_kit_lib::PropertyList::pushComponent<comp_prop_kit_lib::RandomGeneratorComponent>();
template<>
comp_prop_kit_lib::RandomGeneratorComponent& comp_prop_kit_lib::PropertyList::getComponent<comp_prop_kit_lib::RandomGeneratorComponent>();
template<>
comp_prop_kit_lib::RandomGeneratorComponent comp_prop_kit_lib::PropertyList::getComponent<comp_prop_kit_lib::RandomGeneratorComponent>() const;
template<>
void comp_prop_kit_lib::PropertyList::popComponent<comp_prop_kit_lib::RandomGeneratorComponent>();
