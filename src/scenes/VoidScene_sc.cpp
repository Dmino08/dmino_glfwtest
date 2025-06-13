#include "scenes/VoidScene_sc.hpp"
#include "graphics/core/Mesh.hpp"

VoidScene_sc::VoidScene_sc() = default;
VoidScene_sc::~VoidScene_sc() = default;

void VoidScene_sc::init(Engine& engine, Window& window) {
    mesh_ = makeU<Mesh>();
}
void VoidScene_sc::input(InputManager& input, float delta) {

}
void VoidScene_sc::update(float delta) {

}
void VoidScene_sc::draw() {

}
void VoidScene_sc::onClose() {

}
