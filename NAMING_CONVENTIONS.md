# C++ Naming Conventions

These conventions keep the engine consistent, readable, and easy to maintain. Apply them by default unless an external API or a specific design need justifies an exception.

## 1. Naming Rules

| Element | Convention | Example |
| ------- | ---------- | ------- |
| Classes | PascalCase | `Camera`, `InputManager` |
| Structs | PascalCase | `Vertex`, `RenderSettings` |
| Functions and methods | camelCase | `updateCamera()`, `loadTexture()` |
| Member variables | camelCase with trailing underscore | `position_`, `vertexBuffer_` |
| Parameters | camelCase | `deltaTime`, `vertexCount` |
| Local variables | camelCase | `cameraPosition`, `indexCount` |
| Constants | `k` + PascalCase | `kMaxLights`, `kDefaultFov` |
| Namespaces | lowercase | `engine`, `renderer`, `math` |
| Enum types | PascalCase | `TextureFormat` |
| Enum values | PascalCase | `TextureFormat::RGBA8` |
| Macros | UPPER_SNAKE_CASE | `ENGINE_DEBUG` |
| Files | lowercase, snake_case if needed | `mesh.hpp`, `input_manager.cpp` |

## 2. Formatting

Use **Allman-style braces**: opening braces go on a new line.

```cpp
class Camera
{
public:
    void updateCamera(float deltaTime);
    void setPosition(const glm::vec3& position);
    const glm::vec3& getPosition() const;

private:
    glm::vec3 position_;
    float movementSpeed_ = 5.0f;
};
```

Apply this brace style consistently to classes, functions, namespaces, control-flow statements, and other applicable blocks.

## 3. Classes and Member Access

- Keep class members `private` by default.
- Expose an intentional public interface rather than implementation details.
- Use `const` for member functions that do not modify the object.
- Avoid generating getters and setters for every member automatically.
- Add accessors when they protect invariants, validate values, or provide a useful abstraction.
- Use public fields for simple data-oriented structs when appropriate.

## 4. Functions and Variables

- Use descriptive names that communicate intent.
- Prefer `vertexBuffer_` over `vbo_` unless the abbreviation is well established in context.
- Prefix boolean names with meaningful terms such as `is`, `has`, or `should` where appropriate: `isVisible_`, `hasTexture_`, `shouldClose_`.
- Use `const` for values that should not change.
- Pass larger objects by `const` reference when appropriate to avoid unnecessary copies.
- Public members should simply be written in camelCase, and constructor parameters will use _ in order to avoid shadowing

## 5. Constants and Enums

Prefer named constants over unexplained numeric literals.

```cpp
namespace renderer
{
    constexpr int kMaxLights = 16;
}
```

Use `enum class` for scoped enumerations.

```cpp
enum class TextureFormat
{
    RGBA8,
    RGB8,
    R8
};
```

## File and Directory Structure

- Markdown documentation files use **UPPERCASE** filenames, such as `README.md` and `NAMING_CONVENTIONS.md`.
- C++ source and header filenames use **lowercase**, with `snake_case` for multiword names.
- Header files use the `.hpp` extension.
- Source files use the `.cpp` extension.
- Public headers live in `include/`.
- Source implementations live in `src/`.

### Example Project Structure

```text
MyEngine/
├── include/
│   └── engine/
│       ├── camera.hpp
│       ├── mesh.hpp
│       ├── input_manager.hpp
│       └── renderer.hpp
├── src/
│   ├── camera.cpp
│   ├── mesh.cpp
│   ├── input_manager.cpp
│   └── renderer.cpp
├── README.md
├── NAMING_CONVENTIONS.md
└── CMakeLists.txt
```

### Header and Source Pairing

Keep each `.cpp` file associated with its corresponding `.hpp` file by name.

For example:

- `include/engine/camera.hpp`
- `src/camera.cpp`

Use include paths that reflect the public header structure:

```cpp
#include <engine/camera.hpp>
#include <engine/mesh.hpp>
```

### Exceptions

- Header-only components can live entirely in `include/`.
- Private implementation headers can live in `src/` when they should not be part of the public interface.
- Build configuration files, such as `CMakeLists.txt`, retain their required filenames.
- Third-party files retain their original naming conventions.

## 8. Core Principle

**Consistency beats personal preference.**

Use PascalCase for types, camelCase for functions and ordinary variables, trailing underscores for member variables, and `kPascalCase` for constants. Exceptions should be deliberate and limited.
