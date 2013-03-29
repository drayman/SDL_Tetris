#ifndef SQUARE_MESH_H
#define SQUARE_MESH_H

#include "mesh.h"

/** Simple square
 */
struct ButtonMesh : public Mesh {

/// Panel constructor
ButtonMesh() {

floatPerVertex = 2;

/// Vertex positions
vertices = { -0.5f, -0.5f,
              0.5f, -0.5f,
             -0.5f,  0.5f,

              0.5f,  0.5f,
             -0.5f,  0.5f,
              0.5f, -0.5f
};


colors   = {  0.4f,  0.4f, 0.4f, 1.0f,
              0.5f,  0.5f, 0.5f, 1.0f,
              0.6f,  0.6f, 0.6f, 1.0f,

              0.8f,  0.8f, 0.8f, 1.0f,
              0.6f,  0.6f, 0.6f, 1.0f,
              0.5f,  0.5f, 0.5f, 1.0f
};


texices = { 0.0f,  1.0f,
            1.0f,  1.0f,
            0.0f,  0.0f,

            1.0f,  0.0f,
            0.0f,  0.0f,
            1.0f,  1.0f
};

}

};

#endif // BUTTON_MESH_H
