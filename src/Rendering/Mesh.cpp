#include "Rendering/Mesh.hpp"

namespace Engine::Rendering
{
std::shared_ptr<Mesh> Mesh::CreateCube()
{
    {
        std::vector<Vertex> vertices{
            // 24 sommets uniques (4 sommets par face x 6 faces)
            // === FACE AVANT (Z = 1.0f) | Normale pointant vers +Z ===
            // Position           | UV           | Normale
            {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // 0 : Bas-Gauche
            {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},  // 1 : Bas-Droit
            {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},   // 2 : Haut-Droit
            {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},  // 3 : Haut-Gauche

            // === FACE ARRIÈRE (Z = -1.0f) | Normale pointant vers -Z ===
            {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, // 4 : Bas-Gauche (vu de dos)
            {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, // 5 : Bas-Droit
            {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},  // 6 : Haut-Droit
            {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},   // 7 : Haut-Gauche

            // === FACE GAUCHE (X = -1.0f) | Normale pointant vers -X ===
            {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, // 8
            {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},  // 9
            {{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},   // 10
            {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},  // 11

            // === FACE DROITE (X = 1.0f) | Normale pointant vers +X ===
            {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // 12
            {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // 13
            {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},  // 14
            {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},   // 15

            // === FACE HAUT (Y = 1.0f) | Normale pointant vers +Y ===
            {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // 16
            {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},   // 17
            {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},  // 18
            {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, // 19

            // === FACE BAS (Y = -1.0f) | Normale pointant vers -Y ===
            {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, // 20
            {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},  // 21
            {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},   // 22
            {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}   // 23
        };

        // 36 indices reliant les sommets (2 triangles par face x 3 sommets x 6 faces)
        // Le Winding Order est configuré en sens anti-horaire (CCW) pour le Face Culling
        std::vector<unsigned int> indices = {
            0,  1,  2,  2,  3,  0,  // Face Avant
            4,  5,  6,  6,  7,  4,  // Face Arrière
            8,  9,  10, 10, 11, 8,  // Face Gauche
            12, 13, 14, 14, 15, 12, // Face Droite
            16, 17, 18, 18, 19, 16, // Face Haut
            20, 21, 22, 22, 23, 20  // Face Bas
        };

        return std::make_shared<Mesh>(vertices, indices, Primitive::Triangles);
    }
}
} // namespace Engine::Rendering