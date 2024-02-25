#include "../include/Renderer.h"

namespace graphic_sys_lib {
    void Renderer::render() {
        renderBase(mElements);
    }
    void Renderer::renderBase(const std::vector<std::unique_ptr<GraphicElement>>& elements)  const noexcept {
        for (auto& element : elements) {
            element->render();
        }
    }
}
