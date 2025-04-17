#pragma once
#include "raylib.h"
#include "config.h"

class ObjectType {
public:
    virtual ~ObjectType() = default;

    Model& get_model() { return model_; }
    Texture2D& get_texture() { return texture_; }

protected:
    ObjectType(Model model, Texture2D texture)
        : model_(model), texture_(texture) {
        // Set the texture for the model's first material
    }
 
    Model model_;
    Texture2D texture_;
};

class ShipType : public ObjectType {
public:
    // Delete copy and move operations
    ShipType(const ShipType& other) = delete;
    ShipType(ShipType&& other) = delete;
    ShipType& operator=(const ShipType& other) = delete;
    ShipType& operator=(ShipType&& other) = delete;

    // Singleton getter
    static ShipType& get_instance() {
        static ShipType instance;
        return instance;
    }

private:
    // Private constructor
    ShipType()
        : ObjectType(LoadModel(PIRATE_SHIP_MODEL), LoadTexture(TEXTURE3B)) {
        // Additional initialization if needed
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture
    }
};

class OceanType : public ObjectType {
public:
    // Delete copy and move operations
    OceanType(const OceanType& other) = delete;
    OceanType(OceanType&& other) = delete;
    OceanType& operator=(const OceanType& other) = delete;
    OceanType& operator=(OceanType&& other) = delete;

    // Singleton getter
    static OceanType& get_instance() {
        static OceanType instance;
        return instance;
    }

private:
    // Private constructor
    OceanType()
        : ObjectType(LoadModelFromMesh(GenMeshCube(WORLD_X, WORLD_Y * 0.5, WORLD_Z)),
            LoadTexture(WATER_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture
        // Additional initialization if needed
    }
};

class TerrainType : public ObjectType {
public:
    // Delete copy and move operations
    TerrainType(const TerrainType& other) = delete;
    TerrainType(TerrainType&& other) = delete;
    TerrainType& operator=(const TerrainType& other) = delete;
    TerrainType& operator=(TerrainType&& other) = delete;

    // Singleton getter
    static TerrainType& get_instance() {
        static TerrainType instance;
        return instance;
    }

private:
    // Private constructor
    TerrainType()
        : ObjectType(LoadModelFromMesh(GenMeshHeightmap(LoadImage(TERRAIN_IMAGE),
            Vector3{ WORLD_X, WORLD_Y * 0.5, WORLD_Z })),
            LoadTextureFromImage(LoadImage(TERRAIN_IMAGE))) {
        // Additional initialization if needed
    }
};


class HubType : public ObjectType {
public:
    HubType(const HubType& other) = delete;
    HubType(HubType&& other) = delete;
    HubType& operator=(const HubType& other) = delete;
    HubType& operator=(HubType&& other) = delete;

    // Singleton getter
    static HubType& get_instance() {
        static HubType instance;
        return instance;
    }

private:
    // Private constructor
    HubType()
        : ObjectType(LoadModel(HUB_TERRAIAN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};
class CoveType : public ObjectType {
public:
    CoveType(const CoveType& other) = delete;
    CoveType(CoveType&& other) = delete;
    CoveType& operator=(const CoveType& other) = delete;
    CoveType& operator=(CoveType&& other) = delete;

    // Singleton getter
    static CoveType& get_instance() {
        static CoveType instance;
        return instance;
    }

private:
    // Private constructor
    CoveType()
        : ObjectType(LoadModel(COVE_TERRAIN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};
