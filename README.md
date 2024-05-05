# Beamlib

raylib-style imgui setup wrapper

## example usage

```cpp
#include "beamlib/blib.h"
#include "imgui.h"

int main() {
    GLFWwindow *const window = Blib::CreateWindow("Beamlib", 800, 600);
    while (!Blib::WindowShouldClose(window)) {
        Blib::BeginUI();
        ImGui::ShowDemoWindow();
        Blib::EndUI();
    }
    Blib::DestroyWindow(window);
    return 0;
}
```
