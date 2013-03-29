#ifndef PANEL_MESH_H
#define PANEL_MESH_H

#include "mesh.h"

/** Panel widgets in the tetris game scene.
 */
struct PanelMesh : public Mesh {

/// Panel constructor
PanelMesh() {

floatPerVertex = 2;

/// Vertex positions
vertices = {   5.0f,   7.0f,
              95.0f,   7.0f,
               5.0f,  30.0f,
              95.0f,   7.0f,
              95.0f,  30.0f,
               5.0f,  30.0f,

               5.0f,  40.0f,
              95.0f,  40.0f,
               5.0f,  85.0f,
              95.0f,  40.0f,
              95.0f,  85.0f,
               5.0f,  85.0f,

               5.0f,  95.0f,
               95.0f, 95.0f,
               5.0f, 140.0f,
              95.0f,  95.0f,
              95.0f, 140.0f,
               5.0f, 140.0f,

               5.0f, 150.0f,
              95.0f, 150.0f,
               5.0f, 195.0f,
              95.0f, 150.0f,
              95.0f, 195.0f,
               5.0f, 195.0f,

               5.0f, 205.0f,
              95.0f, 205.0f,
               5.0f, 293.0f,
              95.0f, 205.0f,
              95.0f, 293.0f,
               5.0f, 293.0f
};

}

};

#endif // PANEL_MESH_H
